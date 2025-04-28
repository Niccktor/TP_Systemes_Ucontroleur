/*
 * triangle.c
 *
 *  Created on: Mar 13, 2025
 *      Author: BEGUIN
 */
#include <stdio.h>
#include <stdint.h>
#include "sai.h"

extern SAI_HandleTypeDef hsai_BlockA2;

#define TRIANGLE_SAMPLES 96
#define AUDIO_BUFFER_SIZE (TRIANGLE_SAMPLES * 2)

int16_t triangle_wave[TRIANGLE_SAMPLES];
int16_t audio_buffer[AUDIO_BUFFER_SIZE]; // L-R-L-R

void toothwave() {
	for (int i = 0; i < TRIANGLE_SAMPLES; i++) triangle_wave[i] = i* 0xFFFFU/TRIANGLE_SAMPLES;
}

void GenerateTriangleWave(void) {
    for (int i = 0; i < TRIANGLE_SAMPLES / 2; i++) {
        triangle_wave[i] = (i * 65535 / (TRIANGLE_SAMPLES / 2)) - 32768;
    }
    for (int i = TRIANGLE_SAMPLES / 2; i < TRIANGLE_SAMPLES; i++) {
        triangle_wave[i] = ((TRIANGLE_SAMPLES - i) * 65535 / (TRIANGLE_SAMPLES / 2)) - 32768;
    }
}

void FillAudioBufferStereo(void) {
    for (int i = 0; i < TRIANGLE_SAMPLES; i++) {
        audio_buffer[2 * i]     = triangle_wave[i]; // Gauche
        audio_buffer[2 * i + 1] = triangle_wave[i]; // Droite
    }
}

void StartTrianglePlayback_SAI(void) {
    GenerateTriangleWave();
	//toothwave();
    FillAudioBufferStereo();


    HAL_SAI_Transmit_DMA(&hsai_BlockA2, (uint8_t *)audio_buffer, AUDIO_BUFFER_SIZE /2);
}
void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai) {
    // modulation dynamique si besoin
}
