/*
VAST Dynamics

*/

#include "VASTFXBus.h"
#include "VASTEngineHeader.h"
#include "../Plugin/VASTAudioProcessor.h"

CVASTFXBus::CVASTFXBus(VASTAudioProcessor* processor, CVASTSettings& set, int busnr) :
			myProcessor(processor),
			m_Set(&set),
			m_Oversampler(set),
			m_Oversampler2(set),
			m_Oversampler3(set),
			m_Eq(processor, set, busnr),
            m_Chorus(processor, set, busnr),
            m_Distortion(processor, set, busnr),
            m_CombFilter(processor, set, busnr),
            m_CompressorExpander(processor, set, busnr),
            m_MultibandCompressor(processor, set, busnr),
            m_Bitcrush(processor, set, busnr),
            m_FormantFilter(processor, set, busnr),
            m_Phaser(processor, set, busnr),
            m_Flanger(processor, set, busnr),
            m_Atomizer(processor, set, busnr),
            m_StereoDelay(processor, set, busnr),
            m_Reverb(processor, set, busnr),
			m_Waveshaper(processor, set, busnr)
{
	myProcessor = processor; 	
	myBusnr = busnr;

	insertEffect* l_eq = new insertEffect;
	//l_eq->isOn = false;
	l_eq->isChain = false;
	l_eq->needsOversampling = false;
	l_eq->effectPlugin = &m_Eq;
	effectBus.add(l_eq);
	mFXBusSequence.add(0);

	insertEffect* l_chorus = new insertEffect;
	//l_chorus->isOn = false;
	l_chorus->isChain = false;
	l_chorus->needsOversampling = false;
	l_chorus->effectPlugin = &m_Chorus;
	effectBus.add(l_chorus);
	mFXBusSequence.add(1);

	insertEffect* l_phaser = new insertEffect;
	//l_phaser->isOn = false;
	l_phaser->isChain = false;
	l_phaser->needsOversampling = false;
	l_phaser->effectPlugin = &m_Phaser;
	effectBus.add(l_phaser);
	mFXBusSequence.add(2);

	insertEffect* l_flanger = new insertEffect;
	//l_flanger->isOn = false;
	l_flanger->isChain = false;
	l_flanger->needsOversampling = false;
	l_flanger->effectPlugin = &m_Flanger;
	effectBus.add(l_flanger);
	mFXBusSequence.add(3);

	insertEffect* l_atomizer = new insertEffect;
	//l_atomizer->isOn = false;
	l_atomizer->isChain = false;
	l_atomizer->needsOversampling = false;
	l_atomizer->effectPlugin = &m_Atomizer;
	effectBus.add(l_atomizer);
	mFXBusSequence.add(4);

	insertEffect* l_distortion = new insertEffect;
	//l_distortion->isOn = false;
	l_distortion->isChain = false;
	l_distortion->needsOversampling = true;
	l_distortion->effectPlugin = &m_Distortion;
	effectBus.add(l_distortion);
	mFXBusSequence.add(5);

	insertEffect* l_bitcrush = new insertEffect;
	//l_bitcrush->isOn = false;
	l_bitcrush->isChain = false;
	l_bitcrush->needsOversampling = true;
	l_bitcrush->effectPlugin = &m_Bitcrush;
	effectBus.add(l_bitcrush);
	mFXBusSequence.add(6);

	insertEffect* l_combfilter = new insertEffect;
	//l_combfilter->isOn = false;
	l_combfilter->isChain = false;
	l_combfilter->needsOversampling = true;
	l_combfilter->effectPlugin = &m_CombFilter;
	effectBus.add(l_combfilter);
	mFXBusSequence.add(7);
	
	insertEffect* l_formantfilter = new insertEffect;
	//l_formantfilter->isOn = false;
	l_formantfilter->isChain = false;
	l_formantfilter->needsOversampling = false;
	l_formantfilter->effectPlugin = &m_FormantFilter;
	effectBus.add(l_formantfilter);
	mFXBusSequence.add(8);

	insertEffect* l_multibandcompressor = new insertEffect;
	//l_multibandcompressor->isOn = false;
	l_multibandcompressor->isChain = false;
	l_multibandcompressor->needsOversampling = false;
	l_multibandcompressor->effectPlugin = &m_MultibandCompressor;
	effectBus.add(l_multibandcompressor);
	mFXBusSequence.add(9);

	insertEffect* l_limiter = new insertEffect;
	//l_limiter->isOn = false;
	l_limiter->isChain = false;
	l_limiter->needsOversampling = false;
	l_limiter->effectPlugin = &m_CompressorExpander;
	effectBus.add(l_limiter);
	mFXBusSequence.add(10);

	insertEffect* l_stereodelay = new insertEffect;
	//l_stereodelay->isOn = false;
	l_stereodelay->isChain = false;
	l_stereodelay->needsOversampling = false;
	l_stereodelay->effectPlugin = &m_StereoDelay;
	effectBus.add(l_stereodelay);
	mFXBusSequence.add(11);

	insertEffect* l_reverb = new insertEffect;
	//l_reverb->isOn = false;
	l_reverb->isChain = false;
	l_reverb->needsOversampling = false;
	l_reverb->effectPlugin = &m_Reverb;
	effectBus.add(l_reverb);
	mFXBusSequence.add(12);

	insertEffect* l_waveshaper = new insertEffect;
	//l_distortion->isOn = false;
	l_waveshaper->isChain = false;
	l_waveshaper->needsOversampling = true;
	l_waveshaper->effectPlugin = &m_Waveshaper;
	effectBus.add(l_waveshaper);
	mFXBusSequence.add(13);

	//must only add new fx at the end
}

