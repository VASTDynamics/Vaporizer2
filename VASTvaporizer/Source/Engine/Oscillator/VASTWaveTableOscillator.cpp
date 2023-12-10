/*
 VAST Dynamics Audio Software (TM)
 
 //  Adapted from Nigel Redmon on 5/15/12
 //  EarLevel Engineering: earlevel.com
 //  Copyright 2012 Nigel Redmon
 //
 //  For a complete explanation of the wavetable oscillator and code,
 //  read the series of articles by the author, starting here:
 //  www.earlevel.com/main/2012/05/03/a-wavetable-oscillator-introduction/
 
 */

#include "VASTWaveTableOscillator.h"
#include "../Utils/VASTSynthfunctions.h"
#include <ctime>
#include "../VASTEngineHeader.h"
#include "../VASTOscillatorBank.h"
#include <vector>

CVASTWaveTableOscillator::CVASTWaveTableOscillator(CVASTSettings& set, CVASTOscillatorBank* oscBank) : m_Set (&set), m_oscBank(oscBank) {
    //from VASTOscillator
    //NOISE /* Generate a new random seed from system time - do this once in your constructor */
    srand(int(time(0)));
    m_seed = rand();
    m_uOscType = 999; // set invalid so that first check is changed
    
    // sample hold
    m_fRsHoldValue = 0.0f;
    m_fRshCounter = -1.0f;
    
    // silent
    m_bNoteOn = false;
    
    // norm phase
    m_bInvert = false;
    m_fOscMasterTune = 0.0f;
    m_fOscCents = 0.0f;
    m_iOscOctave = 0;
    m_fOscPitchMod = 0.0f;
    m_fLastValue = 0.0f;
    m_fBaseFrequency_Hz = 0.0f;
    m_fPortamento = 0.0f;
    m_fPitchBendNote = 1.0f;
    m_fPitchBendZone = 1.0f;
    m_uPolarity = 1;
    
    int initSize = 16;
    for (int i = 0; i < C_MAX_PARALLEL_OSC; i++) {
        m_fOscDetune[i] = 1.0f;
        //m_fExciterDetune[i] = 0.0f;
        m_fPlayedFrequency_Hz[i] = 0.0f;
        m_phasedPhasorBuffer[i] = std::make_unique<AudioSampleBuffer>(1, initSize);
        m_phasedPhasorBufferPointer[i] = m_phasedPhasorBuffer[i]->getWritePointer(0);
        m_phaseIncBuffer[i] = std::make_unique<AudioSampleBuffer>(1, initSize);
        m_phaseIncBufferPointer[i] = m_phaseIncBuffer[i]->getWritePointer(0);
        phasor[i] = 0.0;       // phase accumulator
        phaseInc[i] = 0.0;
    }
    
    m_fFMFreq = 0.f;
    m_unisonOscis = -1;
}

CVASTWaveTableOscillator::~CVASTWaveTableOscillator(void) {
}

void CVASTWaveTableOscillator::init() {
   
    m_bNoteOn = false;
    m_bIsPlaying = false;
    m_fBaseFrequency_Hz = 0.0f;
    m_fOscMasterTune = 0.0f;
    m_iOscOctave = 0;
    m_fOscCents = 0.0f;
    m_fOscPitchMod = 0.0f;
    m_fLastValue = 0.0f;
    m_fPortamento = 0.0f;
    
    m_bLFO = false;
    m_LFOno = 0;
    m_LFORamp = 1.f;
    
    m_i_resynch_fade_steps = 0;
    
    // sample hold
    m_fRsHoldValue = 0.0f;
    m_fRshCounter = -1.0f;
    
    //m_lowPassBiQuad.flushDelays();
    m_noiseBiQuadLow.flushDelays();
    m_noiseBiQuadHigh.flushDelays();
    m_fNoiseLowCut = 0.f;
    m_fNoiseHighCut = 0.f;
    m_fNoiseResonance = 0.f;
    
    m_noise_skips = 0;
    
    //make those slower
    m_fNoiseLowCut_smoothed.reset(m_iSampleRate, 0.2f);
    m_fNoiseHighCut_smoothed.reset(m_iSampleRate, 0.1f);
    m_fNoiseResonance_smoothed.reset(m_iSampleRate, 0.1f);
      
    for (int i = 0; i < C_MAX_PARALLEL_OSC; i++) {
        reset(i);
        m_fOscDetune[i] = 1.0f;
        //m_fExciterDetune[i] = 0.0f;
        m_fPlayedFrequency_Hz[i] = 0.0f;
        
        m_fDriftLfo[i] = 0.f;
        m_fDriftLfo[i] = 0.f;
    }
    
    //portamento not here
    m_fFMFreq = 0.f;
    resetSmoothers();
}

void CVASTWaveTableOscillator::resetSmoothers() {
    m_bSmoothersTakeNextValue = true;
    m_noise_skips = 0;
}

