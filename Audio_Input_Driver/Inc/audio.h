/****************************************************************************/
/*  Header    : audio														*/
/****************************************************************************/
/*                                                                          */
/*  @file     : audio.h		                                                */
/*                                                                          */
/*  @brief	  : header for audio input										*/
/*                                                                          */
/*  @author   : Francis Liechti (FL)                                        */
/*                                                                          */
/*  @date	  : 11.03.2025	  	FL	Created                         		*/
/*                                                                          */
/*																			*/
/****************************************************************************/
/*																			*/
/****************************************************************************/

#ifndef AUDIO_H_
#define AUDIO_H_

/*----- Header-Files -----------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "arm_math.h"
#include "arm_const_structs.h"

/*----- Defines -----------------------------------------------------*/

#define NUM_FREQ_BANDS 8
#define NUM_SAMPLES 512
#define NUM_SECTIONS 4  // For example, 4 biquad sections per filter

/*----- Data type -----------------------------------------------------*/


/*----- Function declarations -----------------------------------------------------*/
void filterData(uint16_t *RawValues, int16_t *FilteredValues, uint16_t samplenumber, uint16_t Max_entry);
void performFFT(float32_t *Result, int32_t *audiodata);
void process_signal(double* amplitudes, int32_t* audioData_Left, int32_t* audioData_right);
float mean(float* signal, int startpoint, int endpoint);
void sortvalues(double* amplitudes, float *meanValues, uint8_t direction, float scaleValue);


#endif /* AUDIO_H_ */
