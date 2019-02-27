#include "APDS9960.h"

bool APDS9960::init() {
    uint8_t id;

    /* Read ID register and check against known values for APDS-9960 */
    if (!wireReadData8Bit(APDS9960_ID, id)) return false;
    if (!(id == APDS9960_ID_1 || id == APDS9960_ID_2)) return false;

    /* Set ENABLE register to 0 (disable all features) */
    if (!setMode(ALL, OFF)) return false;

    /* Set default values for ambient light and proximity registers */
    if (!wireWriteData8Bit(APDS9960_ATIME, DEFAULT_ATIME)) return false;
    if (!wireWriteData8Bit(APDS9960_WTIME, DEFAULT_WTIME)) return false;
    if (!wireWriteData8Bit(APDS9960_PPULSE, DEFAULT_PROX_PPULSE)) return false;
    if (!wireWriteData8Bit(APDS9960_POFFSET_UR, DEFAULT_POFFSET_UR)) return false;
    if (!wireWriteData8Bit(APDS9960_POFFSET_DL, DEFAULT_POFFSET_DL)) return false;
    if (!wireWriteData8Bit(APDS9960_CONFIG1, DEFAULT_CONFIG1)) return false;
    if (!setProxIntLowThresh(DEFAULT_PILT)) return false;
    if (!setProxIntHighThresh(DEFAULT_PIHT)) return false;
    if (!setLightIntLowThreshold(DEFAULT_AILT)) return false;
    if (!setLightIntHighThreshold(DEFAULT_AIHT)) return false;
    if (!wireWriteData8Bit(APDS9960_PERS, DEFAULT_PERS)) return false;
    if (!wireWriteData8Bit(APDS9960_CONFIG2, DEFAULT_CONFIG2)) return false;
    if (!wireWriteData8Bit(APDS9960_CONFIG3, DEFAULT_CONFIG3)) return false;

    return true;
}

uint8_t APDS9960::getMode() {
    uint8_t enable_value;
    if (!wireReadData8Bit(APDS9960_ENABLE, enable_value)) return ERROR;
    return enable_value;
}

bool APDS9960::setMode(uint8_t mode, uint8_t enable) {
    uint8_t reg_val = getMode();
    if (reg_val == ERROR) return false;
    /* Change bit(s) in ENABLE register */
    enable = enable & 0x01;
    if (mode >= 0 && mode <= 6) {
        if (enable) reg_val |= (1 << mode);
        else reg_val &= ~(1 << mode);
    } else if (mode == ALL) reg_val = (enable)?0x7F:0x00;
    return wireWriteData8Bit(APDS9960_ENABLE, reg_val);
}

bool APDS9960::enableLightSensor(bool interrupts) {
    /* Set default gain, interrupts, enable power, and enable sensor */
    if (!setAmbientLightGain(DEFAULT_AGAIN)) return false;
    if (!setAmbientLightIntEnable(interrupts)) return false;
    if (!enablePower()) return false;
    return setMode(AMBIENT_LIGHT, 1);
}

bool APDS9960::disableLightSensor() {
    return setAmbientLightIntEnable(0) && setMode(AMBIENT_LIGHT, 0);
}

bool APDS9960::enableProximitySensor(bool interrupts) {
    /* Set default gain, LED, interrupts, enable power, and enable sensor */
    if (!setProximityGain(DEFAULT_PGAIN)) return false;
    if (!setLEDDrive(DEFAULT_LDRIVE)) return false;
    if (!setProximityIntEnable(interrupts)) return false;
    if (!enablePower()) return false;
    if (!setMode(PROXIMITY, 1)) return false;
    return true;
}

bool APDS9960::disableProximitySensor() {
    return setProximityIntEnable(0) && setMode(PROXIMITY, 0);
}

bool APDS9960::enablePower() {
    return setMode(POWER, 1);
}

bool APDS9960::disablePower() {
    return setMode(POWER, 0);
}

bool APDS9960::readAmbientLight(uint16_t &val) {
    return wireReadData16Bit(APDS9960_CDATAL, val);
}

bool APDS9960::readRedLight(uint16_t &val) {
    return wireReadData16Bit(APDS9960_RDATAL, val);
}

bool APDS9960::readGreenLight(uint16_t &val) {
    return wireReadData16Bit(APDS9960_GDATAL, val);
}

bool APDS9960::readBlueLight(uint16_t &val) {
    return wireReadData16Bit(APDS9960_BDATAL, val);
}

bool APDS9960::readProximity(uint8_t &val) {
    return wireReadData8Bit(APDS9960_PDATA, val);
}

