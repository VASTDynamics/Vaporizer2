/*
VAST Dynamics
*/

#include "VASTParameterSlider.h"
#include "../../Engine/VASTEngineHeader.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"

VASTParameterSlider::VASTParameterSlider(const juce::String &componentName) {
	this->setComponentID("");
	this->setName(componentName);
	//this->setComponentID(componentName); //set later
	
	this->addMouseListener(&vastPopupHandler, true);
	
	vastPopupHandler.setComponentName(this->getName());

	setRepaintsOnMouseActivity(true); //performance
	setBufferedToImage(true); //performance

	//setOpaque(true); //not good!
	sliderAttachment = nullptr;
	m_processor = nullptr;	

	//startAutoUpdate();
}

VASTParameterSlider::~VASTParameterSlider() {
    stopTimer();
    this->setLookAndFeel(nullptr);
	sliderAttachment = nullptr; //to delete unique ptr
}

void VASTParameterSlider::setAutomationDestination(int ) {
//slider is automatable
	//m_automationModMatDest = destination;
}

void VASTParameterSlider::itemDropped(const SourceDetails& dragSourceDetails) {
	if (m_processor == nullptr) return;
	m_draganddropinterested = false;
	String cid = getComponentID();

	/*
	if (cid.endsWith("_bus2") || cid.endsWith("_bus3")) {
		cid = cid.dropLastCharacters(5);
	}
	*/

	int modmatdest = m_processor->autoParamGetDestination(cid); //name to remove _busx
	vassert(modmatdest != -1);
	String pName = dragSourceDetails.sourceComponent->getName().substring(5);
	//CHECK convention here based on projucer naming!! HACK // remove "c_dd_"
	int modmatsrce = CVASTParamState::comboBoxTextToValueFunction_MODMATSRCE(pName);
	if (modmatsrce < 0)
		return; //is another drag source
	
	if (m_processor->m_pVASTXperience.m_Set.modMatrixIsSet(modmatsrce, modmatdest) == false) {
		int freeSlot = m_processor->m_pVASTXperience.m_Set.modMatrixFindFirstFreeSlot();
		if (freeSlot != -1) {
            m_processor->m_pVASTXperience.setIsInitDefaults(true);
			m_processor->setParameterText("m_uModMatSrce" + String(freeSlot + 1), CVASTParamState::comboBoxValueToTextFunction_MODMATSRCE(modmatsrce), false);
			m_processor->setParameterText("m_uModMatDest" + String(freeSlot + 1), CVASTParamState::comboBoxValueToTextFunction_MODMATDEST(modmatdest), false);
			m_processor->setParameterText("m_fModMatVal" + String(freeSlot + 1), String("100"), false);
			m_processor->setParameterText("m_fModMatCurve" + String(freeSlot + 1), String("0"), false);
            m_processor->m_pVASTXperience.setIsInitDefaults(false);
            m_processor->m_pVASTXperience.parameterUpdatesAfterInit();
			this->repaint();
		}
	}

	//default name for custom modulator
	if (pName.startsWithIgnoreCase("CustomModulator")) {
		auto* param = m_processor->getParameterTree().getParameter(this->getComponentID());
		if (param == nullptr) {
			vassert(false);
			return;
		}
		String paramDesc = param->name;
		VASTDragSource* customMod = (VASTDragSource *)(dragSourceDetails.sourceComponent.get()->getParentComponent());
		if (customMod != nullptr) {
			customMod->setLabelDefaultText(paramDesc);
		}
	}
};

void VASTParameterSlider::bindParameter(VASTAudioProcessorEditor* editor, const String& newID, VASTGUIRuntimeModel::GUIComponents guiComponent, int tabNo) {
	Slider::setComponentID(newID); //call super
    myEditor = editor;
	if (m_processor != nullptr) {
		vassert(sliderAttachment == nullptr);
		sliderAttachment.reset(new SliderAttachment(m_processor->getParameterTree(), this->getComponentID(), (juce::Slider&)*this));

        if (editor != nullptr)
            editor->getGUIRuntimeModel()->registerParameterSlider((VASTParameterSlider*)this, newID, guiComponent, tabNo);

		auto* param = m_processor->getParameterTree().getParameter(newID);
		if (param == nullptr) {
			vassert(false);
			return;
		}
		setTooltip(m_processor->getParameters()[param->getParameterIndex()]->getName(1024));
	}
	else
		vassert(m_processor != nullptr);	
}

void VASTParameterSlider::mouseDrag(const MouseEvent &e) {
	m_draganddropinterested = false;
	m_processor->m_pVASTXperience.m_Set.m_bBeforeV22CompatibilityMode = false; 	//HACK

	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
	if (modifiers.isPopupMenu()) {
		//int starty = e.getMouseDownY();
		//int endy = starty + e.getDistanceFromDragStartY();		

		if (m_processor == nullptr) return;
		String cid = getComponentID();

		/*
		if (cid.endsWith("_bus2") || cid.endsWith("_bus3")) {
			cid = cid.dropLastCharacters(5);
		}
		*/

		int modmatdest = m_processor->autoParamGetDestination(cid); //name to remove _busx
		vassert(modmatdest != -1);
		int slot = m_processor->m_pVASTXperience.m_Set.modMatrixGetFirstSlotWithDestination(modmatdest);
		float newVal = e.getDistanceFromDragStartY() * 0.3f; //-100..100
		newVal = (newVal > 100.f) ? 100.f : newVal;
		newVal = (newVal < -100.f) ? -100.f : newVal;
		m_processor->setParameterText("m_fModMatVal" + String(slot + 1), String(newVal), false);
		this->repaint();
	}
	else Slider::mouseDrag(e);
}

