
/*
	VAST Dynamics Audio Software (TM)
*/

#include "VASTEffect.h"
#include "../../Plugin/VASTAudioProcessor.h"

void CVASTEffect::createAndAddParameter(std::atomic<float>** parameterVar, AudioProcessorValueTreeState& stateTree, const String& paramID, const String& paramName,
	const String& labelText, int uiSequence,
	int modMatrixDestination,
	NormalisableRange<float> r,
	float defaultVal, std::function<String(float)> valueToTextFunction,
	std::function<float(const String&)> textToValueFunction,
	bool isMetaParameter,
	bool isAutomatableParameter,
	bool isDiscreteParameter,
	bool isOnOffParameter,
	bool addListener
) {

	String newId = paramID;
	if (myBusnr > 0) {
		newId = newId + "_bus" + String(myBusnr + 1);
		modMatrixDestination = MODMATDEST::NoDestination; //only bus1 is automatable
	}
	String busId = "";
	if (m_effectName != JucePlugin_Name)
		busId = "FXBUS " + String(myBusnr + 1) + " ";
    
	AudioProcessorParameterWithID* p = stateTree.createAndAddParameter(newId, busId + paramName, labelText, r, defaultVal, valueToTextFunction, textToValueFunction, isMetaParameter, isAutomatableParameter, isDiscreteParameter, AudioProcessorParameter::Category::genericParameter); //deprecated?? but how??
    
	//my_parameters.add(p);
	my_parameters.insert(std::make_pair(uiSequence, p));
	
	my_processor->m_mapModdestToParameterName.insert(std::make_pair(modMatrixDestination, newId));
	my_processor->m_mapParameterNameToModdest.insert(std::make_pair(newId, modMatrixDestination));
	my_processor->addModMatrixLookupTable(modMatrixDestination, r.start, r.end, r.skew, newId, p);
	
	std::atomic<float>* par = stateTree.getRawParameterValue(newId);
	*parameterVar = par;
	if (addListener)
		stateTree.addParameterListener(newId, this);

	if (isOnOffParameter)
		setOnOffParameter(newId);
}

void CVASTEffect::useOversampling(int sampleRate, bool oversample) {
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

bool CVASTEffect::isOff() {
	return m_bIsOff || m_bShallBeOff;
}

void CVASTEffect::switchOff() {
	m_bShallBeOff = true;
	my_processor->requestUIUpdate(true, false, false);
}

void CVASTEffect::switchOn() {
	m_bShallBeOff = false;
	my_processor->requestUIUpdate(true, false, false);
}

bool CVASTEffect::isOffAndShallBeOff() {
	return m_bIsOff && m_bShallBeOff;
}

bool CVASTEffect::shallBeOff() {
	return m_bShallBeOff;
}

void CVASTEffect::checkSoftFade() {
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
		}
		else
			m_iSoftFade++;
	}
}
