#if !defined(DEVICE_NAME)
    #define DEVICE_NAME             "Flysky FRM302"
#endif
// There is some special handling for this target
#define TARGET_TX_FRM302

// GPIO pin definitions
#define GPIO_PIN_NSS                PA15
#define GPIO_PIN_BUSY               PB6
#define GPIO_PIN_DIO1               PB10
#define GPIO_PIN_MOSI               PB5
#define GPIO_PIN_MISO               PB4
#define GPIO_PIN_SCK                PB3
#define GPIO_PIN_RST                PA0
#define GPIO_PIN_TX_ENABLE          PA4  // Controlled by FRIO?
#define GPIO_PIN_RX_ENABLE          PB1  // Controlled by FRIO?
//#define GPIO_PIN_ANT_CTRL_1         PA9 // not sure if present on FRM302?
//#define GPIO_PIN_ANT_CTRL_2         PB13 // not sure if present on FRM302?
#define GPIO_PIN_RCSIGNAL_RX        PA9 // S.PORT (Only needs one wire )
#define GPIO_PIN_RCSIGNAL_TX        PA10  // Needed for CRSF libs but does nothing/not hooked up to JR module.
//#define GPIO_PIN_PA_ENABLE          PB11  // The AWB7232 does not appear to have an EN pin like the SE2622L_202733C
//#define GPIO_PIN_RF_AMP_DET         PA3  // Voltage detector pin, not present on AWB7232?
// #define GPIO_PIN_BUZZER             PC13

// Output Power
#define MinPower                    PWR_10mW
#define MaxPower                    PWR_250mW
#define POWER_OUTPUT_VALUES         {-16,-16,-16,-16,-16}
//#define POWER_OUTPUT_VALUES         {-16,-14,-11,-8,-4}

#define Regulatory_Domain_ISM_2400 1
