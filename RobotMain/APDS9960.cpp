#include "APDS9960.h"

void APDS9960::init() {

    /* Set ENABLE register to 0 (disable all features) */
    setMode(ALL, false);

    /* Set default values for ambient light and proximity registers */
    wireWriteData8Bit(APDS9960_ATIME, DEFAULT_ATIME);
    wireWriteData8Bit(APDS9960_WTIME, DEFAULT_WTIME);
    wireWriteData8Bit(APDS9960_PPULSE, DEFAULT_PROX_PPULSE);
    wireWriteData8Bit(APDS9960_POFFSET_UR, DEFAULT_POFFSET_UR);
    wireWriteData8Bit(APDS9960_POFFSET_DL, DEFAULT_POFFSET_DL);
    wireWriteData8Bit(APDS9960_CONFIG1, DEFAULT_CONFIG1);
    setProxIntLowThresh(DEFAULT_PILT);
    setProxIntHighThresh(DEFAULT_PIHT);
    setLightIntLowThreshold(DEFAULT_AILT);
    setLightIntHighThreshold(DEFAULT_AIHT);
    wireWriteData8Bit(APDS9960_PERS, DEFAULT_PERS);
    wireWriteData8Bit(APDS9960_CONFIG2, DEFAULT_CONFIG2);
    wireWriteData8Bit(APDS9960_CONFIG3, DEFAULT_CONFIG3);
}

bool APDS9960::check() {
  uint8_t id;
  wireReadData8Bit(APDS9960_ID, id);
  return I2C::check(&I2C_1, APDS9960_I2C_ADDR) && (id == APDS9960_ID_1 || id == APDS9960_ID_2);
}

uint8_t APDS9960::getMode() {
    uint8_t enable_value;
    wireReadData8Bit(APDS9960_ENABLE, enable_value);
    return enable_value;
}

void APDS9960::setMode(uint8_t mode, uint8_t enable) {
    uint8_t reg_val = getMode();
    /* Change bit(s) in ENABLE register */
    enable = enable & 0x01;
    if (mode >= 0 && mode <= 6) {
        if (enable) reg_val |= (1 << mode);
        else reg_val &= ~(1 << mode);
    } else if (mode == ALL) reg_val = (enable)?0x7F:0x00;
    wireWriteData8Bit(APDS9960_ENABLE, reg_val);
}

