#ifndef __TLE5012_H
#define __TLE5012_H

// Libraries
#include "Arduino.h"
#include "confg.h"

// Register locations (reading)
#define ENCODER_READ_COMMAND    0x8000 // 8000
#define ENCODER_STATUS_REG     (0x0000U) // Same as base
#define ENCODER_ANGLE_REG      (0x0020U)
#define ENCODER_SPEED_REG      (0x0030U)
#define ENCODER_ANGLE_REV_REG  (0x0040U)
#define ENCODER_TEMP_REG       (0x0050U)

// Register locations (writing)
#define ENCODER_WRITE_COMMAND   0x5000    // 5000
#define ENCODER_ACT_STATUS_REG (0x0010U)  // Activation status

// Calculation constants
#define POW_2_16                    65536.0   // 2^16
#define POW_2_15                    32768.0   // 2^15
#define POW_2_7                     128.0     // 2^7
#define DELETE_BIT_15               0x7FFF    // Used to delete everything except the first 15 bits
#define CHANGE_UINT_TO_INT_15       0x8000    // Used to change unsigned 16 bit integer into signed
#define CHECK_BIT_14                0x4000    // Used to check the 14th bit
#define GET_BIT_14_4                0x7FF0    // Used to check the 14th bit?
#define TEMP_OFFSET                 152.0     // Used to offset the temp reading
#define TEMP_DIV                    2.776     // Used to divide the temperature
#define DELETE_7_BITS               0x01FF    // Used to delete the first 7 bits of a 16 bit integer
#define CHANGE_UNIT_TO_INT_9        0x0200    // Used to change an unsigned 9 bit integer into signed
#define CHECK_BIT_9                 0x0100    // Used to check the 9th bit
#define RADS_IN_CIRCLE              6.28319


// Main address fields
enum Addr_t {
    REG_STAT         = (0x0000U),    //!< \brief STAT status register
    REG_ACSTAT       = (0x0010U),    //!< \brief ACSTAT activation status register
    REG_AVAL         = (0x0020U),    //!< \brief AVAL angle value register
    REG_ASPD         = (0x0030U),    //!< \brief ASPD angle speed register
    REG_AREV         = (0x0040U),    //!< \brief AREV angle revolution register
    REG_FSYNC        = (0x0050U),    //!< \brief FSYNC frame synchronization register
    REG_MOD_1        = (0x0060U),    //!< \brief MOD_1 interface mode1 register
    REG_SIL          = (0x0070U),    //!< \brief SIL register
    REG_MOD_2        = (0x0080U),    //!< \brief MOD_2 interface mode2 register
    REG_MOD_3        = (0x0090U),    //!< \brief MOD_3 interface mode3 register
    REG_OFFX         = (0x00A0U),    //!< \brief OFFX offset x
    REG_OFFY         = (0x00B0U),    //!< \brief OFFY offset y
    REG_SYNCH        = (0x00C0U),    //!< \brief SYNCH synchronicity
    REG_IFAB         = (0x00D0U),    //!< \brief IFAB register
    REG_MOD_4        = (0x00E0U),    //!< \brief MOD_4 interface mode4 register
    REG_TCO_Y        = (0x00F0U),    //!< \brief TCO_Y temperature coefficient register
    REG_ADC_X        = (0x0100U),    //!< \brief ADC_X ADC X-raw value
    REG_ADC_Y        = (0x0110U),    //!< \brief ADC_Y ADC Y-raw value
    REG_D_MAG        = (0x0140U),    //!< \brief D_MAG angle vector magnitude
    REG_T_RAW        = (0x0150U),    //!< \brief T_RAW temperature sensor raw-value
    REG_IIF_CNT      = (0x0200U),    //!< \brief IIF_CNT IIF counter value
    REG_T25O         = (0x0300U)     //!< \brief T25O temperature 25°c offset value
};


// Register access addresses
enum Access_t {
    REG_ACCESS_R    = (0x01U),      //!< \brief Read access register */
    REG_ACCESS_W    = (0x02U),      //!< \brief Write access register */
    REG_ACCESS_RW   = (0x03U),      //!< \brief Read & write access register */
    REG_ACCESS_U    = (0x04U),      //!< \brief Update register */
    REG_ACCESS_RU   = (0x05U),      //!< \brief Read & update register */
    REG_ACCESS_RWU  = (0x07U),      //!< \brief Read & write & update register */
    REG_ACCESS_RES  = (0x10U)       //!< \brief Reserved access register */
};

// Bitfield structure
typedef struct {
    uint8_t  regAccess;              //!< \brief Bitfield register access */
    uint16_t regAddress;             //!< \brief Bitfiled register address */
    uint16_t mask;                   //!< \brief Bitfield mask */
    uint8_t  position;               //!< \brief Bitfiled position */
    uint8_t  resetValue;             //!< \brief Bitfield register reset value */
    uint8_t  posMap;                 //!< \brief Bitfield position of register in regMap */
} BitField_t;

// Address field structure
typedef struct {
    uint16_t regAddress;            //!< \brief Addressfield register address */
    uint8_t  posMap;                //!< \brief Addressfield register regMap position */
} AddressField_t;

