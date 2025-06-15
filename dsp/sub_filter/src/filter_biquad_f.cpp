#ifndef FILTER_BIQUAD_F_H
#define FILTER_BIQUAD_F_H

#include <Arduino.h>
#include "filter_biquad_f.h"

/*
typedef double REAL;
#typedef double REAL;
#define NBQ 2
REAL biquada[]={0.9891548679797922,-1.9889529784426023,0.9740167917470341,-1.9738164386508996};
REAL biquadb[]={1,2,1,2};
REAL gain=395557945.0626531;
REAL xyv[]={0,0,0,0,0,0,0,0,0};

REAL applyfilter(REAL v)
{
   int i,b,xp=0,yp=3,bqp=0;
   REAL out=v/gain;
   for (i=8; i>0; i--) {xyv[i]=xyv[i-1];}
   for (b=0; b<NBQ; b++)
   {
       int len=2;
       xyv[xp]=out;
       for(i=0; i<len; i++) { out+=xyv[xp+len-i]*biquadb[bqp+i]-xyv[yp+len-i]*biquada[bqp+i]; }
       bqp+=len;
       xyv[yp]=out;
       xp=yp; yp+=len+1;
   }
   return out;
}
*/

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
        *data = (int16_t) (y * 32768.0f);
    } 
    transmit(block);
    release(block);
}

#endif