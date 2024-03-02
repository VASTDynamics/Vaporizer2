/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTLookAndFeelThemeDefault.h"
#include "../VASTVaporizerComponent.h"
#include "../VASTControls/VASTComboBrev.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"

#include <sstream>

VASTLookAndFeelThemeDefault::VASTLookAndFeelThemeDefault(CVASTSettings& set, VASTAudioProcessor* processor)
{
	m_Set = &set;
	myProcessor = processor;

	setColour(Label::textColourId, findVASTColour(colLabelText));

	setColour(FileSearchPathListComponent::backgroundColourId, findVASTColour(colFileSearchPathListComponentBackground));
	setColour(FileChooserDialogBox::titleTextColourId, findVASTColour(colFileChooserDialogBoxTitleText));

	setColour(TextEditor::backgroundColourId, findVASTColour(colBoxTextBackground));
	setColour(TextEditor::textColourId, findVASTColour(colBoxText));

	mFontSizeFactor = 0.95f;
	setUIFontSize(0);
	myDefaultFontRegular = m_Set->getCustomFont(CVASTSettings::customFonts::OpenSans);
	myDefaultFontRegular.setDefaultMinimumHorizontalScaleFactor(1.0);
	myDefaultFontRegular.setSizeAndStyle(12.f, myDefaultFontBold.getStyleFlags(), 1.0f, 0.0f); // no squashing, no kerning

	myDefaultFontBold = m_Set->getCustomFont(CVASTSettings::customFonts::OpenSansBold);
	myDefaultFontBold.setDefaultMinimumHorizontalScaleFactor(1.0);
	myDefaultFontBold.setSizeAndStyle(12.f, myDefaultFontBold.getStyleFlags(), 1.0f, 0.0f); // no squashing, no kerning
	
	setColour(VASTVUMeter::lmTextColour, juce::Colours::lightgrey);
	setColour(VASTVUMeter::lmTextClipColour, juce::Colours::lightgrey);
	setColour(VASTVUMeter::lmTextDeactiveColour, juce::Colours::darkgrey);
	setColour(VASTVUMeter::lmTicksColour, juce::Colours::black.withAlpha(0.0f));
	setColour(VASTVUMeter::lmOutlineColour, juce::Colours::black); //CHVAST
	setColour(VASTVUMeter::lmBackgroundColour, juce::Colour(0xff0b0b0b));
	setColour(VASTVUMeter::lmBackgroundClipColour, juce::Colours::red.withAlpha(0.2f));
	setColour(VASTVUMeter::lmMeterForegroundColour, juce::Colours::green);
	setColour(VASTVUMeter::lmMeterOutlineColour, juce::Colours::black);
	setColour(VASTVUMeter::lmMeterBackgroundColour, juce::Colour(0xff0b0b0b).darker(0.2f)); //CHVAST
	setColour(VASTVUMeter::lmMeterMaxNormalColour, juce::Colours::lightgrey);
	setColour(VASTVUMeter::lmMeterMaxWarnColour, juce::Colours::orange.withAlpha(0.3f));
	setColour(VASTVUMeter::lmMeterMaxOverColour, juce::Colours::darkred.withAlpha(0.3f));
	setColour(VASTVUMeter::lmMeterGradientLowColour, juce::Colours::darkgreen.darker(0.1f).withAlpha(0.6f));
	setColour(VASTVUMeter::lmMeterGradientMidColour, juce::Colours::lightgoldenrodyellow.darker(0.1f).withAlpha(0.6f));
	setColour(VASTVUMeter::lmMeterGradientMaxColour, juce::Colours::darkred.darker(0.1f).withAlpha(0.6f));
	setColour(VASTVUMeter::lmMeterReductionColour, juce::Colours::darkorange.withAlpha(0.2f));
}

VASTLookAndFeelThemeDefault::~VASTLookAndFeelThemeDefault()
{
}

