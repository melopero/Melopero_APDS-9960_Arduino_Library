//Author: Leonardo La Rocca
#ifndef Melopero_APDS9960_H_INCLUDED
#define Melopero_APDS9960_H_INCLUDED

#include "Arduino.h"
#include "Wire.h"

//TODO: include is here for int defs. Remove later
#include <stdint.h>

#define DEFAULT_I2C_ADDRESS 0x39

    //Register addresses
#define ENABLE_REG_ADDRESS 0x80
#define CONFIG_1_REG_ADDRESS 0x8D
#define CONFIG_2_REG_ADDRESS 0x90
#define CONFIG_3_REG_ADDRESS 0x9F
#define CONTROL_1_REG_ADDRESS 0x8F
#define INTERRUPT_PERSISTANCE_REG_ADDRESS 0x8C
#define STATUS_REG_ADDRESS 0x93

    //Proximity Registers Addresses
#define PROX_INT_LOW_THR_REG_ADDRESS 0x89
#define PROX_INT_HIGH_THR_REG_ADDRESS 0x8B
#define PROX_PULSE_COUNT_REG_ADDRESS 0x8E
#define PROX_UP_RIGHT_OFFSET_REG_ADDRESS 0x9D
#define PROX_DOWN_LEFT_OFFSET_REG_ADDRESS 0x9E
#define PROX_DATA_REG_ADDRESS 0x9C

    //ALS Register Addresses
#define ALS_ATIME_REG_ADDRESS 0x81
#define ALS_INT_LOW_THR_LOW_BYTE_REG_ADDRESS 0x84  // This register provides the low byte of the low interrupt threshold.
#define ALS_INT_LOW_THR_HIGH_BYTE_REG_ADDRESS 0x85  // This register provides the high byte of the low interrupt threshold.
#define ALS_INT_HIGH_THR_LOW_BYTE_REG_ADDRESS 0x86  //This register provides the low byte of the high interrupt threshold.
#define ALS_INT_HIGH_THR_HIGH_BYTE_REG_ADDRESS 0x87  //This register provides the high byte of the high interrupt threshold.

#define CLEAR_DATA_LOW_BYTE_REG_ADDRESS 0x94  //Low Byte of clear channel data.
#define CLEAR_DATA_HIGH_BYTE_REG_ADDRESS 0x95  //High Byte of clear channel data.
#define RED_DATA_LOW_BYTE_REG_ADDRESS 0x96  //Low Byte of red channel data.
#define RED_DATA_HIGH_BYTE_REG_ADDRESS 0x97  //High Byte of red channel data.
#define GREEN_DATA_LOW_BYTE_REG_ADDRESS 0x98  //Low Byte of green channel data.
#define GREEN_DATA_HIGH_BYTE_REG_ADDRESS 0x99  //High Byte of green channel data.
#define BLUE_DATA_LOW_BYTE_REG_ADDRESS 0x9A  //Low Byte of blue channel data.
#define BLUE_DATA_HIGH_BYTE_REG_ADDRESS 0x9B  //High Byte of blue channel data.

    //Gesture Register Addresses
#define GESTURE_PROX_ENTER_THR_REG_ADDRESS 0xA0
#define GESTURE_EXIT_THR_REG_ADDRESS 0xA1
#define GESTURE_CONFIG_1_REG_ADDRESS 0xA2
#define GESTURE_CONFIG_2_REG_ADDRESS 0xA3
#define GESTURE_CONFIG_3_REG_ADDRESS 0xAA
#define GESTURE_CONFIG_4_REG_ADDRESS 0xAB
#define GESTURE_OFFSET_UP_REG_ADDRESSES 0xA4
#define GESTURE_OFFSET_DOWN_REG_ADDRESSES 0xA5
#define GESTURE_OFFSET_LEFT_REG_ADDRESSES 0xA7
#define GESTURE_OFFSET_RIGHT_REG_ADDRESSES 0xA9
#define GESTURE_PULSE_COUNT_AND_LEN_REG_ADDRESS 0xA6
#define GESTURE_FIFO_LEVEL_REG_ADDRESS 0xAE
#define GESTURE_FIFO_UP_REG_ADDRESS 0xFC
#define GESTURE_FIFO_DOWN_REG_ADDRESS 0xFD
#define GESTURE_FIFO_LEFT_REG_ADDRESS 0xFE
#define GESTURE_FIFO_RIGHT_REG_ADDRESS 0xFF
#define GESTURE_STATUS_REG_ADDRESS 0xAF

    //Wait Registers Addresses