uint8_t APDS9960::getProxIntLowThresh() {
    uint8_t val;
    /* Read value from PILT register */
    if (!wireReadData8Bit(APDS9960_PILT, val)) val = 0;
    return val;
}

bool APDS9960::setProxIntLowThresh(uint8_t threshold) {
    return wireWriteData8Bit(APDS9960_PILT, threshold);
}

uint8_t APDS9960::getProxIntHighThresh() {
    uint8_t val;
    if (!wireReadData8Bit(APDS9960_PIHT, val)) val = 0;
    return val;
}

bool APDS9960::setProxIntHighThresh(uint8_t threshold) {
    return wireWriteData8Bit(APDS9960_PIHT, threshold);
}

uint8_t APDS9960::getLEDDrive() {
    uint8_t val;
    if (!wireReadData8Bit(APDS9960_CONTROL, val)) return ERROR;
    /* Shift and mask out LED drive bits */
    return (val >> 6) & 0b00000011;
}

bool APDS9960::setLEDDrive(uint8_t drive) {
    uint8_t val;
    if (!wireReadData8Bit(APDS9960_CONTROL, val)) return false;
    /* Set bits in register to given value */
    drive &= 0b00000011;
    drive = drive << 6;
    val &= 0b00111111;
    val |= drive;
    return wireWriteData8Bit(APDS9960_CONTROL, val);
}

uint8_t APDS9960::getProximityGain() {
    uint8_t val;
    if (!wireReadData8Bit(APDS9960_CONTROL, val)) return ERROR;
    /* Shift and mask out PDRIVE bits */
    return val = (val >> 2) & 0b00000011;
}

bool APDS9960::setProximityGain(uint8_t drive) {
    uint8_t val;
    if (!wireReadData8Bit(APDS9960_CONTROL, val)) return false;
    /* Set bits in register to given value */
    drive &= 0b00000011;
    drive = drive << 2;
    val &= 0b11110011;
    val |= drive;
    return wireWriteData8Bit(APDS9960_CONTROL, val);
}

uint8_t APDS9960::getAmbientLightGain() {
    uint8_t val;
    if (!wireReadData8Bit(APDS9960_CONTROL, val)) return ERROR;
    /* Shift and mask out ADRIVE bits */
    return val & 0b00000011;
}

bool APDS9960::setAmbientLightGain(uint8_t drive) {
    uint8_t val;
    if (!wireReadData8Bit(APDS9960_CONTROL, val)) return false;
    /* Set bits in register to given value */
    drive &= 0b00000011;
    val &= 0b11111100;
    val |= drive;
    return wireWriteData8Bit(APDS9960_CONTROL, val);
}

uint8_t APDS9960::getLEDBoost() {
    uint8_t val;
    if (!wireReadData8Bit(APDS9960_CONFIG2, val)) return ERROR;
    /* Shift and mask out LED_BOOST bits */
    return (val >> 4) & 0b00000011;
}

bool APDS9960::setLEDBoost(uint8_t boost) {
    uint8_t val;
    if (!wireReadData8Bit(APDS9960_CONFIG2, val)) return false;
    /* Set bits in register to given value */
    boost &= 0b00000011;
    boost = boost << 4;
    val &= 0b11001111;
    val |= boost;
    return wireWriteData8Bit(APDS9960_CONFIG2, val);
}

uint8_t APDS9960::getProxGainCompEnable() {
    uint8_t val;
    if (!wireReadData8Bit(APDS9960_CONFIG3, val)) return ERROR;
    /* Shift and mask out PCMP bits */
    return (val >> 5) & 0b00000001;
}

bool APDS9960::setProxGainCompEnable(uint8_t enable) {
    uint8_t val;
    if (!wireReadData8Bit(APDS9960_CONFIG3, val)) return false;
    /* Set bits in register to given value */
    enable &= 0b00000001;
    enable = enable << 5;
    val &= 0b11011111;
    val |= enable;
    return wireWriteData8Bit(APDS9960_CONFIG3, val);
}

uint8_t APDS9960::getProxPhotoMask() {
    uint8_t val;
    if (!wireReadData8Bit(APDS9960_CONFIG3, val)) return ERROR;
    /* Mask out photodiode enable mask bits */
    return val & 0b00001111;;
}

bool APDS9960::setProxPhotoMask(uint8_t mask) {
    uint8_t val;
    if (!wireReadData8Bit(APDS9960_CONFIG3, val)) return false;
    /* Set bits in register to given value */
    mask &= 0b00001111;
    val &= 0b11110000;
    val |= mask;
    return wireWriteData8Bit(APDS9960_CONFIG3, val);
}

