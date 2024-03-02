/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTDragSource.h"
#include "../VASTAudioProcessorEditor.h"

VASTDragSource::VASTDragSource(int modSource, const juce::String &componentName, const juce::String &dragText, const juce::String &tooltipText) : m_toolTipText(tooltipText), m_modSource(modSource){
	this->setName(componentName);
	this->setComponentID(componentName);

	m_noLabel = false;
	String newtext = dragText;
	if (dragText == "n/a") {
		newtext = "";
		m_noLabel = true;
	}

	//setRepaintsOnMouseActivity(false); //performance
	//setBufferedToImage(true); //performance
	//setColour(GroupComponent::ColourIds::outlineColourId, Colour::fromRGBA(255, 255, 255, 1.0));

	ddImageButton.reset(new VASTImageButton(componentName));
	addAndMakeVisible(ddImageButton.get());
	ddImageButton->setImages(false, true, true,
		ImageCache::getFromMemory(VASTHeaderComponent::dragdrop_png, VASTHeaderComponent::dragdrop_pngSize), 1.000f, Colour(0x451c6d2f),
		ImageCache::getFromMemory(VASTHeaderComponent::dragdrop_over_png, VASTHeaderComponent::dragdrop_over_pngSize), 1.000f, Colour(0x00ff0000),
		ImageCache::getFromMemory(VASTHeaderComponent::dragdrop_png, VASTHeaderComponent::dragdrop_pngSize), 1.000f, Colour(0x00000000));
	
	ddLabel.reset(new Label("new label", newtext));
	addAndMakeVisible(ddLabel.get());
	//ddLabel->setFont(Font("Syntax", 11.00f, Font::plain));
	ddLabel->setJustificationType(Justification::centred);
	ddLabel->setEditable(false, false, false);
	ddLabel->setColour(Label::textColourId, Colour(0xffadadad));
	ddLabel->setColour(TextEditor::textColourId, Colours::black);
	ddLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));
	ddLabel->addListener(this);
    this->addMouseListener(this, true);
    
    ddLabel->setTooltip(m_toolTipText);
    ddImageButton->setTooltip(m_toolTipText);
	if (componentName.startsWithIgnoreCase("c_dd_CustomModulator"))
		ddLabel->setEditable(true);
    clearHighlighted();
	m_processor = NULL;
    m_editor = NULL;    
}

void VASTDragSource::resized() {
	if (ddLabel->getText() != "") {
		ddImageButton->setBounds(proportionOfWidth(0.f), proportionOfHeight(0.f), proportionOfWidth(0.3f), proportionOfHeight(1.f));
		ddLabel->setBounds(proportionOfWidth(0.3f), proportionOfHeight(0.f), proportionOfWidth(0.7f), proportionOfHeight(1.f));
	}
	else {
		ddImageButton->setBounds(proportionOfWidth(0.1f), proportionOfHeight(0.1f), proportionOfWidth(0.8f), proportionOfHeight(0.8f));
		ddLabel->setBounds(proportionOfWidth(0.f), proportionOfHeight(0.f), proportionOfWidth(0.f), proportionOfHeight(0.f));
	}
}

void VASTDragSource::lookAndFeelChanged() {
	if (m_processor == nullptr) return;
    Colour c1 = m_processor->getCurrentVASTLookAndFeel()->findVASTColour(colRotarySliderOuterArcModulationIndicatorInner).darker(.3f);
    if (c1 == m_bufferc1)
        return;
	ddImageButton->setImages(false, true, true,
		ImageCache::getFromMemory(VASTHeaderComponent::dragdrop_png, VASTHeaderComponent::dragdrop_pngSize), 1.000f, c1,
		ImageCache::getFromMemory(VASTHeaderComponent::dragdrop_over_png, VASTHeaderComponent::dragdrop_over_pngSize), 1.000f,
                             c1.contrasting(0.3f),
		ImageCache::getFromMemory(VASTHeaderComponent::dragdrop_png, VASTHeaderComponent::dragdrop_pngSize), 1.000f, c1.contrasting(0.5f));
    m_bufferc1 = c1;
}

void VASTDragSource::setHighlighted() {
    m_isHighlighted = true;
}

void VASTDragSource::clearHighlighted() {
    m_isHighlighted = false;
}

