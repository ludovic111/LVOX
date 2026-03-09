#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

namespace ParamIDs
{
    // Global
    inline constexpr const char* globalInput  = "global_input";
    inline constexpr const char* globalOutput = "global_output";

    // Noise Gate
    inline constexpr const char* gateBypass    = "gate_bypass";
    inline constexpr const char* gateThreshold = "gate_threshold";
    inline constexpr const char* gateRatio     = "gate_ratio";
    inline constexpr const char* gateAttack    = "gate_attack";
    inline constexpr const char* gateRelease   = "gate_release";
    inline constexpr const char* gateRange     = "gate_range";

    // High-Pass Filter
    inline constexpr const char* hpfBypass    = "hpf_bypass";
    inline constexpr const char* hpfFrequency = "hpf_frequency";
    inline constexpr const char* hpfSlope     = "hpf_slope";

    // De-Esser
    inline constexpr const char* deessBypass    = "deess_bypass";
    inline constexpr const char* deessFrequency = "deess_frequency";
    inline constexpr const char* deessThreshold = "deess_threshold";
    inline constexpr const char* deessReduction = "deess_reduction";
    inline constexpr const char* deessAttack    = "deess_attack";
    inline constexpr const char* deessRelease   = "deess_release";
    inline constexpr const char* deessListen    = "deess_listen";

    // Parametric EQ
    inline constexpr const char* eqBypass = "eq_bypass";
    // Band 1
    inline constexpr const char* eqBand1Freq   = "eq_band1_freq";
    inline constexpr const char* eqBand1Gain   = "eq_band1_gain";
    inline constexpr const char* eqBand1Q      = "eq_band1_q";
    inline constexpr const char* eqBand1Type   = "eq_band1_type";
    inline constexpr const char* eqBand1Active = "eq_band1_active";
    // Band 2
    inline constexpr const char* eqBand2Freq   = "eq_band2_freq";
    inline constexpr const char* eqBand2Gain   = "eq_band2_gain";
    inline constexpr const char* eqBand2Q      = "eq_band2_q";
    inline constexpr const char* eqBand2Type   = "eq_band2_type";
    inline constexpr const char* eqBand2Active = "eq_band2_active";
    // Band 3
    inline constexpr const char* eqBand3Freq   = "eq_band3_freq";
    inline constexpr const char* eqBand3Gain   = "eq_band3_gain";
    inline constexpr const char* eqBand3Q      = "eq_band3_q";
    inline constexpr const char* eqBand3Type   = "eq_band3_type";
    inline constexpr const char* eqBand3Active = "eq_band3_active";
    // Band 4
    inline constexpr const char* eqBand4Freq   = "eq_band4_freq";
    inline constexpr const char* eqBand4Gain   = "eq_band4_gain";
    inline constexpr const char* eqBand4Q      = "eq_band4_q";
    inline constexpr const char* eqBand4Type   = "eq_band4_type";
    inline constexpr const char* eqBand4Active = "eq_band4_active";

    // Compressor
    inline constexpr const char* compBypass    = "comp_bypass";
    inline constexpr const char* compThreshold = "comp_threshold";
    inline constexpr const char* compRatio     = "comp_ratio";
    inline constexpr const char* compAttack    = "comp_attack";
    inline constexpr const char* compRelease   = "comp_release";
    inline constexpr const char* compKnee      = "comp_knee";
    inline constexpr const char* compMakeup    = "comp_makeup";
    inline constexpr const char* compScFreq    = "comp_sc_freq";
    inline constexpr const char* compScActive  = "comp_sc_active";
    inline constexpr const char* compMix       = "comp_mix";

    // Saturation
    inline constexpr const char* satBypass = "sat_bypass";
    inline constexpr const char* satDrive  = "sat_drive";
    inline constexpr const char* satType   = "sat_type";
    inline constexpr const char* satTone   = "sat_tone";
    inline constexpr const char* satMix    = "sat_mix";
    inline constexpr const char* satOutput = "sat_output";

    // Reverb
    inline constexpr const char* revBypass   = "rev_bypass";
    inline constexpr const char* revSize     = "rev_size";
    inline constexpr const char* revDamping  = "rev_damping";
    inline constexpr const char* revPredelay = "rev_predelay";
    inline constexpr const char* revLowCut   = "rev_lowcut";
    inline constexpr const char* revHighCut  = "rev_highcut";
    inline constexpr const char* revMix      = "rev_mix";

    // Delay
    inline constexpr const char* dlyBypass   = "dly_bypass";
    inline constexpr const char* dlyTime     = "dly_time";
    inline constexpr const char* dlySync     = "dly_sync";
    inline constexpr const char* dlyNote     = "dly_note";
    inline constexpr const char* dlyFeedback = "dly_feedback";
    inline constexpr const char* dlyLowCut   = "dly_lowcut";
    inline constexpr const char* dlyHighCut  = "dly_highcut";
    inline constexpr const char* dlyMix      = "dly_mix";

    // Limiter
    inline constexpr const char* limBypass  = "lim_bypass";
    inline constexpr const char* limCeiling = "lim_ceiling";
    inline constexpr const char* limRelease = "lim_release";

    // Send/Bus
    inline constexpr const char* sendMode     = "send_mode";
    inline constexpr const char* sendRevLevel = "send_rev_level";
    inline constexpr const char* sendDlyLevel = "send_dly_level";

    // Mic Select
    inline constexpr const char* micSelect = "mic_select";

    // Macro (Simple Mode)
    inline constexpr const char* macroWarmth      = "macro_warmth";
    inline constexpr const char* macroPresence    = "macro_presence";
    inline constexpr const char* macroCompression = "macro_compression";
    inline constexpr const char* macroSpace       = "macro_space";
}

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
