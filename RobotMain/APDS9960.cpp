#include "APDS9960.h"

void APDS9960::begin() {
    /* Set ENABLE register to 0 (disable all features) */
    setMode(ALL, false);

    /* Set default values for ambient light and proximity registers */
    wireWriteData8Bit(ATIME, DEFAULT_ATIME);
    wireWriteData8Bit(WTIME, DEFAULT_WTIME);
    wireWriteData8Bit(PPULSE, DEFAULT_PROX_PPULSE);
    wireWriteData8Bit(POFFSET_UR, DEFAULT_POFFSET_UR);
    wireWriteData8Bit(POFFSET_DL, DEFAULT_POFFSET_DL);
    wireWriteData8Bit(CONFIG1, DEFAULT_CONFIG1);
    setProxIntLowThresh(DEFAULT_PILT);
    setProxIntHighThresh(DEFAULT_PIHT);
    setLightIntLowThreshold(DEFAULT_AILT);
    setLightIntHighThreshold(DEFAULT_AIHT);
    wireWriteData8Bit(PERS, DEFAULT_PERS);
    wireWriteData8Bit(CONFIG2, DEFAULT_CONFIG2);
    wireWriteData8Bit(CONFIG3, DEFAULT_CONFIG3);
}

bool APDS9960::check() {
  uint8_t id = wireReadData8Bit(ID);
  return I2C::check(&I2C_1, APDS9960_I2C_ADDR) && (id == APDS9960_ID_1 || id == APDS9960_ID_2);
}

uint8_t APDS9960::getMode() {
    return wireReadData8Bit(ENABLE);
}

void APDS9960::setMode(Mode mode, uint8_t enable) {
    uint8_t reg_val = getMode();
    /* Change bit(s) in ENABLE register */
    enable = enable & 0x01;
    if (mode >= 0 && mode <= 6) {
        if (enable) reg_val |= (1 << mode);
        else reg_val &= ~(1 << mode);
    } else if (mode == ALL) reg_val = (enable)?0x7F:0x00;
    wireWriteData8Bit(ENABLE, reg_val);
}

void APDS9960::enableLightSensor(bool interrupts) {
    /* Set default gain, interrupts, enable power, and enable sensor */
    setAmbientLightGain(AG4X);
    setAmbientLightIntEnable(interrupts);
    enablePower();
    setMode(AMBIENT_LIGHT, 1);
}

void APDS9960::disableLightSensor() {
    setAmbientLightIntEnable(0);
    setMode(AMBIENT_LIGHT, 0);
}

void APDS9960::enableProximitySensor(bool interrupts) {
    /* Set default gain, LED, interrupts, enable power, and enable sensor */
    setProximityGain(PG4X);
    setLEDDrive(LD100MA);
    setProximityIntEnable(interrupts);
    enablePower();
    setMode(PROXIMITY, 1);
}

void APDS9960::disableProximitySensor() {
    setProximityIntEnable(0);
    setMode(PROXIMITY, 0);
}

void APDS9960::enablePower() {
    setMode(POWER, 1);
}

void APDS9960::disablePower() {
    setMode(POWER, 0);
}

uint16_t APDS9960::readAmbientLight() {
    return wireReadData16Bit(CDATA);
}

uint16_t APDS9960::readRedLight() {
    return wireReadData16Bit(RDATA);
}

uint16_t APDS9960::readGreenLight() {
    return wireReadData16Bit(GDATA);
}

uint16_t APDS9960::readBlueLight() {
    return wireReadData16Bit(BDATA);
}

uint8_t APDS9960::readProximity() {
    return wireReadData8Bit(PDATA);
}

uint8_t APDS9960::getProxIntLowThresh() {
    /* Read value from PILT register */
    return wireReadData8Bit(PILT);
}

void APDS9960::setProxIntLowThresh(uint8_t threshold) {
    wireWriteData8Bit(PILT, threshold);
}

uint8_t APDS9960::getProxIntHighThresh() {
    return wireReadData8Bit(PIHT);
}

void APDS9960::setProxIntHighThresh(uint8_t threshold) {
    wireWriteData8Bit(PIHT, threshold);
}

uint8_t APDS9960::getLEDDrive() {
    /* Shift and mask out LED drive bits */
    return (wireReadData8Bit(CONTROL) >> 6) & 0b00000011;
}

void APDS9960::setLEDDrive(LEDDrive drive) {
    uint8_t val = wireReadData8Bit(CONTROL);
    /* Set bits in register to given value */
    val &= 0b00111111;
    val |= (drive & 0b00000011) << 6;
    wireWriteData8Bit(CONTROL, val);
}

uint8_t APDS9960::getProximityGain() {
    /* Shift and mask out PDRIVE bits */
    return (wireReadData8Bit(CONTROL) >> 2) & 0b00000011;
}

void APDS9960::setProximityGain(ProximityGain gain) {
    uint8_t val = wireReadData8Bit(CONTROL);
    /* Set bits in register to given value */
    val &= 0b11110011;
    val |= (gain & 0b00000011) << 2;
    wireWriteData8Bit(CONTROL, val);
}

uint8_t APDS9960::getAmbientLightGain() {
    /* Shift and mask out ADRIVE bits */
    return wireReadData8Bit(CONTROL) & 0b00000011;
}