void VASTDragSource::paint(Graphics& g)
{
    if (m_processor == nullptr) return;
    if (m_noLabel) return;
    
    Colour c1 = m_processor->getCurrentVASTLookAndFeel()->findVASTColour(colOscillatorComponentHeaderGradientFrom);
    Colour c2 = m_processor->getCurrentVASTLookAndFeel()->findVASTColour(colOscillatorComponentHeaderGradientTo);
    Colour c3 = Colour(0xff4e575c); //TODO
    
    if (m_isHighlighted) {
        c1 = m_processor->getCurrentVASTLookAndFeel()->findVASTColour(colParameterBindingHighlight);
        c2 = c1;
        c3 = c1;
    }
    
    g.setGradientFill(ColourGradient(c1,
		proportionOfWidth(0),
		proportionOfHeight(0),
		c2,
		proportionOfWidth(1),
		proportionOfHeight(1),
		false));
	g.fillRect(0, 0, getWidth(), getHeight());
	g.setColour(c3);
	g.drawRoundedRectangle(proportionOfWidth(0.f), proportionOfHeight(0.f), proportionOfWidth(1.f), proportionOfHeight(1.f), 3.f, 2.f);

    if (getComponentID().startsWithIgnoreCase("c_dd_CustomModulator")) {
		if (ddLabel->getText().trim().equalsIgnoreCase("")) {
			String ending = getComponentID().getLastCharacters(1);
			ddLabel->setText("Custom" + ending, NotificationType::sendNotification);
		}
		if (!ddLabel->getText().containsIgnoreCase("Custom"))
			ddLabel->setComponentID("CustomLabel");
		else 
			ddLabel->setComponentID("");
    }
}

VASTDragSource::~VASTDragSource() {
	this->setLookAndFeel(nullptr);
	ddImageButton = nullptr;
	ddLabel = nullptr;
}

void VASTDragSource::setAudioProcessor(VASTAudioProcessor &processor, VASTAudioProcessorEditor &editor) {
	m_processor = &processor;
    m_editor = &editor;
	ddImageButton->setAudioProcessor(processor);
    m_editor->getGUIRuntimeModel()->registerDragSource(this, m_modSource);
}

VASTAudioProcessor* VASTDragSource::getAudioProcessor() {
	return m_processor;
}

void VASTDragSource::setModString(const juce::String& dragText) {
	ddLabel->setText(dragText, NotificationType::dontSendNotification);
}

void VASTDragSource::editorShown(Label *label, TextEditor &editor) {
	if (m_processor == nullptr) return;
	editor.applyFontToAllText(m_processor->getCurrentVASTLookAndFeel()->getTextEditorFont(editor));
	editor.applyColourToAllText(m_processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxText), true);
	editor.setColour(TextEditor::backgroundColourId, m_processor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colBoxTextBackground));
}

void VASTDragSource::labelTextChanged(Label* labelThatHasChanged) {
	if (m_processor == nullptr) return;
	VASTPresetElement lElem = m_processor->m_presetData.getCurPatchData();
	if (getComponentID().endsWith("1"))
		lElem.customModulator1Text = labelThatHasChanged->getText();
	else if (getComponentID().endsWith("2"))
		lElem.customModulator2Text = labelThatHasChanged->getText();
	else if (getComponentID().endsWith("3"))
		lElem.customModulator3Text = labelThatHasChanged->getText();
	else if (getComponentID().endsWith("4"))
		lElem.customModulator4Text = labelThatHasChanged->getText();
	m_processor->m_presetData.exchangeCurPatchData(lElem);
}

void VASTDragSource::setLabelDefaultText(String defaultText)
{
	ddLabel->setText(defaultText, NotificationType::sendNotificationSync);
	ddLabel->showEditor();
}