bool APDS9960::getLightIntLowThreshold(uint16_t &threshold) {
    return wireReadData16Bit(APDS9960_AILTL, threshold);
}

bool APDS9960::setLightIntLowThreshold(uint16_t threshold) {
    return wireWriteData16Bit(APDS9960_AILTL, threshold);
}

bool APDS9960::getLightIntHighThreshold(uint16_t &threshold) {
    threshold = 0;
    return wireReadData16Bit(APDS9960_AIHTL, threshold);
}

bool APDS9960::setLightIntHighThreshold(uint16_t threshold) {
    return wireWriteData16Bit(APDS9960_AIHTL, threshold);
}

bool APDS9960::getProximityIntLowThreshold(uint8_t &threshold) {
    threshold = 0;
    return wireReadData8Bit(APDS9960_PILT, threshold);
}

bool APDS9960::setProximityIntLowThreshold(uint8_t threshold) {
    return wireWriteData8Bit(APDS9960_PILT, threshold);
}

bool APDS9960::getProximityIntHighThreshold(uint8_t &threshold) {
    threshold = 0;
    return wireReadData8Bit(APDS9960_PIHT, threshold);
}

bool APDS9960::setProximityIntHighThreshold(uint8_t threshold) {
    return wireWriteData8Bit(APDS9960_PIHT, threshold);
}

uint8_t APDS9960::getAmbientLightIntEnable() {
    uint8_t val;
    if (!wireReadData8Bit(APDS9960_ENABLE, val)) return ERROR;
    /* Shift and mask out AIEN bit */
    return (val >> 4) & 0b00000001;
}

bool APDS9960::setAmbientLightIntEnable(uint8_t enable) {
    uint8_t val;
    if (!wireReadData8Bit(APDS9960_ENABLE, val)) return false;
    /* Set bits in register to given value */
    enable &= 0b00000001;
    enable = enable << 4;
    val &= 0b11101111;
    val |= enable;
    return wireWriteData8Bit(APDS9960_ENABLE, val);
}

uint8_t APDS9960::getProximityIntEnable() {
    uint8_t val;
    if (!wireReadData8Bit(APDS9960_ENABLE, val)) return ERROR;
    /* Shift and mask out PIEN bit */
    return (val >> 5) & 0b00000001;
}

bool APDS9960::setProximityIntEnable(uint8_t enable) {
    uint8_t val;
    if (!wireReadData8Bit(APDS9960_ENABLE, val)) return false;
    /* Set bits in register to given value */
    enable &= 0b00000001;
    enable = enable << 5;
    val &= 0b11011111;
    val |= enable;
    return wireWriteData8Bit(APDS9960_ENABLE, val);
}

bool APDS9960::clearAmbientLightInt() {
    uint8_t throwaway;
    return wireReadData8Bit(APDS9960_AICLEAR, throwaway);
}

bool APDS9960::clearProximityInt() {
    uint8_t throwaway;
    return wireReadData8Bit(APDS9960_PICLEAR, throwaway);
}

bool APDS9960::wireWrite8Bit(uint8_t val) {
    I2C_2.beginTransmission(APDS9960_I2C_ADDR);
    I2C_2.write(val);
    return I2C_2.endTransmission() == 0;
}

bool APDS9960::wireWriteData8Bit(uint8_t reg, uint8_t val) {
    I2C_2.beginTransmission(APDS9960_I2C_ADDR);
    I2C_2.write(reg);
    I2C_2.write(val);
    return I2C_2.endTransmission() == 0;
}

bool APDS9960::wireReadData8Bit(uint8_t reg, uint8_t &val) {
    if (!wireWrite8Bit(reg)) return false;
    I2C_2.requestFrom(APDS9960_I2C_ADDR, 1);
    while (I2C_2.available()) val = I2C_2.read();
    return true;
}

bool APDS9960::wireWriteData16Bit(uint8_t reg, uint16_t val) {
    /* Break 16-bit threshold into 2 8-bit values */
    uint8_t val_low = val & 0x00FF;
    uint8_t val_high = (val & 0xFF00) >> 8;
    return wireWriteData8Bit(reg, val_low) && wireWriteData8Bit(reg+1, val_high);
}

bool APDS9960::wireReadData16Bit(uint8_t reg, uint16_t &val) {
    uint8_t val_byte;
    if (!wireReadData8Bit(reg, val_byte)) return false;
    val = val_byte;
    if (!wireReadData8Bit(reg+1, val_byte)) return false;
    val = val + ((uint16_t) val_byte << 8);
    return true;
}
