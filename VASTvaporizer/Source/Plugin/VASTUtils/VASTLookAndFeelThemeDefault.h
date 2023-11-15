/*
VAST Dynamics
*/
#ifndef VASTLOOKANDFEELTHEMEDEFAULT_H_INCLUDED
#define VASTLOOKANDFEELTHEMEDEFAULT_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../VASTVUMeter.h"
#include "VASTLookAndFeel.h"

class VASTAudioProcessor; // forward declaration
class VASTAudioProcessorEditor; // forward declaration
class CVASTSettings; // forward declaration
class VASTLookAndFeelThemeDefault : public VASTLookAndFeel
{
public:
	VASTLookAndFeelThemeDefault(CVASTSettings& set, VASTAudioProcessor* processor);

    ~VASTLookAndFeelThemeDefault();
  
	Colour findVASTColour(int colourID);

	/*
	Font getLabelFont(Label&) override;
	Font getTextButtonFont(TextButton& textButton, int buttonHeight) override;
	//Font getComboBoxFont(ComboBox& box) override;
	Font getPopupMenuFont() override;
	Font getTabButtonFont(TabBarButton &, float height) override;
	Font getTextEditorFont(TextEditor& textEditor);
	Font getDefaultFont();

	//Font myDefaultFontRegular;
	//Font myDefaultFontBold;

	Slider::SliderLayout getSliderLayout(Slider& slider) override;
	void drawGroupComponentOutline(Graphics&, int w, int h, const String& text,	const Justification&, GroupComponent&) override;
	void drawButtonText(Graphics& g, TextButton& button, bool , bool ) override;
	void drawLabel(Graphics& g, Label& label) override;
	//void drawTextEditorOutline(Graphics& g, int width, int height, TextEditor& textEditor);

	Label* createComboBoxTextBox(ComboBox& box) override;

	void drawPopupMenuItem(Graphics &,
	const	Rectangle<int> & 	area,
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

	void drawButtonBackground(Graphics& g,
	Button& button,
	const Colour& backgroundColour,
	bool isMouseOverButton,
	bool isButtonDown) override;

	static void drawGlassLozenge(Graphics&,
	float x, float y, float width, float height,
	const Colour&, float outlineThickness, float cornerSize,
	bool flatOnLeft, bool flatOnRight, bool flatOnTop, bool flatOnBottom) noexcept;

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
	const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override;

	void drawComboBox(Graphics& g, int width, int height, const bool isButtonDown,
	int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& box) override;

	void positionComboBoxText(ComboBox& box, Label& label) override;

	void fillTextEditorBackground(Graphics& g, int , int , TextEditor& textEditor) override;

	void drawScrollbarButton(Graphics& g, ScrollBar& scrollbar,
	int width, int height, int buttonDirection,
	bool ,
	bool ,
	bool isButtonDown) override;

	void drawScrollbar(Graphics& g,
	ScrollBar& scrollbar,
	int x, int y,
	int width, int height,
	bool isScrollbarVertical,
	int thumbStartPosition,
	int thumbSize,
	bool ,
	bool ) override;

	void drawFileBrowserRow(Graphics& g, int width, int height,
	const String& filename, Image* icon,
	const String& fileSizeDescription,
	const String& fileTimeDescription,
	const bool isDirectory, const bool isItemSelected,
	const int , DirectoryContentsDisplayComponent& dcc);

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

	void updateMeterGradients()
	{
	horizontalGradient.clearColours();
	verticalGradient.clearColours();
	}

	Rectangle<float> getMeterInnerBounds(const juce::Rectangle<float> bounds,
	const VASTVUMeter::MeterFlags meterType) const
	{
	if (meterType & VASTVUMeter::HasBorder) {
	const float corner = std::min(bounds.getWidth(), bounds.getHeight()) * 0.01;
	return bounds.reduced(3 + corner);
	}
	return bounds;
	}

	Rectangle<float> getMeterBounds(const juce::Rectangle<float> bounds,
	const VASTVUMeter::MeterFlags meterType,
	const int numChannels,
	const int channel) const
	{
	if (meterType & VASTVUMeter::SingleChannel) {
	return bounds;
	}
	else {
	if (meterType & VASTVUMeter::Horizontal) {
	const float h = bounds.getHeight() / numChannels;
	return bounds.withHeight(h).withY(bounds.getY() + channel * h);
	}
	else {
	const float w = bounds.getWidth() / numChannels;
	return bounds.withWidth(w).withX(bounds.getX() + channel * w);
	}
	}
	return juce::Rectangle<float>();
	}

	Rectangle<float> getMeterBarBounds(const juce::Rectangle<float> bounds,
	const VASTVUMeter::MeterFlags meterType) const
	{
	if (meterType & VASTVUMeter::Minimal) {
	if (meterType & VASTVUMeter::Horizontal) {
	const float margin = bounds.getHeight() * 0.05;
	const float h = bounds.getHeight() - 2.0 * margin;
	const float left = bounds.getX() + margin;
	const float right = bounds.getRight() - (4.0 * margin + h);
	return juce::Rectangle<float>(bounds.getX() + margin,
	bounds.getY() + margin,
	right - left,
	h);
	}
	else {
	const float margin = bounds.getWidth() * 0.05;
	const float top = bounds.getY() + 2.0 * margin + bounds.getWidth() * 0.5;
	const float bottom = (meterType & VASTVUMeter::MaxNumber) ?
	bounds.getBottom() - (3.0f * margin + (bounds.getWidth() - margin * 2.0))
	: bounds.getBottom() - margin;
	return juce::Rectangle<float>(bounds.getX() + margin, top,
	bounds.getWidth() - margin * 2.0, bottom - top);
	}
	}
	else if (meterType & VASTVUMeter::Vintage) {
	return bounds;
	}
	else {
	if (meterType & VASTVUMeter::Horizontal) {
	const float margin = bounds.getHeight() * 0.05;
	const float h = bounds.getHeight() * 0.5 - 2.0 * margin;
	const float left = 60.0 + 3.0 * margin;
	const float right = bounds.getRight() - (4.0 * margin + h * 0.5);
	return juce::Rectangle<float>(bounds.getX() + left,
	bounds.getY() + margin,
	right - left,
	h);
	}
	else {
	const float margin = bounds.getWidth() * 0.05;
	const float w = bounds.getWidth() * 0.45;
	const float top = bounds.getY() + 2.0 * margin + w * 0.5;
	const float bottom = bounds.getBottom() - (2.0 * margin + 25.0);
	return juce::Rectangle<float>(bounds.getX() + margin, top, w, bottom - top);
	}
	}
	return juce::Rectangle<float>();
	}

	Rectangle<float> getMeterTickmarksBounds(const juce::Rectangle<float> bounds,
	const VASTVUMeter::MeterFlags meterType) const
	{
	if (meterType & VASTVUMeter::Minimal) {
	if (meterType & VASTVUMeter::Horizontal) {
	return getMeterBarBounds(bounds, meterType).reduced(0.0, 2.0);
	}
	else {
	return getMeterBarBounds(bounds, meterType).reduced(2.0, 0.0);
	}
	}
	else if (meterType & VASTVUMeter::Vintage) {
	return bounds;
	}
	else {
	if (meterType & VASTVUMeter::Horizontal) {
	const float margin = bounds.getHeight() * 0.05;
	const float h = bounds.getHeight() * 0.5 - 2.0 * margin;
	const float left = 60.0 + 3.0 * margin;
	const float right = bounds.getRight() - (4.0 * margin + h * 0.5);
	return juce::Rectangle<float>(bounds.getX() + left,
	bounds.getCentreY() + margin,
	right - left,
	h);
	}
	else {
	const float margin = bounds.getWidth() * 0.05;
	const float w = bounds.getWidth() * 0.45;
	const float top = bounds.getY() + 2.0 * margin + w * 0.5 + 2.0;
	const float bottom = bounds.getBottom() - (2.0 * margin + 25.0 + 2.0);
	return juce::Rectangle<float>(bounds.getCentreX(), top, w, bottom - top);
	}
	}
	return juce::Rectangle<float>();
	}

	Rectangle<float> getMeterClipIndicatorBounds(const juce::Rectangle<float> bounds,
	const VASTVUMeter::MeterFlags meterType) const
	{
	if (meterType & VASTVUMeter::Minimal) {
	if (meterType & VASTVUMeter::Horizontal) {
	const float margin = bounds.getHeight() * 0.05;
	const float h = bounds.getHeight() - 2.0 * margin;
	return juce::Rectangle<float>(bounds.getRight() - (margin + h),
	bounds.getY() + margin,
	h,
	h);
	}
	else {
	const float margin = bounds.getWidth() * 0.05;
	const float w = bounds.getWidth() - margin * 2.0;
	return juce::Rectangle<float>(bounds.getX() + margin,
	bounds.getY() + margin,
	w,
	w * 0.5);
	}
	}
	else if (meterType & VASTVUMeter::Vintage) {
	return bounds;
	}
	else {
	if (meterType & VASTVUMeter::Horizontal) {
	const float margin = bounds.getHeight() * 0.05;
	const float h = bounds.getHeight() * 0.5 - 2.0 * margin;
	return juce::Rectangle<float>(bounds.getRight() - (margin + h * 0.5),
	bounds.getY() + margin,
	h * 0.5,
	h);
	}
	else {
	const float margin = bounds.getWidth() * 0.05;
	const float w = bounds.getWidth() * 0.45;
	return juce::Rectangle<float>(bounds.getX() + margin,
	bounds.getY() + margin,
	w,
	w * 0.5);
	}
	}
	return juce::Rectangle<float>();
	}

	Rectangle<float> getMeterMaxNumberBounds(const juce::Rectangle<float> bounds,
	const VASTVUMeter::MeterFlags meterType) const
	{
	if (meterType & VASTVUMeter::Minimal) {
	if (meterType & VASTVUMeter::MaxNumber) {
	if (meterType & VASTVUMeter::Horizontal) {
	const float margin = bounds.getHeight() * 0.05;
	const float h = bounds.getHeight() - 2.0 * margin;
	return juce::Rectangle<float>(bounds.getRight() - (margin + h),
	bounds.getY() + margin,
	h, h);
	}
	else {
	const float margin = bounds.getWidth() * 0.05;
	const float w = bounds.getWidth() - margin * 2.0;
	const float h = w * 0.6f;
	return juce::Rectangle<float>(bounds.getX() + margin,
	bounds.getBottom() - (margin + h),
	w, h);
	}
	}
	else {
	return juce::Rectangle<float>();
	}
	}
	else if (meterType & VASTVUMeter::Vintage) {
	return bounds;
	}
	else {
	if (meterType & VASTVUMeter::Horizontal) {
	const float margin = bounds.getHeight() * 0.05;
	return juce::Rectangle<float>(bounds.getX() + margin,
	bounds.getCentreY() + margin,
	60,
	bounds.getHeight() * 0.5 - margin * 2.0);
	}
	else {
	const float margin = bounds.getWidth() * 0.05;
	return juce::Rectangle<float>(bounds.getX() + margin,
	bounds.getBottom() - (margin + 25),
	bounds.getWidth() - 2 * margin,
	25.0);
	}
	}
	}

	Rectangle<float> drawBackground(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds)
	{
	g.setColour(findColour(VASTVUMeter::lmBackgroundColour));
	if (meterType & VASTVUMeter::HasBorder) {
	const float corner = std::min(bounds.getWidth(), bounds.getHeight()) * 0.01;
	g.fillRoundedRectangle(bounds, corner);
	g.setColour(findColour(VASTVUMeter::lmOutlineColour));
	g.drawRoundedRectangle(bounds.reduced(3), corner, 2);
	return bounds.reduced(3 + corner);
	}
	else {
	g.fillRect(bounds);
	return bounds;
	}
	}

	void drawMeterBars(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds,
	const VASTVUMeterSource* source,
	const int fixedNumChannels = -1,
	const int selectedChannel = -1)
	{
	const juce::Rectangle<float> innerBounds = getMeterInnerBounds(bounds, meterType);
	if (source) {
	const int numChannels = source->getNumChannels();
	if (meterType & VASTVUMeter::Minimal) {
	if (meterType & VASTVUMeter::Horizontal) {
	const float height = innerBounds.getHeight() / (2 * numChannels - 1);
	Rectangle<float> meter = innerBounds.withHeight(height);
	for (int channel = 0; channel < numChannels; ++channel) {
	meter.setY(height * channel * 2);
	{
	Rectangle<float> meterBarBounds = getMeterBarBounds(meter, meterType);
	drawMeterBar(g, meterType, meterBarBounds,
	source->getRMSLevel(channel),
	source->getMaxLevel(channel));
	const float reduction = source->getReductionLevel(channel);
	if (reduction < 1.0)
	drawMeterReduction(g, meterType,
	meterBarBounds.withBottom(meterBarBounds.getCentreY()),
	reduction);
	}
	Rectangle<float> clip = getMeterClipIndicatorBounds(meter, meterType);
	if (!clip.isEmpty())
	drawClipIndicator(g, meterType, clip, source->getClipFlag(channel));
	Rectangle<float> maxNum = getMeterMaxNumberBounds(meter, meterType);
	if (!maxNum.isEmpty())
	drawMaxNumber(g, meterType, maxNum, source->getMaxOverallLevel(channel));
	if (channel < numChannels - 1) {
	meter.setY(height * (channel * 2 + 1));
	Rectangle<float> ticks = getMeterTickmarksBounds(meter, meterType);
	if (!ticks.isEmpty())
	drawTickMarks(g, meterType, ticks);
	}
	}
	}
	else {
	const float width = innerBounds.getWidth() / (2 * numChannels - 1);
	Rectangle<float> meter = innerBounds.withWidth(width);
	for (int channel = 0; channel < numChannels; ++channel) {
	meter.setX(width * channel * 2);
	{
	Rectangle<float> meterBarBounds = getMeterBarBounds(meter, meterType);
	drawMeterBar(g, meterType, getMeterBarBounds(meter, meterType),
	source->getRMSLevel(channel),
	source->getMaxLevel(channel));
	const float reduction = source->getReductionLevel(channel);
	if (reduction < 1.0)
	drawMeterReduction(g, meterType,
	meterBarBounds.withLeft(meterBarBounds.getCentreX()),
	reduction);
	}
	Rectangle<float> clip = getMeterClipIndicatorBounds(meter, meterType);
	if (!clip.isEmpty())
	drawClipIndicator(g, meterType, clip, source->getClipFlag(channel));
	Rectangle<float> maxNum = getMeterMaxNumberBounds(innerBounds.withWidth(innerBounds.getWidth() / numChannels).withX(innerBounds.getX() + channel * (innerBounds.getWidth() / numChannels)), meterType);
	if (!maxNum.isEmpty())
	drawMaxNumber(g, meterType, maxNum, source->getMaxOverallLevel(channel));
	if (channel < numChannels - 1) {
	meter.setX(width * (channel * 2 + 1));
	Rectangle<float> ticks = getMeterTickmarksBounds(meter, meterType);
	if (!ticks.isEmpty())
	drawTickMarks(g, meterType, ticks);
	}
	}
	}
	}
	else if (meterType & VASTVUMeter::SingleChannel) {
	if (selectedChannel >= 0) {
	drawMeterChannel(g, meterType, innerBounds, source, selectedChannel);
	}
	}
	else {
	const int numChannels = source->getNumChannels();
	const int numDrawnChannels = fixedNumChannels < 0 ? numChannels : fixedNumChannels;
	for (int channel = 0; channel < numChannels; ++channel) {
	drawMeterChannel(g, meterType,
	getMeterBounds(innerBounds, meterType, numDrawnChannels, channel),
	source, channel);
	}
	}
	}
	}

	void drawMeterBarsBackground(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds,
	const int numChannels,
	const int fixedNumChannels)
	{
	const juce::Rectangle<float> innerBounds = getMeterInnerBounds(bounds, meterType);
	if (meterType & VASTVUMeter::Minimal) {
	if (meterType & VASTVUMeter::Horizontal) {
	const float height = innerBounds.getHeight() / (2 * numChannels - 1);
	Rectangle<float> meter = innerBounds.withHeight(height);
	for (int channel = 0; channel < numChannels; ++channel) {
	meter.setY(height * channel * 2);
	drawMeterBarBackground(g, meterType, getMeterBarBounds(meter, meterType));
	Rectangle<float> clip = getMeterClipIndicatorBounds(meter, meterType);
	if (!clip.isEmpty())
	drawClipIndicatorBackground(g, meterType, clip);
	if (channel < numChannels - 1) {
	meter.setY(height * (channel * 2 + 1));
	Rectangle<float> ticks = getMeterTickmarksBounds(meter, meterType);
	if (!ticks.isEmpty())
	drawTickMarks(g, meterType, ticks);
	}
	}
	}
	else {
	const float width = innerBounds.getWidth() / (2 * numChannels - 1);
	Rectangle<float> meter = innerBounds.withWidth(width);
	for (int channel = 0; channel < numChannels; ++channel) {
	meter.setX(width * channel * 2);
	drawMeterBarBackground(g, meterType, getMeterBarBounds(meter, meterType));
	Rectangle<float> clip = getMeterClipIndicatorBounds(meter, meterType);
	if (!clip.isEmpty())
	drawClipIndicatorBackground(g, meterType, clip);
	if (channel < numChannels - 1) {
	meter.setX(width * (channel * 2 + 1));
	Rectangle<float> ticks = getMeterTickmarksBounds(meter, meterType);
	if (!ticks.isEmpty())
	drawTickMarks(g, meterType, ticks);
	}
	}
	}
	}
	else if (meterType & VASTVUMeter::SingleChannel) {
	drawMeterChannelBackground(g, meterType, innerBounds);
	}
	else {
	for (int channel = 0; channel < numChannels; ++channel) {
	drawMeterChannelBackground(g, meterType,
	getMeterBounds(innerBounds, meterType,
	fixedNumChannels < 0 ? numChannels : fixedNumChannels,
	channel));
	}
	}
	}


	void drawMeterChannel(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds,
	const VASTVUMeterSource* source,
	const int selectedChannel)
	{
	if (source) {
	Rectangle<float> meter = getMeterBarBounds(bounds, meterType);
	if (!meter.isEmpty()) {
	if (meterType & VASTVUMeter::Reduction) {
	drawMeterBar(g, meterType, meter,
	source->getReductionLevel(selectedChannel),
	0.0f);
	}
	else {
	drawMeterBar(g, meterType, meter,
	source->getRMSLevel(selectedChannel),
	source->getMaxLevel(selectedChannel));
	const float reduction = source->getReductionLevel(selectedChannel);
	if (reduction < 1.0) {
	if (meterType & VASTVUMeter::Horizontal)
	drawMeterReduction(g, meterType,
	meter.withBottom(meter.getCentreY()),
	reduction);
	else
	drawMeterReduction(g, meterType,
	meter.withLeft(meter.getCentreX()),
	reduction);
	}
	}
	}
	if (source->getClipFlag(selectedChannel)) {
	Rectangle<float> clip = getMeterClipIndicatorBounds(bounds, meterType);
	if (!clip.isEmpty())
	drawClipIndicator(g, meterType, clip, true);
	}
	Rectangle<float> maxes = getMeterMaxNumberBounds(bounds, meterType);
	if (!maxes.isEmpty()) {
	if (meterType & VASTVUMeter::Reduction) {
	drawMaxNumber(g, meterType, maxes, source->getReductionLevel(selectedChannel));
	}
	else {
	drawMaxNumber(g, meterType, maxes, source->getMaxOverallLevel(selectedChannel));
	}
	}
	}
	}

	void drawMeterChannelBackground(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds)
	{
	Rectangle<float> meter = getMeterBarBounds(bounds, meterType);
	if (!meter.isEmpty()) {
	drawMeterBarBackground(g, meterType, meter);
	}
	Rectangle<float> clip = getMeterClipIndicatorBounds(bounds, meterType);
	if (!clip.isEmpty())
	drawClipIndicatorBackground(g, meterType, clip);
	Rectangle<float> ticks = getMeterTickmarksBounds(bounds, meterType);
	if (!ticks.isEmpty())
	drawTickMarks(g, meterType, ticks);
	Rectangle<float> maxes = getMeterMaxNumberBounds(bounds, meterType);
	if (!maxes.isEmpty()) {
	drawMaxNumberBackground(g, meterType, maxes);
	}
	}

	void drawMeterBar(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds,
	const float rms, const float peak)
	{
	const float infinity = meterType & VASTVUMeter::Reduction ? -30.0f : -80.0f;
	const float rmsDb = juce::Decibels::gainToDecibels(rms, infinity);
	const float peakDb = juce::Decibels::gainToDecibels(peak, infinity);

	const juce::Rectangle<float> floored(ceilf(bounds.getX()) + 1.0f, ceilf(bounds.getY()) + 1.0f,
	floorf(bounds.getRight()) - (ceilf(bounds.getX() + 2.0f)),
	floorf(bounds.getBottom()) - (ceilf(bounds.getY()) + 2.0f));

	if (meterType & VASTVUMeter::Vintage) {
	// TODO
	}
	else if (meterType & VASTVUMeter::Reduction) {
	const float limitDb = juce::Decibels::gainToDecibels(rms, infinity);
	g.setColour(findColour(VASTVUMeter::lmMeterReductionColour));
	if (meterType & VASTVUMeter::Horizontal) {
	g.fillRect(floored.withLeft(floored.getX() + limitDb * floored.getWidth() / infinity));
	}
	else {
	g.fillRect(floored.withBottom(floored.getY() + limitDb * floored.getHeight() / infinity));
	}
	}
	else {
	if (meterType & VASTVUMeter::Horizontal) {
	if (horizontalGradient.getNumColours() < 2) {
	horizontalGradient = juce::ColourGradient(findColour(VASTVUMeter::lmMeterGradientLowColour),
	floored.getX(), floored.getY(),
	findColour(VASTVUMeter::lmMeterGradientMaxColour),
	floored.getRight(), floored.getY(), false);
	horizontalGradient.addColour(0.5, findColour(VASTVUMeter::lmMeterGradientLowColour));
	horizontalGradient.addColour(0.75, findColour(VASTVUMeter::lmMeterGradientMidColour));
	}
	g.setGradientFill(horizontalGradient);
	g.fillRect(floored.withRight(floored.getRight() - rmsDb * floored.getWidth() / infinity));

	if (peakDb > -49.0) {
	g.setColour(findColour((peakDb > -0.3f) ? VASTVUMeter::lmMeterMaxOverColour :
	((peakDb > -5.0) ? VASTVUMeter::lmMeterMaxWarnColour :
	VASTVUMeter::lmMeterMaxNormalColour)));
	g.drawVerticalLine(floored.getRight() - juce::jmax(peakDb * floored.getWidth() / infinity, 0.0f),
	floored.getY(), floored.getBottom());
	}
	}
	else {
	// vertical
	if (verticalGradient.getNumColours() < 2) {
	verticalGradient = juce::ColourGradient(findColour(VASTVUMeter::lmMeterGradientLowColour),
	floored.getX(), floored.getBottom(),
	findColour(VASTVUMeter::lmMeterGradientMaxColour),
	floored.getX(), floored.getY(), false);
	verticalGradient.addColour(0.5, findColour(VASTVUMeter::lmMeterGradientLowColour));
	verticalGradient.addColour(0.75, findColour(VASTVUMeter::lmMeterGradientMidColour));
	}
	g.setGradientFill(verticalGradient);
	g.fillRect(floored.withTop(floored.getY() + rmsDb * floored.getHeight() / infinity));

	if (peakDb > -49.0) {
	g.setColour(findColour((peakDb > -0.3f) ? VASTVUMeter::lmMeterMaxOverColour :
	((peakDb > -5.0) ? VASTVUMeter::lmMeterMaxWarnColour :
	VASTVUMeter::lmMeterMaxNormalColour)));
	g.drawHorizontalLine(floored.getY() + juce::jmax(peakDb * floored.getHeight() / infinity, 0.0f),
	floored.getX(), floored.getRight());
	}
	}
	}
	}

	void drawMeterReduction(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds,
	const float reduction)
	{
	const float infinity = -30.0f;

	const juce::Rectangle<float> floored(ceilf(bounds.getX()) + 1.0f, ceilf(bounds.getY()) + 1.0f,
	floorf(bounds.getRight()) - (ceilf(bounds.getX() + 2.0f)),
	floorf(bounds.getBottom()) - (ceilf(bounds.getY()) + 2.0f));

	const float limitDb = juce::Decibels::gainToDecibels(reduction, infinity);
	g.setColour(findColour(VASTVUMeter::lmMeterReductionColour));
	if (meterType & VASTVUMeter::Horizontal) {
	g.fillRect(floored.withLeft(floored.getX() + limitDb * floored.getWidth() / infinity));
	}
	else {
	g.fillRect(floored.withBottom(floored.getY() + limitDb * floored.getHeight() / infinity));
	}
	}

	void drawMeterBarBackground(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds)
	{
	g.setColour(findColour(VASTVUMeter::lmMeterBackgroundColour));
	g.fillRect(bounds);

	g.setColour(findColour(VASTVUMeter::lmMeterOutlineColour));
	g.drawRect(bounds, 1.0);
	}

	void drawTickMarks(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds)
	{
	const float infinity = meterType & VASTVUMeter::Reduction ? -30.0f : -80.0f;

	g.setColour(findColour(VASTVUMeter::lmTicksColour));
	if (meterType & VASTVUMeter::Minimal) {
	if (meterType & VASTVUMeter::Horizontal) {
	for (int i = 0; i<11; ++i)
	g.drawVerticalLine(bounds.getX() + i * 0.1f * bounds.getWidth(),
	bounds.getY() + 4,
	bounds.getBottom() - 4);
	}
	else {
	const float h = (bounds.getHeight() - 2.0) * 0.1;
	for (int i = 0; i<11; ++i) {
	g.drawHorizontalLine(bounds.getY() + i * h + 1,
	bounds.getX() + 4,
	bounds.getRight());
	}
	if (h > 10 && bounds.getWidth() > 20) {
	// don't print tiny numbers
	g.setFont(h * 0.5f);
	for (int i = 0; i<10; ++i) {
	g.drawFittedText(juce::String(i * 0.1 * infinity),
	bounds.getX(), bounds.getY() + i * h + 2, bounds.getWidth(),
	h * 0.6,
	juce::Justification::centredTop, 1);
	}
	}
	}
	}
	else if (meterType & VASTVUMeter::Vintage) {
	// TODO
	}
	else {
	if (meterType & VASTVUMeter::Horizontal) {
	for (int i = 0; i<11; ++i)
	g.drawVerticalLine(bounds.getX() + i * 0.1f * bounds.getWidth(),
	bounds.getY() + 4,
	bounds.getBottom() - 4);
	}
	else {
	const float h = (bounds.getHeight() - 2.0) * 0.05;
	g.setFont(h * 0.8f);
	for (int i = 0; i<21; ++i) {
	const float y = bounds.getY() + i * h;
	if (i % 2 == 0) {
	g.drawHorizontalLine(y + 1,
	bounds.getX() + 4,
	bounds.getRight());
	if (i < 20) {
	g.drawFittedText(juce::String(i * 0.05 * infinity),
	bounds.getX(), y + 4, bounds.getWidth(), h * 0.6,
	juce::Justification::topRight, 1);
	}
	}
	else {
	g.drawHorizontalLine(y + 2,
	bounds.getX() + 4,
	bounds.getCentreX());
	}
	}
	}
	}
	}

	void drawClipIndicator(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds,
	const bool hasClipped)
	{
	g.setColour(findColour(hasClipped ? VASTVUMeter::lmBackgroundClipColour : VASTVUMeter::lmMeterBackgroundColour));
	g.fillRect(bounds);
	g.setColour(findColour(VASTVUMeter::lmMeterOutlineColour));
	g.drawRect(bounds, 1.0);
	}

	void drawClipIndicatorBackground(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds)
	{
	g.setColour(findColour(VASTVUMeter::lmMeterBackgroundColour));
	g.fillRect(bounds);
	g.setColour(findColour(VASTVUMeter::lmMeterOutlineColour));
	g.drawRect(bounds, 1.0);
	}

	void drawMaxNumber(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds,
	const float maxGain)
	{
	g.setColour(findColour(VASTVUMeter::lmMeterBackgroundColour));
	g.fillRect(bounds);
	const float maxDb = juce::Decibels::gainToDecibels(maxGain, -80.0f);
	g.setColour(findColour(maxDb > 0.0 ? VASTVUMeter::lmTextClipColour : VASTVUMeter::lmTextColour));
	g.setFont(bounds.getHeight() * 0.5f);
	g.drawFittedText(juce::String(maxDb, 1) + " dB",
	bounds.reduced(2.0).toNearestInt(),
	juce::Justification::centred, 1);
	g.setColour(findColour(VASTVUMeter::lmMeterOutlineColour));
	g.drawRect(bounds, 1.0);
	}

	void drawMaxNumberBackground(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds)
	{
	g.setColour(findColour(VASTVUMeter::lmMeterBackgroundColour));
	g.fillRect(bounds);
	}

	int hitTestClipIndicator(const juce::Point<int> position,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds,
	const VASTVUMeterSource* source) const
	{
	if (source) {
	const int numChannels = source->getNumChannels();
	for (int i = 0; i < numChannels; ++i) {
	if (getMeterClipIndicatorBounds(getMeterBounds
	(bounds, meterType, source->getNumChannels(), i), meterType)
	.contains(position.toFloat())) {
	return i;
	}
	}
	}
	return -1;
	}

	int hitTestMaxNumber(const juce::Point<int> position,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds,
	const VASTVUMeterSource* source) const
	{
	if (source) {
	const int numChannels = source->getNumChannels();
	for (int i = 0; i < numChannels; ++i) {
	if (getMeterMaxNumberBounds(getMeterBounds
	(bounds, meterType, source->getNumChannels(), i), meterType)
	.contains(position.toFloat())) {
	return i;
	}
	}
	}
	return -1;
	}



	//VU Meter stuff
	//=====================================================================================================


















	private:
	class SliderLabelComp;

	juce::ColourGradient horizontalGradient;
	juce::ColourGradient verticalGradient;
	*/
};

#endif  // VASTLOOKANDFEELTHEMEDEFAULT_H_INCLUDED