void VASTDragSource::mouseEnter(const MouseEvent &e)
{
    if ((m_processor==nullptr) || (m_editor == nullptr)) return;
    for (int slot = 0; slot<M_MODMATRIX_MAX_SLOTS; slot++) {
        float  l_value = 0.f;
        double l_curvy = 0.f;
        int l_srce = 0;
        int l_dest = 0;
        float lastSrceVals[C_MAX_POLY] {};
        int polarity = 0;
        m_processor->m_pVASTXperience.m_Set.modMatrixSlotGetValues(slot, l_value, l_curvy, l_srce, l_dest, polarity, lastSrceVals);
        if (l_srce == m_modSource) {
            String l_paramName = m_processor->autoDestinationGetParam(l_dest);
            VASTGUIRuntimeModel::sGUIParameterSliders lslider = m_editor->getGUIRuntimeModel()->getParameterSliderByParameterName(l_paramName);
            if (lslider.slider != nullptr) {
                if (lslider.slider->isShowing()) {
                    lslider.slider->setHighlighted();
                    lslider.slider->repaint();
                }
            }
            switch (lslider.guiComponent ) {
                case VASTGUIRuntimeModel::GUIComponents::WavetableEditorView:
                case VASTGUIRuntimeModel::GUIComponents::WavetableEditorComponent:
                case VASTGUIRuntimeModel::GUIComponents::WavetableEditorViewHeader:
                case VASTGUIRuntimeModel::GUIComponents::SamplerEditorComponent:
                case VASTGUIRuntimeModel::GUIComponents::SamplerEditorComponentHeader:
                    m_editor->vaporizerComponent->getTabbedComponent()->setTabBackgroundColour(VASTTabbedComponent::TabSequence::WTEDITOR, m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colParameterBindingHighlight));
                    break;
                case VASTGUIRuntimeModel::GUIComponents::FilterComponent:
                    m_editor->vaporizerComponent->getTabbedComponent()->setTabBackgroundColour(VASTTabbedComponent::TabSequence::FILTER, m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colParameterBindingHighlight));
                    break;
                case VASTGUIRuntimeModel::GUIComponents::MSEGEditorPane:
                {
                    VASTLFOMsegComponent* tab3 = dynamic_cast<VASTLFOMsegComponent*>(m_editor->vaporizerComponent->getTabbedComponent()->getTabContentComponent(VASTTabbedComponent::TabSequence::LFOMSEG));
                    if (tab3 != nullptr)
                        tab3->getEnvTab()->setTabBackgroundColour(lslider.tabNo,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colParameterBindingHighlight));
                }
                    m_editor->vaporizerComponent->getTabbedComponent()->setTabBackgroundColour(VASTTabbedComponent::TabSequence::LFOMSEG, m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colParameterBindingHighlight));
                    break;
                case VASTGUIRuntimeModel::GUIComponents::LFOEditorPane:
                {
                    VASTLFOMsegComponent* tab3 = dynamic_cast<VASTLFOMsegComponent*>(m_editor->vaporizerComponent->getTabbedComponent()->getTabContentComponent(VASTTabbedComponent::TabSequence::LFOMSEG));
                    if (tab3 != nullptr)
                        tab3->getLFOTab()->setTabBackgroundColour(lslider.tabNo,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colParameterBindingHighlight));
                }
                    m_editor->vaporizerComponent->getTabbedComponent()->setTabBackgroundColour(VASTTabbedComponent::TabSequence::LFOMSEG, m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colParameterBindingHighlight));
                    break;
                case VASTGUIRuntimeModel::GUIComponents::MatrixComponent:
                case VASTGUIRuntimeModel::GUIComponents::DragMatrix:
                    m_editor->vaporizerComponent->getTabbedComponent()->setTabBackgroundColour(VASTTabbedComponent::TabSequence::MATRIX, m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colParameterBindingHighlight));
                    break;
                case VASTGUIRuntimeModel::GUIComponents::FXComponent:
                case VASTGUIRuntimeModel::GUIComponents::DragFX:
                case VASTGUIRuntimeModel::GUIComponents::GenericEditor:
                {
                    VASTFXComponent* tab5 = dynamic_cast<VASTFXComponent*>(m_editor->vaporizerComponent->getTabbedComponent()->getTabContentComponent(VASTTabbedComponent::TabSequence::FX));
                    if (tab5 != nullptr)
                        tab5->getFXBusTab()->setTabBackgroundColour(lslider.tabNo,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colParameterBindingHighlight));
                }
                case VASTGUIRuntimeModel::GUIComponents::FXSelectionPane:
                    m_editor->vaporizerComponent->getTabbedComponent()->setTabBackgroundColour(VASTTabbedComponent::TabSequence::FX, m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colParameterBindingHighlight));
                    break;
                case VASTGUIRuntimeModel::GUIComponents::StepSeqEditorPane:
                {
                    VASTArpComponent* tab6 = dynamic_cast<VASTArpComponent*>(m_editor->vaporizerComponent->getTabbedComponent()->getTabContentComponent(VASTTabbedComponent::TabSequence::ARP));
                    if (tab6 != nullptr)
                        tab6->getStepSeqTab()->setTabBackgroundColour(lslider.tabNo,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colParameterBindingHighlight));
                }
                case VASTGUIRuntimeModel::GUIComponents::ARPComponent:
                case VASTGUIRuntimeModel::GUIComponents::ARPEditorPane:
                    m_editor->vaporizerComponent->getTabbedComponent()->setTabBackgroundColour(VASTTabbedComponent::TabSequence::ARP, m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colParameterBindingHighlight));
                    break;
                default:
                    break;
            }
        }
    }
}

