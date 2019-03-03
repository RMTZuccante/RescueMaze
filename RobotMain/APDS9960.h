#ifndef SparkFun_APDS9960_H
#define SparkFun_APDS9960_H

#include <Arduino.h>
#include "I2C.h"

/* APDS-9960 I2C address */
#define APDS9960_I2C_ADDR       0x39

/* Error code for returned values */
#define ERROR                   0xFF

/* Acceptable device IDs */
#define APDS9960_ID_1           0xAB
#define APDS9960_ID_2           0x9C

/* Misc parameters */
#define FIFO_PAUSE_TIME         30      // Wait period (ms) between FIFO reads

/* APDS-9960 register addresses */
#define APDS9960_ENABLE         0x80
#define APDS9960_ATIME          0x81
#define APDS9960_WTIME          0x83
#define APDS9960_AILTL          0x84
#define APDS9960_AILTH          0x85
#define APDS9960_AIHTL          0x86
#define APDS9960_AIHTH          0x87
#define APDS9960_PILT           0x89
#define APDS9960_PIHT           0x8B
#define APDS9960_PERS           0x8C
#define APDS9960_CONFIG1        0x8D
#define APDS9960_PPULSE         0x8E
#define APDS9960_CONTROL        0x8F
#define APDS9960_CONFIG2        0x90
#define APDS9960_ID             0x92
#define APDS9960_STATUS         0x93
#define APDS9960_CDATAL         0x94
#define APDS9960_CDATAH         0x95
#define APDS9960_RDATAL         0x96
#define APDS9960_RDATAH         0x97
#define APDS9960_GDATAL         0x98
#define APDS9960_GDATAH         0x99
#define APDS9960_BDATAL         0x9A
#define APDS9960_BDATAH         0x9B
#define APDS9960_PDATA          0x9C
#define APDS9960_POFFSET_UR     0x9D
#define APDS9960_POFFSET_DL     0x9E
#define APDS9960_CONFIG3        0x9F
#define APDS9960_IFORCE         0xE4
#define APDS9960_PICLEAR        0xE5
#define APDS9960_CICLEAR        0xE6
#define APDS9960_AICLEAR        0xE7

/* Acceptable parameters for setMode */
#define POWER                   0
#define AMBIENT_LIGHT           1
#define PROXIMITY               2
#define WAIT                    3
#define AMBIENT_LIGHT_INT       4
#define PROXIMITY_INT           5
#define ALL                     7

/* LED Drive values */
#define LED_DRIVE_100MA         0
#define LED_DRIVE_50MA          1
#define LED_DRIVE_25MA          2
#define LED_DRIVE_12_5MA        3

/* Proximity Gain (PGAIN) values */
#define PGAIN_1X                0
#define PGAIN_2X                1
#define PGAIN_4X                2
#define PGAIN_8X                3

/* ALS Gain (AGAIN) values */
#define AGAIN_1X                0
#define AGAIN_4X                1
#define AGAIN_16X               2
#define AGAIN_64X               3

/* LED Boost values */
#define LED_BOOST_100           0
#define LED_BOOST_150           1
#define LED_BOOST_200           2
#define LED_BOOST_300           3

/* Default values */
#define DEFAULT_ATIME           219     // 103ms
#define DEFAULT_WTIME           246     // 27ms
#define DEFAULT_PROX_PPULSE     0x87    // 16us, 8 pulses
#define DEFAULT_POFFSET_UR      0       // 0 offset
#define DEFAULT_POFFSET_DL      0       // 0 offset
#define DEFAULT_CONFIG1         0x60    // No 12x wait (WTIME) factor
#define DEFAULT_LDRIVE          LED_DRIVE_100MA
#define DEFAULT_PGAIN           PGAIN_4X
#define DEFAULT_AGAIN           AGAIN_4X
#define DEFAULT_PILT            0       // Low proximity threshold
#define DEFAULT_PIHT            50      // High proximity threshold
#define DEFAULT_AILT            0xFFFF  // Force interrupt for calibration
#define DEFAULT_AIHT            0
#define DEFAULT_PERS            0x11    // 2 consecutive prox or ALS for int.
#define DEFAULT_CONFIG2         0x01    // No saturation interrupts or LED boost
#define DEFAULT_CONFIG3         0       // Enable all photodiodes, no SAI

/* APDS9960 Class */
class APDS9960 {
public:

    /* Initialization methods */
    void begin();
    bool check();
    uint8_t getMode();
    void setMode(uint8_t mode, uint8_t enable);

    /* Turn the APDS-9960 on and off */
    void enablePower();
    void disablePower();

    /* Enable or disable specific sensors */
    void enableLightSensor(bool interrupts = false);
    void disableLightSensor();
    void enableProximitySensor(bool interrupts = false);
    void disableProximitySensor();

    /* LED drive strength control */
    uint8_t getLEDDrive();
    void setLEDDrive(uint8_t drive);

    /* Gain control */
    uint8_t getAmbientLightGain();
    void setAmbientLightGain(uint8_t gain);
    uint8_t getProximityGain();
    void setProximityGain(uint8_t gain);

    /* Get and set light interrupt thresholds */
    uint16_t getLightIntLowThreshold();
    void setLightIntLowThreshold(uint16_t threshold);
    uint16_t getLightIntHighThreshold();
    void setLightIntHighThreshold(uint16_t threshold);

    /* Get and set proximity interrupt thresholds */
    uint8_t getProximityIntLowThreshold();
    void setProximityIntLowThreshold(uint8_t threshold);
    uint8_t getProximityIntHighThreshold();
    void setProximityIntHighThreshold(uint8_t threshold);

    /* Get and set interrupt enables */
    uint8_t getAmbientLightIntEnable();
    void setAmbientLightIntEnable(uint8_t enable);
    uint8_t getProximityIntEnable();
    void setProximityIntEnable(uint8_t enable);

    /* Clear interrupts */
    void clearAmbientLightInt();
    void clearProximityInt();

    /* Ambient light methods */
    uint16_t readAmbientLight();
    uint16_t readRedLight();
    uint16_t readGreenLight();
    uint16_t readBlueLight();

    /* Proximity methods */
    uint8_t readProximity();

private:

    /* Proximity Interrupt Threshold */
    uint8_t getProxIntLowThresh();
    void setProxIntLowThresh(uint8_t threshold);
    uint8_t getProxIntHighThresh();
    void setProxIntHighThresh(uint8_t threshold);

    /* LED Boost Control */
    uint8_t getLEDBoost();
    void setLEDBoost(uint8_t boost);

    /* Proximity photodiode select */
    uint8_t getProxGainCompEnable();
    void setProxGainCompEnable(uint8_t enable);
    uint8_t getProxPhotoMask();
    void setProxPhotoMask(uint8_t mask);

    /* Raw I2C Commands */
    void wireWrite8Bit(uint8_t val);
    void wireWriteData8Bit(uint8_t reg, uint8_t val);
    uint8_t wireReadData8Bit(uint8_t reg);
    void wireWriteData16Bit(uint8_t reg, uint16_t val);
    uint16_t wireReadData16Bit(uint8_t reg);
};

#endif