void APDS9960::setAmbientLightGain(ALSGain gain) {
    uint8_t val = wireReadData8Bit(CONTROL);
    /* Set bits in register to given value */
    val &= 0b11111100;
    val |= gain & 0b00000011;
    wireWriteData8Bit(CONTROL, val);
}

uint8_t APDS9960::getLEDBoost() {
    uint8_t val = wireReadData8Bit(CONFIG2);
    /* Shift and mask out LED_BOOST bits */
    return (val >> 4) & 0b00000011;
}

void APDS9960::setLEDBoost(uint8_t boost) {
    uint8_t val = wireReadData8Bit(CONFIG2);
    /* Set bits in register to given value */
    boost &= 0b00000011;
    boost = boost << 4;
    val &= 0b11001111;
    val |= boost;
    wireWriteData8Bit(CONFIG2, val);
}

uint8_t APDS9960::getProxGainCompEnable() {
    /* Shift and mask out PCMP bits */
    return (wireReadData8Bit(CONFIG3) >> 5) & 0b00000001;
}

void APDS9960::setProxGainCompEnable(uint8_t enable) {
    uint8_t val = wireReadData8Bit(CONFIG3);
    /* Set bits in register to given value */
    enable &= 0b00000001;
    enable = enable << 5;
    val &= 0b11011111;
    val |= enable;
    wireWriteData8Bit(CONFIG3, val);
}

uint8_t APDS9960::getProxPhotoMask() {
    /* Mask out photodiode enable mask bits */
    return wireReadData8Bit(CONFIG3) & 0b00001111;;
}

void APDS9960::setProxPhotoMask(uint8_t mask) {
    uint8_t val = wireReadData8Bit(CONFIG3);
    /* Set bits in register to given value */
    mask &= 0b00001111;
    val &= 0b11110000;
    val |= mask;
    wireWriteData8Bit(CONFIG3, val);
}

uint16_t APDS9960::getLightIntLowThreshold() {
    return wireReadData16Bit(AILT);
}

void APDS9960::setLightIntLowThreshold(uint16_t threshold) {
    wireWriteData16Bit(AILT, threshold);
}

uint16_t APDS9960::getLightIntHighThreshold() {
    return wireReadData16Bit(AIHT);
}

void APDS9960::setLightIntHighThreshold(uint16_t threshold) {
    wireWriteData16Bit(AIHT, threshold);
}

uint8_t APDS9960::getProximityIntLowThreshold() {
    return wireReadData8Bit(PILT);
}

void APDS9960::setProximityIntLowThreshold(uint8_t threshold) {
    wireWriteData8Bit(PILT, threshold);
}

uint8_t APDS9960::getProximityIntHighThreshold() {
    return wireReadData8Bit(PIHT);
}

void APDS9960::setProximityIntHighThreshold(uint8_t threshold) {
    wireWriteData8Bit(PIHT, threshold);
}

uint8_t APDS9960::getAmbientLightIntEnable() {
    /* Shift and mask out AIEN bit */
    return (wireReadData8Bit(ENABLE) >> 4) & 0b00000001;
}

void APDS9960::setAmbientLightIntEnable(uint8_t enable) {
    uint8_t val = wireReadData8Bit(ENABLE);
    /* Set bits in register to given value */
    enable &= 0b00000001;
    enable = enable << 4;
    val &= 0b11101111;
    val |= enable;
    wireWriteData8Bit(ENABLE, val);
}

uint8_t APDS9960::getProximityIntEnable() {
    /* Shift and mask out PIEN bit */
    return (wireReadData8Bit(ENABLE) >> 5) & 0b00000001;
}

void APDS9960::setProximityIntEnable(uint8_t enable) {
    uint8_t val;
    val = wireReadData8Bit(ENABLE);
    /* Set bits in register to given value */
    enable &= 0b00000001;
    enable = enable << 5;
    val &= 0b11011111;
    val |= enable;
    wireWriteData8Bit(ENABLE, val);
}

void APDS9960::clearAmbientLightInt() {
    wireReadData8Bit(AICLEAR);
}

void APDS9960::clearProximityInt() {
    wireReadData8Bit(PICLEAR);
}

void APDS9960::wireWrite8Bit(uint8_t val) {
    I2C_1.beginTransmission(APDS9960_I2C_ADDR);
    I2C_1.write(val);
    I2C_1.endTransmission();
}

void APDS9960::wireWriteData8Bit(uint8_t reg, uint8_t val) {
    I2C_1.beginTransmission(APDS9960_I2C_ADDR);
    I2C_1.write(reg);
    I2C_1.write(val);
    I2C_1.endTransmission();
}

uint8_t APDS9960::wireReadData8Bit(uint8_t reg) {
    uint8_t val;
    wireWrite8Bit(reg);
    I2C_1.requestFrom(APDS9960_I2C_ADDR, 1);
    while (I2C_1.available()) val = I2C_1.read();
    return val;
}

void APDS9960::wireWriteData16Bit(uint8_t reg, uint16_t val) {
    /* Break 16-bit threshold into 2 8-bit values */
    wireWriteData8Bit(reg, val & 0x00FF);
    wireWriteData8Bit(reg+1, (val & 0xFF00) >> 8);
}

uint16_t APDS9960::wireReadData16Bit(uint8_t reg) {
    uint16_t val = wireReadData8Bit(reg);
    return val | (wireReadData8Bit(reg+1) << 8);
}