CVASTFXBus::~CVASTFXBus() { //destructor
	m_lowbandMono = nullptr;
	m_chainBuffer = nullptr;
	m_chainProc = nullptr;
	m_chainResult = nullptr;

	/*
	for (int i = 0; i < effectBus.size(); i++) {
		std::shared_ptr<insertEffect> fx = effectBus[i];
		//fx.reset();
		delete fx->effectPlugin.get();
	}
	*/
}

void CVASTFXBus::init() {
	m_Oversampler.init();
	m_Oversampler2.init();
	m_Oversampler3.init();
	int initSize = 16;
	
	m_inBufferOversampled = std::make_unique<AudioSampleBuffer>(2, initSize);
	m_lowbandMono = std::make_unique<AudioSampleBuffer>(1, initSize);
	m_chainBuffer = std::make_unique<AudioSampleBuffer>(2, initSize);
	m_chainResult = std::make_unique<AudioSampleBuffer>(2, initSize);
	m_chainProc = std::make_unique<AudioSampleBuffer>(2, initSize);
	mFXBusSequence.clear();
	for (int i = 0; i < effectBus.size(); i++) {
		effectBus[i]->effectPlugin->init();
		mFXBusSequence.add(i); //initial sequence
	}
}

void CVASTFXBus::initCompatibilityParameters() {
	for (int i = 0; i < effectBus.size(); i++) {
		effectBus[i]->effectPlugin->initCompatibilityParameters();
	}
}

void CVASTFXBus::initCompatibilityParameters5() {
	for (int i = 0; i < effectBus.size(); i++) {
		effectBus[i]->effectPlugin->initCompatibilityParameters5();
	}
}

void CVASTFXBus::updateTiming() {
	for (int i = 0; i < effectBus.size(); i++) {
		effectBus[i]->effectPlugin->updateTiming();
	}
}

void CVASTFXBus::prepareToPlay(double sampleRate, int samplesPerBlock) {	
	/*
	bool l_oversample = false;
	switch (myBusnr) {
	case 0:
		l_oversample = *m_Set->m_State->m_bOversampleBus;
		break;
	case 1:
		l_oversample = *m_Set->m_State->m_bOversampleBus_Bus2;
		break;
	case 2:
		l_oversample = *m_Set->m_State->m_bOversampleBus_Bus3;
		break;
	}
	*/

	//resize buffers
	m_inBufferOversampled->setSize(2, samplesPerBlock * C_OVERSAMPLING_RATIO, false, false, false); // release memory
	m_lowbandMono->setSize(1, samplesPerBlock, false, false, false); // release memory
	m_chainBuffer->setSize(2, samplesPerBlock, false, false, false); // release memory
	m_chainResult->setSize(2, samplesPerBlock, false, false, false); // release memory
	m_chainProc->setSize(2, samplesPerBlock, false, false, false); // release memory
	
	// Multiband processing
	m_lowPassBiQuadL.flushDelays();
	m_lowPassBiQuadR.flushDelays();

	m_Oversampler.prepareForPlay();
	m_Oversampler2.prepareForPlay();
	m_Oversampler3.prepareForPlay();

	for (int i = 0; i < effectBus.size(); i++) {
		//effectBus[i]->effectPlugin->useOversampling(m_Set->m_nSampleRate, effectBus[i]->effectPlugin);
		effectBus[i]->effectPlugin->useOversampling(m_Set->m_nSampleRate, effectBus[i]->needsOversampling);
		effectBus[i]->effectPlugin->prepareToPlay(m_Set->m_nSampleRate, samplesPerBlock);
	}	
}

void CVASTFXBus::swapSlots(int first, int second) {
	mFXBusSequence.swap(first, second);
}

int CVASTFXBus::getSequence(int slot) const {
	return mFXBusSequence[slot];
}

int CVASTFXBus::findPosition(int slot) {
	for (int i = 0; i < mFXBusSequence.size(); i++) {
		if (mFXBusSequence[i] == slot) return i;
	}
	return -1;
}