void APDS9960::enableLightSensor(bool interrupts) {
    /* Set default gain, interrupts, enable power, and enable sensor */
    setAmbientLightGain(DEFAULT_AGAIN);
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
    setProximityGain(DEFAULT_PGAIN);
    setLEDDrive(DEFAULT_LDRIVE);
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

void APDS9960::readAmbientLight(uint16_t &val) {
    wireReadData16Bit(APDS9960_CDATAL, val);
}

void APDS9960::readRedLight(uint16_t &val) {
    wireReadData16Bit(APDS9960_RDATAL, val);
}

void APDS9960::readGreenLight(uint16_t &val) {
    wireReadData16Bit(APDS9960_GDATAL, val);
}

void APDS9960::readBlueLight(uint16_t &val) {
    wireReadData16Bit(APDS9960_BDATAL, val);
}

void APDS9960::readProximity(uint8_t &val) {
    wireReadData8Bit(APDS9960_PDATA, val);
}

uint8_t APDS9960::getProxIntLowThresh() {
    uint8_t val;
    /* Read value from PILT register */
    wireReadData8Bit(APDS9960_PILT, val);
    return val;
}

void APDS9960::setProxIntLowThresh(uint8_t threshold) {
    wireWriteData8Bit(APDS9960_PILT, threshold);
}

uint8_t APDS9960::getProxIntHighThresh() {
    uint8_t val;
    wireReadData8Bit(APDS9960_PIHT, val);
    return val;
}

void APDS9960::setProxIntHighThresh(uint8_t threshold) {
    wireWriteData8Bit(APDS9960_PIHT, threshold);
}

uint8_t APDS9960::getLEDDrive() {
    uint8_t val;
    wireReadData8Bit(APDS9960_CONTROL, val);
    /* Shift and mask out LED drive bits */
    return (val >> 6) & 0b00000011;
}

void APDS9960::setLEDDrive(uint8_t drive) {
    uint8_t val;
    wireReadData8Bit(APDS9960_CONTROL, val);
    /* Set bits in register to given value */
    drive &= 0b00000011;
    drive = drive << 6;
    val &= 0b00111111;
    val |= drive;
    wireWriteData8Bit(APDS9960_CONTROL, val);
}

uint8_t APDS9960::getProximityGain() {
    uint8_t val;
    wireReadData8Bit(APDS9960_CONTROL, val);
    /* Shift and mask out PDRIVE bits */
    return val = (val >> 2) & 0b00000011;
}

void APDS9960::setProximityGain(uint8_t drive) {
    uint8_t val;
    wireReadData8Bit(APDS9960_CONTROL, val);
    /* Set bits in register to given value */
    drive &= 0b00000011;
    drive = drive << 2;
    val &= 0b11110011;
    val |= drive;
    wireWriteData8Bit(APDS9960_CONTROL, val);
}

uint8_t APDS9960::getAmbientLightGain() {
    uint8_t val;
    wireReadData8Bit(APDS9960_CONTROL, val);
    /* Shift and mask out ADRIVE bits */
    return val & 0b00000011;
}

void APDS9960::setAmbientLightGain(uint8_t drive) {
    uint8_t val;
    wireReadData8Bit(APDS9960_CONTROL, val);
    /* Set bits in register to given value */
    drive &= 0b00000011;
    val &= 0b11111100;
    val |= drive;
    wireWriteData8Bit(APDS9960_CONTROL, val);
}

uint8_t APDS9960::getLEDBoost() {
    uint8_t val;
    wireReadData8Bit(APDS9960_CONFIG2, val);
    /* Shift and mask out LED_BOOST bits */
    return (val >> 4) & 0b00000011;
}

void APDS9960::setLEDBoost(uint8_t boost) {
    uint8_t val;
    wireReadData8Bit(APDS9960_CONFIG2, val);
    /* Set bits in register to given value */
    boost &= 0b00000011;
    boost = boost << 4;
    val &= 0b11001111;
    val |= boost;
    wireWriteData8Bit(APDS9960_CONFIG2, val);
}

uint8_t APDS9960::getProxGainCompEnable() {
    uint8_t val;
    wireReadData8Bit(APDS9960_CONFIG3, val);
    /* Shift and mask out PCMP bits */
    return (val >> 5) & 0b00000001;
}

void APDS9960::setProxGainCompEnable(uint8_t enable) {
    uint8_t val;
    wireReadData8Bit(APDS9960_CONFIG3, val);
    /* Set bits in register to given value */
    enable &= 0b00000001;
    enable = enable << 5;
    val &= 0b11011111;
    val |= enable;
    wireWriteData8Bit(APDS9960_CONFIG3, val);
}

uint8_t APDS9960::getProxPhotoMask() {
    uint8_t val;
    wireReadData8Bit(APDS9960_CONFIG3, val);
    /* Mask out photodiode enable mask bits */
    return val & 0b00001111;;
}

void APDS9960::setProxPhotoMask(uint8_t mask) {
    uint8_t val;
    wireReadData8Bit(APDS9960_CONFIG3, val);
    /* Set bits in register to given value */
    mask &= 0b00001111;
    val &= 0b11110000;
    val |= mask;
    wireWriteData8Bit(APDS9960_CONFIG3, val);
}

void APDS9960::getLightIntLowThreshold(uint16_t &threshold) {
    return wireReadData16Bit(APDS9960_AILTL, threshold);
}

void APDS9960::setLightIntLowThreshold(uint16_t threshold) {
    return wireWriteData16Bit(APDS9960_AILTL, threshold);
}

void APDS9960::getLightIntHighThreshold(uint16_t &threshold) {
    threshold = 0;
    return wireReadData16Bit(APDS9960_AIHTL, threshold);
}

void APDS9960::setLightIntHighThreshold(uint16_t threshold) {
    return wireWriteData16Bit(APDS9960_AIHTL, threshold);
}

void APDS9960::getProximityIntLowThreshold(uint8_t &threshold) {
    threshold = 0;
    return wireReadData8Bit(APDS9960_PILT, threshold);
}

void APDS9960::setProximityIntLowThreshold(uint8_t threshold) {
    return wireWriteData8Bit(APDS9960_PILT, threshold);
}

void APDS9960::getProximityIntHighThreshold(uint8_t &threshold) {
    threshold = 0;
    return wireReadData8Bit(APDS9960_PIHT, threshold);
}

void APDS9960::setProximityIntHighThreshold(uint8_t threshold) {
    wireWriteData8Bit(APDS9960_PIHT, threshold);
}

uint8_t APDS9960::getAmbientLightIntEnable() {
    uint8_t val;
    wireReadData8Bit(APDS9960_ENABLE, val);
    /* Shift and mask out AIEN bit */
    return (val >> 4) & 0b00000001;
}

void APDS9960::setAmbientLightIntEnable(uint8_t enable) {
    uint8_t val;
    wireReadData8Bit(APDS9960_ENABLE, val);
    /* Set bits in register to given value */
    enable &= 0b00000001;
    enable = enable << 4;
    val &= 0b11101111;
    val |= enable;
    wireWriteData8Bit(APDS9960_ENABLE, val);
}

uint8_t APDS9960::getProximityIntEnable() {
    uint8_t val;
    wireReadData8Bit(APDS9960_ENABLE, val);
    /* Shift and mask out PIEN bit */
    return (val >> 5) & 0b00000001;
}

void APDS9960::setProximityIntEnable(uint8_t enable) {
    uint8_t val;
    wireReadData8Bit(APDS9960_ENABLE, val);
    /* Set bits in register to given value */
    enable &= 0b00000001;
    enable = enable << 5;
    val &= 0b11011111;
    val |= enable;
    wireWriteData8Bit(APDS9960_ENABLE, val);
}

void APDS9960::clearAmbientLightInt() {
    uint8_t throwaway;
    wireReadData8Bit(APDS9960_AICLEAR, throwaway);
}

void APDS9960::clearProximityInt() {
    uint8_t throwaway;
    wireReadData8Bit(APDS9960_PICLEAR, throwaway);
}

void APDS9960::wireWrite8Bit(uint8_t val) {
    I2C_2.beginTransmission(APDS9960_I2C_ADDR);
    I2C_2.write(val);
    I2C_2.endTransmission();
}

void APDS9960::wireWriteData8Bit(uint8_t reg, uint8_t val) {
    I2C_2.beginTransmission(APDS9960_I2C_ADDR);
    I2C_2.write(reg);
    I2C_2.write(val);
    I2C_2.endTransmission();
}

void APDS9960::wireReadData8Bit(uint8_t reg, uint8_t &val) {
    wireWrite8Bit(reg);
    I2C_2.requestFrom(APDS9960_I2C_ADDR, 1);
    while (I2C_2.available()) val = I2C_2.read();
}

void APDS9960::wireWriteData16Bit(uint8_t reg, uint16_t val) {
    /* Break 16-bit threshold into 2 8-bit values */
    uint8_t val_low = val & 0x00FF;
    uint8_t val_high = (val & 0xFF00) >> 8;
    wireWriteData8Bit(reg, val_low);
    wireWriteData8Bit(reg+1, val_high);
}

void APDS9960::wireReadData16Bit(uint8_t reg, uint16_t &val) {
    uint8_t val_byte;
    wireReadData8Bit(reg, val_byte);
    val = val_byte;
    wireReadData8Bit(reg+1, val_byte);
    val = val + ((uint16_t) val_byte << 8);
}
