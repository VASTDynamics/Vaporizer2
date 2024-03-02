/*
VAST Dynamics
*/
#ifndef VASTLOOKANDFEEL_H_INCLUDED
#define VASTLOOKANDFEEL_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../VASTVUMeter.h"

enum VASTColours {
	colLabelText,
	colHeaderLabelText,
	colLabelOutline,
	colLabelOutlineEnabled,
	colLabelBackground,
	colLabelCustom,
	colBoxText,
	colBoxTextBackground,
	colBoxArrow,
	colBoxOutline,
	colMidiLearn,
	colDisabled,
	colAlmostBlack,
	colFileSearchPathListComponentBackground,
	colFileChooserDialogBoxTitleText,
	colLinearSliderThumbKnob,
	colLinearSliderThumbKnobMouseOver,
	colGlassLozengeGradient,
	colRotarySliderMarkerShadow,
	colRotarySliderMarker,
	colRotarySliderMarkerTip,
	colScrollbarButton,
	colScrollbarButtonDown,
	colScrollBarGradient,
	colFileBrowserIcon,
	colRotarySliderOuterArcBackground,
	colRotarySliderOuterArcValueIndicator,
	colRotarySliderOuterArcValueIndicatorInner,
	colRotarySliderOuterArcModulationIndicator,
	colRotarySliderOuterArcModulationIndicatorInner,
	colRotarySliderOuterArcNonParameter,
	colRotarySliderOuterArcNonParameterInner,
	colRotarySliderKnobHighlightGradientFrom,
	colRotarySliderKnobHighlightGradientTo,
	colRotarySliderKnobCenterGradientFrom,
	colRotarySliderKnobCenterGradientTo,
	colRotarySliderKnobCenterMouseOverGradientFrom,
	colRotarySliderKnobCenterMouseOverGradientTo,
	colRotarySliderKnobShadow,
	colToggleButtonShade,
	colToggleButtonShadeMouseOver,
	colToggleButtonInner,
	colToggleButtonLight,
	colToggleButtonOuter,
	colToggleButtonShadeOn,
	colToggleButtonShadeMouseOverOn,
	colToggleButtonInnerOn,
	colToggleButtonLightOn,
	colToggleButtonOuterOn,
	colToggleButtonBackground,
	colVaporizerComponentBackgroundGradientFrom,
	colVaporizerComponentBackgroundGradientTo,
	colPresetEditorBackgroundGradientFrom,
	colPresetEditorBackgroundGradientTo,
	colGroupComponentText,
	colGroupComponentTextBackground,
	colGroupComponentOutline,
	colOscillatorComponentBackgroundGradientFrom,
	colOscillatorComponentBackgroundGradientTo,
	colOscillatorComponentHeaderGradientFrom,
	colOscillatorComponentHeaderGradientTo,
	colOscilloscopeBackground,
	colOscilloscopeOff,
	colOscilloscopeLine,
	colOscilloscopeLineSolo,
	colOscilloscopeArea,
	colOscilloscopeAreaSolo,
	colOscilloscopeSelection,
	colVaporizerComponentTabBarWTEDitorBackground,   // WT EDITOR
	colVaporizerComponentTabBarFilterBackground,   // FILTER
	colVaporizerComponentTabBarLFOMSEGBackground,   // LFO MSEG ENV
	colVaporizerComponentTabBarMatrixBackground,   // MATRIX
	colVaporizerComponentTabBarFXBackground,  // FX
	colVaporizerComponentTabBarARPBackground,   // ARP
	colVaporizerComponentTabBarPresetBackground, // PRESET
	colVaporizerComponentTabBarTextSelected,
	colVaporizerComponentTabBarText,
	colVaporizerComponentBackground,
	colHeaderComponentBackgroundGradientFrom,
	colHeaderComponentBackgroundGradientTo,
	colMasterVoicingComponentBackgroundGradientFrom,
	colMasterVoicingComponentBackgroundGradientTo,
	colWTEditorHeaderBackgroundGradientFrom,
	colWTEditorHeaderBackgroundGradientTo,
	colPositionViewportText,
	colPositionViewportPlusSign,
	colPositionViewportDragBegin,
	colPositionViewportDragEnd,
	colPositionViewportSelected,
	colPositionViewportMultiSelected,
	colPositionViewportFrame,
	colSamplerLine,
	colSamplerArea,
	colSamplerText,
	colButtonBackground,
	colPresetComponentBackgroundGradientFrom,
	colPresetComponentBackgroundGradientTo,
	colPresetComponentRowSelected,
	colFreqDomainBinBackground,
	colFreqDomainBinBorder,
	colFreqDomainBinBorderMouseover,
	colFreqDomainBinBackgroundOctaves,
	colFreqDomainBinMagnitude,
	colFreqDomainBinPhases,
    colFilterComponentBackgroundGradientFrom,
    colFilterComponentBackgroundGradientTo,
    colFilterDisplayArea,
    colFilterDisplayLine,
    colFilterDisplayGrid,
    colFilterDisplayText,
    colMatrixComponentBackgroundGradientFrom,
    colMatrixComponentBackgroundGradientTo,
    colArpComponentBackgroundGradientFrom,
    colArpComponentBackgroundGradientTo,
    colFXComponentBackgroundGradientFrom,
    colFXComponentBackgroundGradientTo,
	colFXComponentTabBarFXBUS1,
	colFXComponentTabBarFXBUS2,
	colFXComponentTabBarFXBUS3,
    colFXSelectionPaneBackground,
    colFXEffectPaneBackground,
	colFXEditorBackgroundFrom,
	colFXEditorBackgroundTo,
	colLFOMsegComponentTabBarMSEG1,
	colLFOMsegComponentTabBarMSEG2,
	colLFOMsegComponentTabBarMSEG3,
	colLFOMsegComponentTabBarMSEG4,
	colLFOMsegComponentTabBarMSEG5,
	colLFOMsegComponentTabBarLFO1,
	colLFOMsegComponentTabBarLFO2,
	colLFOMsegComponentTabBarLFO3,
	colLFOMsegComponentTabBarLFO4,
	colLFOMsegComponentTabBarLFO5,
	colLFOMsegComponentBackgroundGradientFrom,
	colLFOMsegComponentBackgroundGradientTo,
	colArpComponentTabBarSTEPSEQ1,
	colArpComponentTabBarSTEPSEQ2,
	colArpComponentTabBarSTEPSEQ3,
	colArpComponentTabBarARP1,
	colMSEGEditorPaneBackgroundGradientFrom,
	colMSEGEditorPaneBackgroundGradientTo,
	colMSEGEditorPaneHeaderGradientFrom,
	colMSEGEditorPaneHeaderGradientTo,
	colLFOEditorPaneBackgroundGradientFrom,
	colLFOEditorPaneBackgroundGradientTo,
	colLFOEditorPaneHeaderGradientFrom,
	colLFOEditorPaneHeaderGradientTo,
	colARPEditorPaneBackgroundGradientFrom,
	colARPEditorPaneBackgroundGradientTo,
	colARPEditorPaneHeaderGradientFrom,
	colARPEditorPaneHeaderGradientTo,
	colStepSeqEditorPaneBackgroundGradientFrom,
	colStepSeqEditorPaneBackgroundGradientTo,
	colStepSeqEditorPaneHeaderGradientFrom,
	colStepSeqEditorPaneHeaderGradientTo,
	colDragSource,
	colARPEditorPosMarker,
	colARPEditorVelocityFrom,
	colARPEditorVelocityTo,
	colARPEditorStep,
	colARPEditorStepText,
	colARPEditorStepOctave,
	colARPEditorGrid,
	colStepSeqEditorPosMarker,
	colStepSeqEditorEdges,
	colStepSeqEditorLine,
	colStepSeqEditorText,
	colLFOEditorLine,
	colLFOEditorArea,
	colMSEGEditorPosMarker,
	colMSEGEditorEdge,
	colMSEGEditorLine,		
	colMSEGEditorCurvepoint,
	colMSEGEditorCurvepointOutline,
	colMSEGEditorControlpoint,
    colParameterBindingHighlight
};

