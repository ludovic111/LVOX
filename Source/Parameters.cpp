#include "Parameters.h"

using namespace juce;

static NormalisableRange<float> makeLogRange (float min, float max)
{
    return { min, max, 0.0f, std::log (0.5f) / std::log ((500.0f - min) / (max - min)) };
}

static NormalisableRange<float> makeFreqRange (float min, float max)
{
    return { min, max, 1.0f, 0.25f };
}

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<AudioProcessorParameterGroup>> groups;

    // ---- Global ----
    {
        auto g = std::make_unique<AudioProcessorParameterGroup> ("global", "Global", "|");
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::globalInput, 1 },  "Input Gain",  NormalisableRange<float> (-24.0f, 24.0f, 0.1f), 0.0f, AudioParameterFloatAttributes().withLabel ("dB")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::globalOutput, 1 }, "Output Gain", NormalisableRange<float> (-24.0f, 24.0f, 0.1f), 0.0f, AudioParameterFloatAttributes().withLabel ("dB")));
        groups.push_back (std::move (g));
    }

    // ---- Noise Gate ----
    {
        auto g = std::make_unique<AudioProcessorParameterGroup> ("gate", "Noise Gate", "|");
        g->addChild (std::make_unique<AudioParameterBool>  (ParameterID { ParamIDs::gateBypass, 1 },    "Gate Bypass", false));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::gateThreshold, 1 }, "Gate Threshold", NormalisableRange<float> (-80.0f, 0.0f, 0.1f), -40.0f, AudioParameterFloatAttributes().withLabel ("dB")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::gateRatio, 1 },     "Gate Ratio",     NormalisableRange<float> (1.0f, 100.0f, 0.1f, 0.5f), 10.0f));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::gateAttack, 1 },    "Gate Attack",    NormalisableRange<float> (0.1f, 100.0f, 0.1f, 0.4f), 5.0f, AudioParameterFloatAttributes().withLabel ("ms")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::gateRelease, 1 },   "Gate Release",   NormalisableRange<float> (1.0f, 500.0f, 0.1f, 0.5f), 50.0f, AudioParameterFloatAttributes().withLabel ("ms")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::gateRange, 1 },     "Gate Range",     NormalisableRange<float> (-80.0f, 0.0f, 0.1f), -80.0f, AudioParameterFloatAttributes().withLabel ("dB")));
        groups.push_back (std::move (g));
    }

    // ---- High-Pass Filter ----
    {
        auto g = std::make_unique<AudioProcessorParameterGroup> ("hpf", "High-Pass Filter", "|");
        g->addChild (std::make_unique<AudioParameterBool>  (ParameterID { ParamIDs::hpfBypass, 1 },    "HPF Bypass", false));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::hpfFrequency, 1 }, "HPF Frequency", makeFreqRange (20.0f, 500.0f), 80.0f, AudioParameterFloatAttributes().withLabel ("Hz")));
        g->addChild (std::make_unique<AudioParameterChoice>(ParameterID { ParamIDs::hpfSlope, 1 },     "HPF Slope", StringArray { "12 dB/oct", "24 dB/oct", "48 dB/oct" }, 1));
        groups.push_back (std::move (g));
    }

    // ---- De-Esser ----
    {
        auto g = std::make_unique<AudioProcessorParameterGroup> ("deesser", "De-Esser", "|");
        g->addChild (std::make_unique<AudioParameterBool>  (ParameterID { ParamIDs::deessBypass, 1 },    "De-Esser Bypass", false));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::deessFrequency, 1 }, "De-Esser Freq", makeFreqRange (2000.0f, 12000.0f), 6500.0f, AudioParameterFloatAttributes().withLabel ("Hz")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::deessThreshold, 1 }, "De-Esser Threshold", NormalisableRange<float> (-40.0f, 0.0f, 0.1f), -20.0f, AudioParameterFloatAttributes().withLabel ("dB")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::deessReduction, 1 }, "De-Esser Reduction", NormalisableRange<float> (0.0f, 24.0f, 0.1f), 6.0f, AudioParameterFloatAttributes().withLabel ("dB")));
        g->addChild (std::make_unique<AudioParameterBool>  (ParameterID { ParamIDs::deessListen, 1 },    "De-Esser Listen", false));
        groups.push_back (std::move (g));
    }

    // ---- Parametric EQ ----
    {
        auto g = std::make_unique<AudioProcessorParameterGroup> ("eq", "Parametric EQ", "|");
        g->addChild (std::make_unique<AudioParameterBool> (ParameterID { ParamIDs::eqBypass, 1 }, "EQ Bypass", false));

        struct BandDefaults { const char* freqID; const char* gainID; const char* qID; const char* typeID; const char* activeID; float defaultFreq; int defaultType; };
        BandDefaults bands[] = {
            { ParamIDs::eqBand1Freq, ParamIDs::eqBand1Gain, ParamIDs::eqBand1Q, ParamIDs::eqBand1Type, ParamIDs::eqBand1Active, 100.0f,  1 },
            { ParamIDs::eqBand2Freq, ParamIDs::eqBand2Gain, ParamIDs::eqBand2Q, ParamIDs::eqBand2Type, ParamIDs::eqBand2Active, 500.0f,  0 },
            { ParamIDs::eqBand3Freq, ParamIDs::eqBand3Gain, ParamIDs::eqBand3Q, ParamIDs::eqBand3Type, ParamIDs::eqBand3Active, 3000.0f, 0 },
            { ParamIDs::eqBand4Freq, ParamIDs::eqBand4Gain, ParamIDs::eqBand4Q, ParamIDs::eqBand4Type, ParamIDs::eqBand4Active, 8000.0f, 2 },
        };

        StringArray filterTypes { "Peak", "Low Shelf", "High Shelf" };

        for (auto& b : bands)
        {
            g->addChild (std::make_unique<AudioParameterFloat>  (ParameterID { b.freqID, 1 },   String (b.freqID),   makeFreqRange (20.0f, 20000.0f), b.defaultFreq, AudioParameterFloatAttributes().withLabel ("Hz")));
            g->addChild (std::make_unique<AudioParameterFloat>  (ParameterID { b.gainID, 1 },   String (b.gainID),   NormalisableRange<float> (-18.0f, 18.0f, 0.1f), 0.0f, AudioParameterFloatAttributes().withLabel ("dB")));
            g->addChild (std::make_unique<AudioParameterFloat>  (ParameterID { b.qID, 1 },     String (b.qID),     NormalisableRange<float> (0.1f, 10.0f, 0.01f, 0.5f), 1.0f));
            g->addChild (std::make_unique<AudioParameterChoice> (ParameterID { b.typeID, 1 },   String (b.typeID),   filterTypes, b.defaultType));
            g->addChild (std::make_unique<AudioParameterBool>   (ParameterID { b.activeID, 1 }, String (b.activeID), true));
        }
        groups.push_back (std::move (g));
    }

    // ---- Compressor ----
    {
        auto g = std::make_unique<AudioProcessorParameterGroup> ("comp", "Compressor", "|");
        g->addChild (std::make_unique<AudioParameterBool>  (ParameterID { ParamIDs::compBypass, 1 },    "Comp Bypass", false));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::compThreshold, 1 }, "Comp Threshold", NormalisableRange<float> (-60.0f, 0.0f, 0.1f), -18.0f, AudioParameterFloatAttributes().withLabel ("dB")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::compRatio, 1 },     "Comp Ratio",     NormalisableRange<float> (1.0f, 20.0f, 0.1f, 0.5f), 4.0f));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::compAttack, 1 },    "Comp Attack",    NormalisableRange<float> (0.1f, 200.0f, 0.1f, 0.4f), 10.0f, AudioParameterFloatAttributes().withLabel ("ms")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::compRelease, 1 },   "Comp Release",   NormalisableRange<float> (5.0f, 1000.0f, 0.1f, 0.5f), 100.0f, AudioParameterFloatAttributes().withLabel ("ms")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::compKnee, 1 },      "Comp Knee",      NormalisableRange<float> (0.0f, 20.0f, 0.1f), 6.0f, AudioParameterFloatAttributes().withLabel ("dB")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::compMakeup, 1 },    "Comp Makeup",    NormalisableRange<float> (0.0f, 30.0f, 0.1f), 0.0f, AudioParameterFloatAttributes().withLabel ("dB")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::compScFreq, 1 },    "Comp SC Freq",   makeFreqRange (20.0f, 500.0f), 80.0f, AudioParameterFloatAttributes().withLabel ("Hz")));
        g->addChild (std::make_unique<AudioParameterBool>  (ParameterID { ParamIDs::compScActive, 1 },  "Comp SC Active", false));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::compMix, 1 },       "Comp Mix",       NormalisableRange<float> (0.0f, 100.0f, 0.1f), 100.0f, AudioParameterFloatAttributes().withLabel ("%")));
        groups.push_back (std::move (g));
    }

    // ---- Saturation ----
    {
        auto g = std::make_unique<AudioProcessorParameterGroup> ("sat", "Saturation", "|");
        g->addChild (std::make_unique<AudioParameterBool>   (ParameterID { ParamIDs::satBypass, 1 }, "Sat Bypass", false));
        g->addChild (std::make_unique<AudioParameterFloat>  (ParameterID { ParamIDs::satDrive, 1 },  "Sat Drive",  NormalisableRange<float> (0.0f, 100.0f, 0.1f), 20.0f, AudioParameterFloatAttributes().withLabel ("%")));
        g->addChild (std::make_unique<AudioParameterChoice> (ParameterID { ParamIDs::satType, 1 },   "Sat Type",   StringArray { "Tape", "Tube", "Soft Clip", "Hard Clip" }, 0));
        g->addChild (std::make_unique<AudioParameterFloat>  (ParameterID { ParamIDs::satTone, 1 },   "Sat Tone",   NormalisableRange<float> (-100.0f, 100.0f, 0.1f), 0.0f));
        g->addChild (std::make_unique<AudioParameterFloat>  (ParameterID { ParamIDs::satMix, 1 },    "Sat Mix",    NormalisableRange<float> (0.0f, 100.0f, 0.1f), 50.0f, AudioParameterFloatAttributes().withLabel ("%")));
        g->addChild (std::make_unique<AudioParameterFloat>  (ParameterID { ParamIDs::satOutput, 1 }, "Sat Output", NormalisableRange<float> (-12.0f, 12.0f, 0.1f), 0.0f, AudioParameterFloatAttributes().withLabel ("dB")));
        groups.push_back (std::move (g));
    }

    // ---- Reverb ----
    {
        auto g = std::make_unique<AudioProcessorParameterGroup> ("rev", "Reverb", "|");
        g->addChild (std::make_unique<AudioParameterBool>  (ParameterID { ParamIDs::revBypass, 1 },   "Rev Bypass", false));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::revSize, 1 },     "Rev Size",     NormalisableRange<float> (0.0f, 100.0f, 0.1f), 50.0f, AudioParameterFloatAttributes().withLabel ("%")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::revDamping, 1 },  "Rev Damping",  NormalisableRange<float> (0.0f, 100.0f, 0.1f), 50.0f, AudioParameterFloatAttributes().withLabel ("%")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::revPredelay, 1 }, "Rev Pre-Delay", NormalisableRange<float> (0.0f, 200.0f, 0.1f), 20.0f, AudioParameterFloatAttributes().withLabel ("ms")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::revLowCut, 1 },   "Rev Low Cut",  makeFreqRange (20.0f, 1000.0f), 100.0f, AudioParameterFloatAttributes().withLabel ("Hz")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::revHighCut, 1 },  "Rev High Cut", makeFreqRange (1000.0f, 20000.0f), 8000.0f, AudioParameterFloatAttributes().withLabel ("Hz")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::revMix, 1 },      "Rev Mix",      NormalisableRange<float> (0.0f, 100.0f, 0.1f), 20.0f, AudioParameterFloatAttributes().withLabel ("%")));
        groups.push_back (std::move (g));
    }

    // ---- Delay ----
    {
        auto g = std::make_unique<AudioProcessorParameterGroup> ("dly", "Delay", "|");
        g->addChild (std::make_unique<AudioParameterBool>   (ParameterID { ParamIDs::dlyBypass, 1 },   "Delay Bypass", false));
        g->addChild (std::make_unique<AudioParameterFloat>  (ParameterID { ParamIDs::dlyTime, 1 },     "Delay Time",     NormalisableRange<float> (1.0f, 2000.0f, 0.1f, 0.5f), 250.0f, AudioParameterFloatAttributes().withLabel ("ms")));
        g->addChild (std::make_unique<AudioParameterBool>   (ParameterID { ParamIDs::dlySync, 1 },     "Delay Sync", false));
        g->addChild (std::make_unique<AudioParameterChoice> (ParameterID { ParamIDs::dlyNote, 1 },     "Delay Note",     StringArray { "1/4", "1/8", "1/8D", "1/8T", "1/16", "1/16D", "1/16T" }, 0));
        g->addChild (std::make_unique<AudioParameterFloat>  (ParameterID { ParamIDs::dlyFeedback, 1 }, "Delay Feedback", NormalisableRange<float> (0.0f, 95.0f, 0.1f), 30.0f, AudioParameterFloatAttributes().withLabel ("%")));
        g->addChild (std::make_unique<AudioParameterFloat>  (ParameterID { ParamIDs::dlyLowCut, 1 },   "Delay Low Cut",  makeFreqRange (20.0f, 1000.0f), 200.0f, AudioParameterFloatAttributes().withLabel ("Hz")));
        g->addChild (std::make_unique<AudioParameterFloat>  (ParameterID { ParamIDs::dlyHighCut, 1 },  "Delay High Cut", makeFreqRange (1000.0f, 20000.0f), 6000.0f, AudioParameterFloatAttributes().withLabel ("Hz")));
        g->addChild (std::make_unique<AudioParameterFloat>  (ParameterID { ParamIDs::dlyMix, 1 },      "Delay Mix",      NormalisableRange<float> (0.0f, 100.0f, 0.1f), 15.0f, AudioParameterFloatAttributes().withLabel ("%")));
        groups.push_back (std::move (g));
    }

    // ---- Limiter ----
    {
        auto g = std::make_unique<AudioProcessorParameterGroup> ("lim", "Limiter", "|");
        g->addChild (std::make_unique<AudioParameterBool>  (ParameterID { ParamIDs::limBypass, 1 },  "Limiter Bypass", false));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::limCeiling, 1 }, "Limiter Ceiling", NormalisableRange<float> (-12.0f, 0.0f, 0.1f), -0.3f, AudioParameterFloatAttributes().withLabel ("dB")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::limRelease, 1 }, "Limiter Release", NormalisableRange<float> (1.0f, 200.0f, 0.1f, 0.5f), 50.0f, AudioParameterFloatAttributes().withLabel ("ms")));
        groups.push_back (std::move (g));
    }

    // ---- Macros (Simple Mode) ----
    {
        auto g = std::make_unique<AudioProcessorParameterGroup> ("macros", "Macros", "|");
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::macroWarmth, 1 },      "Warmth",      NormalisableRange<float> (0.0f, 100.0f, 0.1f), 50.0f, AudioParameterFloatAttributes().withLabel ("%")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::macroPresence, 1 },    "Presence",    NormalisableRange<float> (0.0f, 100.0f, 0.1f), 50.0f, AudioParameterFloatAttributes().withLabel ("%")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::macroCompression, 1 }, "Compression", NormalisableRange<float> (0.0f, 100.0f, 0.1f), 50.0f, AudioParameterFloatAttributes().withLabel ("%")));
        g->addChild (std::make_unique<AudioParameterFloat> (ParameterID { ParamIDs::macroSpace, 1 },       "Space",       NormalisableRange<float> (0.0f, 100.0f, 0.1f), 0.0f, AudioParameterFloatAttributes().withLabel ("%")));
        groups.push_back (std::move (g));
    }

    return { groups.begin(), groups.end() };
}
