
/*
	VAST Dynamics Audio Software (TM)
*/

#include "VASTEffect.h"
#include "../../Plugin/VASTAudioProcessor.h"

void CVASTEffect::createAndAddParameter(std::atomic<float>** parameterVar, AudioProcessorValueTreeState& stateTree, int versionHint, const String& paramID, const String& paramName,
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
    
    /*
	AudioProcessorParameterWithID* p = stateTree.createAndAddParameter( newId, busId + paramName, labelText, r, defaultVal, valueToTextFunction, textToValueFunction, isMetaParameter, isAutomatableParameter, isDiscreteParameter, AudioProcessorParameter::Category::genericParameter); //deprecated
     */
	
	jassert(newId.length() <= 31); //AAX max parameter ID length

#ifdef _DEBUG
	int lFirstParamVers = 0;
	for (int i = 0; i < my_processor->getParameters().size(); i++) {
		AudioProcessorParameterWithID* param = (AudioProcessorParameterWithID*)my_processor->getParameters()[i];
		ParameterID pid = param->getParameterID();
		if (pid.getVersionHint() <= 1)
			lFirstParamVers++;
	}
	jassert(lFirstParamVers<755); //755 parameters before VersionHint was introduced --> 1. All new parameters need to get higher VersionHints.
#endif

    String newParamName = busId + paramName;
    newParamName = newParamName.dropLastCharacters(newParamName.length()-31); //keep only 31 in name // hack, check for compatibility, better define new short names for each parameter
    
    using Parameter = AudioProcessorValueTreeState::Parameter; //in future change to: https://docs.juce.com/master/tutorial_audio_processor_value_tree_state.html

    AudioProcessorParameterWithID* p = stateTree.createAndAddParameter (std::make_unique<Parameter>
        (
			// IMPORTANT
			ParameterID { newId, versionHint }, //the version number is important: new parameters have to always get higher numbers
		    // IMPORTANT

            newParamName, labelText, r, defaultVal, valueToTextFunction, textToValueFunction, isMetaParameter,
			isAutomatableParameter, isDiscreteParameter, AudioProcessorParameter::Category::genericParameter));
    
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
	return m_bIsOff.load() || m_bShallBeOff.load();
}

void CVASTEffect::switchOff() {
	m_bShallBeOff.store(true);
	my_processor->requestUIUpdate(true, false, false);
}

void CVASTEffect::switchOn() {
	m_bShallBeOff.store(false);
	my_processor->requestUIUpdate(true, false, false);
}

bool CVASTEffect::isOffAndShallBeOff() {
	return m_bIsOff.load() && m_bShallBeOff.load();
}

bool CVASTEffect::shallBeOff() {
	return m_bShallBeOff.load();
}

void CVASTEffect::checkSoftFade() {
	if (m_bShallBeOff.load() == true) {
		if (m_bIsOff.load() == true) {
			m_iSoftFade.store(0);
			return;
		}
		if (m_iSoftFade.load() <= 0) {
			//reset();
			m_bIsOff.store(true);
			m_iSoftFade.store(0);
		}
		else
			m_iSoftFade-=1;
	}
	else { //shall be on
		if (m_bIsOff.load() == false) {
			m_iSoftFade.store(C_MAX_SOFTFADE);
			return;
		}
		if (m_iSoftFade.load() >= C_MAX_SOFTFADE) {
			m_bIsOff.store(false);
			m_iSoftFade.store(C_MAX_SOFTFADE);
		}
		else
			m_iSoftFade+=1;
	}
}