void CVASTWaveTableOscillator::updateNoiseBiquad(int skips) {
    modMatrixInputState inputstate = m_Set->bufferInputState.load();
    float noiseLowCut = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fNoiseLowCut, MODMATDEST::NoiseLowCut, &inputstate);
    float noiseHighCut = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fNoiseHighCut, MODMATDEST::NoiseHighCut, &inputstate);
    float noiseResonance = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fNoiseResonance, MODMATDEST::NoiseResonance, &inputstate);
    
    m_bSmoothersTakeNextValue ? m_fNoiseLowCut_smoothed.setCurrentAndTargetValue(noiseLowCut) : m_fNoiseLowCut_smoothed.setTargetValue(noiseLowCut);
    m_fNoiseLowCut_smoothed.skip(skips);
    noiseLowCut = m_fNoiseLowCut_smoothed.getNextValue();
    m_bSmoothersTakeNextValue ? m_fNoiseHighCut_smoothed.setCurrentAndTargetValue(noiseHighCut) : m_fNoiseHighCut_smoothed.setTargetValue(noiseHighCut);
    m_fNoiseHighCut_smoothed.skip(skips);
    noiseHighCut = m_fNoiseHighCut_smoothed.getNextValue();
    m_bSmoothersTakeNextValue ? m_fNoiseResonance_smoothed.setCurrentAndTargetValue(noiseResonance) : m_fNoiseResonance_smoothed.setTargetValue(noiseResonance);
    m_fNoiseResonance_smoothed.skip(skips);
    noiseResonance = (m_fNoiseResonance_smoothed.getNextValue() + 0.707f) * 0.1f; //as in vcf
    
    if ((noiseLowCut != m_fNoiseLowCut) || (noiseResonance != m_fNoiseResonance)) {
        m_noiseBiQuadLow.calcBiquad(CVASTBiQuad::HIGHPASS, noiseLowCut, m_iSampleRate, noiseResonance, 0.0f);  //standard value for reso is 0.707f
    }
    if ((noiseHighCut != m_fNoiseHighCut) || (noiseResonance != m_fNoiseResonance)) {
        m_noiseBiQuadHigh.calcBiquad(CVASTBiQuad::LOWPASS, noiseHighCut, m_iSampleRate, noiseResonance, 0.0f);  //standard value for reso is 0.707f
    }
    m_fNoiseLowCut = noiseLowCut;
    m_fNoiseHighCut = noiseHighCut;
    m_fNoiseResonance = noiseResonance;
}

float CVASTWaveTableOscillator::drift_noise(float& lastval)
{
    //const float c_driftfilter = 0.00001f;
    //const float c_driftm = 1.f / sqrt(c_driftfilter);
    
    //__m128 mvec = _mm_rsqrt_ss(_mm_load_ss(&filter));
    //_mm_store_ss(&m,mvec);
    
    float rand11 = (((float)rand() / RAND_MAX) * 2.f - 1.f);
    lastval = lastval * (1.f - c_driftfilter) + rand11 * c_driftfilter;
    return lastval * c_driftm;
}

// note: if you don't keep this in the range of 0-1, you'll need to make changes elsewhere
void CVASTWaveTableOscillator::setFrequency(int unisonOsci, bool restart) {
    //new drift
    float driftAmount = 0.f;
    if (m_oscBank != nullptr) {
        int bankNo = m_oscBank->getBankno();
        if (((bankNo == 0) && (*m_Set->m_State->m_bExciterOnOff_OscA) == static_cast<int>(SWITCH::SWITCH_ON)) ||
            ((bankNo == 1) && (*m_Set->m_State->m_bExciterOnOff_OscB) == static_cast<int>(SWITCH::SWITCH_ON)) ||
            ((bankNo == 2) && (*m_Set->m_State->m_bExciterOnOff_OscC) == static_cast<int>(SWITCH::SWITCH_ON)) ||
            ((bankNo == 3) && (*m_Set->m_State->m_bExciterOnOff_OscD) == static_cast<int>(SWITCH::SWITCH_ON))) {
            driftAmount = 1.0f;
            m_fDriftLfo[unisonOsci] = drift_noise(m_fDriftLfo2[unisonOsci]);
        }
        else m_fDriftLfo[unisonOsci] = 0.f;
    }
    
    float lBaseFrequency = m_fBaseFrequency_Hz;
    if (m_bLFO == false) {
        if (*m_Set->m_State->m_fPortamento > 0.0f) {
            lBaseFrequency = m_fBaseFreqPortamento_smoothed.getNextValue();
        }
    }
    
    if (m_uOscType == WAVE::wavetable) {
        
        //float lOctCentFactor = lOctCentFactor = std::powf(2.f, m_iOscOctave + m_fOscCents / 1200.0f); //pow 2 //std::exp2f is 3 time slower tha std::pow!
        float lOctCentFactor = m_Set->getFrequencyFactorFromLUT(m_iOscOctave + m_fOscCents / 1200.0f); //factor 3 faster than pow
        
        m_fPlayedFrequency_Hz[unisonOsci] =
        lBaseFrequency * lOctCentFactor	* m_fOscDetune[unisonOsci] * m_fPitchBendNote * m_fPitchBendZone * (m_fOscPitchMod + 1.0f) + driftAmount * m_fDriftLfo[unisonOsci] + m_fFMFreq;
    }
    else {
        //standard
        m_fPlayedFrequency_Hz[unisonOsci] = lBaseFrequency * powf(2.0, float(m_iOscOctave) + m_fOscCents / 1200.0f) * m_fPitchBendNote * m_fPitchBendZone * (m_fOscPitchMod + 1.0f) /* + m_fExciterDetune[unisonOsci] */ + driftAmount * m_fDriftLfo[unisonOsci];
    }
    
    phaseInc[unisonOsci] = (m_fPlayedFrequency_Hz[unisonOsci]) / (float)m_iSampleRate;
}

