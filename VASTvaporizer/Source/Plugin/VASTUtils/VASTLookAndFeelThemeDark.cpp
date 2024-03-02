/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTLookAndFeelThemeDark.h"
#include "../VASTVaporizerComponent.h"
#include "../VASTControls/VASTComboBrev.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"

#include <sstream>

VASTLookAndFeelThemeDark::VASTLookAndFeelThemeDark(CVASTSettings & set, VASTAudioProcessor * processor)
{
	m_Set = &set;
	myProcessor = processor;

	setColour(Label::textColourId, findVASTColour(colLabelText));

	setColour(FileSearchPathListComponent::backgroundColourId, findVASTColour(colFileSearchPathListComponentBackground));
	setColour(FileChooserDialogBox::titleTextColourId, findVASTColour(colFileChooserDialogBoxTitleText));

	setColour(TextEditor::backgroundColourId, findVASTColour(colBoxTextBackground));
	setColour(TextEditor::textColourId, findVASTColour(colBoxText));

	mFontSizeFactor = 0.85f;	
	setUIFontSize(0);

	myDefaultFontRegular = m_Set->getCustomFont(CVASTSettings::customFonts::TradeGothicLT);
	myDefaultFontRegular.setDefaultMinimumHorizontalScaleFactor(1.0);
	myDefaultFontRegular.setSizeAndStyle(12.f, myDefaultFontBold.getStyleFlags(), 1.0f, 0.0f); // no squashing, no kerning

	myDefaultFontBold = m_Set->getCustomFont(CVASTSettings::customFonts::TradeGothicLTBold);
	myDefaultFontBold.setDefaultMinimumHorizontalScaleFactor(1.0);
	myDefaultFontBold.setSizeAndStyle(12.f, myDefaultFontBold.getStyleFlags(), 1.0f, 0.0f); // no squashing, no kerning
	
	setColour(VASTVUMeter::lmTextColour, juce::Colours::lightgrey);
	setColour(VASTVUMeter::lmTextClipColour, juce::Colours::lightgrey);
	setColour(VASTVUMeter::lmTextDeactiveColour, juce::Colours::darkgrey);
	setColour(VASTVUMeter::lmTicksColour, juce::Colours::black.withAlpha(0.0f));
	setColour(VASTVUMeter::lmOutlineColour, juce::Colours::black); //CHVAST
	setColour(VASTVUMeter::lmBackgroundColour, juce::Colour(0xff0b0b0b));
	setColour(VASTVUMeter::lmBackgroundClipColour, juce::Colours::red.withAlpha(0.2f));
	setColour(VASTVUMeter::lmMeterForegroundColour, findVASTColour(colOscilloscopeLine));
	setColour(VASTVUMeter::lmMeterOutlineColour, juce::Colours::black);
	setColour(VASTVUMeter::lmMeterBackgroundColour, juce::Colour(0xff0b0b0b).darker(0.2f)); //CHVAST
	setColour(VASTVUMeter::lmMeterMaxNormalColour, juce::Colours::lightgrey);
	setColour(VASTVUMeter::lmMeterMaxWarnColour, juce::Colours::orange.withAlpha(0.3f));
	setColour(VASTVUMeter::lmMeterMaxOverColour, juce::Colours::darkred.withAlpha(0.3f));
	setColour(VASTVUMeter::lmMeterGradientLowColour, findVASTColour(colOscilloscopeLine).withAlpha(0.6f));
	setColour(VASTVUMeter::lmMeterGradientMidColour, juce::Colours::lightgoldenrodyellow.darker(0.1f).withAlpha(0.6f));
	setColour(VASTVUMeter::lmMeterGradientMaxColour, juce::Colours::darkred.darker(0.1f).withAlpha(0.6f));
	setColour(VASTVUMeter::lmMeterReductionColour, juce::Colours::darkorange.withAlpha(0.2f));
}

VASTLookAndFeelThemeDark::~VASTLookAndFeelThemeDark()
{
}

