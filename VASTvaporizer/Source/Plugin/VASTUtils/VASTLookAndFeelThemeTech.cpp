/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTLookAndFeelThemeTech.h"
#include "../VASTVaporizerComponent.h"
#include "../VASTControls/VASTComboBrev.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"

#include <sstream>

VASTLookAndFeelThemeTech::VASTLookAndFeelThemeTech(CVASTSettings& set, VASTAudioProcessor* processor)
{
	m_Set = &set;
	myProcessor = processor;
	
	setColour(Label::textColourId, findVASTColour(colLabelText));

	setColour(FileSearchPathListComponent::backgroundColourId, findVASTColour(colFileSearchPathListComponentBackground));
	setColour(FileChooserDialogBox::titleTextColourId, findVASTColour(colFileChooserDialogBoxTitleText));

	setColour(TextEditor::backgroundColourId, findVASTColour(colBoxTextBackground));
	setColour(TextEditor::textColourId, findVASTColour(colBoxText));

	mFontSizeFactor = 0.9f;
	setUIFontSize(0);
	myDefaultFontRegular = m_Set->getCustomFont(CVASTSettings::customFonts::SFUIDisplayRegular);
	myDefaultFontRegular.setDefaultMinimumHorizontalScaleFactor(1.0);
	myDefaultFontRegular.setSizeAndStyle(12.f, myDefaultFontBold.getStyleFlags(), 1.0f, 0.0f); // no squashing, no kerning

	myDefaultFontBold = m_Set->getCustomFont(CVASTSettings::customFonts::SFUIDisplayBold);
	myDefaultFontBold.setDefaultMinimumHorizontalScaleFactor(1.0);
	myDefaultFontBold.setSizeAndStyle(12.f, myDefaultFontBold.getStyleFlags(), 1.0f, 0.0f); // no squashing, no kerning	
	
	setColour(VASTVUMeter::lmTextColour, juce::Colours::lightgrey);
	setColour(VASTVUMeter::lmTextClipColour, juce::Colours::lightgrey);
	setColour(VASTVUMeter::lmTextDeactiveColour, juce::Colours::darkgrey);
	setColour(VASTVUMeter::lmTicksColour, juce::Colours::black.withAlpha(0.0f));
	setColour(VASTVUMeter::lmOutlineColour, juce::Colours::black); //CHVAST
	setColour(VASTVUMeter::lmBackgroundColour, juce::Colour(0xff0b0b0b));
	setColour(VASTVUMeter::lmBackgroundClipColour, juce::Colours::red.withAlpha(0.2f));
	setColour(VASTVUMeter::lmMeterForegroundColour, findVASTColour(colOscilloscopeBackground).withAlpha(0.3f));
	setColour(VASTVUMeter::lmMeterOutlineColour, juce::Colours::black);
	setColour(VASTVUMeter::lmMeterBackgroundColour, findVASTColour(colOscilloscopeOff));
	setColour(VASTVUMeter::lmMeterMaxNormalColour, findVASTColour(colOscilloscopeLine).withAlpha(1.0f));
	setColour(VASTVUMeter::lmMeterMaxWarnColour, findVASTColour(colOscilloscopeLineSolo).withAlpha(0.5f));
	setColour(VASTVUMeter::lmMeterMaxOverColour, findVASTColour(colOscilloscopeLineSolo).withAlpha(1.0f));
	setColour(VASTVUMeter::lmMeterGradientLowColour, findVASTColour(colOscilloscopeLine).withAlpha(0.5f));
	setColour(VASTVUMeter::lmMeterGradientMidColour, findVASTColour(colOscilloscopeLine).withAlpha(1.0f));
	setColour(VASTVUMeter::lmMeterGradientMaxColour, findVASTColour(colOscilloscopeLineSolo).withAlpha(1.0f));
	setColour(VASTVUMeter::lmMeterReductionColour, juce::Colours::darkorange.withAlpha(0.2f));
}

VASTLookAndFeelThemeTech::~VASTLookAndFeelThemeTech()
{
}

