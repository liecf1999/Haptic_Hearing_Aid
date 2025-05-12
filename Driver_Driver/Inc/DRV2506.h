/*
 * DRV2506.h
 *
 *  Created on: March 11, 2024
 *      Author: Francis Liechti
 *      File that contains all definitions of the DRV2506 driver
 */

#ifndef INC_DRV2506_H_
#define INC_DRV2506_H_

// Register definitions
#define REG_STATUS 0x00
#define REG_MODE 0x01
#define REG_REAL_TIME_PLAYBACK_INPUT 0x02 //Real Time Playback Input
#define REG_LIBRARY 0x03
#define REG_WF_SEQ_1 0x04 // Waveform Sequencer, from register 0x04 to 0x0B
#define REG_WF_SEQ_2 0x05
#define REG_WF_SEQ_3 0x06
#define REG_WF_SEQ_4 0x07
#define REG_WF_SEQ_5 0x08
#define REG_WF_SEQ_6 0x09
#define REG_WF_SEQ_7 0x0A
#define REG_WF_SEQ_8 0x0B
#define REG_GO 0x0C
#define REG_OVERDRIVE_TIME_OFFSET 0x0D
#define REG_SUSTAIN_TIME_OFFSET_POSTIVIE 0x0E
#define REG_SUSTAIN_TIME_OFFSET_NEGATIVE 0x0F
#define REG_BREAK_TIME_OFFSET 0x10
#define REG_AUDIO_VIBE_CTRL 0x11 // Control
#define REG_AUDIO_VIBE_MIN_IL 0x12 // Minimum Input Level
#define REG_AUDIO_VIBE_MAX_IL 0x13 // Maximum Input Level
#define REG_AUDIO_VIBE_MIN_ODR 0x14 // Minimum Output Drive Register
#define REG_AUDIO_VIBE_MAX_ODR 0x15 // Maximum Output Drive Register
#define REG_RATED_VOLTAGE 0x16
#define REG_OVERDRIVE_CLAMP_VOLTAGE 0x17
#define REG_AUTO_CALIBRATION_RESULT 0x18
#define REG_AUTO_CALIBRATION_BACK_EMF_RESULT 0x19
#define REG_FEEDBACK_CTRL 0x1A
#define REG_CONTROL1 0x1B
#define REG_CONTROL2 0x1C
#define REG_CONTROL3 0x1D
#define REG_CONTROL4 0x1E
#define REG_CONTROL5 0x1F
#define REG_LRA_OPEN_LOOP_PERIOD 0x20
#define REG_VOLTAGE_MONITOR 0x21
#define REG_LRA_RESONANCE_PERIOD 0x22


// Status Declarations
#define STATUS_OCDETECT 0x01
#define STATUS_OVERTEMP 0x02
#define STATUS_DIAG_RESULT 0x08
#define STATUS_ID_0 0x20
#define STATUS_ID_1 0x40
#define STATUS_ID_2 0x80

// Mode Declarations
#define MODE_INTERNAL_TRIGGER 0x00
#define MODE_EDGE 0x01 // External Trigger (Edge Mode)
#define MODE_LEVEL 0x02 // External Trigger (Level Mode)
#define MODE_PWM 0x03 // PWM mode and analog mode
#define MODE_AUDIO_VIBE 0x04
#define MODE_RTP 0x05 // Real time playback
#define MODE_DIAGNOSTICS 0x06
#define MODE_AUTO_CALIBRATION 0x07
#define MODE_STANDBY 0x40
#define MODE_RESET 0x80

#define DRIVE_TIME 0x33 // see calculation data sheet (p.44)
#define STARTUP_BOOST 0x80

// Feedback Control
#define MODE_LRA 0x80
#define FB_BRAKE_FACTOR 0x30
#define LOOP_GAIN 0x04
#define BACK_EMF_GAIN 0x02

// Control 2
#define BIDIR_INPUT 0x80
#define BRAKE_STABILIZER 0x40
#define SAMPLE_TIME_300 0x30
#define SAMPLE_TIME_250 0x20
#define SAMPLE_TIME_200 0x10
#define SAMPLE_TIME_150 0x00
#define BLANKING_TIME_75 0x04
#define IDISS_TIME_75 0x01
// Control 3
#define AUTO_CALIBRATION 0x00
#define LRA_OPEN_LOOP 0x01
#define DATA_FORMAT_RTP_UNSIGNED 0x04
#define NG_THRESHOLD 0x40

// Control 5
#define LRA_AUTO_OPEN_LOOP 0x20
// Clamp (maximum possible peak voltage no matter which voltage is set)
//#define CLAMP 0x3D // corresponds to 1.5V (see data sheet (p.22)) (Library Mode)
#define CLAMP 0x9C // corresponds to 3.3V (see data sheet (p.22)) (Library Mode new actuator)

// Rated Voltage
//#define RATED_VOLTAGE 0x1A
#define RATED_VOLTAGE 0x5B

// LRA Open Loop Period
#define LRA_PERIOD 0x39









#endif INC_DRV2506_H_
