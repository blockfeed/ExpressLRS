#if !defined(DEVICE_NAME)
    #define DEVICE_NAME             "Flysky FRM302"
#endif
// There is some special handling for this target
#define TARGET_TX_FRM302

// GPIO pin definitions
#define GPIO_PIN_RST                PA0  // 10
#define GPIO_PIN_BUSY               PB6  // 42
#define GPIO_PIN_DIO1               PB10 // 21 USART3, nothing on DIO2 / DIO3
#define GPIO_PIN_NSS                PA15 // 38
#define GPIO_PIN_MOSI               PB5  // 41
#define GPIO_PIN_MISO               PB4  // 40
#define GPIO_PIN_SCK                PB3  // 39
//#define GPIO_PIN_TX_ENABLE          PA4  // connects to "something_EN" pad L
//#define GPIO_PIN_RX_ENABLE          PB1  // connects to "something_EN" pad R
//#define GPIO_PIN_PA_ENABLE          PA12    // CSD - lifted from namimno
//#define GPIO_PIN_ANT_CTRL_1         19 // not sure if present on FRM302?
//#define GPIO_PIN_ANT_CTRL_2         14 // not sure if present on FRM302?

/* S.Port input signal */
#define GPIO_PIN_RCSIGNAL_RX        PA9   // RCSIGNAL is sport, only needs one wire, USART1
#define GPIO_PIN_RCSIGNAL_TX        PA10  // Needed for CRSF libs but does nothing/not hooked up to JR module. USART1
//#define GPIO_PIN_DEBUG_RX       PA3 // USART2 JR PPM
//#define GPIO_PIN_DEBUG_TX       PA2 // USART2 JR TX

//#define GPIO_PIN_FAN_EN             PB0

// Output Power
#define MinPower                    PWR_10mW
#define MaxPower                    PWR_250mW
#define POWER_OUTPUT_VALUES         {-16,-16,-16,-16,-16}
//#define POWER_OUTPUT_VALUES         {-16,-14,-11,-8,-4}

#define Regulatory_Domain_ISM_2400 1