void VASTDragSource::mouseExit(const MouseEvent &e) {
    for (int slot = 0; slot<M_MODMATRIX_MAX_SLOTS; slot++) {
        float  l_value = 0.f;
        double l_curvy = 0.f;
        int l_srce = 0;
        int l_dest = 0;
        float lastSrceVals[C_MAX_POLY] {};
        int polarity = 0;
        m_processor->m_pVASTXperience.m_Set.modMatrixSlotGetValues(slot, l_value, l_curvy, l_srce, l_dest, polarity, lastSrceVals);
        if (l_srce == m_modSource) {
            String l_paramName = m_processor->autoDestinationGetParam(l_dest);
            VASTGUIRuntimeModel::sGUIParameterSliders lslider = m_editor->getGUIRuntimeModel()->getParameterSliderByParameterName(l_paramName);
            if (lslider.slider != nullptr) {
                if (lslider.slider->isShowing()) {
                    lslider.slider->clearHighlighted();
                    lslider.slider->repaint();
                }
            }
            m_editor->vaporizerComponent->getTabbedComponent()->setTabBackgroundColour(VASTTabbedComponent::TabSequence::WTEDITOR, m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colVaporizerComponentTabBarWTEDitorBackground));
            m_editor->vaporizerComponent->getTabbedComponent()->setTabBackgroundColour(VASTTabbedComponent::TabSequence::FILTER, m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colVaporizerComponentTabBarFilterBackground));
            m_editor->vaporizerComponent->getTabbedComponent()->setTabBackgroundColour(VASTTabbedComponent::TabSequence::LFOMSEG, m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colVaporizerComponentTabBarLFOMSEGBackground));
            m_editor->vaporizerComponent->getTabbedComponent()->setTabBackgroundColour(VASTTabbedComponent::TabSequence::MATRIX, m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colVaporizerComponentTabBarMatrixBackground));
            m_editor->vaporizerComponent->getTabbedComponent()->setTabBackgroundColour(VASTTabbedComponent::TabSequence::FX, m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colVaporizerComponentTabBarFXBackground));
            m_editor->vaporizerComponent->getTabbedComponent()->setTabBackgroundColour(VASTTabbedComponent::TabSequence::ARP, m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colVaporizerComponentTabBarARPBackground));
            m_editor->vaporizerComponent->getTabbedComponent()->setTabBackgroundColour(VASTTabbedComponent::TabSequence::PRESET, m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colVaporizerComponentTabBarPresetBackground));
                        
            VASTLFOMsegComponent* tab3 = dynamic_cast<VASTLFOMsegComponent*>(m_editor->vaporizerComponent->getTabbedComponent()->getTabContentComponent(VASTTabbedComponent::TabSequence::LFOMSEG));
            if (tab3 != nullptr) {
                    tab3->getEnvTab()->setTabBackgroundColour(0,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colLFOMsegComponentTabBarMSEG1));
                    tab3->getEnvTab()->setTabBackgroundColour(1,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colLFOMsegComponentTabBarMSEG2));
                    tab3->getEnvTab()->setTabBackgroundColour(2,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colLFOMsegComponentTabBarMSEG3));
                    tab3->getEnvTab()->setTabBackgroundColour(3,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colLFOMsegComponentTabBarMSEG4));
                    tab3->getEnvTab()->setTabBackgroundColour(4,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colLFOMsegComponentTabBarMSEG5));
                    tab3->getLFOTab()->setTabBackgroundColour(0,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colLFOMsegComponentTabBarLFO1));
                    tab3->getLFOTab()->setTabBackgroundColour(1,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colLFOMsegComponentTabBarLFO2));
                    tab3->getLFOTab()->setTabBackgroundColour(2,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colLFOMsegComponentTabBarLFO3));
                    tab3->getLFOTab()->setTabBackgroundColour(3,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colLFOMsegComponentTabBarLFO4));
                    tab3->getLFOTab()->setTabBackgroundColour(4,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colLFOMsegComponentTabBarLFO5));
                }

            VASTFXComponent* tab5 = dynamic_cast<VASTFXComponent*>(m_editor->vaporizerComponent->getTabbedComponent()->getTabContentComponent(VASTTabbedComponent::TabSequence::FX));
            if (tab5 != nullptr) {
                tab5->getFXBusTab()->setTabBackgroundColour(0,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colFXComponentTabBarFXBUS1));
                tab5->getFXBusTab()->setTabBackgroundColour(1,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colFXComponentTabBarFXBUS2));
                tab5->getFXBusTab()->setTabBackgroundColour(2,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colFXComponentTabBarFXBUS3));
            }
            
            VASTArpComponent* tab6 = dynamic_cast<VASTArpComponent*>(m_editor->vaporizerComponent->getTabbedComponent()->getTabContentComponent(VASTTabbedComponent::TabSequence::ARP));
            if (tab6 != nullptr){
                tab6->getStepSeqTab()->setTabBackgroundColour(0,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colArpComponentTabBarSTEPSEQ1));
                tab6->getStepSeqTab()->setTabBackgroundColour(1,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colArpComponentTabBarSTEPSEQ2));

                tab6->getStepSeqTab()->setTabBackgroundColour(2,  m_editor->getCurrentVASTLookAndFeel()->findVASTColour(colArpComponentTabBarSTEPSEQ3));
            }
        }
    }
}

int VASTDragSource::getModSource() {
    return m_modSource;
}