void CVASTWaveTableOscillator::setFMFreq(float fmFreq) {
    m_fFMFreq = fmFreq;
}

bool CVASTWaveTableOscillator::updatePhase(int unisonOsci) {
    phasor[unisonOsci] += phaseInc[unisonOsci];
    
    bool wrap = false;
    float curPhase = phasor[unisonOsci];
    if (curPhase < 0.0) { //for FM negative freqs
        phasor[unisonOsci] -= int(curPhase) - 1; //- * - = +
    } else if (curPhase >= 1.0) {
        phasor[unisonOsci] -= int(curPhase);
        wrap = true;
    }
    //else if (phasor[unisonOsci] == 0.f) wrap = true; //does not happen??
    return wrap;
}

void CVASTWaveTableOscillator::setPortamentoStart(int midinote, bool reset) {
    float startFreq = m_Set->midiNoteGetBaseFreq(midinote, m_fOscMasterTune);
    if (m_fBaseFreqPortamento_smoothed.isSmoothing()) {
        float currentFreq = m_fBaseFreqPortamento_smoothed.getNextValue();
        startFreq = currentFreq;
    }
    float keepTarget = m_fBaseFreqPortamento_smoothed.getTargetValue();
    
    if (!reset) {
        m_fBaseFreqPortamento_smoothed.setCurrentAndTargetValue(startFreq); //force take it!
        m_fBaseFreqPortamento_smoothed.setTargetValue(keepTarget); //old target freq
    }
    else {
        m_fBaseFreqPortamento_smoothed.setCurrentAndTargetValue(keepTarget); //force take it!
    }
}

void CVASTWaveTableOscillator::setPortamentoTime(float time) {
    //portamento
    m_fBaseFreqPortamento_smoothed.reset(m_iSampleRate, time * 0.001f);
}

void CVASTWaveTableOscillator::doPortamentoIfRequired(int skips) {
    if (m_bLFO == false)
        if (*m_Set->m_State->m_fPortamento > 0.0f) {
            if (m_fBaseFreqPortamento_smoothed.isSmoothing()) {
                m_fBaseFreqPortamento_smoothed.skip(skips);
            }
        }
}

void CVASTWaveTableOscillator::prepareForPlay(int expectedSamplesPerBlock) {
    bool releaseMemory = true;
    for (int i = 0; i < C_MAX_PARALLEL_OSC; i++) {
        m_phasedPhasorBuffer[i]->setSize(1, expectedSamplesPerBlock, false, false, !releaseMemory);
        m_phasedPhasorBufferPointer[i] = m_phasedPhasorBuffer[i]->getWritePointer(0);
        m_phaseIncBuffer[i]->setSize(1, expectedSamplesPerBlock, false, false, !releaseMemory);
        m_phaseIncBufferPointer[i] = m_phaseIncBuffer[i]->getWritePointer(0);
    }
}

void CVASTWaveTableOscillator::doWavetableBufferInit(int unisonOsci, sRoutingBuffers& routingBuffers, modMatrixInputState& inputState) {
    //resize buffers
    m_phasedPhasorBuffer[unisonOsci]->setSize(1, routingBuffers.getNumSamples(), false, false, true);
    m_phasedPhasorBufferPointer[unisonOsci] = m_phasedPhasorBuffer[unisonOsci]->getWritePointer(0);
    m_phaseIncBuffer[unisonOsci]->setSize(1, routingBuffers.getNumSamples(), false, false, true);
    m_phaseIncBufferPointer[unisonOsci] = m_phaseIncBuffer[unisonOsci]->getWritePointer(0);
}

