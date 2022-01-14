#include "targets.h"
#include "common.h"
#include "device.h"
#include "devVTXSPI.h"
#include "helpers.h"
#include "ESP8266_hwTimer.h"

#if defined(GPIO_PIN_SPI_VTX_NSS) && (GPIO_PIN_SPI_VTX_NSS != UNDEF_PIN)
#include "logging.h"
#include <SPI.h>

#define SYNTHESIZER_REGISTER_A                  0x00
#define SYNTHESIZER_REGISTER_B                  0x01
#define SYNTHESIZER_REGISTER_C                  0x02
#define RF_VCO_DFC_CONTROL_REGISTER             0x03
#define VCO_CONTROL_REGISTER                    0x04
#define VCO_CONTROL_REGISTER_CONT               0x05
#define AUDIO_MODULATOR_CONTROL_REGISTER        0x06
#define PRE_DRIVER_AND_PA_CONTROL_REGISTER      0x07
#define STATE_REGISTER                          0x0F

#define SYNTH_REG_A_DEFAULT                     0x0190

#define POWER_AMP_ON                            0b00000100111110111111
#define POWER_AMP_OFF                           0x00
#define MIN_PWM                                 1000 // Testing required.
#define MAX_PWM                                 3500 // Absolute max is 4095.  But above 3500 does nothing.
#define VPD_BUFFER                              5

#define READ_BIT                                0x00
#define WRITE_BIT                               0x01

#define RTC6705_BOOT_DELAY                      350
#define RTC6705_PLL_SETTLE_TIME_MS              500 // ms - after changing frequency turn amps back on after this time for clean switching
#define VTX_POWER_INTERVAL_MS                   20

#define BUF_PACKET_SIZE                         4 // 25b packet in 4 bytes

void VTxOutputMinimum(void);

bool isTick;

uint8_t vtxSPIBandChannelIdx = 255;
uint8_t vtxSPIBandChannelIdxCurrent = 255;
uint8_t vtxSPIPowerIdx = 0; 
uint8_t vtxSPIPitmode = 1;
uint8_t rtc6705PowerAmpState = 1;
uint16_t vtxSPIPWM = MAX_PWM;
uint16_t VpdSetPoint = 0;
constexpr uint16_t VpdSetPointArray[] = VPD_VALUES;
constexpr uint8_t VpdSetPointCount =  ARRAY_SIZE(VpdSetPointArray);

const uint16_t freqTable[48] = {
    5865, 5845, 5825, 5805, 5785, 5765, 5745, 5725, // A
    5733, 5752, 5771, 5790, 5809, 5828, 5847, 5866, // B
    5705, 5685, 5665, 5645, 5885, 5905, 5925, 5945, // E
    5740, 5760, 5780, 5800, 5820, 5840, 5860, 5880, // F
    5658, 5695, 5732, 5769, 5806, 5843, 5880, 5917, // R
    5333, 5373, 5413, 5453, 5493, 5533, 5573, 5613  // L
};

static void rtc6705WriteRegister(uint32_t regData)
{
    uint8_t buf[BUF_PACKET_SIZE];
    memcpy (buf, (byte *) &regData, BUF_PACKET_SIZE);

    SPI.setBitOrder(LSBFIRST);
    digitalWrite(GPIO_PIN_SPI_VTX_NSS, LOW);
    SPI.transfer(buf, BUF_PACKET_SIZE);
    digitalWrite(GPIO_PIN_SPI_VTX_NSS, HIGH);
    SPI.setBitOrder(MSBFIRST);
}

static void rtc6705ResetSynthRegA()
{
  uint32_t regData = SYNTHESIZER_REGISTER_A | (WRITE_BIT << 4) | (SYNTH_REG_A_DEFAULT << 5);
  rtc6705WriteRegister(regData);
}

static void rtc6705SetFrequency(uint32_t freq)
{
    VTxOutputMinimum(); // Set power to zero for clear channel switching
  
    uint32_t f = 25 * freq;
    uint32_t SYN_RF_N_REG = f / 64;
    uint32_t SYN_RF_A_REG = f % 64;

    uint32_t regData = SYNTHESIZER_REGISTER_B | (WRITE_BIT << 4) | (SYN_RF_A_REG << 5) | (SYN_RF_N_REG << 12);

    rtc6705WriteRegister(regData);
}

static void rtc6705SetFrequencyByIdx(uint8_t idx)
{
    rtc6705SetFrequency((uint32_t)freqTable[idx]);
}

