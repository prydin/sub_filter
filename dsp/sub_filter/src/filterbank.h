
#ifndef FILTERBANK_H    
#define FILTERBANK_H

#include "filter_biquad_f.h"

#define NUM_STAGES 2

extern biquad_num_t filterBank[200 * NUM_STAGES * STAGE_COEFFICIENTS];
#endif