bool CVASTWaveTableOscillator::doWavetableBufferStep(int unisonOsci, int currentFrame, float fPhaseOffset) {
    vassert(m_oscBank != nullptr); //otherwise must not be called
    m_bSmoothersTakeNextValue = false;
    
    float lphasedPhasor = (phasor[unisonOsci] + fPhaseOffset);
    if (lphasedPhasor >= 1.0f) lphasedPhasor -= int(lphasedPhasor);
    if (lphasedPhasor < 0.0f) lphasedPhasor -= int(lphasedPhasor) - 1; //fm negative freqs, - * - = +
    
    m_phaseIncBufferPointer[unisonOsci][currentFrame] = phaseInc[unisonOsci]; //keep per osc
    m_phasedPhasorBufferPointer[unisonOsci][currentFrame] = lphasedPhasor; //keep per osc
    vassert(m_phasedPhasorBufferPointer[unisonOsci][currentFrame] >= 0.f);
    vassert(m_phasedPhasorBufferPointer[unisonOsci][currentFrame] < 1.f);
    
    bool wrap = updatePhase(unisonOsci); //update phasor
    return wrap;
}

bool CVASTWaveTableOscillator::getOscillation(float* pOutput) {
    getOsci(pOutput);
    return true;
}

bool CVASTWaveTableOscillator::getOsci(float* pOutput) {
    
    // our output value for this cycle
    float fOutSample = 0;
    
    // interpolate the output
    if (m_bIsPlaying == true) {// no calc when silent?
        switch (m_uOscType) {
            case WAVE::sine:
            case WAVE::saw:
            case WAVE::tri:
            case WAVE::square:
            case WAVE::sawsine:
            {
                if (m_bLFO) {
                    if (m_LFOno == 0) {
                        if (*m_Set->m_State->m_fLFORamp_LFO1 == 0)
                            m_LFORamp = 1.0f;
                        else {
                            //samples per second: m_iSampleRate
                            //seconds for ramp: ramp * dawbeattime
                            float secondsPerBeat = float(m_Set->getMillisecondsPerBeat() * .001);
                            float rampseconds = *m_Set->m_State->m_fLFORamp_LFO1 * secondsPerBeat;
                            float rampsamples = rampseconds * m_iSampleRate;
                            m_LFORamp += 1.0f / rampsamples;
                            if (m_LFORamp > 1.f) m_LFORamp = 1.f;
                        }
                    }
                    else if (m_LFOno == 1) {
                        if (*m_Set->m_State->m_fLFORamp_LFO2 == 0)
                            m_LFORamp = 1.0f;
                        else {
                            //samples per second: m_iSampleRate
                            //seconds for ramp: ramp * dawbeattime
                            float secondsPerBeat = float(m_Set->getMillisecondsPerBeat() * .001);
                            float rampseconds = *m_Set->m_State->m_fLFORamp_LFO2 * secondsPerBeat;
                            float rampsamples = rampseconds * m_iSampleRate;
                            m_LFORamp += 1.0f / rampsamples;
                            if (m_LFORamp > 1.f) m_LFORamp = 1.f;
                        }
                    }
                    else if (m_LFOno == 2) {
                        if (*m_Set->m_State->m_fLFORamp_LFO3 == 0)
                            m_LFORamp = 1.0f;
                        else {
                            //samples per second: m_iSampleRate
                            //seconds for ramp: ramp * dawbeattime
                            float secondsPerBeat = float(m_Set->getMillisecondsPerBeat() * .001);
                            float rampseconds = *m_Set->m_State->m_fLFORamp_LFO3 * secondsPerBeat;
                            float rampsamples = rampseconds * m_iSampleRate;
                            m_LFORamp += 1.0f / rampsamples;
                            if (m_LFORamp > 1.f) m_LFORamp = 1.f;
                        }
                    }
                    else if (m_LFOno == 3) {
                        if (*m_Set->m_State->m_fLFORamp_LFO4 == 0)
                            m_LFORamp = 1.0f;
                        else {
                            //samples per second: m_iSampleRate
                            //seconds for ramp: ramp * dawbeattime
                            float secondsPerBeat = float(m_Set->getMillisecondsPerBeat() * .001);
                            float rampseconds = *m_Set->m_State->m_fLFORamp_LFO4 * secondsPerBeat;
                            float rampsamples = rampseconds * m_iSampleRate;
                            m_LFORamp += 1.0f / rampsamples;
                            if (m_LFORamp > 1.f) m_LFORamp = 1.f;
                        }
                    }
                    else if (m_LFOno == 4) {
                        if (*m_Set->m_State->m_fLFORamp_LFO5 == 0)
                            m_LFORamp = 1.0f;
                        else {
                            //samples per second: m_iSampleRate
                            //seconds for ramp: ramp * dawbeattime
                            float secondsPerBeat = float(m_Set->getMillisecondsPerBeat() * .001);
                            float rampseconds = *m_Set->m_State->m_fLFORamp_LFO5 * secondsPerBeat;
                            float rampsamples = rampseconds * m_iSampleRate;
                            m_LFORamp += 1.0f / rampsamples;
                            if (m_LFORamp > 1.f) m_LFORamp = 1.f;
                        }
                    }
                    else { //chorus lfos etc
                        m_LFORamp = 1.0f;
                    }
                    
                    // calc explicitly, not bandlimited!
                    switch (m_uOscType) {
                        case WAVE::sine:
                            fOutSample = sinf(float(2.0f * M_PI * phasor[0]));
                            break;
                            
                        case WAVE::saw:
                            // 0  ..0.5	 -  0 .. 1
                            // 0.5..1	 - -1 .. 0
                            
                            fOutSample = (phasor[0] <= 0.5f) ? (phasor[0] * 2.0f) : (phasor[0] * 2.0f) - 2.0f;
                            break;
                            
                        case WAVE::tri:
                            // 0..0.25	 -  0 .. 1
                            // 0.25..0.5 -  1 .. 0
                            // 0.5..0.75 -  0 ..-1
                            // 0.75..1	 - -1 .. 0
                            
                            fOutSample = (phasor[0] < 0.25f) ? phasor[0] * 4.0f : ((phasor[0] < 0.75f) ? 2.0f - (phasor[0] * 4.0f) : phasor[0] * 4.0f - 4.0f);
                            break;
                            
                        case WAVE::square:
                            // 0  ..0.5	 -  1
                            // 0.5..1	 - -1
                            
                            fOutSample = (phasor[0] <= 0.5f) ? 1.0f : -1.0f;
                            break;
                            
                        case WAVE::sawsine:
                            
                            fOutSample = (phasor[0] <= 0.5f) ? (phasor[0] * 2.0f) : sinf(float(2.0f * M_PI * phasor[0]));
                            break;
                    }
                    fOutSample *= m_LFORamp;
                }
                break;
            }
            case WAVE::noise:
            {
                int skips = 64;
                m_noise_skips--;
                if (m_noise_skips < 0) {
                    updateNoiseBiquad(skips);
                    m_noise_skips = skips;
                }
                switch (int(*m_Set->m_State->m_uNoiseType)) {
                    case NOISETYPE::White:
                    {
                        // http://musicdsp.org/archive.php?classid=1#216
                        // white noise
                    
                        //fOutSample = doWhiteNoiseFast();
                        fOutSample = m_Set->m_whiteNoiseBuffer[m_whiteNoisePhasor];
                        m_whiteNoisePhasor++;
                        if (m_whiteNoisePhasor>=C_MAX_SAMPLE_RATE * 3)
                            m_whiteNoisePhasor=0;
                    }
                        break;
                    case NOISETYPE::Pink:
                    {
                        // based on Paul Kellet's "instrumentation grade" algorithm.
                        // 0.129f is an experimental normalization factor.
                        
                        float amplitude = 0.129f;
                        //float white = doWhiteNoiseFast();
                        
                        float white  = m_Set->m_whiteNoiseBuffer[m_whiteNoisePhasor];
                        m_whiteNoisePhasor++;
                        if (m_whiteNoisePhasor>=C_MAX_SAMPLE_RATE * 3)
                            m_whiteNoisePhasor=0;
                        
                        m_buf0 = 0.99886f * m_buf0 + 0.0555179f * white;
                        m_buf1 = 0.99332f * m_buf1 + 0.0750759f * white;
                        m_buf2 = 0.96900f * m_buf2 + 0.1538520f * white;
                        m_buf3 = 0.86650f * m_buf3 + 0.3104856f * white;
                        m_buf4 = 0.55000f * m_buf4 + 0.5329522f * white;
                        m_buf5 = -0.7616f * m_buf5 - 0.0168980f * white;
                        fOutSample = amplitude *
                        (m_buf0 + m_buf1 + m_buf2 + m_buf3 + m_buf4 + m_buf5 + m_buf6 + white * 0.5362f);
                        m_buf6 = white * 0.115926f;
                    }
                        break;
                    case NOISETYPE::Brown:
                    {
                        //float leakage=0.997; // experimental value at 44.1kHz
                        //double scaling = 0.05; // experimental value at 44.1kHz
                        // min and max protect against instability at extreme sample rates.
                        
                        float leakage = ((m_iSampleRate - 144.f) / m_iSampleRate < 0.9999f)
                        ? (m_iSampleRate - 144.0f) / m_iSampleRate
                        : 0.9999f;
                        
                        float scaling = (9.0f / float(sqrt(m_iSampleRate)) > 0.01f)
                        ? 9.0f / float(sqrt(m_iSampleRate))
                        : 0.01f;
                        
                        float whiteb  = m_Set->m_whiteNoiseBuffer[m_whiteNoisePhasor];
                        m_whiteNoisePhasor++;
                        if (m_whiteNoisePhasor>=C_MAX_SAMPLE_RATE * 3)
                            m_whiteNoisePhasor=0;
                        
                        m_z = leakage * m_y + whiteb * scaling;
                        m_y = fabs(m_z) > 1.0f
                        ? leakage * m_y - whiteb * scaling
                        : m_z;
                        fOutSample = m_y;
                    }
                        break;
                }
                fOutSample = m_noiseBiQuadLow.doBiQuad(fOutSample);
                fOutSample = m_noiseBiQuadHigh.doBiQuad(fOutSample);
                
                break;
            }
            case WAVE::samplehold:
            {
                if (m_fRshCounter < 0.0f) {
                    m_fRsHoldValue = doWhiteNoiseFast();
                    m_fRshCounter = 1.0f;
                }
                else {
                    if (m_fRshCounter > (float)(m_iSampleRate / m_fPlayedFrequency_Hz[0])) {
                        m_fRshCounter -= (float)(m_iSampleRate / m_fPlayedFrequency_Hz[0]);
                        m_fRsHoldValue = doWhiteNoiseFast();
                    }
                    m_fRshCounter += 1.0f;
                }
                fOutSample = m_fRsHoldValue;
                break;
            }
            case WAVE::wavetable:
                break;
        }
    }

    // invert?
    if (m_bInvert)
    {
        fOutSample *= -1.0f;
    }
    
    if (m_uPolarity == 0) //unipolar
    {
        fOutSample /= 2.0f;
        fOutSample += 0.5f;
    }
    
    //fade resynch
    if (m_i_resynch_fade_steps > 0) {
        fOutSample = fOutSample * float(abs(float(m_i_resynch_fade_steps - M_MAX_DELTA_STEPS * .5f)) / float(M_MAX_DELTA_STEPS *.5f));
        if (m_i_resynch_fade_steps == M_MAX_DELTA_STEPS * .5f) { //gerade zahl benoetigt
            resynch(0, m_bLFO);
        }
        m_i_resynch_fade_steps--;
    } //not for quad
    
    float lFiltered = fOutSample;
    m_fLastValue = lFiltered;
    *pOutput = lFiltered;
    
    // advance - very important! Call to getOsci advances the phasors!
    updatePhase(0);
    m_bSmoothersTakeNextValue = false;
    return true;
}

