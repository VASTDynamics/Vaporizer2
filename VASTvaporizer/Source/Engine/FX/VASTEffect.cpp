
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
};


void CVASTEffect::switchOff() {
	m_bShallBeOff = true;
	my_processor->requestUIUpdate(true, false, false);
}

void CVASTEffect::switchOn() {
	m_bShallBeOff = false;
	my_processor->requestUIUpdate(true, false, false);
}