#define WAIT_TIME_REG_ADDRESS 0x83

    //Clear interrupt regs
#define FORCE_INTERRUPT_REG_ADDRESS 0xE4
#define PROXIMITY_INT_CLEAR_REG_ADDRESS 0xE5
#define ALS_INT_CLEAR_REG_ADDRESS 0xE6
#define CLEAR_ALL_NON_GEST_INT_REG_ADDRESS 0xE7

    //Proximity pulse lengths
#define PULSE_LEN_4_MICROS 0
#define PULSE_LEN_8_MICROS 1
#define PULSE_LEN_16_MICROS 2
#define PULSE_LEN_32_MICROS 3

    //Led drive levels
#define LED_DRIVE_100_mA 0
#define LED_DRIVE_50_mA 1
#define LED_DRIVE_25_mA 2
#define LED_DRIVE_12_5_mA 3

    //Led Boost levels
#define LED_BOOST_100 0
#define LED_BOOST_150 1
#define LED_BOOST_200 2
#define LED_BOOST_300 3

    //Proximity gain
#define PROXIMITY_GAIN_1X 0
#define PROXIMITY_GAIN_2X 1
#define PROXIMITY_GAIN_4X 2
#define PROXIMITY_GAIN_8X 3

    //ALS Gain
#define ALS_GAIN_1X 0
#define ALS_GAIN_4X 1
#define ALS_GAIN_16X 2
#define ALS_GAIN_64X 3

    //Gesture FIFO interrupt levels
#define FIFO_INT_AFTER_1_DATASET 0
#define FIFO_INT_AFTER_4_DATASETS 1
#define FIFO_INT_AFTER_8_DATASETS 2
#define FIFO_INT_AFTER_16_DATASETS 3

    //Gesture exit persistences
#define EXIT_AFTER_1_GESTURE_END 0
#define EXIT_AFTER_2_GESTURE_END 1
#define EXIT_AFTER_4_GESTURE_END 2
#define EXIT_AFTER_7_GESTURE_END 3

    //Gesture wait time
#define GESTURE_WAIT_0_MILLIS 0
#define GESTURE_WAIT_2_8_MILLIS 1
#define GESTURE_WAIT_5_6_MILLIS 2
#define GESTURE_WAIT_8_4_MILLIS 3
#define GESTURE_WAIT_14_MILLIS 4
#define GESTURE_WAIT_22_4_MILLIS 5
#define GESTURE_WAIT_30_8_MILLIS 6
#define GESTURE_WAIT_39_2_MILLIS 7

#define NO_ERROR 0
#define I2C_ERROR -1
#define INVALID_ARGUMENT -2

class Melopero_APDS9960 {

    public:
        uint8_t i2cAddress;
        uint8_t deviceStatus;
        uint8_t proximityData;

    public:
        Melopero_APDS9960(uint8_t i2cAddr = DEFAULT_I2C_ADDRESS);

    //=========================================================================
    //    I2C functions
    //=========================================================================

    int8_t read(uint8_t registerAddress, uint8_t* buffer, uint8_t amount);
        
    int8_t write(uint8_t registerAddress, uint8_t* values, uint8_t len);

    int8_t andOrRegister(uint8_t registerAddress, uint8_t andValue, uint8_t orValue);

    int8_t addressAccess(uint8_t registerAddress);

    //=========================================================================
    //    Device Methods
    //=========================================================================

    /*! @brief Toggles between IDLE and SLEEP state. In sleep state the device can still receive and process I2C messages.\n
     *  @param[in] wakeUP : Enter the IDLE state if True else enter SLEEP state, by default the value is True. 
     *  @return the status of the execution. */
    int8_t wakeUp(wakeUp = true);

    int8_t reset();

    /*! @brief calling this function resets also the Proximity and ALS interrupt settings. */
    int8_t enableAllEnginesAndPowerUp(bool enable = true);