class VASTAudioProcessor; // forward declaration
class VASTAudioProcessorEditor; // forward declaration
class CVASTSettings; // forward declaration
class VASTLookAndFeel : public LookAndFeel_V4
{
public:
	//Virtuals -------------------------------------------------

	CVASTSettings *m_Set;
	VASTAudioProcessor * myProcessor;

	void initAll();

	virtual Colour findVASTColour(int colourID) = 0;

	int presetTableSortColumn = -1;
	bool presetTableSortColumnForward = true;
	float m_lastTextEditorFontHeight = 16.f;
	
	bool m_isUsingBackgroundImages = false;

	// Explicits -----------------------------------------------
	
	Colour createBaseColour(Colour buttonColour,
		bool hasKeyboardFocus,
		bool isMouseOverButton,
		bool isButtonDown) noexcept;

	//Typeface::Ptr getTypefaceForFont(const Font &font) override;

	Array<Image> m_knobBufferArray;
	Array<Image> m_knobBufferArrayMouseOver;

	void setUIFontSize(int size);

	float mFontSizeFactor = 1.f;
	float mFontSize = 1.f;
	Font getLabelFont(Label&) override;
	Font getTextButtonFont(TextButton& textButton, int buttonHeight) override;
	Font getPopupMenuFont() override;
	Font getTabButtonFont(TabBarButton &, float height) override;
	Font getTextEditorFont(TextEditor& textEditor);
	Font getDefaultFont();
	Font getDefaultFontBold();