// Address field variable
static const AddressField_t addrFields[] =
{
	{REG_STAT,     1    },    //!< \brief STAT status register
	{REG_ACSTAT,   2    },    //!< \brief ACSTAT activation status register
	{REG_AVAL,     3    },    //!< \brief AVAL angle value register
	{REG_ASPD,     4    },    //!< \brief ASPD angle speed register
	{REG_AREV,     5    },    //!< \brief AREV angle revolution register
	{REG_FSYNC,    6    },    //!< \brief FSYNC frame synchronization register
	{REG_MOD_1,    7    },    //!< \brief MOD_1 interface mode1 register
	{REG_SIL,      8    },    //!< \brief SIL register
	{REG_MOD_2,    9    },    //!< \brief MOD_2 interface mode2 register
	{REG_MOD_3,   10    },    //!< \brief MOD_3 interface mode3 register
	{REG_OFFX,    11    },    //!< \brief OFFX offset x
	{REG_OFFY,    12    },    //!< \brief OFFY offset y
	{REG_SYNCH,   13    },    //!< \brief SYNCH synchronicity
	{REG_IFAB,    14    },    //!< \brief IFAB register
	{REG_MOD_4,   15    },    //!< \brief MOD_4 interface mode4 register
	{REG_TCO_Y,   16    },    //!< \brief TCO_Y temperature coefficient register
	{REG_ADC_X,   17    },    //!< \brief ADC_X ADC X-raw value
	{REG_ADC_Y,   18    },    //!< \brief ADC_Y ADC Y-raw value
	{REG_D_MAG,   19    },    //!< \brief D_MAG angle vector magnitude
	{REG_T_RAW,   20    },    //!< \brief T_RAW temperature sensor raw-value
	{REG_IIF_CNT, 21    },    //!< \brief IIF_CNT IIF counter value
	{REG_T25O,    22    },    //!< \brief T25O temperature 25°c offset value
};

#define MAX_NUM_REG 0x16      //!< \brief defines the value for temporary data to read all readable registers


// Bit fields
enum BitFieldReg_t
{
    REG_STAT_SRST,
    REG_STAT_SWD,
    REG_STAT_SVR,
    REG_STAT_SFUSE,
    REG_STAT_SDSPU,
    REG_STAT_SOV,
    REG_STAT_SXYOL,
    REG_STAT_SMAGOL,
    REG_STAT_RESERVED,
    REG_STAT_SADCT,
    REG_STAT_SROM,
    REG_STAT_NOGMRXY,
    REG_STAT_NOGMRA,
    REG_STAT_SNR,
    REG_STAT_RDST,

    REG_ACSTAT_ASRST,
    REG_ACSTAT_ASWD,
    REG_ACSTAT_ASVR,
    REG_ACSTAT_ASFUSE,
    REG_ACSTAT_ASDSPU,
    REG_ACSTAT_ASOV,
    REG_ACSTAT_ASVECXY,
    REG_ACSTAT_ASVEGMAG,
    REG_ACSTAT_RESERVED1,
    REG_ACSTAT_ASADCT,
    REG_ACSTAT_ASFRST,
    REG_ACSTAT_RESERVED2,

    REG_AVAL_ANGVAL,
    REG_AVAL_RDAV,

    REG_ASPD_ANGSPD,
    REG_ASPD_RDAS,

    REG_AREV_REVOL,
    REG_AREV_FCNT,
    REG_AREV_RDREV,

    REG_FSYNC_TEMPR,
    REG_FSYNC_FSYNC,

    REG_MOD_1_IIFMOD,
    REG_MOD_1_DSPUHOLD,
    REG_MOD_1_RESERVED1,
    REG_MOD_1_CLKSEL,
    REG_MOD_1_RESERVED2,
    REG_MOD_1_FIRMD,

    REG_SIL_ADCTVX,
    REG_SIL_ADCTVY,
    REG_SIL_ADCTVEN,
    REG_SIL_RESERVED1,
    REG_SIL_FUSEREL,
    REG_SIL_RESERVED2,
    REG_SIL_FILTINV,
    REG_SIL_FILTPAR,

    REG_MOD_2_AUTOCAL,
    REG_MOD_2_PREDICT,
    REG_MOD_2_ANGDIR,
    REG_MOD_2_ANGRANGE,
    REG_MOD_2_RESERVED1,

    REG_MOD_3_PADDRV,
    REG_MOD_3_SSCOD,
    REG_MOD_3_SPIKEF,
    REG_MOD_3_ANG_BASE,

    REG_OFFX_RESERVED1,
    REG_OFFX_XOFFSET,

    REG_OFFY_RESERVED1,
    REG_OFFY_YOFFSET,

    REG_SYNCH_RESERVED1,
    REG_SYNCH_SYNCH,

    REG_IFAB_IFADHYST,
    REG_IFAB_IFABOD,
    REG_IFAB_FIRUDR,
    REG_IFAB_ORTHO,

    REG_MOD_4_IFMD,
    REG_MOD_4_RESERVED1,
    REG_MOD_4_IFABRES,
    REG_MOD_4_HSMPLP,
    REG_MOD_4_TCOXT,

    REG_TCO_Y_CRCPAR,
    REG_TCO_Y_SBIST,
    REG_TCO_Y_TCOYT,

    REG_ADC_X_ADCX,

    REG_ADC_Y_ADCY,

    REG_D_MAG_MAG,
    REG_D_MAG_RESERVED1,

    REG_T_RAW_TRAW,
    REG_T_RAW_RESERVED1,
    REG_T_RAW_TTGL,

    REG_IIF_CNT_IIFCNT,
    REG_IIF_CNT_RESERVED1,

    REG_T25O_T250,
    REG_T25O_RESERVED1,
};

// Functions
void initEncoder();
double getAngle();
double getEncoderSpeed();

#endif
