//Author: Leonardo La Rocca

#include "Melopero_APDS9960.h"

Melopero_APDS9960::Melopero_APDS9960(uint8_t i2cAddr){
    i2cAddress = i2cAddr;
}

//=========================================================================
//    I2C functions
//=========================================================================

int8_t Melopero_APDS9960::read(uint8_t registerAddress, uint8_t* buffer, uint8_t amount){
    Wire.beginTransmission(i2cAddress);
    Wire.write(registerAddress);
    uint8_t i2cStatus = Wire.endTransmission();
    if (i2cStatus != 0) return I2C_ERROR;

    uint32_t dataIndex = 0;
    do {
        uint8_t request = amount > 32 ? 32 : amount;
        Wire.requestFrom(i2cAddress, request);
        for (uint8_t i = 0; i < request; i++){
            if (Wire.available()){
                buffer[dataIndex] = Wire.read();
                dataIndex++;
                amount--;
            }
            else {
                return I2C_ERROR;
            }
        }
    }
    while (amount > 0);
    return NO_ERROR;
}
    
int8_t Melopero_APDS9960::write(uint8_t registerAddress, uint8_t* values, uint8_t len){
    Wire.beginTransmission(i2cAddress);
    Wire.write(registerAddress);
    Wire.write(values, len);
    uint8_t i2cStatus = Wire.endTransmission();
    if (i2cStatus != 0)
        return I2C_ERROR;
    else 
        return NO_ERROR;
}

int8_t Melopero_APDS9960::andOrRegister(uint8_t registerAddress, uint8_t andValue, uint8_t orValue){
    uint8_t value = 0;
    int8_t status = read(registerAddress, &value, 1);
    if (status != NO_ERROR) return status;
    value &= andValue;
    value |= orValue;
    return write(registerAddress, &value, 1);
}

int8_t Melopero_APDS9960::addressAccess(uint8_t registerAddress){
    Wire.beginTransmission(i2cAddress);
    Wire.write(registerAddress);
    uint8_t i2cStatus = Wire.endTransmission();
    if (i2cStatus != 0)
        return I2C_ERROR;
    else 
        return NO_ERROR;
}

// =========================================================================
//     Device Methods
// =========================================================================

int8_t wakeUp(bool wakeUp){
    int8_t status = NO_ERROR;
    status = andOrRegister(ENABLE_REG_ADDRESS, ((uint8_t) wakeUp) | 0xFE, (uint8_t) wakeUp);
    delay(10);
    return status;
}

int8_t Melopero_APDS9960::reset();

int8_t Melopero_APDS9960::enableAllEnginesAndPowerUp(bool enable){
    uint8_t value = enable ? 0b01001111 : 0;
    int8_t status = write(ENABLE_REG_ADDRESS, &value, 1);
    delay(10);
    return status;
}

int8_t Melopero_APDS9960::setSleepAfterInterrupt(bool enable){
    int8_t status = NO_ERROR;
    status = andOrRegister(CONFIG_3_REG_ADDRESS, (enable << 4) | 0xEF, enable << 4);
    return status;
}

int8_t Melopero_APDS9960::setLedDrive(uint8_t ledDrive){
    if (!(LED_DRIVE_100_mA <= ledDrive && ledDrive <= APDS_9960.LED_DRIVE_12_5_mA))
        return INVALID_ARGUMENT;

    return andOrRegister(CONTROL_1_REG_ADDRESS, (ledDrive << 6) | 0x3F, ledDrive << 6));
}

int8_t Melopero_APDS9960::setLedBoost(uint8_t ledBoost){
        if (!(LED_BOOST_100 <= ledBoost && ledBoost <= LED_BOOST_300))
            return INVALID_ARGUMENT;

        return andOrRegister(CONFIG_2_REG_ADDRESS, (ledBoost << 4) | 0xCF, (ledBoost << 4));
}

int8_t Melopero_APDS9960::updateStatus(){
    // TODO: add simple way to retrieve status flags:
    // Clear photodiode saturation (0x80) | Proximity/Gesture saturation (0x40) | Proximity interrupt (0x20) | ALS Interrupt (0x10)
    // | Gesture Interrupt (0x04) | Proximity valid (0x02) | ALS valid (0x01)
    return read(STATUS_REG_ADDRESS, &deviceStatus, 1);
}

