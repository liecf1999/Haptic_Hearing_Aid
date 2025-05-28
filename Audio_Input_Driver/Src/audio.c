/****************************************************************************/
/*  Source    : audio.c														*/
/****************************************************************************/
/*                                                                          */
/*  @file     : audio.c														*/
/*                                                                          */
/*  @brief	  : File to handle audio input									*/
/*                                                                          */
/*  @author   : Francis Liechti (FL)                                        */
/*                                                                          */
/*  @date	  : 11.03.2025  				                        		*/
/*                                                                          */
/****************************************************************************/

/*----- Header-Files -----------------------------------------------------*/
#include "audio.h"

/*----- Defines  ---------------------------------------------------------*/
#define TIMEOUT HAL_MAX_DELAY


#define FFT_SIZE 512

// Sample number boundaries of frequency bands
#define FREQ_BAND_0 8 // 250Hz
#define FREQ_BAND_1 13 // 406Hz (round up 377Hz)
#define FREQ_BAND_2 19 // 593Hz (round up 569Hz)
#define FREQ_BAND_3 28 // 875Hz (round up 857Hz)
#define FREQ_BAND_4 42 // 1325Hz (round up 1291Hz)
#define FREQ_BAND_5 63 // 1968Hz (round up 1944Hz)
#define FREQ_BAND_6 94 // 2937Hz (round up 2926Hz)
#define FREQ_BAND_7 141 // 4406Hz (round up 4402Hz)
#define FREQ_BAND_8 256 // 8000Hz


/*----- Data ---------------------------------------------------------*/

/*----- Private variables ---------------------------------------------------------*/
float fftSignal[NUM_SAMPLES];
float amplitudeMeans[NUM_FREQ_BANDS];

const int freqbands[NUM_FREQ_BANDS+1] = {FREQ_BAND_0, FREQ_BAND_1, FREQ_BAND_2, FREQ_BAND_3,
									FREQ_BAND_4, FREQ_BAND_5, FREQ_BAND_6, FREQ_BAND_7, FREQ_BAND_8
};

// Number of actuator mapped to its frequency
const int sort_frequencies_left[NUM_FREQ_BANDS] = {0, 1, 2, 3, 8, 9, 10, 11};
const int sort_frequencies_right[NUM_FREQ_BANDS] = {7, 6, 5, 4, 15, 14, 13, 12};

float32_t input[2 * FFT_SIZE];      // Interleaved real/imag for FFT


/**
 * @brief		FFT
 *
 * @param      	float32_t* Result
 *
 * @param		int32_t* input
 *
 * @return		void
 *
 * @details		Function that performs fft based on arm library
 *
 * @author		Francis Liechti (FL)
 * @date		27.05.2025	FL	Created
 *
 ****************************************************************************/
void performFFT(float *Result, int32_t *audiodata){
	// 1. Fill input
	for (int i = 0; i < FFT_SIZE; i++) {
		// Normalize if desired: value ∈ [-131072, +131071]
//		float normalized = ((float) audiodata[i]) / 131072.0f;
		float normalized = ((float) audiodata[i]);

		input[2*i] = normalized;      // Real part
		input[2*i + 1] = 0.0f;        // Imaginary part (0 for real signals)
	}

	// 2. Perform the complex FFT in-place
	arm_cfft_f32(&arm_cfft_sR_f32_len512, input, 0, 1);  // Forward FFT, with bit reversal

	// 3. Compute magnitude from real and imaginary parts
	for (int i = 0; i < FFT_SIZE; i++) {
		float real = input[2 * i];
		float imag = input[2 * i + 1];
		Result[i] = 20*log10f(sqrtf(real * real + imag * imag));
	}
}

/**
 * @brief		process_signal
 *
 * @param      	double* amplitudes
 *
 * @param		int32_t* audioData_Left
 *
 * @param		int32_t* audioData_Right
 *
 * @return		void
 *
 * @details		Function that performs fft based on arm library
 *
 * @author		Francis Liechti (FL)
 * @date		27.05.2025	FL	Created
 *
 ****************************************************************************/
void process_signal(double* amplitudes, int32_t* audioData_Left, int32_t* audioData_right) {

	// Beamforming
	uint8_t direction = 0; //0 = dominating left, 1 = dominating right
	float scaleValue = 0; // for the non-dominating side

	// FFT
	performFFT(fftSignal, audioData_Left);

	for(int i=0; i<NUM_FREQ_BANDS; i++){
		amplitudeMeans[i] = mean(fftSignal, freqbands[i], freqbands[i+1]);
	}

	// add some magical scaling

	// Set amplitudes
	sortvalues(amplitudes, amplitudeMeans, direction, scaleValue);

}


/**
 * @brief		mean
 *
 * @param      	float* signal
 *
 * @param		int startpoint
 *
 * @param		int endpoint
 *
 * @return		mean
 *
 * @details		Function that calculates mean of a signal between start and endpoint
 *
 * @author		Francis Liechti (FL)
 * @date		28.05.2025	FL	Created
 *
 ****************************************************************************/
float mean(float* signal, int startpoint, int endpoint){
	int size = endpoint-startpoint;
	float sum = 0;

	for(int i=startpoint; i<=endpoint; i++){
		sum += signal[i];
	}
	return (sum/size);
}

/**
 * @brief		sortvalues
 *
 * @param      	double* amplitudes
 *
 * @param		float* meanValues
 *
 * @param		uint8_t direction
 *
 * @param 		float scaleValue
 *
 * @return		void
 *
 * @details		Function that maps (and scales) the values on the specific actuator.
 *
 * @author		Francis Liechti (FL)
 * @date		28.05.2025	FL	Created
 *
 ****************************************************************************/
void sortvalues(double* amplitudes, float *meanValues, uint8_t direction, float scaleValue){

	if(direction == 0){
		for(int i=0; i<NUM_FREQ_BANDS; i++){
			amplitudes[sort_frequencies_left[i]] = (double) meanValues[i];
			amplitudes[sort_frequencies_right[i]] = (double) meanValues[i] * scaleValue;
		}
	} else {
		for(int i=0; i<NUM_FREQ_BANDS; i++){
			amplitudes[sort_frequencies_left[i]] = (double) meanValues[i]*scaleValue;
			amplitudes[sort_frequencies_right[i]] = (double) meanValues[i];
		}
	}
}