void CVASTWaveTableOscillator::startLFOFrequency(float LFOFreq, int LFOno)
{
    m_bLFO = true;
    m_LFOno = LFOno;
    
    //for LFO create a lowpass below 120hz to remove aliasing
    //m_lowPassBiQuad.calcBiquad(CVASTBiQuad::LOWPASS, 120.0, m_iSampleRate, 0.707f, 0.0f);
    
    m_fBaseFrequency_Hz = LFOFreq;
    setFrequency(0, false); //keep running
    m_bIsPlaying = true;
}

void CVASTWaveTableOscillator::updateMainVariables(int samplerate, MYUINT OscType, int unisonOscis, float OscMasterTune, int OscOctave, float OscCents)
{
    m_iSampleRate = samplerate;
    if ((m_uOscType == OscType) && (m_fOscMasterTune == OscMasterTune) && (m_fOscCents == OscCents) && (m_iOscOctave == OscOctave) &&
        (m_iPolyMode == static_cast<int>(m_Set->m_uMaxPoly)) &&
        (m_fPortamento == *m_Set->m_State->m_fPortamento) &&
        (m_unisonOscis == unisonOscis)
        ) { // unchanged?
        return;
    }
    
    m_unisonOscis = unisonOscis;
    jassert(unisonOscis <= C_MAX_PARALLEL_OSC);
    
    m_uOscType = OscType;
    m_fOscMasterTune = OscMasterTune;
    m_fOscCents = OscCents;
    m_iOscOctave = OscOctave;
    m_iPolyMode = static_cast<int>(m_Set->m_uMaxPoly);
    m_fPortamento = *m_Set->m_State->m_fPortamento;
    
    signed int key = m_uMIDINote;
    
    float divi = (key - 45.0f - (24.f)) / 12.0f; //todo there are two places for this formula!
    m_fBaseFrequency_Hz = powf(2.0, divi) * m_fOscMasterTune;
    
    for (int i=0; i< C_MAX_PARALLEL_OSC; i++)
        setFrequency(i, true); // have to restart here, major change
}