// =========================================================================
//     Proximity Engine Methods
// =========================================================================

int8_t Melopero_APDS960::enableProximityEngine(bool enable){
    return andOrRegister(ENABLE_REG_ADDRESS, (enable << 2) | 0xFB, enable << 2);
}

int8_t Melopero_APDS9960::enableProximityInterrupts(bool enable){
    return andOrRegister(ENABLE_REG_ADDRESS, (enable << 5) | 0xDF, enable << 5);
}

int8_t Melopero_APDS9960::enableProximitySaturationInterrupts(bool enable){
    return andOrRegister(CONFIG_2_REG_ADDRESS, (enable << 7) | 0x7F, enable << 7);
}

// Interrupts are cleared by “address accessing” the appropriate register. This is special I2C transaction
// consisting of only two bytes: chip address with R/W = 0, followed by a register address.
int8_t Melopero_APDS9960::clearProximityInterrupts(){ 
    return addressAccess(PROXIMITY_INT_CLEAR_REG_ADDRESS);
}

int8_t Melopero_APDS9960::setProximityGain(uint8_t proxGain){
    if (!(PROXIMITY_GAIN_1X <= proxGain && proxGain <= PROXIMITY_GAIN_8X)
        return INVALID_ARGUMENT;

    return andOrRegister(CONTROL_1_REG_ADDRESS, (proxGain << 2) | 0xF3, proxGain << 2);
}

int8_t Melopero_APDS9960::setProximityInterruptThresholds(uint8_t lowThr, uint8_t highThr){
    int8_t status = NO_ERROR;
    status = write(PROX_INT_LOW_THR_REG_ADDRESS, &lowThr, 1);
    if (status != NO_ERROR) return status;
    status = write(PROX_INT_HIGH_THR_REG_ADDRESS, &highThr, 1);
    return status;
}

int8_t Melopero_APDS9960::setProximityInterruptPersistence(uint8_t persistence){
    if (!(0 <= persistence && persistence <= 15))
        return INVALID_ARGUMENT;

    return andOrRegister(INTERRUPT_PERSISTANCE_REG_ADDRESS, (persistence << 4) | 0x0F, persistence << 4);
}

int8_t Melopero_APDS9960::setProximityPulseCountAndLength(uint8_t pulseCount, uint8_t pulseLength){
    if (!(1 <= pulseCount <= 64))
        return INVALID_ARGUMENT;
    if (!(PULSE_LEN_4_MICROS <= pulseLength <= PULSE_LEN_32_MICROS))
        return INVALID_ARGUMENT;       

    uint8_t regValue = pulseLength << 6;
    regValue |= pulseCount - 1
    return write(PROX_PULSE_COUNT_REG_ADDRESS, &regValue, 1);
}

int8_t Melopero_APDS9960::setProximityOffset(int8_t upRightOffset, int8_t downLeftOffset){
    int8_t status = NO_ERROR;
    uint8_t upRightRegValue = upRightOffset < 0 ? 0x80 | ((uint8_t) (-upRightOffset)) : (uint8_t) upRightOffset;
    uint8_t downLeftRegValue = downLeftOffset < 0 ? 0x80 | ((uint8_t) (-downLeftOffset)) : (uint8_t) downLeftOffset;

    status = write(PROX_UP_RIGHT_OFFSET_REG_ADDRESS, &upRightRegValue, 1);
    if (status != NO_ERROR) return status;
    status = write(PROX_DOWN_LEFT_OFFSET_REG_ADDRESS, &downLeftRegValue, 1);
    return status;
}

int8_t Melopero_APDS9960::disablePhotodiodes(bool mask_up, bool mask_down, bool mask_left, bool mask_right, bool proximity_gain_compensation){
    uint8_t and_flag = 0xD0 | (proximity_gain_compensation << 5) | (mask_up << 3) | (mask_down << 2) | (mask_left << 1) | ((uint8_t) mask_right);
    uint8_t or_flag = (proximity_gain_compensation << 5) | (mask_up << 3) | (mask_down << 2) | (mask_left << 1) | ((uint8_t) mask_right);

    return andOrRegister(CONFIG_3_REG_ADDRESS, and_flag, or_flag);
}

int8_t Melopero_APDS9960::updateProximityData(){
    return read(PROX_DATA_REG_ADDRESS, &proximityData, 1);
}