	Font myDefaultFontRegular;
	Font myDefaultFontBold;

	void setCurrentScalingFactors(float scaleFactorWidth, float scaleFactorHeight);

	void drawTooltip(Graphics& g, const String& text, int width, int height) override;
	juce::Rectangle<int> getTooltipBounds(const String& tipText, Point<int> screenPos, juce::Rectangle<int> parentArea) override;

	Font getSidePanelTitleFont(SidePanel&) override;	
	Justification getSidePanelTitleJustification(SidePanel&) override;	
	Path getSidePanelDismissButtonShape(SidePanel&) override;
	
	Slider::SliderLayout getSliderLayout(Slider& slider) override;
	void drawGroupComponentOutline(Graphics&, int w, int h, const String& text, const Justification&, GroupComponent&) override;
	void drawButtonText(Graphics& g, TextButton& button, bool /*isMouseOverButton*/, bool /*isButtonDown*/) override;
	void drawLabel(Graphics& g, Label& label) override;
	BorderSize<int> getLabelBorderSize(Label& label) override;

	Array<int> getWidthsForTextButtons(AlertWindow&, const Array<TextButton*>& buttons) override;
	int getAlertWindowButtonHeight() override;

	int getCallOutBoxBorderSize(const CallOutBox &) override;
	float getCallOutBoxCornerSize(const CallOutBox &) override;

	Label* createComboBoxTextBox(ComboBox& box) override;

	void drawTabButton(TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown) override;
	virtual void createTabTextLayout(const TabBarButton& button, float length, float depth, Colour colour, TextLayout& textLayout);
	int getTabButtonBestWidth(TabBarButton&, int tabDepth) override;

	bool shouldPopupMenuScaleWithTargetComponent(const PopupMenu::Options &options) override;
	void getIdealPopupMenuItemSize(const String& text, const bool isSeparator,
		int standardMenuItemHeight, int& idealWidth, int& idealHeight) override;
	void drawPopupMenuSectionHeader(Graphics& g, const juce::Rectangle<int>& area, const String& sectionName) override;
	void drawPopupMenuItem(Graphics &,
		const	juce::Rectangle<int> & 	area,
		bool 	isSeparator,
		bool 	isActive,
		bool 	isHighlighted,
		bool 	isTicked,
		bool 	hasSubMenu,
		const String & 	text,
		const String & 	shortcutKeyText,
		const Drawable * 	icon,
		const Colour * 	textColour
	) override;
	
