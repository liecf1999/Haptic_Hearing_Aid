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

/*----- Defines -----------------------------------------------------*/
#define DEV_ADDRESS 0x57 << 1
#define REG_FIFO_DATA 0x05

/*----- Data type -----------------------------------------------------*/


/*----- Function declarations -----------------------------------------------------*/
void filterData(uint16_t *RawValues, int16_t *FilteredValues, uint16_t samplenumber, uint16_t Max_entry);
void performFFT(float32_t *Result, int32_t *audiodata);



#endif /* AUDIO_H_ */
