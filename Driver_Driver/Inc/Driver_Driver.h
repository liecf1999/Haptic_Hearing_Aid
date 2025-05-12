/****************************************************************************/
/*  Header    : Driver_Driver												*/
/****************************************************************************/
/*                                                                          */
/*  @file     : driver_driver.h                                             */
/*                                                                          */
/*  @brief	  : header for LRA driver										*/
/*                                                                          */
/*  @author   : Francis Liechti (FL)                                        */
/*                                                                          */
/*  @date	  : 11.03.2025	  	FL	Created                         		*/
/*                                                                          */
/*																			*/
/****************************************************************************/
/*																			*/
/****************************************************************************/

#ifndef DRIVER_DRIVER_H_
#define DRIVER_DRIVER_H_

/*----- Header-Files -----------------------------------------------------*/
#include "main.h"
#include "i2c.h"

/*----- Defines -----------------------------------------------------*/
#define DEV_ADDRESS 0x5A << 1

/*----- Data type -----------------------------------------------------*/


/*----- Function declarations -----------------------------------------------------*/
void init_drivers(void);
void set_amplitude(double *amplitudes);
void select_driver(uint8_t number);


#endif /* DRIVER_DRIVER_H_ */
