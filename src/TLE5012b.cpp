#include "TLE5012b.h"

// SPI init structure
SPI_HandleTypeDef spiConfig;

// Main initialization structure
GPIO_InitTypeDef GPIO_InitStructure;

TLE5012B::TLE5012B(){}

// Function to setup the encoder
void TLE5012B::init() {

    // Setup pin A5, A6, and A7
    GPIO_InitStructure.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Enable the clock for the SPI bus
    __HAL_RCC_SPI1_CLK_ENABLE();

    // Set the peripheral to be used
    spiConfig.Instance = SPI1;

    // Configure the settings for transactions
    spiConfig.Init.Direction = SPI_DIRECTION_2LINES;
    spiConfig.Init.Mode = SPI_MODE_MASTER;
    spiConfig.Init.DataSize = SPI_DATASIZE_8BIT;
    spiConfig.Init.CLKPolarity = SPI_POLARITY_LOW;
    spiConfig.Init.CLKPhase = SPI_PHASE_2EDGE;
    spiConfig.Init.NSS = SPI_NSS_SOFT;
    spiConfig.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    spiConfig.Init.FirstBit = SPI_FIRSTBIT_MSB;
    spiConfig.Init.CRCPolynomial = 7;

    // Initialize the SPI bus with the parameters we set
    if (HAL_SPI_Init(&spiConfig) != HAL_OK) {
        Serial.println(F("SPI not initialized!"));
    }

    // Set the chip select pin high, disabling the encoder's communication
    pinMode(ENCODER_SS, OUTPUT);
    digitalWrite(ENCODER_SS, HIGH);

}

// Read the value of a register
uint16_t readEncoderRegister(uint16_t registerAddress) {

    // Pull CS low to select encoder
    digitalWrite(ENCODER_SS, LOW);

    // Add read bit to address
    registerAddress |= ENCODER_READ_COMMAND + 1;

    // Setup RX and TX buffers
    uint8_t rxbuf[2];
    uint8_t txbuf[2] = { uint8_t(registerAddress >> 8), uint8_t(registerAddress) };

    // Send address we want to read, response seems to be equal to request
    HAL_SPI_TransmitReceive(&spiConfig, txbuf, rxbuf, 2, 100);

    // Set the MOSI pin to open drain
    GPIO_InitStructure.Pin = GPIO_PIN_7;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Send 0xFFFF (like BTT code), this returns the wanted value
    txbuf[0] = 0xFF, txbuf[1] = 0xFF;
    HAL_SPI_TransmitReceive(&spiConfig, txbuf, rxbuf, 2, 100);

    // Set MOSI back to Push/Pull
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Deselect encoder
    digitalWrite(ENCODER_SS, HIGH);

    // Return value as uint16
    return rxbuf[0] << 8 | rxbuf[1];
}

// Read multiple registers
void readMultipleEncoderRegisters(uint16_t registerAddress, uint16_t* data, uint16_t dataLength) {

    // Pull CS low to select encoder
    digitalWrite(ENCODER_SS, LOW);

    // Setup TX and RX buffers
    registerAddress |= ENCODER_READ_COMMAND + dataLength;
    uint8_t txbuf[dataLength * 2] = { uint8_t(registerAddress >> 8), uint8_t(registerAddress) };
    uint8_t rxbuf[dataLength * 2];

    // Send address we want to read, response seems to be equal to request
    HAL_SPI_TransmitReceive(&spiConfig, txbuf, rxbuf, 2, 100);

    // Set the MOSI pin to open drain
    GPIO_InitStructure.Pin = GPIO_PIN_7;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Send 0xFFFF (like BTT code), this returns the wanted value
    // Array length is doubled as we're using 8 bit values instead of 16
    for (uint8_t i = 0; i < dataLength * 2; i++) {
        txbuf[i] = 0xFF;
    }
    HAL_SPI_TransmitReceive(&spiConfig, txbuf, rxbuf, dataLength * 2, 100);
    
    // Write the received data into the array
    for (uint8_t i = 0; i < dataLength; i++) {
        data[i] = rxbuf[i * 2] << 8 | rxbuf[i * 2 + 1];
    }

    // Set MOSI back to Push/Pull
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Deselect encoder
    digitalWrite(ENCODER_SS, HIGH);
}

// Reads the value for the angle of the encoder in Radians
float TLE5012B::getAngle() {

    // Get the value of the angle register
    uint16_t rawData = readEncoderRegister(ENCODER_ANGLE_REG);

    // Delete the first bit, saving the last 15
    rawData = (rawData & (DELETE_BIT_15));

    // Add the averaged value (equation from TLE5012 library)
    return (RADS_IN_CIRCLE / POW_2_15) * ((float) rawData);
}

float TLE5012B::getVelocity(){
    // Prepare the variables to store data in
	uint16_t rawData[5];

    // Read the encoder, modifying the array
    readMultipleEncoderRegisters(ENCODER_SPEED_REG, rawData, sizeof(rawData) / sizeof(uint16_t));

	// Get raw speed reading
	int16_t rawSpeed = rawData[0];
	rawSpeed = rawSpeed & DELETE_BIT_15;

	// If bit 14 is set, the value is negative
	if (rawSpeed & CHECK_BIT_14) {
		rawSpeed = rawSpeed - CHANGE_UINT_TO_INT_15;
	}

	// Get FIR_MD from bits 15 and 16 of register 0x06
	uint16_t firMD = rawData[3] >> 14;

    // get prediction mode
    uint16_t intMode2Prediction = rawData[5];
	if (intMode2Prediction & 0x0004)
	{
		intMode2Prediction = 3;
	}else{
		intMode2Prediction = 2;
	}

    uint16_t rawAngleRange = rawData[5];
	rawAngleRange &= GET_BIT_14_4;
	rawAngleRange >>= 4;
	float angleRange = RADS_IN_CIRCLE * (POW_2_7 / (double) (rawAngleRange));

	// Determine sensor update rate from FIR_MD
	float firMDVal;
    switch (firMD) {
        case 0:
            firMDVal = 21.3;
            break;
        case 1:
            firMDVal = 42.7;
            break;
        case 2:
            firMDVal = 85.3;
            break;
        case 3:
            firMDVal = 170.6;
            break;
        default:
            firMDVal = 0.0;
            break;
    }

    // rad/s
    return ((angleRange / POW_2_15) * ((float) rawSpeed)) / (((float) intMode2Prediction) * firMDVal * 0.000001);
}

// Reads the speed of the encoder in Radians a Second
double getEncoderSpeed() {

    
}