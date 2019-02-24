#ifndef VL53L0X_h
#define VL53L0X_h

#include <Arduino.h>
#include "Debug.h"
#include "I2C.h"

class VL53L0X {
  public:
    VL53L0X();
    void setAddress(uint8_t new_addr);
    void begin();
    bool check();
    uint16_t read();
    void start(uint32_t period_ms = 0);
    void stop();

  private:
    // register addresses from API vl53l0x_device.h (ordered as listed there)
    enum regAddr {
      SYSRANGE_START = 0x00,
      SYSTEM_SEQUENCE_CONFIG = 0x01,
      SYSTEM_INTERMEASUREMENT_PERIOD = 0x04,
      SYSTEM_INTERRUPT_CONFIG_GPIO = 0x0A,
      GPIO_HV_MUX_ACTIVE_HIGH = 0x84,
      SYSTEM_INTERRUPT_CLEAR = 0x0B,
      RESULT_INTERRUPT_STATUS = 0x13,
      RESULT_RANGE_STATUS = 0x14,
      I2C_SLAVE_DEVICE_ADDRESS = 0x8A,
      MSRC_CONFIG_CONTROL = 0x60,
      FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT = 0x44,
      PRE_RANGE_CONFIG_VCSEL_PERIOD = 0x50,
      PRE_RANGE_CONFIG_TIMEOUT_MACROP_HI = 0x51,
      FINAL_RANGE_CONFIG_VCSEL_PERIOD = 0x70,
      FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI = 0x71,
      MSRC_CONFIG_TIMEOUT_MACROP = 0x46,
      OSC_CALIBRATE_VAL = 0xF8,
      GLOBAL_CONFIG_SPAD_ENABLES_REF_0 = 0xB0,
      GLOBAL_CONFIG_REF_EN_START_SELECT = 0xB6,
      DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD = 0x4E,
      DYNAMIC_SPAD_REF_EN_START_OFFSET = 0x4F,
      VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV = 0x89,
    };
    enum vcselPeriodType {
      VcselPeriodPreRange, VcselPeriodFinalRange
    };

    // variables
    struct SequenceStepEnables {
      boolean tcc, msrc, dss, pre_range, final_range;
    };
    struct SequenceStepTimeouts {
      uint16_t pre_range_vcsel_period_pclks, final_range_vcsel_period_pclks;
      uint16_t msrc_dss_tcc_mclks, pre_range_mclks, final_range_mclks;
      uint32_t msrc_dss_tcc_us, pre_range_us, final_range_us;
    };
    bool continuous = false;
    uint8_t address;
    uint16_t io_timeout;
    bool did_timeout;
    uint16_t timeout_start_ms;
    uint8_t stop_variable; // read by init and used when starting measurement; is StopVariable field of VL53L0X_DevData_t structure in API
    uint32_t measurement_timing_budget_us;

    // everything
    bool init(bool io_2v8 = true);
    bool setSignalRateLimit(float limit_Mcps);
    bool setMeasurementTimingBudget(uint32_t budget_us);
    uint32_t getMeasurementTimingBudget();
    uint8_t getVcselPulsePeriod(vcselPeriodType type);
    void startContinuous(uint32_t period_ms = 0);
    void stopContinuous();
    uint16_t readRangeContinuousMillimeters();
    uint16_t readRangeSingleMillimeters();
    inline void setTimeout(uint16_t timeout) {
      io_timeout = timeout;
    }
    void writeReg(uint8_t reg, uint8_t value);
    void writeReg16Bit(uint8_t reg, uint16_t value);
    void writeReg32Bit(uint8_t reg, uint32_t value);
    uint8_t readReg(uint8_t reg);
    uint16_t readReg16Bit(uint8_t reg);
    void writeMulti(uint8_t reg, uint8_t const *src, uint8_t count);
    void readMulti(uint8_t reg, uint8_t *dst, uint8_t count);
    bool getSpadInfo(uint8_t *count, bool *type_is_aperture);
    void getSequenceStepEnables(SequenceStepEnables *enables);
    void getSequenceStepTimeouts(SequenceStepEnables const *enables, SequenceStepTimeouts *timeouts);
    bool performSingleRefCalibration(uint8_t vhv_init_byte);
    static uint16_t decodeTimeout(uint16_t value);
    static uint16_t encodeTimeout(uint16_t timeout_mclks);
    static uint32_t timeoutMclksToMicroseconds(uint16_t timeout_period_mclks, uint8_t vcsel_period_pclks);
    static uint32_t timeoutMicrosecondsToMclks(uint32_t timeout_period_us, uint8_t vcsel_period_pclks);
};

#endif