//VAST Colours
Colour VASTLookAndFeelThemeDark::findVASTColour(int colourID) {
	switch (colourID) {
	case colLabelText:
		return Colour(0xffe2e2e2);
		break;
	case colHeaderLabelText:
		return Colour(0xffe2e2e2);
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
		return findVASTColour(colOscilloscopeLine).darker(0.3f);
		break;
	case colBoxTextBackground:
		return Colours::darkgreen.darker(1.0f).darker(1.0f).darker(0.4f);
		break;
	case colBoxArrow:
		return Colours::white;
		break;
	case colBoxOutline:
		return Colours::lightgrey.darker(0.2f).withMultipliedAlpha(0.5f);
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
		return Colours::black;
		break;
	case colRotarySliderMarker:
		return Colours::white;
		break;
	case colRotarySliderMarkerTip:
		return Colours::darkcyan;
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
		return Colour::fromFloatRGBA(0.55f, 0.6f, 0.6f, 1.0f);
		break;
	case colRotarySliderOuterArcValueIndicator:
		return Colour::fromFloatRGBA(0.2f, 0.8f, 0.4f, 0.7f);
		break;
	case colRotarySliderOuterArcValueIndicatorInner:
		return Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 0.7f);
		break;
	case colRotarySliderOuterArcModulationIndicator:
		return Colours::darkgoldenrod;
		break;
	case colRotarySliderOuterArcModulationIndicatorInner:
		return Colours::yellow;
		break;
	case colRotarySliderOuterArcNonParameter:
		return Colour::fromFloatRGBA(0.25f, 0.4f, 0.7f, 1.0f); //non parameter slider value ranges bluish
		break;
	case colRotarySliderOuterArcNonParameterInner:
		return Colour::fromFloatRGBA(0.55f, 0.55f, 0.9f, 1.0f); //non parameter slider value ranges bluish
		break;
	case colRotarySliderKnobHighlightGradientFrom:
		return Colour::fromFloatRGBA(0.9f, 0.9f, 1.0f, 1.0f);
		break;
	case colRotarySliderKnobHighlightGradientTo:
		return Colour::fromFloatRGBA(0.1f, 0.1f, 0.1f, 1.0f);
		break;
	case colRotarySliderKnobCenterGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo).brighter(0.6f);
		break;
	case colRotarySliderKnobCenterGradientTo:
		return findVASTColour(colOscillatorComponentHeaderGradientFrom).brighter(0.1f);
		break;
	case colRotarySliderKnobCenterMouseOverGradientFrom:
		return Colour::fromFloatRGBA(0.60f, 0.65f, 0.65f, 1.0f);
		break;
	case colRotarySliderKnobCenterMouseOverGradientTo:
		return Colour::fromFloatRGBA(0.2f, 0.2f, 0.25f, 1.0f);
		break;
	case colRotarySliderKnobShadow:
		return Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 1.0f);
		break;	
	case colToggleButtonShade:
		return Colour::fromFloatRGBA(0.7f, 0.7f, 0.7f, 0.1f); //transparent
		break;
	case colToggleButtonShadeMouseOver:
		return Colour::fromFloatRGBA(0.7f, 0.7f, 0.7f, 0.4f); //transparent
		break;
	case colToggleButtonInner:
		return Colour::fromFloatRGBA(0.6f, 0.6f, 0.6f, 1.0f); //transparent
		break;
	case colToggleButtonLight:
		return Colour::fromFloatRGBA(0.5f, 0.5f, 0.5f, 1.0f); //transparent
		break;
	case colToggleButtonOuter:
		return Colour::fromFloatRGBA(0.15f, 0.15f, 0.15f, 1.0f); //transparent
		break;
	case colToggleButtonShadeOn:
		return Colour::fromFloatRGBA(0.7f, 0.9f, 1.0f, 0.35f);
		break;
	case colToggleButtonShadeMouseOverOn:
		return Colour::fromFloatRGBA(0.7f, 0.9f, 1.0f, 0.75f);
		break;
	case colToggleButtonInnerOn:
		return Colour::fromFloatRGBA(0.7f, 0.9f, 1.0f, 1.0f);
		break;
	case colToggleButtonLightOn:
		return Colour::fromFloatRGBA(0.8f, 0.8f, 1.0f, 1.0f);
		break;
	case colToggleButtonOuterOn:
		return Colour::fromFloatRGBA(0.1f, 0.4f, 0.7f, 1.0f);
		break;
	case colToggleButtonBackground:
		return Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case colVaporizerComponentBackgroundGradientFrom:
		return Colour(0xff2a3032);
		break;
	case colVaporizerComponentBackgroundGradientTo:
		return Colour(0xff141516);
		break;
	case colPresetEditorBackgroundGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colPresetEditorBackgroundGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colGroupComponentText:
		return Colour(0xffe2e2e2);
		break;
	case colGroupComponentTextBackground:
		return Colour(0xffe2e2e2);
		break;
	case colGroupComponentOutline:
		return Colour::fromFloatRGBA(0.8f, 0.8f, 0.8f, 0.1f);
		break;
	case colOscillatorComponentBackgroundGradientFrom:
		return Colour(0xff2b3134); //43 49 52
		break;
	case colOscillatorComponentBackgroundGradientTo:
		return Colour(0xff2e2a37); //46 42 55
		break;
	case colOscillatorComponentHeaderGradientFrom:
		return Colour(0xff000000); //black
		break;
	case colOscillatorComponentHeaderGradientTo:
		return Colour(0xff000000); //black
		break;
	case colOscilloscopeBackground:
		return Colour::fromFloatRGBA(0.f, 0.f, 0.f, 1.0f);
		break;		
	case colOscilloscopeOff:
		return Colour::fromFloatRGBA(0.f, 0.f, 0.f, 1.0f);
		break;
	case colOscilloscopeLine:
		return Colour::fromFloatRGBA(0.38f, 0.9f, 0.76f, 1.0f); //72 116 97
		break;
	case colOscilloscopeLineSolo:
		return Colour::fromFloatRGBA(0.1f, 0.3f, 0.9f, 1.0f);  //blueish
		break;
	case colOscilloscopeArea:
		return Colour::fromFloatRGBA(0.18f, 0.5f, 0.5f, 1.0f);
		break;
	case colOscilloscopeAreaSolo:
		return findVASTColour(colOscilloscopeLineSolo).withAlpha(0.6f);
		break;
	case colOscilloscopeSelection:
		return Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 1.0f);  
		break;
	case colVaporizerComponentTabBarWTEDitorBackground:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colVaporizerComponentTabBarFilterBackground:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colVaporizerComponentTabBarLFOMSEGBackground:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colVaporizerComponentTabBarMatrixBackground:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colVaporizerComponentTabBarFXBackground:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colVaporizerComponentTabBarARPBackground:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colVaporizerComponentTabBarPresetBackground:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colVaporizerComponentTabBarTextSelected:
		return Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case colVaporizerComponentTabBarText:
		return Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case colVaporizerComponentBackground:
		return Colour::fromFloatRGBA(0.0f, 0.05f, 0.05f, 1.0f); 
		break;
	case colHeaderComponentBackgroundGradientFrom:
		return findVASTColour(colVaporizerComponentBackground);
		//return Colour(0xff212527);
		//return Colour(0xff0b0b0b);
		break;
	case colHeaderComponentBackgroundGradientTo:
		return Colour(0xff0b0b0b);
		break;
	case colMasterVoicingComponentBackgroundGradientFrom:
		return Colour(0xff0b0b0b);
		break;
	case colMasterVoicingComponentBackgroundGradientTo:
		return Colour(0xff0b0b0b);
		break;
	case colWTEditorHeaderBackgroundGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colWTEditorHeaderBackgroundGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colPositionViewportText:
		return Colour::fromFloatRGBA(0.8f, 0.8f, 0.8f, 1.0f);
		break;		
	case colPositionViewportPlusSign:
		return Colour::fromFloatRGBA(0.7f, 0.7f, 0.1f, 1.0f); 
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
		return Colour::fromFloatRGBA(0.05f, 0.12f, 0.4f, 1.0f); // blue 
		break;
	case colSamplerArea:
		return Colour::fromFloatRGBA(0.25f, 0.45f, 0.6f, 0.3f); // blue dark
		break;
	case colSamplerText:
		return Colour(0xff777777);
		break;
	case colButtonBackground:
		return Colour(0xff181f22);
		break;
	case colPresetComponentBackgroundGradientFrom:
		//return Colour(0xb3ffffff);
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colPresetComponentBackgroundGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		//return Colour(0x00000000);
		break;
	case colPresetComponentRowSelected:
		return Colours::lightblue;
		break;
	case colFreqDomainBinBackground:
		return Colour::fromFloatRGBA(0.05f, 0.08f, 0.05f, 1.0f);
		break;
	case colFreqDomainBinBorder:
		return juce::Colour::fromFloatRGBA(0.33f, 0.33f, 0.35f, 1.0f); //grey;
		break;
	case colFreqDomainBinBorderMouseover:
		return juce::Colour::fromFloatRGBA(0.53f, 0.53f, 0.55f, 1.0f); //grey;
		break;
	case colFreqDomainBinBackgroundOctaves:
		return juce::Colour::fromFloatRGBA(0.13f, 0.32f, 0.18f, 0.5f); //greenish
		break;
	case colFreqDomainBinMagnitude:
		return Colour::fromFloatRGBA(0.1f, 0.50f, 0.55f, 1.0f);
		break;
	case colFreqDomainBinPhases:
		return Colour::fromFloatRGBA(0.1f, 0.50f, 0.55f, 0.7f);
		break;
    case colFilterComponentBackgroundGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
        break;
    case colFilterComponentBackgroundGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
        break;
    case colFilterDisplayArea:
		return Colour::fromFloatRGBA(0.29f, 0.38f, 0.5f, 0.6f); //72 116 97 / b g exchange
        break;
    case colFilterDisplayLine:
		return Colour::fromFloatRGBA(0.58f, 0.76f, 1.0f, 1.0f); //72 116 97 / b g exchange
        break;
    case colFilterDisplayGrid:
        return Colour::fromFloatRGBA(0.33f, 0.33f, 0.33f, 1.0f);
        break;
    case colFilterDisplayText:
		return findVASTColour(colLabelText); //(0xffaaaaaa);
        break;
    case colMatrixComponentBackgroundGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
        break;
    case colMatrixComponentBackgroundGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
        break;
    case colArpComponentBackgroundGradientFrom:
        //return Colour(0xbd1d0727);
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
        break;
    case colArpComponentBackgroundGradientTo:
        //return Colour(0x4301151d);
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
        break;
    case colFXComponentBackgroundGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
        break;
    case colFXComponentBackgroundGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
        break;
	case colFXComponentTabBarFXBUS1:
		return Colour(0xff7e7e60);
		break;
	case colFXComponentTabBarFXBUS2:
		return Colour(0xff8a9362);
		break;
	case colFXComponentTabBarFXBUS3:
		return Colour(0xff9a9a90);
		break;
    case colFXSelectionPaneBackground:
        return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
        break;
    case colFXEffectPaneBackground:
        return Colours::black;
        break;
	case colFXEditorBackgroundFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colFXEditorBackgroundTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colLFOMsegComponentTabBarMSEG1:
		return findVASTColour(colMSEGEditorLine).withAlpha(0.5f);
		break;
	case colLFOMsegComponentTabBarMSEG2:
		return findVASTColour(colMSEGEditorLine).withAlpha(0.45f);
		break;
	case colLFOMsegComponentTabBarMSEG3:
		return findVASTColour(colMSEGEditorLine).withAlpha(0.4f);
		break;
	case colLFOMsegComponentTabBarMSEG4:
		return findVASTColour(colMSEGEditorLine).withAlpha(0.35f);
		break;
	case colLFOMsegComponentTabBarMSEG5:
		return findVASTColour(colMSEGEditorLine).withAlpha(0.3f);
		break;
	case colLFOMsegComponentTabBarLFO1:
		return findVASTColour(colLFOEditorLine).withAlpha(0.5f);
		break;
	case colLFOMsegComponentTabBarLFO2:
		return findVASTColour(colLFOEditorLine).withAlpha(0.45f);
		break;
	case colLFOMsegComponentTabBarLFO3:
		return findVASTColour(colLFOEditorLine).withAlpha(0.4f);
		break;
	case colLFOMsegComponentTabBarLFO4:
		return findVASTColour(colLFOEditorLine).withAlpha(0.35f);
		break;
	case colLFOMsegComponentTabBarLFO5:
		return findVASTColour(colLFOEditorLine).withAlpha(0.3f);
		break;
	case colLFOMsegComponentBackgroundGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colLFOMsegComponentBackgroundGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colArpComponentTabBarSTEPSEQ1:
		return findVASTColour(colStepSeqEditorLine).withAlpha(0.5f);
		break;
	case colArpComponentTabBarSTEPSEQ2:
		return findVASTColour(colStepSeqEditorLine).withAlpha(0.45f);
		break;
	case colArpComponentTabBarSTEPSEQ3:
		return findVASTColour(colStepSeqEditorLine).withAlpha(0.4f);
		break;
	case colArpComponentTabBarARP1:
		return Colour(0xff002a32);
		break;
	case colMSEGEditorPaneBackgroundGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colMSEGEditorPaneBackgroundGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colMSEGEditorPaneHeaderGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colMSEGEditorPaneHeaderGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colLFOEditorPaneBackgroundGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colLFOEditorPaneBackgroundGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colLFOEditorPaneHeaderGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colLFOEditorPaneHeaderGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colARPEditorPaneBackgroundGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colARPEditorPaneBackgroundGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colARPEditorPaneHeaderGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colARPEditorPaneHeaderGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
		break;
	case colStepSeqEditorPaneBackgroundGradientFrom:
		return findVASTColour(colOscillatorComponentBackgroundGradientFrom);
		break;
	case colStepSeqEditorPaneBackgroundGradientTo:
		return findVASTColour(colOscillatorComponentBackgroundGradientTo);
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
		return findVASTColour(colOscilloscopeLine).darker(0.2f).withAlpha(0.2f);
		break;
	case colARPEditorVelocityFrom:
		return  Colours::white.withAlpha(0.5f);
		break;
	case colARPEditorVelocityTo:
		return Colours::white.withAlpha(0.05f);
		break;
	case colARPEditorStep:
		return findVASTColour(colFilterDisplayLine).darker(0.2f);
		break;
	case colARPEditorStepText:
		return Colours::white;
		break;
	case colARPEditorStepOctave:
		return findVASTColour(colOscilloscopeLine).darker(0.2f);
		break;
	case colARPEditorGrid:
		return Colours::black;
		break;
	case colStepSeqEditorPosMarker:
		return findVASTColour(colOscilloscopeLine).darker(0.2f).withAlpha(0.2f);
		break;
	case colStepSeqEditorEdges:
		return Colours::lightblue.withAlpha(0.3f);
		break;
	case colStepSeqEditorLine:
		return findVASTColour(colOscilloscopeLine).darker(0.2f);
		break;
	case colStepSeqEditorText:
		return findVASTColour(colLabelText); // Colour(0xff002a32);
		break;
	case colLFOEditorLine:
		return findVASTColour(colFilterDisplayLine);
		break;
	case colLFOEditorArea:
		return findVASTColour(colFilterDisplayLine).withAlpha(0.6f);
		break;
	case colMSEGEditorPosMarker:
		return Colours::yellow.withAlpha(0.3f);
		break;
	case colMSEGEditorEdge:
		return Colours::lightblue.withAlpha(0.3f);
		break;
	case colMSEGEditorLine:
		return Colour::fromFloatRGBA(0.7f, 1.0f, 0.8f, 1.f);
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