//VAST Colours
Colour VASTLookAndFeelThemeTech::findVASTColour(int colourID) {
	switch (colourID) {
	case colLabelText:
		return Colour::fromFloatRGBA(0.72f, 0.72f, 0.72f, 1.0f); //europa
		break;
	case colHeaderLabelText:
		return Colour::fromFloatRGBA(0.95f, 0.95f, 0.98f, 1.0f); //nexus
		break;
	case colLabelOutline:
		return Colour(0x00000000); //transparent
		break;
	case colLabelOutlineEnabled:
		return Colour(0xBFE0EC65);
		break;
	case colLabelBackground:
		return Colour(0x00000000); //transparent
		break;
	case colLabelCustom:
		return Colours::yellow;
		break;
	case colBoxText:
		return findVASTColour(colOscilloscopeLine).brighter(.2f);
		break;
	case colBoxTextBackground:
		return findVASTColour(colOscilloscopeBackground).darker(0.2f);
		break;
	case colBoxArrow:
		return Colours::white;
		break;
	case colBoxOutline:
		return Colours::white.withMultipliedAlpha(0.5f);
		break;
	case colMidiLearn:
		return Colours::darkred;
		break;
	case colDisabled:
		return Colours::darkgrey;
		break;
	case colAlmostBlack:
		return Colours::darkgrey.darker(1.0f).darker(0.6f);
		break;
	case colFileSearchPathListComponentBackground:
		return Colours::red;
		break;
	case colFileChooserDialogBoxTitleText:
		return Colours::red;
		break;
	case colLinearSliderThumbKnob:
		return Colours::darkgrey;
		break;
	case colLinearSliderThumbKnobMouseOver:
		return Colours::darkgrey.darker();
		break;
	case colGlassLozengeGradient:
		return Colours::transparentBlack;
		break;
	case colRotarySliderMarkerShadow:
		return Colour::fromFloatRGBA(0.1f, 0.1f, 0.15f, 0.4f);
		break;
	case colRotarySliderMarker:
		return Colours::white;
		break;
	case colRotarySliderMarkerTip:
		return Colours::orangered;
		break;
	case colScrollbarButton:
		return Colours::grey;
		break;
	case colScrollbarButtonDown:
		return Colours::lightgrey;
		break;
	case colScrollBarGradient:
		return Colours::transparentBlack;
		break;
	case colFileBrowserIcon:
		return Colours::red;
		break;
	case colRotarySliderOuterArcBackground:
		return Colour::fromFloatRGBA(0.8f, 0.8f, 0.8f, 0.5f);
		break;
	case colRotarySliderOuterArcValueIndicator:
		return Colours::orange.darker(0.5f); //sylenth
		break;
	case colRotarySliderOuterArcValueIndicatorInner:
		return Colours::orange;
		break;
	case colRotarySliderOuterArcModulationIndicator:
		return Colours::darkgoldenrod;
		break;
	case colRotarySliderOuterArcModulationIndicatorInner:
		return Colours::yellow;
		break;
	case colRotarySliderOuterArcNonParameter:
		return Colour::fromFloatRGBA(0.2f, 0.0f, 0.0f, 1.0f);
		break;
	case colRotarySliderOuterArcNonParameterInner:
		return Colour::fromFloatRGBA(0.4f, 0.1f, 0.0f, 1.0f);
		break;
	case colRotarySliderKnobHighlightGradientFrom:
		return findVASTColour(colOscilloscopeLine);
		break;
	case colRotarySliderKnobHighlightGradientTo:
		return findVASTColour(colOscilloscopeArea);
		break;
	case colRotarySliderKnobCenterGradientFrom:
		return Colour::fromFloatRGBA(0.6f, 0.6f, 0.6f, 1.0f);
		break;
	case colRotarySliderKnobCenterGradientTo:
		return Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case colRotarySliderKnobCenterMouseOverGradientFrom:
		return Colour::fromFloatRGBA(0.7f, 0.7f, 0.7f, 1.0f);
		break;
	case colRotarySliderKnobCenterMouseOverGradientTo:
		return Colour::fromFloatRGBA(0.1f, 0.1f, 0.1f, 1.0f);
		break;
	case colRotarySliderKnobShadow:
		return Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 1.f);
		break;
	case colToggleButtonShade:
		return Colour::fromFloatRGBA(0.8f, 0.7f, 0.7f, 0.1f); //transparent
		break;
	case colToggleButtonShadeMouseOver:
		return Colour::fromFloatRGBA(0.8f, 0.7f, 0.7f, 0.4f); //transparent
		break;
	case colToggleButtonInner:
		return Colour::fromFloatRGBA(0.42f, 0.0, 0.0f, 1.0f); //europa
		break;
	case colToggleButtonLight:
		return Colour::fromFloatRGBA(0.6f, 0.4f, 0.4f, 1.0f); //transparent
		break;
	case colToggleButtonOuter:
		return Colour::fromFloatRGBA(0.20f, 0.0, 0.0f, 1.0f); //europa
		break;
	case colToggleButtonShadeOn:
		return Colour::fromFloatRGBA(0.95f, 0.47f, 0.08f, 0.35f); //europa
		break;
	case colToggleButtonShadeMouseOverOn:
		return Colour::fromFloatRGBA(1.0f, 1.0f, 0.9f, 0.75f);
		break;
	case colToggleButtonInnerOn:
		return Colour::fromFloatRGBA(1.0f, 0.55f, 0.f, 1.0f); //europa
		break;
	case colToggleButtonLightOn:
		return Colour::fromFloatRGBA(1.0f, 1.0f, 0.7f, 1.0f); //yellowish
		break;
	case colToggleButtonOuterOn:
		return Colour::fromFloatRGBA(0.95f, 0.27f, 0.08f, 1.0f); //europa
		break;
	case colToggleButtonBackground:
		return Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case colVaporizerComponentBackgroundGradientFrom:
		//return Colour(0xff2a3032);
		//return Colour::fromFloatRGBA(0.25f, 0.35f, 0.39f, 1.0f).darker(.3);
		//return Colour::fromFloatRGBA(0.10f, 0.10f, 0.21f, 1.0f);
		return findVASTColour(colOscilloscopeOff);
		break;
	case colVaporizerComponentBackgroundGradientTo:
		//return Colour(0xff141516);
		//return Colour::fromFloatRGBA(0.21f, 0.33f, 0.39f, 1.0f).darker(.3);
		//return Colour::fromFloatRGBA(0.10f, 0.10f, 0.21f, 1.0f).darker(.3f);
		return findVASTColour(colOscilloscopeOff);
		break;
	case colPresetEditorBackgroundGradientFrom:
		//return Colour(0x4f616d7e);
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colPresetEditorBackgroundGradientTo:
		//return Colour(0xbd03070a);
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colGroupComponentText:
		return findVASTColour(colLabelText);
		//return Colour::fromFloatRGBA(0.95f, 0.95f, 0.98f, 1.0f); //nexus
		break;
	case colGroupComponentTextBackground:
		return Colour::fromFloatRGBA(0.23f, 0.23f, 0.26f, 1.0f); //nexus;
		break;
	case colGroupComponentOutline:
		return Colour::fromFloatRGBA(0.8f, 0.8f, 0.6f, 0.2f);
		break;
	case colOscillatorComponentBackgroundGradientFrom:
		return Colour::fromFloatRGBA(0.17f, 0.18f, 0.19f, 1.0f); //europa
		break;
	case colOscillatorComponentBackgroundGradientTo:
		return Colour::fromFloatRGBA(0.16f, 0.17f, 0.19f, 1.0f); //europa
		break;
	case colOscillatorComponentHeaderGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom).brighter(.2f);
		break;
	case colOscillatorComponentHeaderGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo).brighter(.2f);
		break;
	case colOscilloscopeBackground:
		return Colour::fromFloatRGBA(0.11f, 0.06f, 0.06f, 1.0f); //europa
		break;
	case colOscilloscopeOff:
		return findVASTColour(colOscilloscopeBackground).darker(0.7f);
		break;
	case colOscilloscopeLine:
		return Colour::fromFloatRGBA(0.82f, 0.13f, 0.03f, 1.0f); //europa
		break;
	case colOscilloscopeLineSolo:
		return Colour::fromFloatRGBA(0.9f, 0.7f, 0.05f, 1.0f);  //yellowish
		break;
	case colOscilloscopeArea:
		return findVASTColour(colOscilloscopeLine).darker(0.5f);
		break;
	case colOscilloscopeAreaSolo:
		return findVASTColour(colOscilloscopeLineSolo).darker(0.5f);
		break;
	case colOscilloscopeSelection:
		return Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case colVaporizerComponentTabBarWTEDitorBackground:
		return findVASTColour(colButtonBackground);
		break;
	case colVaporizerComponentTabBarFilterBackground:
		return findVASTColour(colButtonBackground);
		break;
	case colVaporizerComponentTabBarLFOMSEGBackground:
		return findVASTColour(colButtonBackground);
		break;
	case colVaporizerComponentTabBarMatrixBackground:
		return findVASTColour(colButtonBackground);
		break;
	case colVaporizerComponentTabBarFXBackground:
		return findVASTColour(colButtonBackground);
		break;
	case colVaporizerComponentTabBarARPBackground:
		return findVASTColour(colButtonBackground);
		break;
	case colVaporizerComponentTabBarPresetBackground:
		return findVASTColour(colButtonBackground);
		break;
	case colVaporizerComponentTabBarTextSelected:
		return Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case colVaporizerComponentTabBarText:
		return Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case colVaporizerComponentBackground:
		//return Colour::fromFloatRGBA(0.18f, 0.23f, 0.31f, 1.0f);
		return findVASTColour(colVaporizerComponentBackgroundGradientTo);
		//return Colour::fromFloatRGBA(0.82f, 0.83f, 0.86f, 1.0f).darker(.5f); //nexus
		break;
	case colHeaderComponentBackgroundGradientFrom:
		return findVASTColour(colVaporizerComponentBackground);
		//return findVASTColour(colVaporizerComponentBackgroundGradientTo);
		//return Colour::fromFloatRGBA(0.95f, 0.95f, 0.98f, 1.0f); //nexus
		break;
	case colHeaderComponentBackgroundGradientTo:
		return findVASTColour(colVaporizerComponentBackgroundGradientTo);
		//return Colour::fromFloatRGBA(0.82f, 0.83f, 0.86f, 1.0f); //nexus
		break;
	case colMasterVoicingComponentBackgroundGradientFrom:
		return findVASTColour(colHeaderComponentBackgroundGradientTo);
		//return Colour::fromFloatRGBA(0.95f, 0.95f, 0.98f, 1.0f); //nexus
		break;
	case colMasterVoicingComponentBackgroundGradientTo:
		return findVASTColour(colHeaderComponentBackgroundGradientTo);
		//return Colour::fromFloatRGBA(0.82f, 0.83f, 0.86f, 1.0f); //nexus
		break;
	case colWTEditorHeaderBackgroundGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colWTEditorHeaderBackgroundGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colPositionViewportText:
		return findVASTColour(colHeaderLabelText);
		break;
	case colPositionViewportPlusSign:
		return Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case colPositionViewportDragBegin:
		return Colour::fromFloatRGBA(1.0f, 0.0f, 0.0f, 1.0f); //red
		break;
	case colPositionViewportDragEnd:
		return Colour::fromFloatRGBA(0.0f, 1.0f, 0.0f, 1.0f); //green
		break;
	case colPositionViewportSelected:
		return Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 1.0f); //white
		break;
	case colPositionViewportMultiSelected:
		return Colour::fromFloatRGBA(1.0f, 1.0f, 0.0f, 1.0f); //yellow
		break;
	case colPositionViewportFrame:
		return Colour::fromFloatRGBA(0.23f, 0.23f, 0.25f, 1.0f); //grey
		break;
	case colSamplerLine:
		return findVASTColour(colOscilloscopeLine);
		break;
	case colSamplerArea:
		return findVASTColour(colOscilloscopeArea);
		break;
	case colSamplerText:
		return Colour(0xff777777);
		break;
	case colButtonBackground:
		return Colour::fromFloatRGBA(0.32f, 0.33f, 0.36f, 1.0f);
		break;
	case colPresetComponentBackgroundGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colPresetComponentBackgroundGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colPresetComponentRowSelected:
		return Colours::white;
		break;
	case colFreqDomainBinBackground:
		//return Colour::fromFloatRGBA(0.95f, 0.95f, 0.98f, 1.0f); //nexus
		return findVASTColour(colOscilloscopeBackground);
		break;
	case colFreqDomainBinBorder:
		return juce::Colour::fromFloatRGBA(0.4f, 0.4f, 0.4f, 1.0f); //grey;
		break;
	case colFreqDomainBinBorderMouseover:
		return juce::Colour::fromFloatRGBA(0.6f, 0.6f, 0.6f, 1.0f); //grey;
		break;
	case colFreqDomainBinBackgroundOctaves:
		return findVASTColour(colOscilloscopeBackground).brighter(0.4f);
		break;
	case colFreqDomainBinMagnitude:
		//return juce::Colour::fromFloatRGBA(0.43f, 0.63f, 0.95f, 1.0f);
		return findVASTColour(colOscilloscopeLine);
		break;
	case colFreqDomainBinPhases:
		//return juce::Colour::fromFloatRGBA(0.23f, 0.33f, 0.95f, 1.0f);
		return findVASTColour(colOscilloscopeArea);
		break;
	case colFilterComponentBackgroundGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colFilterComponentBackgroundGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colFilterDisplayArea:
		return findVASTColour(colOscilloscopeArea);
		//return Colour::fromFloatRGBA(0.13f, 0.13f, 0.55f, 1.0f);
		break;
	case colFilterDisplayLine:
		return findVASTColour(colOscilloscopeLine);
		//return Colour::fromFloatRGBA(0.43f, 0.43f, 0.75f, 1.0f);
		break;
	case colFilterDisplayGrid:
		return Colour::fromFloatRGBA(0.33f, 0.33f, 0.33f, 1.0f);
		break;
	case colFilterDisplayText:
		return findVASTColour(colLabelText);
		break;
	case colMatrixComponentBackgroundGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colMatrixComponentBackgroundGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colArpComponentBackgroundGradientFrom:
		//return Colour(0xbd1d0727);
		return findVASTColour(colVaporizerComponentBackground);
		break;
	case colArpComponentBackgroundGradientTo:
		//return Colour(0x4301151d);
		return findVASTColour(colVaporizerComponentBackground);
		break;
	case colFXComponentBackgroundGradientFrom:
		return findVASTColour(colVaporizerComponentBackground);
		break;
	case colFXComponentBackgroundGradientTo:
		return findVASTColour(colVaporizerComponentBackground);
		break;
	case colFXComponentTabBarFXBUS1:
		return findVASTColour(colVaporizerComponentTabBarWTEDitorBackground);
		break;
	case colFXComponentTabBarFXBUS2:
		return findVASTColour(colVaporizerComponentTabBarWTEDitorBackground);
		break;
	case colFXComponentTabBarFXBUS3:
		return findVASTColour(colVaporizerComponentTabBarWTEDitorBackground);
		break;
	case colFXSelectionPaneBackground:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colFXEffectPaneBackground:
		return findVASTColour(colOscilloscopeOff);
		break;
	case colFXEditorBackgroundFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colFXEditorBackgroundTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colLFOMsegComponentTabBarMSEG1:
		return findVASTColour(colVaporizerComponentTabBarWTEDitorBackground);
		break;
	case colLFOMsegComponentTabBarMSEG2:
		return findVASTColour(colVaporizerComponentTabBarWTEDitorBackground);
		break;
	case colLFOMsegComponentTabBarMSEG3:
		return findVASTColour(colVaporizerComponentTabBarWTEDitorBackground);
		break;
	case colLFOMsegComponentTabBarMSEG4:
		return findVASTColour(colVaporizerComponentTabBarWTEDitorBackground);
		break;
	case colLFOMsegComponentTabBarMSEG5:
		return findVASTColour(colVaporizerComponentTabBarWTEDitorBackground);
		break;
	case colLFOMsegComponentTabBarLFO1:
		return findVASTColour(colVaporizerComponentTabBarWTEDitorBackground);
		break;
	case colLFOMsegComponentTabBarLFO2:
		return findVASTColour(colVaporizerComponentTabBarWTEDitorBackground);
		break;
	case colLFOMsegComponentTabBarLFO3:
		return findVASTColour(colVaporizerComponentTabBarWTEDitorBackground);
		break;
	case colLFOMsegComponentTabBarLFO4:
		return findVASTColour(colVaporizerComponentTabBarWTEDitorBackground);
		break;
	case colLFOMsegComponentTabBarLFO5:
		return findVASTColour(colVaporizerComponentTabBarWTEDitorBackground);
		break;
	case colLFOMsegComponentBackgroundGradientFrom:
		return findVASTColour(colVaporizerComponentBackground);
		break;
	case colLFOMsegComponentBackgroundGradientTo:
		return findVASTColour(colVaporizerComponentBackground);
		break;
	case colArpComponentTabBarSTEPSEQ1:
		return findVASTColour(colVaporizerComponentTabBarWTEDitorBackground);
		break;
	case colArpComponentTabBarSTEPSEQ2:
		return findVASTColour(colVaporizerComponentTabBarWTEDitorBackground);
		break;
	case colArpComponentTabBarSTEPSEQ3:
		return findVASTColour(colVaporizerComponentTabBarWTEDitorBackground);
		break;
	case colArpComponentTabBarARP1:
		return findVASTColour(colVaporizerComponentTabBarWTEDitorBackground);
		break;
	case colMSEGEditorPaneBackgroundGradientFrom:
		return findVASTColour(colOscilloscopeBackground);
		break;
	case colMSEGEditorPaneBackgroundGradientTo:
		return findVASTColour(colOscilloscopeBackground);
		break;
	case colMSEGEditorPaneHeaderGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colMSEGEditorPaneHeaderGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colLFOEditorPaneBackgroundGradientFrom:
		return findVASTColour(colOscilloscopeBackground);
		break;
	case colLFOEditorPaneBackgroundGradientTo:
		return findVASTColour(colOscilloscopeBackground);
		break;
	case colLFOEditorPaneHeaderGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colLFOEditorPaneHeaderGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colARPEditorPaneBackgroundGradientFrom:
		return findVASTColour(colOscilloscopeBackground);
		break;
	case colARPEditorPaneBackgroundGradientTo:
		return findVASTColour(colOscilloscopeBackground);
		break;
	case colARPEditorPaneHeaderGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colARPEditorPaneHeaderGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colStepSeqEditorPaneBackgroundGradientFrom:
		return findVASTColour(colOscilloscopeBackground);
		break;
	case colStepSeqEditorPaneBackgroundGradientTo:
		return findVASTColour(colOscilloscopeBackground);
		break;
	case colStepSeqEditorPaneHeaderGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colStepSeqEditorPaneHeaderGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colDragSource:
		return Colours::lightgoldenrodyellow;
		break;
	case colARPEditorPosMarker:
		return Colours::yellow.withAlpha(0.3f);
		break;
	case colARPEditorVelocityFrom:
		return findVASTColour(colOscilloscopeArea);
		break;
	case colARPEditorVelocityTo:
		return findVASTColour(colOscilloscopeArea).withAlpha(0.05f);
		break;
	case colARPEditorStep:
		return findVASTColour(colOscilloscopeLineSolo);
		break;
	case colARPEditorStepText:
		return Colours::white;
		break;
	case colARPEditorStepOctave:
		return findVASTColour(colOscilloscopeLine);
		break;
	case colARPEditorGrid:
		return Colours::black.withAlpha(0.5f);
		break;
	case colStepSeqEditorPosMarker:
		return Colours::yellow.withAlpha(0.4f);
		break;
	case colStepSeqEditorEdges:
		return findVASTColour(colOscilloscopeLineSolo).withAlpha(0.4f);
		break;
	case colStepSeqEditorLine:
		return findVASTColour(colOscilloscopeLine);
		break;
	case colStepSeqEditorText:
		return Colour(0xff002a32);
		break;
	case colLFOEditorLine:
		return findVASTColour(colOscilloscopeLine);
		break;
	case colLFOEditorArea:
		return findVASTColour(colOscilloscopeArea).withAlpha(0.4f);
		break;
	case colMSEGEditorPosMarker:
		return Colours::yellow.withAlpha(0.3f);
		break;
	case colMSEGEditorEdge:
		return findVASTColour(colOscilloscopeLineSolo).withAlpha(0.4f);
		break;
	case colMSEGEditorLine:
		return findVASTColour(colOscilloscopeLine);
		break;
	case colMSEGEditorCurvepoint:
		return Colour(0xff002a32);
		break;
	case colMSEGEditorCurvepointOutline:
		return Colours::lightcyan;
		break;
	case colMSEGEditorControlpoint:
		return Colours::yellow.withAlpha(0.8f);
		break;
    case colParameterBindingHighlight:
        return Colour(255,100,0); 
        break;
	}
	
	vassertfalse;
	return Colours::darkgrey; //default
}


/*
void VASTLookAndFeelThemeTech::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
	const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) {
	LookAndFeel_V4::drawRotarySlider(g, x, y, width, height, sliderPos,
		rotaryStartAngle, rotaryEndAngle, slider);
}

void VASTLookAndFeelThemeTech::drawToggleButton(Graphics& g, ToggleButton& button,
	bool isMouseOverButton, bool isButtonDown) {
	LookAndFeel_V4::drawToggleButton(g, button,
		isMouseOverButton, isButtonDown);
}

void VASTLookAndFeelThemeTech::drawButtonText(Graphics& g, TextButton& button, bool isMouseOverButton, bool isButtonDown) {
	LookAndFeel_V4::drawButtonText(g, button, isMouseOverButton, isButtonDown);
}

void VASTLookAndFeelThemeTech::drawLabel(Graphics& g, Label& label) {
	LookAndFeel_V4::drawLabel(g, label);
}

void VASTLookAndFeelThemeTech::drawTextEditorOutline(Graphics& g, int width, int height, TextEditor& textEditor) {
	LookAndFeel_V4::drawTextEditorOutline(g, width, height, textEditor);
}
*/