//first fade - then either reset (0 pos in wt) or random retrigger (start somwhere in wavetable)
//resynch is called for each new played note
void CVASTWaveTableOscillator::resynch(int unisonOsci, bool lfo) {
    if (!(m_bLFO == true))
        if (m_oscBank != nullptr) {
            if (((m_oscBank->getBankno() == 0) && (*m_Set->m_State->m_bOscRetrigOnOff_OscA == static_cast<int>(SWITCH::SWITCH_OFF))) ||
                ((m_oscBank->getBankno() == 1) && (*m_Set->m_State->m_bOscRetrigOnOff_OscB == static_cast<int>(SWITCH::SWITCH_OFF))) ||
                ((m_oscBank->getBankno() == 2) && (*m_Set->m_State->m_bOscRetrigOnOff_OscC == static_cast<int>(SWITCH::SWITCH_OFF))) ||
                ((m_oscBank->getBankno() == 3) && (*m_Set->m_State->m_bOscRetrigOnOff_OscD == static_cast<int>(SWITCH::SWITCH_OFF))))
            {
                random_retrig(unisonOsci); // in the middle random retrig it
            }
            else {
                reset(unisonOsci); // in the middle resynch it
            }
        }
        else reset(unisonOsci);
        else
            reset(unisonOsci);
    m_bLFO = lfo; //what???
}

