#ifndef MACRO_SAMPLER_STATES_H
#define MACRO_SAMPLER_STATES_H

#include"../HLSLMacro//SamplerStateNames.h"

// ------------------------- SamplerState用のマクロをここに記述する -------------------------

#define LinerClampSampler SamplerState SS_LINER_CLAMP : register(s0);

#endif // MACRO_SAMPLER_STATES_H