//VAST Colours
Colour VASTLookAndFeelThemeDefault::findVASTColour(int colourID) {
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
		return Colours::lightblue.darker(0.7f);
		break;
	case colBoxTextBackground:
		return Colours::darkgreen.darker(1.0f).darker(1.0f).darker(0.8f);
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
		return Colour::fromFloatRGBA(0.50f, 0.55f, 0.55f, 1.0f);
		break;
	case colRotarySliderKnobCenterGradientTo:
		return Colour::fromFloatRGBA(0.1f, 0.1f, 0.15f, 1.0f);
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
		return Colour(0x4f616d7e);
		break;
	case colPresetEditorBackgroundGradientTo:
		return Colour(0xbd03070a);
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
		return Colour(0xff56646c);
		break;
	case colOscillatorComponentBackgroundGradientTo:
		return Colour(0xff232527);
		break;
	case colOscillatorComponentHeaderGradientFrom:
		return Colour(0xff3a4145);
		break;
	case colOscillatorComponentHeaderGradientTo:
		return Colour(0xff33393c);
		break;
	case colOscilloscopeBackground:
		return Colour::fromFloatRGBA(0.1f, 0.1f, 0.12f, 1.0f);
		break;		
	case colOscilloscopeOff:
		return Colour::fromFloatRGBA(0.05f, 0.08f, 0.05f, 1.0f);
		break;
	case colOscilloscopeLine:
		return Colour::fromFloatRGBA(0.4f, 0.9f, 0.1f, 1.0f); //greenish
		break;
	case colOscilloscopeLineSolo:
		return Colour::fromFloatRGBA(0.1f, 0.3f, 0.9f, 1.0f);  //blueish
		break;
	case colOscilloscopeArea:
		return Colour::fromFloatRGBA(0.4f, 0.9f, 0.1f, 0.3f); //greenish
		break;
	case colOscilloscopeAreaSolo:
		return Colour::fromFloatRGBA(0.1f, 0.3f, 0.9f, 0.5f);  //blueish
		break;
	case colOscilloscopeSelection:
		return Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 1.0f);  
		break;
	case colVaporizerComponentTabBarWTEDitorBackground:
		return Colour(0xff3e3e3e);
		break;
	case colVaporizerComponentTabBarFilterBackground:
		return Colour(0xff334761);
		break;
	case colVaporizerComponentTabBarLFOMSEGBackground:
		return Colour(0xff68493e);
		break;
	case colVaporizerComponentTabBarMatrixBackground:
		return Colour(0xff2d5f33);
		break;
	case colVaporizerComponentTabBarFXBackground:
		return Colour(0xff7d7d3b);
		break;
	case colVaporizerComponentTabBarARPBackground:
		return Colour(0xff73376c);
		break;
	case colVaporizerComponentTabBarPresetBackground:
		return Colour(0xff3d6065);
		break;
	case colVaporizerComponentTabBarTextSelected:
		return Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case colVaporizerComponentTabBarText:
		return Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case colVaporizerComponentBackground:
		return Colour::fromFloatRGBA(0.1f, 0.1f, 0.1f, 1.0f);
		break;
	case colHeaderComponentBackgroundGradientFrom:
		return findVASTColour(colVaporizerComponentBackground);
		//return Colour(0xff212527);
		//return Colour(0xff212527);
		break;
	case colHeaderComponentBackgroundGradientTo:
		return Colour(0xff212527);
		break;
	case colMasterVoicingComponentBackgroundGradientFrom:
		return Colour(0xff212527);
		//return Colour(0xff171818);		
		break;
	case colMasterVoicingComponentBackgroundGradientTo:
		return Colour(0xff212527);
		break;
	case colWTEditorHeaderBackgroundGradientFrom:
		return Colour(0x4fa2a7b5);
		break;
	case colWTEditorHeaderBackgroundGradientTo:
		return Colour(0xbd141618);
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
		return Colour::fromFloatRGBA(0.1f, 0.3f, 0.7f, 1.0f); // blue 
		break;
	case colSamplerArea:
		return Colour::fromFloatRGBA(0.1f, 0.3f, 0.7f, 0.3f); // blue dark
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
		return juce::Colour::fromFloatRGBA(0.63f, 0.63f, 0.95f, 1.0f);
		break;
	case colFreqDomainBinPhases:
		return juce::Colour::fromFloatRGBA(0.83f, 0.83f, 0.95f, 1.0f);
		break;
    case colFilterComponentBackgroundGradientFrom:
        return Colour (0xbd142b47);
        break;
    case colFilterComponentBackgroundGradientTo:
        return Colour (0x43121314);
        break;
    case colFilterDisplayArea:
        return Colour::fromFloatRGBA(0.13f, 0.13f, 0.55f, 1.0f);
        break;
    case colFilterDisplayLine:
        return Colour::fromFloatRGBA(0.53f, 0.63f, 0.95f, 1.0f);
        break;
    case colFilterDisplayGrid:
        return Colour::fromFloatRGBA(0.33f, 0.33f, 0.33f, 1.0f);
        break;
    case colFilterDisplayText:
		return findVASTColour(colLabelText); //(0xffaaaaaa);
        break;
    case colMatrixComponentBackgroundGradientFrom:
        return Colour(0xbd1c341a);
        break;
    case colMatrixComponentBackgroundGradientTo:
        return Colour(0x4307200a);
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
		return Colour(0xff7e7e60);
		break;
	case colFXComponentTabBarFXBUS2:
		return Colour(0xff8a9362);
		break;
	case colFXComponentTabBarFXBUS3:
		return Colour(0xff9a9a90);
		break;
    case colFXSelectionPaneBackground:
        return Colour(0xff323e44);
        break;
    case colFXEffectPaneBackground:
        return Colours::black;
        break;
	case colFXEditorBackgroundFrom:
		return Colour::fromFloatRGBA(0.25f, 0.30f, 0.34f, 1.0f).darker(.1f);
		break;
	case colFXEditorBackgroundTo:
		return Colour::fromFloatRGBA(0.08f, 0.09f, 0.10f, 1.0f).darker(.1f);
		break;
	case colLFOMsegComponentTabBarMSEG1:
		return Colour(0x90bc6445);
		break;
	case colLFOMsegComponentTabBarMSEG2:
		return Colour(0x80bc6445);
		break;
	case colLFOMsegComponentTabBarMSEG3:
		return Colour(0x70bc6445);
		break;
	case colLFOMsegComponentTabBarMSEG4:
		return Colour(0x60bc6445);
		break;
	case colLFOMsegComponentTabBarMSEG5:
		return Colour(0x50bc6445);
		break;
	case colLFOMsegComponentTabBarLFO1:
		return Colour(0xff002a32);
		break;
	case colLFOMsegComponentTabBarLFO2:
		return Colour(0xf0002a32);
		break;
	case colLFOMsegComponentTabBarLFO3:
		return Colour(0xe0002a32);
		break;
	case colLFOMsegComponentTabBarLFO4:
		return Colour(0xd0002a32);
		break;
	case colLFOMsegComponentTabBarLFO5:
		return Colour(0xc0002a32);
		break;
	case colLFOMsegComponentBackgroundGradientFrom:
		return findVASTColour(colVaporizerComponentBackground);
		break;
	case colLFOMsegComponentBackgroundGradientTo:
		return findVASTColour(colVaporizerComponentBackground);
		break;
	case colArpComponentTabBarSTEPSEQ1:
		return Colour(0x90bc6445);
		break;
	case colArpComponentTabBarSTEPSEQ2:
		return Colour(0x80bc6445);
		break;
	case colArpComponentTabBarSTEPSEQ3:
		return Colour(0x70bc6445);
		break;
	case colArpComponentTabBarARP1:
		return Colour(0xff002a32);
		break;
	case colMSEGEditorPaneBackgroundGradientFrom:
		return Colour(0x50bc6445);
		break;
	case colMSEGEditorPaneBackgroundGradientTo:
		return Colour(0xff141516);
		break;
	case colMSEGEditorPaneHeaderGradientFrom:
		return Colour(0xff3f4446);
		break;
	case colMSEGEditorPaneHeaderGradientTo:
		return Colour(0xff4c5457);
		break;
	case colLFOEditorPaneBackgroundGradientFrom:
		return Colour(0x50bc6445);
		break;
	case colLFOEditorPaneBackgroundGradientTo:
		return Colour(0xff141516);
		break;
	case colLFOEditorPaneHeaderGradientFrom:
		return Colour(0xff3f4446);
		break;
	case colLFOEditorPaneHeaderGradientTo:
		return Colour(0xff4c5457);
		break;
	case colARPEditorPaneBackgroundGradientFrom:
		return Colour(0x50bc6445);
		break;
	case colARPEditorPaneBackgroundGradientTo:
		return Colour(0xff141516);
		break;
	case colARPEditorPaneHeaderGradientFrom:
		return Colour(0xff3f4446);
		break;
	case colARPEditorPaneHeaderGradientTo:
		return Colour(0xff4c5457);
		break;
	case colStepSeqEditorPaneBackgroundGradientFrom:
		return Colour(0x50bc6445);
		break;
	case colStepSeqEditorPaneBackgroundGradientTo:
		return Colour(0xff141516);
		break;
	case colStepSeqEditorPaneHeaderGradientFrom:
		return Colour(0xff3f4446);
		break;
	case colStepSeqEditorPaneHeaderGradientTo:
		return Colour(0xff4c5457);
		break;
	case colDragSource:
		return Colours::lightgoldenrodyellow;
		break;
	case colARPEditorPosMarker:
		return Colours::yellow.withAlpha(0.3f);
		break;
	case colARPEditorVelocityFrom:
		return  Colours::white.withAlpha(0.5f);
		break;
	case colARPEditorVelocityTo:
		return Colours::white.withAlpha(0.05f);
		break;
	case colARPEditorStep:
		return Colours::orange.withAlpha(0.5f).darker(0.4f);
		break;
	case colARPEditorStepText:
		return Colours::white;
		break;
	case colARPEditorStepOctave:
		return Colours::lightgreen.darker(0.7f);
		break;
	case colARPEditorGrid:
		return Colours::black;
		break;
	case colStepSeqEditorPosMarker:
		return Colours::yellow.withAlpha(0.3f);
		break;
	case colStepSeqEditorEdges:
		return Colours::lightblue.withAlpha(0.3f);
		break;
	case colStepSeqEditorLine:
		return Colours::lightcyan;
		break;
	case colStepSeqEditorText:
		return findVASTColour(colLabelText); // Colour(0xff002a32);
		break;
	case colLFOEditorLine:
		return Colour::fromFloatRGBA(0.8f, 0.8f, 0.8f, 0.8f);
		break;
	case colLFOEditorArea:
		return Colours::cornflowerblue.withAlpha(0.35f);
		break;
	case colMSEGEditorPosMarker:
		return Colours::yellow.withAlpha(0.3f);
		break;
	case colMSEGEditorEdge:
		return Colours::lightblue.withAlpha(0.3f);
		break;
	case colMSEGEditorLine:
		return Colours::lightcyan;
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