    /*! @brief Sleep After Interrupt. When enabled, the device will automatically enter low power mode when the INT pin is asserted. 
     *  Normal operation is resumed when INT pin is cleared over I2C.*/
    int8_t setSleepAfterInterrupt(bool enable = true);
    
    /*! LED drive strength.\n
     * @param[in] ledDrive : must be one of LED_DRIVE_N_mA.*/
    int8_t setLedDrive(uint8_t ledDrive);

    /*! @brief The LED_BOOST allows the LDR pin to sink more current above the 
     * maximum setting. Additional LDR current during proximity and gesture 
     * LED pulses. Current value, set by LDRIVE, is increased by the 
     * percentage of LED_BOOST.\n
     * @param[in] led_boost must be one of LED_BOOST_N */
    int8_t setLedBoost(uint8_t ledBoost);

    /*! @brief Updates the status variable that contains status information. */
    int8_t updateStatus();

    // =========================================================================
    //     Proximity Engine Methods
    // =========================================================================

    int8_t enableProximityEngine(bool enable = true);

    int8_t enableProximityInterrupts(bool enable = true);

    int8_t enableProximitySaturationInterrupts(bool enable = true);

    int8_t clearProximityInterrupts();

    /*! Proximity Gain Control.\n
     * @param[in] prox_gain must be one of PROXIMITY_GAIN_NX. */
    int8_t setProximityGain(uint8_t proxGain);

    /*! The Proximity Interrupt Threshold sets the high and low trigger points
     *  for the comparison function which generates an interrupt. If the value 
     *  generated by the proximity channel, crosses below the lower threshold 
     *  or above the higher threshold, an interrupt may be signaled to the host 
     *  processor. Interrupt generation is subject to the value set in 
     *  persistence. \n
     *  @param[in] lowThr the low trigger point value.
     *  @param[in] highThr the high trigger point value. */ 
    int8_t setProximityInterruptThresholds(uint8_t lowThr, uint8_t highThr);

    /*! The Interrupt Persistence sets a value which is compared with the 
     *  accumulated amount Proximity cycles in which results were outside 
     *  threshold values. Any Proximity result that is inside threshold values 
     *  resets the count. \n
     *  @param[in] persistence: int in range [0-15].
     *      0 : an interrupt is triggered every cycle.
     *      N > 0 : an interrupt is triggered after N results over the threshold. */
    int8_t setProximityInterruptPersistence(uint8_t persistence);

    /*! The proximity pulse count is the number of pulses to be output on
     *  the LDR pin. The proximity pulse length is the amount of time the LDR 
     *  pin is sinking current during a proximity pulse.
     *  @param[in] pulseCount must be in range [1-64].
     *  @param[in] pulseLength must be one of PULSE_LEN_N_MICROS. */
    int8_t setProximityPulseCountAndLength(uint8_t pulseCount, uint8_t pulseLength);

    /*! In proximity mode, the UP and RIGHT and the DOWN and LEFT
     *  photodiodes are connected forming diode pairs. The offset is an 8-bit 
     *  value used to scale an internal offset correction factor to compensate 
     *  for crosstalk in the application.
     *  @param[in] upRightOffset the up-right pair offset.
     *  @param[in] downLeftOffset the down-left pair offset. */
    int8_t setProximityOffset(int8_t upRightOffset, int8_t downLeftOffset);

    /*! @brief Select which photodiodes are used for proximity.
     *  @param[in] mask_up if True disables the up photodiode.
     *  @param[in] mask_down if True disables the down photodiode.
     *  @param[in] mask_left if True disables the left photodiode.
     *  @param[in] mask_right if True disables the right photodiode.
     *  @param[in] proximity_gain_compensation provides gain compensation when proximity
     *      photodiode signal is reduced as a result of sensor masking. If only 
     *      one diode of the diode pair is contributing, then only half of the 
     *      signal is available at the ADC; this results in a maximum ADC value
     *      of 127. Enabling enables an additional gain of 2X, resulting in a 
     *      maximum ADC value of 255. */
    int8_t disablePhotodiodes(bool mask_up, bool mask_down, bool mask_left, bool mask_right, bool proximity_gain_compensation);
        
    int8_t updateProximityData();

};

#endif // Melopero_APDS9960_H_INCLUDED