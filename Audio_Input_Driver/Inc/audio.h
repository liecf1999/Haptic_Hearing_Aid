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
#include <stdlib.h>

/*----- Defines -----------------------------------------------------*/

#define NUM_FREQ_BANDS 8
#define NUM_SAMPLES 512
#define NUM_SECTIONS 4  // For example, 4 biquad sections per filter

/*----- Data type -----------------------------------------------------*/


/*----- Function declarations -----------------------------------------------------*/
void filterData(uint16_t *RawValues, int16_t *FilteredValues, uint16_t samplenumber, uint16_t Max_entry);
void performFFT(float32_t *Result, float32_t *audiodata);
void process_signal(double* amplitudes, int32_t* audioData_Left, int32_t* audioData_Right, float* fft_signal, float* WindowedSignal);
float mean(float* signal, int startpoint, int endpoint);
void sortvalues(double* amplitudes, float *meanValues, uint8_t direction, float scaleValue);
void generateHammingWindow(void);
void generateHannWindow(void);
void applyWindow(int32_t *input, float32_t *output);
float compute_energy(float32_t* signal, int size);
int estimate_delay(const float* left, const float* right, int size, int max_delay);
int beamform_direction(float* left, float* right, float *scaleValue);
int estimate_delay_fast(const float32_t *left,
                        const float32_t *right,
                        int numSamples,
                        int maxDelay);
void delay_estimator_init(void);


#endif /* AUDIO_H_ */
