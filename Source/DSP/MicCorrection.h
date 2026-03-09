#pragma once

struct MicCorrection
{
    float hpfFreqOffset        = 0.0f;
    float deessFreqOffset      = 0.0f;
    float deessThreshOffset    = 0.0f;
    float deessReductionOffset = 0.0f;
    float eqBand1GainOffset    = 0.0f;
    float eqBand2GainOffset    = 0.0f;
    float eqBand3GainOffset    = 0.0f;
    float eqBand4GainOffset    = 0.0f;
    float compRatioOffset      = 0.0f;
    float compAttackOffset     = 0.0f;
    float satDriveOffset       = 0.0f;
};
