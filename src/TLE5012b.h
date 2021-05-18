#ifndef __TLE5012B_H_
#define __TLE5012B_H_

#include "Arduino.h"
#include "SimpleFOC.h"
#include "s42b-2.0-pins.h"


// constants
#define CHANGE_UINT_TO_INT_15       0x8000    // Used to change unsigned 16 bit integer into signed
#define CHECK_BIT_14                0x4000    // Used to check the 14th bit
#define DELETE_BIT_15               0x7FFF    // Used to delete everything except the first 15 bits
#define ENCODER_ANGLE_REG      (0x0020U)
#define ENCODER_READ_COMMAND    0x8000 // 8000
#define ENCODER_SPEED_REG      (0x0030U)
#define GET_BIT_14_4                0x7FF0    // Used to check the 14th bit?
#define POW_2_7                     128.0     // 2^7
#define POW_2_15                    32768.0   // 2^15
#define RADS_IN_CIRCLE              6.28319

class TLE5012B: public Sensor{
    public:
        TLE5012B();
        void init();
        
        // Abstract functions of the Sensor class implementation
        /** get current angle (rad) */
        float getAngle() override;
        /**  get current angular velocity (rad/s) */
        float getVelocity() override; 

};

#endif // __TLE5012B_H_