void CVASTFXBus::processBuffers(sRoutingBuffers& routingBuffers, MidiBuffer& midiBuffer) {
	modMatrixInputState inputState;
	inputState = myProcessor->m_pVASTXperience.m_Poly.getOldestNotePlayedInputState(0); // make parameter oldest or newest

	AudioSampleBuffer* inBuffer;
	//bool l_oversample = false;
	float l_monoFrequency = 0.0f;
	switch (myBusnr) {
	case 0:
		inBuffer = routingBuffers.FxBusBuffer[0].get();
		//l_oversample = *m_Set->m_State->m_bOversampleBus;
		l_monoFrequency = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fMBMonoFrequency, MODMATDEST::Fxbus1MonoFrequency, &inputState);

		//Do Routing
		//bus1
		if ((*m_Set->m_State->m_uOscRouting1_OscA == static_cast<int>(OSCROUTE::OSCROUTE_FXBus1)) || (*m_Set->m_State->m_uOscRouting2_OscA == static_cast<int>(OSCROUTE::OSCROUTE_FXBus1))) {
			routingBuffers.FxBusBuffer[0]->addFrom(0, 0, routingBuffers.OscBuffer[0]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[0]->addFrom(1, 0, routingBuffers.OscBuffer[0]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if ((*m_Set->m_State->m_uOscRouting1_OscB == static_cast<int>(OSCROUTE::OSCROUTE_FXBus1)) || (*m_Set->m_State->m_uOscRouting2_OscB == static_cast<int>(OSCROUTE::OSCROUTE_FXBus1))) {
			routingBuffers.FxBusBuffer[0]->addFrom(0, 0, routingBuffers.OscBuffer[1]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[0]->addFrom(1, 0, routingBuffers.OscBuffer[1]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if ((*m_Set->m_State->m_uOscRouting1_OscC == static_cast<int>(OSCROUTE::OSCROUTE_FXBus1)) || (*m_Set->m_State->m_uOscRouting2_OscC == static_cast<int>(OSCROUTE::OSCROUTE_FXBus1))) {
			routingBuffers.FxBusBuffer[0]->addFrom(0, 0, routingBuffers.OscBuffer[2]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[0]->addFrom(1, 0, routingBuffers.OscBuffer[2]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if ((*m_Set->m_State->m_uOscRouting1_OscD == static_cast<int>(OSCROUTE::OSCROUTE_FXBus1)) || (*m_Set->m_State->m_uOscRouting2_OscD == static_cast<int>(OSCROUTE::OSCROUTE_FXBus1))) {
			routingBuffers.FxBusBuffer[0]->addFrom(0, 0, routingBuffers.OscBuffer[3]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[0]->addFrom(1, 0, routingBuffers.OscBuffer[3]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if ((*m_Set->m_State->m_uNoiseRouting1 == static_cast<int>(OSCROUTE::OSCROUTE_FXBus1)) || (*m_Set->m_State->m_uNoiseRouting2 == static_cast<int>(OSCROUTE::OSCROUTE_FXBus1))) {
			routingBuffers.FxBusBuffer[0]->addFrom(0, 0, routingBuffers.NoiseBuffer->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[0]->addFrom(1, 0, routingBuffers.NoiseBuffer->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if ((*m_Set->m_State->m_uSamplerRouting1 == static_cast<int>(OSCROUTE::OSCROUTE_FXBus1)) || (*m_Set->m_State->m_uSamplerRouting2 == static_cast<int>(OSCROUTE::OSCROUTE_FXBus1))) {
			routingBuffers.FxBusBuffer[0]->addFrom(0, 0, routingBuffers.SamplerBuffer->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[0]->addFrom(1, 0, routingBuffers.SamplerBuffer->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFilterRouting_Filter1 == static_cast<int>(FILTER1ROUTE::FILTER1ROUTE_FXBus1)) {
			routingBuffers.FxBusBuffer[0]->addFrom(0, 0, routingBuffers.FilterBuffer[0]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[0]->addFrom(1, 0, routingBuffers.FilterBuffer[0]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFilterRouting2_Filter1 == static_cast<int>(FILTER1ROUTE::FILTER1ROUTE_FXBus1)) {
			routingBuffers.FxBusBuffer[0]->addFrom(0, 0, routingBuffers.FilterBuffer[0]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[0]->addFrom(1, 0, routingBuffers.FilterBuffer[0]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFilterRouting_Filter2 == static_cast<int>(FILTER2ROUTE::FILTER2ROUTE_FXBus1)) {
			routingBuffers.FxBusBuffer[0]->addFrom(0, 0, routingBuffers.FilterBuffer[1]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[0]->addFrom(1, 0, routingBuffers.FilterBuffer[1]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFilterRouting2_Filter2 == static_cast<int>(FILTER2ROUTE::FILTER2ROUTE_FXBus1)) {
			routingBuffers.FxBusBuffer[0]->addFrom(0, 0, routingBuffers.FilterBuffer[1]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[0]->addFrom(1, 0, routingBuffers.FilterBuffer[1]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFilterRouting_Filter3 == static_cast<int>(FILTER3ROUTE::FILTER3ROUTE_FXBus1)) {
			routingBuffers.FxBusBuffer[0]->addFrom(0, 0, routingBuffers.FilterBuffer[2]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[0]->addFrom(1, 0, routingBuffers.FilterBuffer[2]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFilterRouting2_Filter3 == static_cast<int>(FILTER3ROUTE::FILTER3ROUTE_FXBus1)) {
			routingBuffers.FxBusBuffer[0]->addFrom(0, 0, routingBuffers.FilterBuffer[2]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[0]->addFrom(1, 0, routingBuffers.FilterBuffer[2]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		break;
	case 1:
		inBuffer = routingBuffers.FxBusBuffer[1].get();
		//l_oversample = *m_Set->m_State->m_bOversampleBus_Bus2;
		l_monoFrequency = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fMBMonoFrequency_Bus2, MODMATDEST::Fxbus2MonoFrequency, &inputState);

		//Do Routing
		//bus2
		if ((*m_Set->m_State->m_uOscRouting1_OscA == static_cast<int>(OSCROUTE::OSCROUTE_FXBus2)) || (*m_Set->m_State->m_uOscRouting2_OscA == static_cast<int>(OSCROUTE::OSCROUTE_FXBus2))) {
			routingBuffers.FxBusBuffer[1]->addFrom(0, 0, routingBuffers.OscBuffer[0]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[1]->addFrom(1, 0, routingBuffers.OscBuffer[0]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if ((*m_Set->m_State->m_uOscRouting1_OscB == static_cast<int>(OSCROUTE::OSCROUTE_FXBus2)) || (*m_Set->m_State->m_uOscRouting2_OscB == static_cast<int>(OSCROUTE::OSCROUTE_FXBus2))) {
			routingBuffers.FxBusBuffer[1]->addFrom(0, 0, routingBuffers.OscBuffer[1]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[1]->addFrom(1, 0, routingBuffers.OscBuffer[1]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if ((*m_Set->m_State->m_uOscRouting1_OscC == static_cast<int>(OSCROUTE::OSCROUTE_FXBus2)) || (*m_Set->m_State->m_uOscRouting2_OscC == static_cast<int>(OSCROUTE::OSCROUTE_FXBus2))) {
			routingBuffers.FxBusBuffer[1]->addFrom(0, 0, routingBuffers.OscBuffer[2]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[1]->addFrom(1, 0, routingBuffers.OscBuffer[2]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if ((*m_Set->m_State->m_uOscRouting1_OscD == static_cast<int>(OSCROUTE::OSCROUTE_FXBus2)) || (*m_Set->m_State->m_uOscRouting2_OscD == static_cast<int>(OSCROUTE::OSCROUTE_FXBus2))) {
			routingBuffers.FxBusBuffer[1]->addFrom(0, 0, routingBuffers.OscBuffer[3]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[1]->addFrom(1, 0, routingBuffers.OscBuffer[3]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if ((*m_Set->m_State->m_uNoiseRouting1 == static_cast<int>(OSCROUTE::OSCROUTE_FXBus2)) || (*m_Set->m_State->m_uNoiseRouting2 == static_cast<int>(OSCROUTE::OSCROUTE_FXBus2))) {
			routingBuffers.FxBusBuffer[1]->addFrom(0, 0, routingBuffers.NoiseBuffer->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[1]->addFrom(1, 0, routingBuffers.NoiseBuffer->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if ((*m_Set->m_State->m_uSamplerRouting1 == static_cast<int>(OSCROUTE::OSCROUTE_FXBus2)) || (*m_Set->m_State->m_uSamplerRouting2 == static_cast<int>(OSCROUTE::OSCROUTE_FXBus2))) {
			routingBuffers.FxBusBuffer[1]->addFrom(0, 0, routingBuffers.SamplerBuffer->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[1]->addFrom(1, 0, routingBuffers.SamplerBuffer->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFilterRouting_Filter1 == static_cast<int>(FILTER1ROUTE::FILTER1ROUTE_FXBus2)) {
			routingBuffers.FxBusBuffer[1]->addFrom(0, 0, routingBuffers.FilterBuffer[0]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[1]->addFrom(1, 0, routingBuffers.FilterBuffer[0]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFilterRouting2_Filter1 == static_cast<int>(FILTER1ROUTE::FILTER1ROUTE_FXBus2)) {
			routingBuffers.FxBusBuffer[1]->addFrom(0, 0, routingBuffers.FilterBuffer[0]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[1]->addFrom(1, 0, routingBuffers.FilterBuffer[0]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFilterRouting_Filter2 == static_cast<int>(FILTER2ROUTE::FILTER2ROUTE_FXBus2)) {
			routingBuffers.FxBusBuffer[1]->addFrom(0, 0, routingBuffers.FilterBuffer[1]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[1]->addFrom(1, 0, routingBuffers.FilterBuffer[1]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFilterRouting2_Filter2 == static_cast<int>(FILTER2ROUTE::FILTER2ROUTE_FXBus2)) {
			routingBuffers.FxBusBuffer[1]->addFrom(0, 0, routingBuffers.FilterBuffer[1]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[1]->addFrom(1, 0, routingBuffers.FilterBuffer[1]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFilterRouting_Filter3 == static_cast<int>(FILTER3ROUTE::FILTER3ROUTE_FXBus2)) {
			routingBuffers.FxBusBuffer[1]->addFrom(0, 0, routingBuffers.FilterBuffer[2]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[1]->addFrom(1, 0, routingBuffers.FilterBuffer[2]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFilterRouting2_Filter3 == static_cast<int>(FILTER3ROUTE::FILTER3ROUTE_FXBus2)) {
			routingBuffers.FxBusBuffer[1]->addFrom(0, 0, routingBuffers.FilterBuffer[2]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[1]->addFrom(1, 0, routingBuffers.FilterBuffer[2]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFxBusRouting == static_cast<int>(FXBUSROUTE::FXBUSROUTE_FXBus2)) {
			routingBuffers.FxBusBuffer[1]->addFrom(0, 0, routingBuffers.FxBusBuffer[0]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[1]->addFrom(1, 0, routingBuffers.FxBusBuffer[0]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		break;
	case 2:
		inBuffer = routingBuffers.FxBusBuffer[2].get();
		//l_oversample = *m_Set->m_State->m_bOversampleBus_Bus3;
		l_monoFrequency = m_Set->getParameterValueWithMatrixModulation(m_Set->m_State->m_fMBMonoFrequency_Bus3, MODMATDEST::Fxbus3MonoFrequency, &inputState);

		//Do Routing
		//bus3
		if ((*m_Set->m_State->m_uOscRouting1_OscA == static_cast<int>(OSCROUTE::OSCROUTE_FXBus3)) || (*m_Set->m_State->m_uOscRouting2_OscA == static_cast<int>(OSCROUTE::OSCROUTE_FXBus3))) {
			routingBuffers.FxBusBuffer[2]->addFrom(0, 0, routingBuffers.OscBuffer[0]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[2]->addFrom(1, 0, routingBuffers.OscBuffer[0]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if ((*m_Set->m_State->m_uOscRouting1_OscB == static_cast<int>(OSCROUTE::OSCROUTE_FXBus3)) || (*m_Set->m_State->m_uOscRouting2_OscB == static_cast<int>(OSCROUTE::OSCROUTE_FXBus3))) {
			routingBuffers.FxBusBuffer[2]->addFrom(0, 0, routingBuffers.OscBuffer[1]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[2]->addFrom(1, 0, routingBuffers.OscBuffer[1]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if ((*m_Set->m_State->m_uOscRouting1_OscC == static_cast<int>(OSCROUTE::OSCROUTE_FXBus3)) || (*m_Set->m_State->m_uOscRouting2_OscC == static_cast<int>(OSCROUTE::OSCROUTE_FXBus3))) {
			routingBuffers.FxBusBuffer[2]->addFrom(0, 0, routingBuffers.OscBuffer[2]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[2]->addFrom(1, 0, routingBuffers.OscBuffer[2]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if ((*m_Set->m_State->m_uOscRouting1_OscD == static_cast<int>(OSCROUTE::OSCROUTE_FXBus3)) || (*m_Set->m_State->m_uOscRouting2_OscD == static_cast<int>(OSCROUTE::OSCROUTE_FXBus3))) {
			routingBuffers.FxBusBuffer[2]->addFrom(0, 0, routingBuffers.OscBuffer[3]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[2]->addFrom(1, 0, routingBuffers.OscBuffer[3]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if ((*m_Set->m_State->m_uNoiseRouting1 == static_cast<int>(OSCROUTE::OSCROUTE_FXBus3)) || (*m_Set->m_State->m_uNoiseRouting2 == static_cast<int>(OSCROUTE::OSCROUTE_FXBus3))) {
			routingBuffers.FxBusBuffer[2]->addFrom(0, 0, routingBuffers.NoiseBuffer->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[2]->addFrom(1, 0, routingBuffers.NoiseBuffer->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if ((*m_Set->m_State->m_uSamplerRouting1 == static_cast<int>(OSCROUTE::OSCROUTE_FXBus3)) || (*m_Set->m_State->m_uSamplerRouting2 == static_cast<int>(OSCROUTE::OSCROUTE_FXBus3))) {
			routingBuffers.FxBusBuffer[2]->addFrom(0, 0, routingBuffers.SamplerBuffer->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[2]->addFrom(1, 0, routingBuffers.SamplerBuffer->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFilterRouting_Filter1 == static_cast<int>(FILTER1ROUTE::FILTER1ROUTE_FXBus3)) {
			routingBuffers.FxBusBuffer[2]->addFrom(0, 0, routingBuffers.FilterBuffer[0]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[2]->addFrom(1, 0, routingBuffers.FilterBuffer[0]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFilterRouting2_Filter1 == static_cast<int>(FILTER1ROUTE::FILTER1ROUTE_FXBus3)) {
			routingBuffers.FxBusBuffer[2]->addFrom(0, 0, routingBuffers.FilterBuffer[0]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[2]->addFrom(1, 0, routingBuffers.FilterBuffer[0]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFilterRouting_Filter2 == static_cast<int>(FILTER2ROUTE::FILTER2ROUTE_FXBus3)) {
			routingBuffers.FxBusBuffer[2]->addFrom(0, 0, routingBuffers.FilterBuffer[1]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[2]->addFrom(1, 0, routingBuffers.FilterBuffer[1]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFilterRouting2_Filter2 == static_cast<int>(FILTER2ROUTE::FILTER2ROUTE_FXBus3)) {
			routingBuffers.FxBusBuffer[2]->addFrom(0, 0, routingBuffers.FilterBuffer[1]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[2]->addFrom(1, 0, routingBuffers.FilterBuffer[1]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFilterRouting_Filter3 == static_cast<int>(FILTER3ROUTE::FILTER3ROUTE_FXBus3)) {
			routingBuffers.FxBusBuffer[2]->addFrom(0, 0, routingBuffers.FilterBuffer[2]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[2]->addFrom(1, 0, routingBuffers.FilterBuffer[2]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFilterRouting2_Filter3 == static_cast<int>(FILTER3ROUTE::FILTER3ROUTE_FXBus3)) {
			routingBuffers.FxBusBuffer[2]->addFrom(0, 0, routingBuffers.FilterBuffer[2]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[2]->addFrom(1, 0, routingBuffers.FilterBuffer[2]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFxBusRouting == static_cast<int>(FXBUSROUTE::FXBUSROUTE_FXBus3)) {
			routingBuffers.FxBusBuffer[2]->addFrom(0, 0, routingBuffers.FxBusBuffer[0]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[2]->addFrom(1, 0, routingBuffers.FxBusBuffer[0]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		if (*m_Set->m_State->m_uFxBusRouting_Bus2 == static_cast<int>(FXBUSROUTE::FXBUSROUTE_FXBus3)) {
			routingBuffers.FxBusBuffer[2]->addFrom(0, 0, routingBuffers.FxBusBuffer[1]->getReadPointer(0, 0), routingBuffers.getNumSamples()); //gain??
			routingBuffers.FxBusBuffer[2]->addFrom(1, 0, routingBuffers.FxBusBuffer[1]->getReadPointer(1, 0), routingBuffers.getNumSamples()); //gain??
		}
		break;
	default:
        jassert(false);
        return;
    }
		
	//=================================================================================================	

	//=================================================================================================	
	//Stereo Multiband
	bool bLowBandCalculated = false;
	if (l_monoFrequency > 0.0f) {
		if (!m_lastCycleLowBandCalculated) {
			m_lowPassBiQuadL.flushDelays();
			m_lowPassBiQuadR.flushDelays();
		}
		m_lastCycleLowBandCalculated = true;
		bLowBandCalculated = true;
		m_lowbandMono->setSize(1, routingBuffers.getNumSamples(), false, false, true);
		m_lowbandMono->clear();
		//see http://www.earlevel.com/main/2013/10/13/biquad-calculator-v2/

		float fFrequency = l_monoFrequency + 50.0f; //minimum 50hz
		m_lowPassBiQuadL.calcBiquad(CVASTBiQuad::HIGHSHELF, fFrequency, m_Set->m_nSampleRate, 0.707f, -18.0f);
		m_lowPassBiQuadR.calcBiquad(CVASTBiQuad::HIGHSHELF, fFrequency, m_Set->m_nSampleRate, 0.707f, -18.0f);
		for (int i = 0; i < routingBuffers.getNumSamples(); i++) {
			float lLowL = m_lowPassBiQuadL.doBiQuad(inBuffer->getSample(0, i));
			float lLowR = m_lowPassBiQuadR.doBiQuad(inBuffer->getSample(1, i));
			inBuffer->setSample(0, i, inBuffer->getSample(0, i) - lLowL);
			inBuffer->setSample(1, i, inBuffer->getSample(1, i) - lLowR);
			m_lowbandMono->setSample(0, i, (lLowL + lLowR) * 0.5f);
		}
	}
	else
		m_lastCycleLowBandCalculated = false;

	/*
	//=================================================================================================	
	if (l_oversample) {
		m_inBufferOversampled->setSize(2, routingBuffers.getNumSamples() * C_OVERSAMPLING_RATIO, false, false, true);
		m_inBufferOversampled->clear();
		m_Oversampler.upsampleAudioBuffer(dsp::AudioBlock<float>(*inBuffer), dsp::AudioBlock<float>(*m_inBufferOversampled), routingBuffers.getNumSamples());
	}
	*/

	//=================================================================================================
	bool b_upsampled = false;
	int useOversampler = 0;
	for (int i = 0; i < effectBus.size(); i++) {

		int curFXno = mFXBusSequence[i];

		bool l_oversample = effectBus[curFXno]->needsOversampling;

		//=================================================================================================
		//main

		if (l_oversample)  {
			
			effectBus[curFXno]->effectPlugin->useOversampling(m_Set->m_nSampleRate, true);
			
			if (!b_upsampled) {
				if (!effectBus[curFXno]->effectPlugin->isOffAndShallBeOff()) { //upsamle only if not off to save cpu
					//if (!b_upsampled) {
					m_inBufferOversampled->setSize(2, routingBuffers.getNumSamples() * C_OVERSAMPLING_RATIO, false, false, true);
					m_inBufferOversampled->clear();
					m_Oversampler.upsampleAudioBuffer4(dsp::AudioBlock<float>(*inBuffer), dsp::AudioBlock<float>(*m_inBufferOversampled), routingBuffers.getNumSamples());
					b_upsampled = true;
				}
			}

			/*
			//chain
			if (effectBus[curFXno]->isChain && !effectBus[mFXBusSequence[i - 1]]->isChain) { //chain must not be first
				m_chainBuffer->setSize(2, m_inBufferOversampled->getNumSamples(), false, false, true);
				m_chainResult->setSize(2, m_inBufferOversampled->getNumSamples(), false, false, true);
				m_chainResult->clear();
				m_chainProc->setSize(2, m_inBufferOversampled->getNumSamples(), false, false, true);
				m_chainBuffer->copyFrom(0, 0, m_inBufferOversampled->getReadPointer(0, 0), m_inBufferOversampled->getNumSamples());
				m_chainBuffer->copyFrom(1, 0, m_inBufferOversampled->getReadPointer(1, 0), m_inBufferOversampled->getNumSamples());
				m_chainEffects = 0;
			}

			//effect
			if (!effectBus[curFXno]->isChain)
			*/

			effectBus[curFXno]->effectPlugin->processBlock(*m_inBufferOversampled, midiBuffer, m_inBufferOversampled->getNumSamples());

			/*
			else {
				m_chainResult->copyFrom(0, 0, m_chainBuffer->getReadPointer(0, 0), m_chainBuffer->getNumSamples());
				m_chainResult->copyFrom(1, 0, m_chainBuffer->getReadPointer(1, 0), m_chainBuffer->getNumSamples());

				effectBus[curFXno]->effectPlugin->processBlock(*m_chainProc, midiBuffer, m_chainBuffer->getNumSamples());

				if (effectBus[curFXno]->effectPlugin->isOffAndShallBeOff() == false) {
					m_chainEffects++;
					m_inBufferOversampled->addFrom(0, 0, m_chainProc->getReadPointer(0, 0), m_chainProc->getNumSamples());
					m_inBufferOversampled->addFrom(1, 0, m_chainProc->getReadPointer(1, 0), m_chainProc->getNumSamples());
				}

				if (!effectBus[mFXBusSequence[i + 1]]->isChain) { //last in chain
					if (m_chainEffects > 0) {
						m_inBufferOversampled->copyFrom(0, 0, m_chainResult->getReadPointer(0, 0), m_chainResult->getNumSamples(), M_SQRT2 / m_chainEffects);
						m_inBufferOversampled->copyFrom(1, 0, m_chainResult->getReadPointer(1, 0), m_chainResult->getNumSamples(), M_SQRT2 / m_chainEffects);
					}
					else {
						m_inBufferOversampled->copyFrom(0, 0, m_chainBuffer->getReadPointer(0, 0), m_chainBuffer->getNumSamples());
						m_inBufferOversampled->copyFrom(1, 0, m_chainBuffer->getReadPointer(1, 0), m_chainBuffer->getNumSamples());
					}
				}
			}
			*/
		
			if (b_upsampled) {
				if ((i == (mFXBusSequence.size() - 1)) || (!
					((effectBus[mFXBusSequence[i + 1]]->needsOversampling) && !(effectBus[mFXBusSequence[i + 1]]->effectPlugin->isOffAndShallBeOff())))
					) { //last in oversampling row
					//=================================================================================================
					//Downsample factor n

					CVASTOversampler* lOversampler;
					if (useOversampler == 0)
						lOversampler = &m_Oversampler;
					else if (useOversampler == 1)
						lOversampler = &m_Oversampler2;
					else if (useOversampler == 2)
						lOversampler = &m_Oversampler3;
                    else {
                        lOversampler = &m_Oversampler;
                        vassertfalse;
                    }
					lOversampler->downsampleAudioBuffer4(dsp::AudioBlock<float>(*m_inBufferOversampled), dsp::AudioBlock<float>(*inBuffer), m_inBufferOversampled->getNumSamples());
					b_upsampled = false;
					useOversampler++;
				}
			}
		}
		else {
			
			effectBus[curFXno]->effectPlugin->useOversampling(m_Set->m_nSampleRate, false);

			/*
			//chain
			if (effectBus[curFXno]->isChain && !effectBus[mFXBusSequence[i - 1]]->isChain) { //chain must not be first
				m_chainBuffer->setSize(2, routingBuffers.getNumSamples(), false, false, true);
				m_chainResult->setSize(2, routingBuffers.getNumSamples(), false, false, true);
				m_chainResult->clear();
				m_chainProc->setSize(2, routingBuffers.getNumSamples(), false, false, true);
				m_chainBuffer->copyFrom(0, 0, inBuffer->getReadPointer(0, 0), routingBuffers.getNumSamples());
				m_chainBuffer->copyFrom(1, 0, inBuffer->getReadPointer(1, 0), routingBuffers.getNumSamples());
				m_chainEffects = 0;
			}
			*/

			//effect
			//if (!effectBus[curFXno]->isChain)
									
			effectBus[curFXno]->effectPlugin->processBlock(*inBuffer, midiBuffer, routingBuffers.getNumSamples());
			
			/*
			else {
				m_chainProc->copyFrom(0, 0, m_chainBuffer->getReadPointer(0, 0), m_chainBuffer->getNumSamples());
				m_chainProc->copyFrom(1, 0, m_chainBuffer->getReadPointer(1, 0), m_chainBuffer->getNumSamples());

				effectBus[curFXno]->effectPlugin->processBlock(*m_chainProc, midiBuffer, routingBuffers.getNumSamples());

				if (effectBus[curFXno]->effectPlugin->isOffAndShallBeOff() == false) {
					m_chainEffects++;
					m_chainResult->addFrom(0, 0, m_chainProc->getReadPointer(0, 0), m_chainProc->getNumSamples());
					m_chainResult->addFrom(1, 0, m_chainProc->getReadPointer(1, 0), m_chainProc->getNumSamples());
				}

				if (!effectBus[mFXBusSequence[i + 1]]->isChain) { //last in chain
					if (m_chainEffects > 0) {
						inBuffer->copyFrom(0, 0, m_chainResult->getReadPointer(0, 0), m_chainResult->getNumSamples(), M_SQRT2 / m_chainEffects);
						inBuffer->copyFrom(1, 0, m_chainResult->getReadPointer(1, 0), m_chainResult->getNumSamples(), M_SQRT2 / m_chainEffects);
					}
					else {
						inBuffer->copyFrom(0, 0, m_chainBuffer->getReadPointer(0, 0), m_chainBuffer->getNumSamples());
						inBuffer->copyFrom(1, 0, m_chainBuffer->getReadPointer(1, 0), m_chainBuffer->getNumSamples());
					}
				}
			}
			*/

		}
		
		//=================================================================================================	
#ifdef _DEBUG
		//Safety check
		Range<float> minmaxL = inBuffer->findMinMax(0, 0, routingBuffers.getNumSamples());
		Range<float> minmaxR = inBuffer->findMinMax(1, 0, routingBuffers.getNumSamples());
		if ((minmaxL.getStart() <= -3.0f) && (minmaxL.getEnd() >= 3.0f) &&
			(minmaxR.getStart() <= -3.0f) && (minmaxR.getEnd() >= 3.0f)) {
			//jassert(false);
		}
#endif
	}

	/*
	//=================================================================================================
	//Downsample factor n
	if (l_oversample) {
	m_Oversampler.downsampleAudioBuffer(dsp::AudioBlock<float>(*m_inBufferOversampled), dsp::AudioBlock<float>(*inBuffer), m_inBufferOversampled->getNumSamples());
	}
	*/

	//=================================================================================================	
	//Stereo Multiband
	if (bLowBandCalculated == true) {
		FloatVectorOperations::add(inBuffer->getWritePointer(0, 0), m_lowbandMono->getReadPointer(0, 0), routingBuffers.getNumSamples()); //is mono
		FloatVectorOperations::add(inBuffer->getWritePointer(1, 0), m_lowbandMono->getReadPointer(0, 0), routingBuffers.getNumSamples()); //is mono
	}
}

void CVASTFXBus::initSequence() {
	mFXBusSequence.clear();
	for (int i = 0; i < effectBus.size(); i++) {
		mFXBusSequence.add(i); //initial sequence
	}
}

//-------------------------------------------------------------------------------------------
// valueTree state

void CVASTFXBus::getValueTreeState(ValueTree* tree, UndoManager* undoManager) { //save
	tree->removeAllChildren(undoManager);
	tree->removeAllProperties(undoManager);

	tree->setProperty("numFX", int(mFXBusSequence.size()), undoManager);
	for (int i = 0; i < mFXBusSequence.size(); i++) {
        std::unique_ptr<ValueTree> subtree(new ValueTree(Identifier("fxSequence" + String(i))));
		subtree->setProperty("fxNo", mFXBusSequence[i], undoManager);
		tree->appendChild(*subtree.get(), undoManager);
	}
}

void CVASTFXBus::setValueTreeState(ValueTree* tree) { //load
	initSequence();

	//points
	int numFX = tree->getProperty("numFX");
	for (int i = 0; i < numFX; i++) {
		ValueTree subtree = tree->getChildWithName(Identifier("fxSequence" + String(i)));
		vassert(subtree.isValid());
		mFXBusSequence.set(i, subtree.getProperty("fxNo"));
	}

	//check sanity
	bool invalid = false;
	for (int i = 0; i < effectBus.size(); i++) {
		if (findPosition(i) < 0) 
			invalid = true;
	}
	if (invalid)
		initSequence();
}
//-------------------------------------------------------------------------------------------
