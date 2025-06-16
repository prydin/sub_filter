#ifndef FILTER_BIQUAD_F_H
#define FILTER_BIQUAD_F_H

#include <Arduino.h>
#include "filter_biquad_f.h"

void AudioFilterBiquadFloat::update(void)
{
    audio_block_t *block;
    block = receiveWritable();
    if (!block)
    {
        return;
    }
    if (num_stages == 0)
    {
        // No stages defined, just pass through the input
        transmit(block);
        release(block);
        return;
    }

    int16_t *data = block->data;
    int16_t *end = block->data + AUDIO_BLOCK_SAMPLES;
    for(; data < end; data++)
    {
        biquad_num_t x = ((biquad_num_t) *data) / 32768.0f;
        biquad_num_t y = 0.0f;

        // Second direct form
        for(uint32_t i = 0; i < num_stages; i++)
        {
            biquad_num_t *my_coeff = coeff + i * STAGE_COEFFICIENTS;
            biquad_num_t *my_state = state + i * 2;
            biquad_num_t w = x - my_state[0] * my_coeff[3] - my_state[1] * my_coeff[4];
            y = w * my_coeff[0] + my_state[0] * my_coeff[1] + my_state[1] * my_coeff[2];
            state[i * 2 + 1] = state[i * 2];
            state[i * 2] = w;
            x = y;
        }
        *data = (int16_t) (max(-1.0f, min(1.0f, min(y, 1.0f))) * 32768.0f);
    } 
    transmit(block);
    release(block);
}

#endif