bool VASTParameterSlider::isInterestedInDragSource(const SourceDetails& ) {
	if (m_processor == nullptr) return false;
	String cid = getComponentID();

	/*
	if (cid.endsWith("_bus2") || cid.endsWith("_bus3")) {
		cid = cid.dropLastCharacters(5);
	}
	*/
    
    if (m_draganddropinterested == false) {
        m_draganddropinterested = true;
        repaint();
    }
    
	return (m_processor->autoParamGetDestination(cid) != -1); //name to remove _busx
};

void VASTParameterSlider::setAudioProcessor(VASTAudioProcessor &processor) {
	m_processor = &processor;
}

VASTAudioProcessor* VASTParameterSlider::getAudioProcessor() {
	return m_processor;
}

void VASTParameterSlider::updateContent(bool ) {
	if (m_processor == nullptr)
		return;
	if (m_processor->m_disableOpenGLGFX)
		return;
	if (!isShowing())
		return;
	String cid = getComponentID();
	int modmatdest = m_processor->autoParamGetDestination(cid); //name to remove _busx
	vassert(modmatdest != -1);
	int slot = m_processor->m_pVASTXperience.m_Set.modMatrixGetFirstSlotWithDestination(modmatdest);
	if (slot < 0)
		return;
	float modVal = 0.f;
	double curvy = 0.f;
	int polarity = 0;
	float lastSrceVals[C_MAX_POLY] {};
	m_processor->m_pVASTXperience.m_Set.modMatrixSlotGetValues(slot, modVal, curvy, polarity, lastSrceVals);

	bool bchanged = false;
	for (int voice = 0; voice < C_MAX_POLY; voice++) if (lastSrceVals[voice] != m_lastSrceVals[voice]) { bchanged = true; break; }
	if (bchanged) {
		repaint();
		std::copy(std::begin(lastSrceVals), std::end(lastSrceVals), std::begin(m_lastSrceVals));
	}
}

void VASTParameterSlider::startAutoUpdate() {
	startTimer(25);
	m_timerRunning = true;
}

void VASTParameterSlider::stopAutoUpdate() {
	stopTimer();
	m_timerRunning = false;
}

void VASTParameterSlider::timerCallback() {
	updateContent(false);
}

void VASTParameterSlider::paint(Graphics& g) {
	Slider::paint(g);
	if (!m_timerRunning) {
		startAutoUpdate();
	}
}

void VASTParameterSlider::setHighlighted() {
    m_highlighted = true;
}

bool VASTParameterSlider::getHighlighted() {
    return m_highlighted;
}

void VASTParameterSlider::clearHighlighted() {
    m_highlighted = false;
}

void VASTParameterSlider::mouseEnter(const MouseEvent &e) {
    if (m_processor == nullptr) return;
    if (myEditor == nullptr) return;
    VASTSidePanelComponent* sidePanelComponent = ((VASTAudioProcessorEditor*)m_processor->getActiveEditor())->vaporizerComponent->getSidePanelComponent();
    if (sidePanelComponent==nullptr)
        return;
    if (sidePanelComponent->isVisible() == false)
        return;
    String cid = getComponentID();
    int modmatdest = m_processor->autoParamGetDestination(cid); //name to remove _busx
    vassert(modmatdest != -1);
    int slot = m_processor->m_pVASTXperience.m_Set.modMatrixGetFirstSlotWithDestination(modmatdest);
    if (slot<0)
        return;
    float  l_value = 0.f;
    double l_curvy = 0.f;
    int l_srce = 0;
    int l_dest = 0;
    float lastSrceVals[C_MAX_POLY] {};
    int polarity = 0;
    m_processor->m_pVASTXperience.m_Set.modMatrixSlotGetValues(slot, l_value, l_curvy, l_srce, l_dest, polarity, lastSrceVals);
    VASTDragSource* dragSource = myEditor->getGUIRuntimeModel()->getDragSourceByModMatrxiSource(l_srce);
    if (dragSource!=nullptr) {
        dragSource->setHighlighted();
        dragSource->repaint();
    }
}

void VASTParameterSlider::mouseExit(const MouseEvent &e) {
    if (m_processor == nullptr) return;
    if (myEditor == nullptr) return;
    VASTSidePanelComponent* sidePanelComponent = ((VASTAudioProcessorEditor*)m_processor->getActiveEditor())->vaporizerComponent->getSidePanelComponent();
    if (sidePanelComponent==nullptr)
        return;
    if (sidePanelComponent->isVisible() == false)
        return;
    String cid = getComponentID();
    int modmatdest = m_processor->autoParamGetDestination(cid); //name to remove _busx
    vassert(modmatdest != -1);
    int slot = m_processor->m_pVASTXperience.m_Set.modMatrixGetFirstSlotWithDestination(modmatdest);
    if (slot<0)
        return;
    float  l_value = 0.f;
    double l_curvy = 0.f;
    int l_srce = 0;
    int l_dest = 0;
    float lastSrceVals[C_MAX_POLY] {};
    int polarity = 0;
    m_processor->m_pVASTXperience.m_Set.modMatrixSlotGetValues(slot, l_value, l_curvy, l_srce, l_dest, polarity, lastSrceVals);
    VASTDragSource* dragSource = myEditor->getGUIRuntimeModel()->getDragSourceByModMatrxiSource(l_srce);
    if (dragSource!=nullptr) {
        dragSource->clearHighlighted();
        dragSource->repaint();
    }
}