	void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const Slider::SliderStyle style, Slider& slider) override;

	void drawLinearSliderBackground(Graphics&, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const Slider::SliderStyle, Slider&) override;

	void drawLinearSliderThumb(Graphics&, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const Slider::SliderStyle, Slider&) override;

	Button* createSliderButton(Slider&, bool isIncrement) override;
	Label* createSliderTextBox(Slider & slider) override;

	void drawTableHeaderBackground(Graphics& g, TableHeaderComponent& header) override;
	void drawTableHeaderColumn(Graphics& g, TableHeaderComponent& header,
		const String& columnName, int /*columnId*/,
		int width, int height, bool isMouseOver, bool isMouseDown,
		int columnFlags) override;

	void drawButtonBackground(Graphics& g,
		Button& button,
		const Colour& backgroundColour,
		bool isMouseOverButton,
		bool isButtonDown) override;
	
	static void drawGlassLozenge(Graphics&,
		float x, float y, float width, float height,
		const Colour&, float outlineThickness, float cornerSize,
		bool flatOnLeft, bool flatOnRight, bool flatOnTop, bool flatOnBottom) noexcept;

	static void drawRoundShape(Graphics& g,
		const float x, const float y, const float width, const float height,
		const Colour& colour,
		const bool upperIsLower,
		const bool convex);

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override;

	void drawComboBox(Graphics& g, int width, int height, const bool isButtonDown,
		int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& box) override;

	void positionComboBoxText(ComboBox& box, Label& label) override;

	void fillTextEditorBackground(Graphics& g, int /*width*/, int /*height*/, TextEditor& textEditor) override;
	void drawTextEditorOutline(Graphics& g, int width, int height, TextEditor& textEditor) override;

	int getDefaultScrollbarWidth() override;

	void drawScrollbarButton(Graphics& g, ScrollBar& scrollbar,
		int width, int height, int buttonDirection,
		bool /*isScrollbarVertical*/,
		bool /*isMouseOverButton*/,
		bool isButtonDown) override;

	void drawScrollbar(Graphics& g,
		ScrollBar& scrollbar,
		int x, int y,
		int width, int height,
		bool isScrollbarVertical,
		int thumbStartPosition,
		int thumbSize,
		bool /*isMouseOver*/,
		bool /*isMouseDown*/) override;

	void drawFileBrowserRow(Graphics& g, int width, int height,
		const String& filename, Image* icon,
		const String& fileSizeDescription,
		const String& fileTimeDescription,
		const bool isDirectory, const bool isItemSelected,
		const int /*itemIndex*/, DirectoryContentsDisplayComponent& dcc);

	void layoutFileBrowserComponent(FileBrowserComponent& browserComp,
		DirectoryContentsDisplayComponent* fileListComponent,
		FilePreviewComponent* previewComp,
		ComboBox* currentPathBox,
		TextEditor* filenameBox,
		Button* goUpButton) override;

	void drawImageButton(Graphics& g, Image* image,
		int imageX, int imageY, int imageW, int imageH,
		const Colour& overlayColour,
		float imageOpacity,
		ImageButton& button) override;

	void drawToggleButton(Graphics& g, ToggleButton& button,
		bool isMouseOverButton, bool isButtonDown) override;

	
	//=====================================================================================================
	//VU Meter stuff

	void updateMeterGradients() ;

	juce::Rectangle<float> getMeterInnerBounds(const juce::Rectangle<float> bounds,
		const VASTVUMeter::MeterFlags meterType) const ;

	juce::Rectangle<float> getMeterBounds(const juce::Rectangle<float> bounds,
		const VASTVUMeter::MeterFlags meterType,
		const int numChannels,
		const int channel) const ;

	juce::Rectangle<float> getMeterBarBounds(const juce::Rectangle<float> bounds,
		const VASTVUMeter::MeterFlags meterType) const ;

	/** Override this callback to define the placement of the tickmarks.
	To disable this feature return an empty rectangle. */
	juce::Rectangle<float> getMeterTickmarksBounds(const juce::Rectangle<float> bounds,
		const VASTVUMeter::MeterFlags meterType) const ;

	/** Override this callback to define the placement of the clip indicator light.
	To disable this feature return an empty rectangle. */
	juce::Rectangle<float> getMeterClipIndicatorBounds(const juce::Rectangle<float> bounds,
		const VASTVUMeter::MeterFlags meterType) const ;

	/** Override this callback to define the placement of the max level.
	To disable this feature return an empty rectangle. */
	juce::Rectangle<float> getMeterMaxNumberBounds(const juce::Rectangle<float> bounds,
		const VASTVUMeter::MeterFlags meterType) const ;

	juce::Rectangle<float> drawBackground(juce::Graphics& g,
		const VASTVUMeter::MeterFlags meterType,
		const juce::Rectangle<float> bounds) ;

	void drawMeterBars(juce::Graphics& g,
		const VASTVUMeter::MeterFlags meterType,
		const juce::Rectangle<float> bounds,
		const VASTVUMeterSource* source,
		const int fixedNumChannels,
		const int selectedChannel);

	void drawMeterBarsBackground(juce::Graphics& g,
		const VASTVUMeter::MeterFlags meterType,
		const juce::Rectangle<float> bounds,
		const int numChannels,
		const int fixedNumChannels);

	void drawMeterChannel(juce::Graphics& g,
		const VASTVUMeter::MeterFlags meterType,
		const juce::Rectangle<float> bounds,
		const VASTVUMeterSource* source,
		const int selectedChannel) ;

	void drawMeterChannelBackground(juce::Graphics& g,
		const VASTVUMeter::MeterFlags meterType,
		const juce::Rectangle<float> bounds) ;

	void drawMeterBar(juce::Graphics& g,
		const VASTVUMeter::MeterFlags meterType,
		const juce::Rectangle<float> bounds,
		const float rms, const float peak) ;

	void drawMeterReduction(juce::Graphics& g,
		const VASTVUMeter::MeterFlags meterType,
		const juce::Rectangle<float> bounds,
		const float reduction) ;

	void drawMeterBarBackground(juce::Graphics& g,
		const VASTVUMeter::MeterFlags meterType,
		const juce::Rectangle<float> bounds) ;

	void drawTickMarks(juce::Graphics& g,
		const VASTVUMeter::MeterFlags meterType,
		const juce::Rectangle<float> bounds) ;

	void drawClipIndicator(juce::Graphics& g,
		const VASTVUMeter::MeterFlags meterType,
		const juce::Rectangle<float> bounds,
		const bool hasClipped) ;

	void drawClipIndicatorBackground(juce::Graphics& g,
		const VASTVUMeter::MeterFlags meterType,
		const juce::Rectangle<float> bounds) ;
	
	void drawMaxNumber(juce::Graphics& g,
		const VASTVUMeter::MeterFlags meterType,
		const juce::Rectangle<float> bounds,
		const float maxGain) ;

	void drawMaxNumberBackground(juce::Graphics& g,
		const VASTVUMeter::MeterFlags meterType,
		const juce::Rectangle<float> bounds) ;
	
	int hitTestClipIndicator(const juce::Point<int> position,
		const VASTVUMeter::MeterFlags meterType,
		const juce::Rectangle<float> bounds,
		const VASTVUMeterSource* source) const ;

	int hitTestMaxNumber(const juce::Point<int> position,
		const VASTVUMeter::MeterFlags meterType,
		const juce::Rectangle<float> bounds,
		const VASTVUMeterSource* source) const ;

	//VU Meter stuff
	//=====================================================================================================



private:
	class SliderLabelComp;	

	juce::ColourGradient horizontalGradient;
	juce::ColourGradient verticalGradient;

	float m_scaleFactorWidth = 1.f;
	float m_scaleFactorHeight = 1.f;
};



#endif  // VASTLOOKANDFEEL_H_INCLUDED