void rtc6705PowerAmpOff(void)
{
    if (rtc6705PowerAmpState)
    {
        uint32_t regData = PRE_DRIVER_AND_PA_CONTROL_REGISTER | (WRITE_BIT << 4) | (POWER_AMP_OFF << 5);
        rtc6705WriteRegister(regData);
        rtc6705PowerAmpState = 0;
    }

    digitalWrite(GPIO_PIN_RF_AMP_VREF, LOW);
}

void rtc6705PowerAmpOn(void)
{
    if (!rtc6705PowerAmpState)
    {
        uint32_t regData = PRE_DRIVER_AND_PA_CONTROL_REGISTER | (WRITE_BIT << 4) | (POWER_AMP_ON << 5);
        rtc6705WriteRegister(regData);
        rtc6705PowerAmpState = 1;
    }

    digitalWrite(GPIO_PIN_RF_AMP_VREF, HIGH);
}

void VTxOutputMinimum(void)
{
    rtc6705PowerAmpOff();

    vtxSPIPWM = MAX_PWM;
    analogWrite(GPIO_PIN_RF_AMP_PWM, vtxSPIPWM);
}

void VTxOutputIncrease(void)
{
    if (vtxSPIPWM > MIN_PWM) vtxSPIPWM -= 5;
    analogWrite(GPIO_PIN_RF_AMP_PWM, vtxSPIPWM);
}

void VTxOutputDecrease(void)
{
    if (vtxSPIPWM < MAX_PWM) vtxSPIPWM += 5;
    analogWrite(GPIO_PIN_RF_AMP_PWM, vtxSPIPWM);
}

static void checkOutputPower()
{
    if (vtxSPIPitmode)
    {
        INFOLN("Pitmode On...");

        VTxOutputMinimum();
    }
    else
    {
        INFOLN("Pitmode Off...");

        rtc6705PowerAmpOn();
    
        uint16_t Vpd = analogRead(GPIO_PIN_RF_AMP_VPD); // WARNING - Max input 1.0V !!!!

        if (Vpd < (VpdSetPoint - VPD_BUFFER))
        {
            VTxOutputIncrease();
        }
        else if (Vpd > (VpdSetPoint + VPD_BUFFER))
        {
            VTxOutputDecrease();
        }

        LOGGING_UART.println(Vpd);
        LOGGING_UART.println(vtxSPIPWM);
    }
}

static void initialize()
{
    pinMode(GPIO_PIN_SPI_VTX_NSS, OUTPUT);
    digitalWrite(GPIO_PIN_SPI_VTX_NSS, HIGH);
    
    pinMode(GPIO_PIN_RF_AMP_VREF, OUTPUT);
    digitalWrite(GPIO_PIN_RF_AMP_VREF, LOW);

    pinMode(GPIO_PIN_RF_AMP_PWM, OUTPUT);
    analogWriteFreq(10000); // 10kHz
    analogWriteResolution(12); // 0 - 4095
    analogWrite(GPIO_PIN_RF_AMP_PWM, vtxSPIPWM);

    delay(RTC6705_BOOT_DELAY);
}

static int start()
{
    return VTX_POWER_INTERVAL_MS;
}

static int event()
{
    if (vtxSPIBandChannelIdxCurrent != vtxSPIBandChannelIdx)
    {
        return DURATION_IMMEDIATELY;
    }

    return DURATION_IGNORE;
}

static int timeout()
{
    if (isTick) // Only run spi and analog reads during rx free time.
    {
        return DURATION_IMMEDIATELY;
    }

    if (vtxSPIBandChannelIdxCurrent != vtxSPIBandChannelIdx)
    {        
        rtc6705ResetSynthRegA();
        rtc6705SetFrequencyByIdx(vtxSPIBandChannelIdx);
        vtxSPIBandChannelIdxCurrent = vtxSPIBandChannelIdx;

        INFOLN("VTx set frequency...");

        return RTC6705_PLL_SETTLE_TIME_MS;
    }
    else
    {
        if (vtxSPIPowerIdx > VpdSetPointCount) vtxSPIPowerIdx = VpdSetPointCount;
        VpdSetPoint = VpdSetPointArray[vtxSPIPowerIdx - 1];

        checkOutputPower();
        
        LOGGING_UART.println(vtxSPIPowerIdx);
        LOGGING_UART.println(VpdSetPoint);
        
        INFOLN("VTx check output...");

        return VTX_POWER_INTERVAL_MS;
    }
}

device_t VTxSPI_device = {
    .initialize = initialize,
    .start = start,
    .event = event,
    .timeout = timeout
};

#endif