void CVASTWaveTableOscillator::resynchWithFade(bool lfo) {
    m_i_resynch_fade_steps = M_MAX_DELTA_STEPS;
    m_bLFO = lfo; //what???
}

inline void CVASTWaveTableOscillator::syncAllPhasorsToMaster() {
    for (int osci = 1; osci < m_unisonOscis; osci++)
        phasor[osci] = phasor[0];
}

void CVASTWaveTableOscillator::random_retrig(int unisonOsci) {
    phasor[unisonOsci] = float((double)rand() / RAND_MAX); // top of buffer
}

void CVASTWaveTableOscillator::reset(int unisonOsci) {
    if (m_bLFO == false) {
        phasor[unisonOsci] = 0.0f;
    }
    else {
        if (m_LFOno == 0) {
            phasor[unisonOsci] = *m_Set->m_State->m_fLFOPhase_LFO1 / 100.f;
        }
        else if (m_LFOno == 1) {
            phasor[unisonOsci] = *m_Set->m_State->m_fLFOPhase_LFO2 / 100.f;
        }
        else if (m_LFOno == 2) {
            phasor[unisonOsci] = *m_Set->m_State->m_fLFOPhase_LFO3 / 100.f;
        }
        else if (m_LFOno == 3) {
            phasor[unisonOsci] = *m_Set->m_State->m_fLFOPhase_LFO4 / 100.f;
        }
        else if (m_LFOno == 4) {
            phasor[unisonOsci] = *m_Set->m_State->m_fLFOPhase_LFO5 / 100.f;
        }
    }
    
    m_fDriftLfo[unisonOsci] = 0.f;
    m_fDriftLfo2[unisonOsci] = 0.f;
    
    // sample hold
    m_fRsHoldValue = 0.0f;
    m_fRshCounter = -1.0f;
    
    //LFO
    m_LFORamp = 0.f;
    
    //noise
    m_y = m_z = m_buf0 = m_buf1 = m_buf2 = m_buf3 = m_buf4 = m_buf5 = m_buf6 = 0.0f;
}

void CVASTWaveTableOscillator::setDetune(int unisonOsci, float OscDetune, bool updateFrequency) {
    if (m_fOscDetune[unisonOsci] != OscDetune) {
        m_fOscDetune[unisonOsci] = OscDetune;
        if (updateFrequency)
            setFrequency(unisonOsci, false); // keep running dont restart
    }
}

void CVASTWaveTableOscillator::setCents(float OscCents) {
    m_fOscCents = OscCents;
}

void CVASTWaveTableOscillator::setPitchBendZone(float pitchbend) { //multiplier
    m_fPitchBendZone = pitchbend; //is expected to be smoothed before
}

void CVASTWaveTableOscillator::setPitchBendNote(float pitchbend) { //multiplier
    m_fPitchBendNote = pitchbend; //is expected to be smoothed before
}

MYUINT CVASTWaveTableOscillator::getOscType() const {
    return m_uOscType;
}

void CVASTWaveTableOscillator::updatePitchMod(float pitchMod) {
    m_fOscPitchMod = pitchMod; //-1..1 center 0
}

bool CVASTWaveTableOscillator::isPlaying() {
    return m_bIsPlaying;
}

long unsigned int CVASTWaveTableOscillator::rand31_next()
{ // 0 --> 2147483647
    //see http://www.firstpr.com.au/dsp/rand31/
    long unsigned int hi, lo;
    lo = 16807 * (m_seed & 0xFFFF);
    hi = 16807 * (m_seed >> 16);
    lo += (hi & 0x7fff) << 16;
    lo += hi >> 15;
    if (lo > 0x7FFFFFFF) lo -= 0x7FFFFFFF;
    return (m_seed = (long)lo);
}

