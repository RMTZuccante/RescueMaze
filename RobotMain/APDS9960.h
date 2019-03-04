#ifndef SparkFun_APDS9960_H
#define SparkFun_APDS9960_H

#include <Arduino.h>
#include "I2C.h"

/* APDS-9960 I2C address */
#define APDS9960_I2C_ADDR       0x39

/* Acceptable device IDs */
#define APDS9960_ID_1           0xAB
#define APDS9960_ID_2           0x9C

/* Misc parameters */
#define FIFO_PAUSE_TIME         30      // Wait period (ms) between FIFO reads

/* Default values */
#define DEFAULT_ATIME           219     // 103ms
#define DEFAULT_WTIME           246     // 27ms
#define DEFAULT_PROX_PPULSE     0x87    // 16us, 8 pulses
#define DEFAULT_POFFSET_UR      0       // 0 offset
#define DEFAULT_POFFSET_DL      0       // 0 offset
#define DEFAULT_CONFIG1         0x60    // No 12x wait (WTIME) factor
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
  /* Acceptable parameters for setMode */
  enum Mode : uint8_t {
    POWER, AMBIENT_LIGHT, PROXIMITY, WAIT, AMBIENT_LIGHT_INT, PROXIMITY_INT, ALL
  };
  /* LED Drive values */
  enum LEDDrive : uint8_t {
    LD100MA, LD50MA, LD25MA, LD12_5MA
  };
  /* Proximity Gain (PGAIN) values */
  enum ProximityGain : uint8_t {
    PG1X, PG2X, PG4X, PG8X
  };
  /* ALS Gain (AGAIN) values */
  enum ALSGain : uint8_t {
    AG1X, AG4X, AG16X, AG64X
  };
  /* LED Boost values */
  enum LEDBoost : uint8_t {
    LB100, LB150, LB200, LB300
  };

private:
    /* APDS-9960 register addresses */
    enum Register : uint8_t {
      ENABLE = 0x80,
      ATIME = 0x81,
      WTIME = 0x83,
      AILT = 0x84,
      AIHT = 0x86,
      PILT = 0x89,
      PIHT = 0x8B,
      PERS = 0x8C,
      CONFIG1 = 0x8D,
      PPULSE = 0x8E,
      CONTROL = 0x8F,
      CONFIG2 = 0x90,
      ID = 0x92,
      STATUS = 0x93,
      CDATA = 0x94,
      RDATA = 0x96,
      GDATA = 0x98,
      BDATA = 0x9A,
      PDATA = 0x9C,
      POFFSET_UR = 0x9D,
      POFFSET_DL = 0x9E,
      CONFIG3 = 0x9F,
      IFORCE = 0xE4,
      PICLEAR = 0xE5,
      CICLEAR = 0xE6,
      AICLEAR = 0xE7
    };

public:
    /* Initialization methods */
    void begin();
    bool check();
    uint8_t getMode();
    void setMode(Mode mode, uint8_t enable);

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
    void setLEDDrive(LEDDrive drive);

    /* Gain control */
    uint8_t getAmbientLightGain();
    void setAmbientLightGain(ALSGain gain);
    uint8_t getProximityGain();
    void setProximityGain(ProximityGain gain);

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
