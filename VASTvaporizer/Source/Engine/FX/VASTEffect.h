
/*
	VAST Dynamics Audio Software (TM)
*/

#pragma once

#include "../VASTEngineHeader.h"
#include "../VASTPluginConstants.h"
#include <map> 

enum VASTParameterType {toggleButtonParameter = 100, sliderParameter = 101, choiceParameter = 102};

class VASTAudioProcessor; //forward declaration
class CVASTSettings; //forward declaration
class CVASTEffect : public AudioProcessorValueTreeState::Listener {
public:
	//interface
	virtual void init(CVASTSettings &set) = 0;
	virtual void initCompatibilityParameters() = 0; //new parameters go here
	virtual void initCompatibilityParameters5() = 0; //new parameters go here
	virtual void prepareToPlay(double sampleRate, int expedtedSamplesPerBlock) = 0;
	//virtual void reset() = 0; //called initially and when switched off
	virtual void updateTiming() = 0;
	virtual void releaseResources() = 0;
	virtual void processBlock(AudioSampleBuffer& audioSampleBuffer, MidiBuffer& midiBuffer, const int numSamples) = 0;
	virtual void getStateInformation(MemoryBlock& destData) = 0;
	virtual void setStateInformation(const void* data, int sizeInBytes) = 0;

	virtual void parameterChanged(const String& parameterID, float newValue) = 0;
	//interface
		
	void setEffectName(String effectName) { m_effectName = effectName;  };
	String getEffectName() { return m_effectName; };
	
	const Array<AudioProcessorParameterWithID*> getParameters() {  //returns them in display sequence from left to right
		//return my_parameters;

		Array<AudioProcessorParameterWithID*> params;
		std::multimap<int, AudioProcessorParameterWithID*> ::iterator itr;
		for (itr = my_parameters.begin(); itr != my_parameters.end(); ++itr)
		{
			params.add(itr->second);
		}
		return params;		
	}

	void setOnOffParameter(const String& paramID) {
		m_onOffParamID = paramID;
	}

	String getOnOffParameter() {
		return m_onOffParamID;
	}

	void createAndAddParameter(std::atomic<float>** parameterVar, AudioProcessorValueTreeState& stateTree, const String& paramID, const String& paramName,
		const String& labelText, int uiSequence,
		int modMatrixDestination,
		NormalisableRange<float> r,
		float defaultVal, std::function<String(float)> valueToTextFunction,
		std::function<float(const String&)> textToValueFunction,
		bool isMetaParameter,
		bool isAutomatableParameter,
		bool isDiscreteParameter,
		bool isOnOffParameter,
		bool addListener = false);


	virtual void useOversampling(int sampleRate, bool oversample) {
		if (m_bOversampling)
			m_iSampleRate = sampleRate * C_OVERSAMPLING_RATIO;
		else 
			m_iSampleRate = sampleRate;

		if (oversample != m_bOversampling) {
			m_bOversampling = oversample;
			if (m_bOversampling) {
				prepareToPlay(m_iSampleRate, m_iExpectedSamplesPerBlock);
			}
			else {
				prepareToPlay(m_iSampleRate, m_iExpectedSamplesPerBlock);
			}
		}
	}

	virtual bool isOff() {
		return m_bIsOff || m_bShallBeOff;
	}

	virtual void switchOff();
	
	virtual void switchOn();
	
	bool isOffAndShallBeOff() { 
		return m_bIsOff && m_bShallBeOff; 
	};

	bool shallBeOff() { 
		return m_bShallBeOff; 
	};

	virtual void checkSoftFade() {
		if (m_bShallBeOff == true) {
			if (m_bIsOff == true) {
				m_iSoftFade = 0;
				return;
			}
			if (m_iSoftFade <= 0) {
				//reset();
				m_bIsOff = true;
				m_iSoftFade = 0;
			}
			else
				m_iSoftFade--;
		}
		else { //shall be on
			if (m_bIsOff == false) {
				m_iSoftFade = C_MAX_SOFTFADE;
				return;
			}
			if (m_iSoftFade >= C_MAX_SOFTFADE) {
				m_bIsOff = false;
				m_iSoftFade = C_MAX_SOFTFADE;
			} else 			
				m_iSoftFade++;
		}
	}

	bool m_bOversampling = false;
	int m_iSampleRate = 41000;
	int m_iExpectedSamplesPerBlock = 100;
	bool m_bIsOff = false;
	bool m_bShallBeOff = false;
	int m_iSoftFade = 0;
	int myBusnr = 0;
	
	VASTAudioProcessor *my_processor;

protected:
	CVASTSettings *m_Set;

private:
	//Array<AudioProcessorParameterWithID*> my_parameters;  
	std::multimap<int, AudioProcessorParameterWithID*> my_parameters;

	String m_effectName = "";
	String m_onOffParamID = "";

	JUCE_LEAK_DETECTOR(CVASTEffect)
};