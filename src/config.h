#ifndef __CONFIG_H_
#define __CONFIG_H_

// generic config values
#define BOARD_VOLTAGE           3.3 // The voltage of the main processor
#define CURRENT_SENSE_RESISTOR  0.2 // Value of the board's current calculation resistor. An incorrect value here will cause current inaccuracies


/*
// * = F103C8-CB    | DIGITAL | ANALOG | USART      | TWI       | SPI        | SPECIAL   |
//                  |---------|--------|------------|-----------|------------|-----------|
#define PA0  A0  // | 0       | A0     |            |           |            |           |
#define PA1  A1  // | 1       | A1     |            |           |            |           |
#define PA2  A2  // | 2       | A2     | USART2_TX  |           |            |           |
#define PA3  A3  // | 2       | A2     | USART2_RX  |           |            |           |
#define PA4  A4  // | 4       | A4     |            |           | SPI1_SS    |           |
#define PA5  A5  // | 5       | A5     |            |           | SPI1_SCK   |           |
#define PA6  A6  // | 6       | A6     |            |           | SPI1_MISO  |           |
#define PA7  A7  // | 7       | A7     |            |           | SPI1_MOSI  |           |
#define PA8  8   // | 8       |        |            |           |            |           |
#define PA9  9   // | 9       |        | USART1_TX  |           |            |           |
#define PA10 10  // | 10      |        | USART1_RX  |           |            |           |
#define PA11 11  // | 11      |        |            |           |            | USB_DN    |
#define PA12 12  // | 12      |        |            |           |            | USB_DP    |
#define PA13 13  // | 13      |        |            |           |            | SWD_SWDIO |
#define PA14 14  // | 14      |        |            |           |            | SWD_SWCLK |
#define PA15 15  // | 15      |        |            |           | SPI1_SS    |           |
//                  |---------|--------|------------|-----------|------------|-----------|
#define PB0  A8  // | 16      | A8     |            |           |            |           |
#define PB1  A9  // | 17      | A9     |            |           |            |           |
#define PB2  18  // | 18      |        |            |           |            | BOOT1     |
#define PB3  19  // | 19      |        |            |           | SPI1_SCK   |           |
#define PB4  20  // | 20      |        |            |           | SPI1_MISO  |           |
#define PB5  21  // | 21      |        |            |           | SPI1_MOSI  |           |
#define PB6  22  // | 22      |        | USART1_TX  | TWI1_SCL  |            |           |
#define PB7  23  // | 23      |        | USART1_RX  | TWI1_SDA  |            |           |
#define PB8  24  // | 24      |        |            | TWI1_SCL  |            |           |
#define PB9  25  // | 25      |        |            | TWI1_SDA  |            |           |
#define PB10 26  // | 26      |        | USART3_TX* | TWI2_SCL* |            |           |
#define PB11 27  // | 27      |        | USART3_RX* | TWI2_SDA* |            |           |
#define PB12 28  // | 28      |        |            |           | SPI2_SS*   |           |
#define PB13 29  // | 29      |        |            |           | SPI2_SCK*  |           |
#define PB14 30  // | 30      |        |            |           | SPI2_MISO* |           |
#define PB15 31  // | 31      |        |            |           | SPI2_MOSI* |           |
//                  |---------|--------|------------|-----------|------------|-----------|
#define PC13 32  // | 32      |        |            |           |            |           |
#define PC14 33  // | 33      |        |            |           |            | OSC32_IN  |
#define PC15 34  // | 34      |        |            |           |            | OSC32_OUT |
//                  |---------|--------|------------|-----------|------------|-----------|
#define PD0  35  // | 35      |        |            |           |            | OSC_IN    |
#define PD1  36  // | 36      |        |            |           |            | OSC_OUT   |
//                  |---------|--------|------------|-----------|------------|-----------|
*/

// OLED Mappings
#define OLED_CS_PIN   PB12
#define OLED_RST_PIN  PA8
#define OLED_SCLK_PIN PB13
#define OLED_SDIN_PIN PB14 // D1
#define OLED_DC_PIN   PB15 // ! Needs checked!
#define OLED_CS_GPIO_PORT  GPIOB
#define OLED_RST_GPIO_PORT GPIOB
#define OLED_DC_GPIO_PORT  GPIOB

// Button mappings
#define DOWN_BUTTON_PIN         PA3
#define BACK_BUTTON_PIN         PB0
#define SELECT_BUTTON_PIN       PB1

// Dip switch mappings
#define DIP_CALIBRATED          PA15
#define DIP_CLOSED_LOOP         PB3
#define DIP_MICRO_1             PB10
#define DIP_MICRO_2             PB11

// LED pin
#define LED_PIN PC13

// Motor mappings
#define COIL_A_DIR_1        PB8
#define COIL_A_DIR_2        PB9
#define COIL_B_DIR_1        PB6
#define COIL_B_DIR_2        PB7
/**
 * Filter schematic
 * 
 * VREF ---|R|----uC
 *       |
 *       +--|C|  
*/
#define COIL_A_PWM PB4 // U2
#define COIL_B_PWM PB5 // U4

// Encoder SPI interface - This is all defined in the platform specific variant.h as: PIN_SPI_SS, PIN_SPI_MOSI, PIN_SPI_MISO, PIN_SPI_SCK 
#define ENCODER_SS   PIN_SPI_SS
#define ENCODER_SCK  PIN_SPI_SCK
#define ENCODER_MISO PIN_SPI_MISO
#define ENCODER_MOSI PIN_SPI_MOSI

// Stepping interface
#define STEP_PIN      PA1 // ! according to the previous files, but this is the same as the dir pin
#define ENABLE_PIN    PA2
#define DIRECTION_PIN PA1

// CAN bus pins
#define CAN_IN_PIN  PA11
#define CAN_OUT_PIN PA12

#endif //__CONFIG_H_