float CVASTWaveTableOscillator::doWhiteNoiseFast()
{
    float fNoise = 0.0;
    
#if defined _WINDOWS || defined _WINDLL || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 36)
    // fNoise is 0 -> 2147483647.0
    fNoise = (float)rand31_next();
    
    // normalize and make bipolar
    fNoise = 2.0f * (fNoise / 2147483647.0f) - 1.0f;
#else
    // fNoise is 0 -> ARC4RANDOMMAX
    fNoise = (float)arc4random();
    
    // normalize and make bipolar
    fNoise = 2.0f * (fNoise / ARC4RANDOMMAX) - 1.0f;
#endif
    
    return fNoise;
}

/*
 // PolyBLEP by Tale
 // (slightly modified)
 // http://www.kvraudio.com/forum/viewtopic.php?t=375517
 float CVASTWaveTableOscillator::poly_blep(float t, float dt)
 {
 //float t = 0.; // 0 <= t < 1
 //float dt = freq / sample_rate;
 
 // 0 <= t < 1
 if (t < dt)
 {
 t /= dt;
 // 2 * (t - t^2/2 - 0.5)
 return t + t - t*t - 1.;
 }
 
 // -1 < t < 0
 else if (t > 1. - dt)
 {
 t = (t - 1.) / dt;
 // 2 * (t^2/2 + t + 0.5)
 return t*t + t + t + 1.;
 }
 
 // 0 otherwise
 else
 {
 return 0.;
 }
 }
 */

void CVASTWaveTableOscillator::syncPhasor(int unisonOsci) {
    phasor[unisonOsci] = phasor[0];
}

//*************************************************************************************************************

void CVASTWaveTableOscillator::noteOn(MYUINT uChannel, MYUINT uMIDINote, MYUINT uVelocity)
{
    m_uMIDINote = uMIDINote;
    m_fBaseFrequency_Hz  = m_Set->midiNoteGetBaseFreq(uMIDINote, m_fOscMasterTune);
    
    //update smoothers
    resetSmoothers();
    if (!m_fBaseFreqPortamento_smoothed.isSmoothing())
        m_fBaseFreqPortamento_smoothed.setCurrentAndTargetValue(m_fBaseFrequency_Hz); //is set afterwards
    else
        m_fBaseFreqPortamento_smoothed.setTargetValue(m_fBaseFrequency_Hz); //is set afterwards
    
    /*
     if (m_oscBank != nullptr) {
     if (((m_oscBank->getBankno() == 0) && (*m_Set->m_State->m_bExciterOnOff_OscA)) ||
     ((m_oscBank->getBankno() == 1) && (*m_Set->m_State->m_bExciterOnOff_OscB)) ||
     ((m_oscBank->getBankno() == 2) && (*m_Set->m_State->m_bExciterOnOff_OscC)) ||
     ((m_oscBank->getBankno() == 3) && (*m_Set->m_State->m_bExciterOnOff_OscD)))
     for (int i=0; i< C_MAX_PARALLEL_OSC; i++)
     m_fExciterDetune[i] = float(((rand() % 10000) / 20000.0) - 0.25f); // random detune by max +- 0.25hz
     else
     for (int i = 0; i< C_MAX_PARALLEL_OSC; i++)
     m_fExciterDetune[i] = 0.0f;
     }
     else
     for (int i = 0; i< C_MAX_PARALLEL_OSC; i++)
     m_fExciterDetune[i] = 0.0f;
     */
    
    // see http://en.wikipedia.org/wiki/Piano_key_frequencies
    for (int i = 0; i< C_MAX_PARALLEL_OSC; i++)
        setFrequency(i, true); //major change
    m_bIsPlaying = true;
    
    /*
     if (m_oscBank != nullptr) {
     if ((m_Set->modMatrixDestinationSetFast(MODMATDEST::OscAWTPos) == false) && (m_oscBank->getBankno() == 0))
     m_fOscWTPos_smoothed.setValue(*m_Set->m_State->m_fOscWTPos_OscA * 0.01f, true); //hack for startup
     else if ((m_Set->modMatrixDestinationSetFast(MODMATDEST::OscBWTPos) == false) && (m_oscBank->getBankno() == 1))
     m_fOscWTPos_smoothed.setValue(*m_Set->m_State->m_fOscWTPos_OscB * 0.01f, true);
     else if ((m_Set->modMatrixDestinationSetFast(MODMATDEST::OscCWTPos) == false) && (m_oscBank->getBankno() == 2))
     m_fOscWTPos_smoothed.setValue(*m_Set->m_State->m_fOscWTPos_OscC * 0.01f, true);
     else if ((m_Set->modMatrixDestinationSetFast(MODMATDEST::OscDWTPos) == false) && (m_oscBank->getBankno() == 3))
     m_fOscWTPos_smoothed.setValue(*m_Set->m_State->m_fOscWTPos_OscD * 0.01f, true);
     }
     */
}

void CVASTWaveTableOscillator::noteOff()
{
    m_Set->sendStatusMessage("---------CVASTOscillator::noteOff", m_uMIDINote);
}



