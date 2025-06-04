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
#define MAX_DELAY 64  // ~4ms max delay = 64 samples at 16kHz

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
float32_t window[NUM_SAMPLES];
float32_t windowed_Signal_Left[NUM_SAMPLES];
float32_t windowed_Signal_Right[NUM_SAMPLES];


/**
 * @brief		FFT
 *
 * @param      	float32_t* Result
 *
 * @param		float32_t* input
 *
 * @return		void
 *
 * @details		Function that performs fft based on arm library
 *
 * @author		Francis Liechti (FL)
 * @date		27.05.2025	FL	Created
 *
 ****************************************************************************/
void performFFT(float *Result, float32_t *audiodata){
	// 1. Fill input
	for (int i = 0; i < FFT_SIZE; i++) {
		// Normalize if desired: value ∈ [-131072, +131071]
//		float normalized = ((float) audiodata[i]) / 131072.0f;

		input[2*i] = audiodata[i];      // Real part
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
void process_signal(double* amplitudes, int32_t* audioData_Left, int32_t* audioData_Right) {

	// Beamforming
	uint8_t direction = 0; //0 = dominating left, 1 = dominating right
	float scaleValue = 0; // for the non-dominating side
	applyWindow(audioData_Left, windowed_Signal_Left);
	applyWindow(audioData_Right, windowed_Signal_Right);

	// direction detection
	direction = beamform_direction(windowed_Signal_Left, windowed_Signal_Right, &scaleValue);
	// FFT
	if(direction == 1){
		performFFT(fftSignal, windowed_Signal_Left);
	} else {
		performFFT(fftSignal, windowed_Signal_Right);
	}

	for(int i=0; i<NUM_FREQ_BANDS; i++){
		amplitudeMeans[i] = mean(fftSignal, freqbands[i], freqbands[i+1]);
		// add some magical scaling
		amplitudeMeans[i] = amplitudeMeans[i]/2;
	}

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

	if(direction == 1){
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

/**
 * @brief		generateHammingWindow
 *
 * @param      	float32_t* amplitudes
 *
 * @param		uint32_t length
 *
 * @return		void
 *
 * @details		Function that generates a hamming window to apply on the signal (written by ChatGPT).
 *
 * @author		Francis Liechti (FL)
 * @date		03.06.2025	FL	Created
 *
 ****************************************************************************/
void generateHammingWindow(void) {
    for (uint32_t n = 0; n < NUM_SAMPLES; n++) {
        window[n] = 0.54f - 0.46f * arm_cos_f32((2.0f * PI * n) / (NUM_SAMPLES - 1));
    }
}

/**
 * @brief		generateHannWindow
 *
 * @param      	void
 *
 * @return		void
 *
 * @details		Function that generates a hanning window to apply on the signal (written by ChatGPT, adapted).
 *
 * @author		Francis Liechti (FL)
 * @date		03.06.2025	FL	Created
 *
 ****************************************************************************/
void generateHannWindow(void) {
    for (uint32_t n = 0; n < NUM_SAMPLES; n++) {
        window[n] = 0.5f * (1.0f - arm_cos_f32((2.0f * PI * n) / (NUM_SAMPLES - 1)));
    }
}

/**
 * @brief		applyWindow
 *
 * @param      	int32_t* input
 *
 * @param      	float32_t* output
 *
 * @return		void
 *
 * @details		Function that applies window on the signal (written by ChatGPT).
 *
 * @author		Francis Liechti (FL)
 * @date		03.06.2025	FL	Created
 *
 ****************************************************************************/
void applyWindow(int32_t *input, float32_t *output) {
    for (uint32_t i = 0; i < NUM_SAMPLES; i++) {
        output[i] = input[i] * window[i];
    }
}


/**
 * @brief		computeEnergy
 *
 * @param      	float32_t* signal
 *
 * @param		int size
 *
 * @return		float energy
 *
 * @details		Function that calculates energy of a signal (written by ChatGPT).
 *
 * @author		Francis Liechti (FL)
 * @date		04.06.2025	FL	Created
 *
 ****************************************************************************/
float compute_energy(float32_t* signal, int size) {
    float sum = 0.0f;
    for (int i = 0; i < size; ++i)
        sum += (float) (signal[i] * signal[i]);
    return sqrtf(sum / size);
}


/**
 * @brief		computeEnergy
 *
 * @param      	const float* left
 *
 * @param		const float* right
 *
 * @param		int size
 *
 * @param 		int max_delay
 *
 * @return		int delay
 *
 * @details		Function that estimates delay of two signals based on cross-correlation (written by ChatGPT).
 *
 * @author		Francis Liechti (FL)
 * @date		04.06.2025	FL	Created
 *
 ****************************************************************************/
int estimate_delay(const float* left, const float* right, int size, int max_delay) {
    float max_corr = -1.0f;
    int best_lag = 0;

    for (int lag = -max_delay; lag <= max_delay; ++lag) {
        float sum = 0.0f;
        for (int i = 0; i < size; ++i) {
            int j = i + lag;
            if (j >= 0 && j < size)
                sum += left[i] * right[j];
        }
        if (sum > max_corr) {
            max_corr = sum;
            best_lag = lag;
        }
    }

    return best_lag;  // Positive: right delayed → sound came from left
}

/**
 * @brief		beamform_direction
 *
 * @param      	float* left
 *
 * @param		float* right
 *
 * @param		float* scaleValue
 *
 * @param 		int max_delay
 *
 * @return		int direction
 *
 * @details		Function that estimates delay of two signals based on cross-correlation (written by ChatGPT but adapted).
 * 				Beamforming decision: returns +1 for left, -1 for right
 * 				returns the scaleValue on the other hand
 *
 * @author		Francis Liechti (FL)
 * @date		04.06.2025	FL	Created
 *
 ****************************************************************************/
int beamform_direction(float* left, float* right, float *scaleValue){

    float e_left = compute_energy(left, NUM_SAMPLES);
    float e_right = compute_energy(right, NUM_SAMPLES);
    int delay = estimate_delay(left, right, NUM_SAMPLES, MAX_DELAY);

    // ratio for scaleValue weighted from delay and energy
    float e_ratio = e_left / (e_left + e_right + 1e-6f);
	float d_ratio = fminf(fabsf(delay) / (float)MAX_DELAY, 1.0f);

	// Combine energy and delay factors, weighted average
	*scaleValue = (float) (0.5 * e_ratio + 0.5 * d_ratio);

    // Decide based on delay and energy
    if (abs(delay) > 4)  // ~0.25ms
        return (delay > 0) ? +1 : -1;  // left or right
    else
        return (e_left > e_right) ? +1 : -1;
}
