/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTLookAndFeel.h"
#include "../VASTVaporizerComponent.h"
#include "../VASTControls/VASTComboBrev.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"

#include <sstream>

void VASTLookAndFeel::initAll() {
	m_knobBufferArray.clear();
	m_knobBufferArrayMouseOver.clear();
}

inline Colour VASTLookAndFeel::createBaseColour(Colour buttonColour, bool hasKeyboardFocus, bool isMouseOverButton, bool isButtonDown) noexcept
{
	const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
	const Colour baseColour(buttonColour.withMultipliedSaturation(sat));

	if (isButtonDown)      return baseColour.contrasting(0.2f);
	if (isMouseOverButton) return baseColour.contrasting(0.1f);

	return baseColour;
}

void VASTLookAndFeel::setUIFontSize(int size) {
	float perc = 1.f;
	if (size == 1) //larger
		perc = 1.2f;
	else if (size == 2) //smaller
		perc = 0.8f;
	mFontSize = perc * mFontSizeFactor;
}

Font VASTLookAndFeel::getTabButtonFont(TabBarButton &, float height) {
	return myDefaultFontRegular;
}

Font VASTLookAndFeel::getPopupMenuFont() {
	return myDefaultFontRegular; //make larger
}

Font VASTLookAndFeel::getDefaultFont() {
	return myDefaultFontRegular;
}

Font VASTLookAndFeel::getDefaultFontBold() {
	return myDefaultFontBold;
}

Font VASTLookAndFeel::getLabelFont(Label& label) {
	Font newFont = myDefaultFontRegular;
	if ((label.getFont().getStyleFlags() == Font::bold) || (label.getFont().getTypefaceName() == getDefaultFontBold().getTypefaceName())) {
		newFont = myDefaultFontBold;
	}
	else {
		newFont = myDefaultFontRegular;
	}

	float newheight = 0.f;
	if (label.getComponentID().equalsIgnoreCase("IncDecButtons")) {//inc dec buttons label
		newheight = jmin(label.getHeight() * 1.3f, label.getWidth() * 0.42f);
	}
	else if (label.getComponentID().equalsIgnoreCase("ComboBox")) {
		newheight = jmin(label.getHeight() * 1.2f, label.getWidth() * 0.82f);
	}
	else if (label.getComponentID().equalsIgnoreCase("GroupComponent")) {
		//newheight = jmin(label.getHeight() * 1.2f, label.getWidth() * 0.82f);
		newheight = m_lastTextEditorFontHeight * 0.9f;
	}
	else { //normal
		newheight = jmin(label.getHeight() * 1.3f, label.getWidth() * 0.22f);
		String text = label.getText();
		if (text.length() <= 2) {
			newheight = label.getHeight() * 0.75f;
		}
	}
	newheight *= mFontSize;
	newFont.setSizeAndStyle(newheight, newFont.getStyleFlags(), 1.0f, 0.0f);
	return newFont;
}

Font VASTLookAndFeel::getTextEditorFont(TextEditor& textEditor) {
	Font newFont = myDefaultFontBold;

	//float newheight = jmin(textEditor.getHeight() * 1.2f, textEditor.getWidth() * 0.20f); //same as combobox above!!
	float newheight = jmin(textEditor.getHeight() * 1.2f, textEditor.getWidth() * 0.82f); //same as combobox above!!
	
	newheight *= mFontSize;
	m_lastTextEditorFontHeight = newheight;
	newFont.setSizeAndStyle(newheight, newFont.getStyleFlags(), 1.0f, 0.0f);
	return newFont;
}

void VASTLookAndFeel::setCurrentScalingFactors(float scaleFactorWidth, float scaleFactorHeight) {
	m_scaleFactorWidth = scaleFactorWidth;
	m_scaleFactorHeight = scaleFactorHeight;
}

int VASTLookAndFeel::getAlertWindowButtonHeight() { 
	return 28; 
};

Array<int> VASTLookAndFeel::getWidthsForTextButtons(AlertWindow&, const Array<TextButton*>& buttons)
{
	const int n = buttons.size();
	Array<int> buttonWidths;

	const int buttonHeight = getAlertWindowButtonHeight();

	for (int i = 0; i < n; ++i)
		//buttonWidths.add(getTextButtonWidthToFitText(*buttons.getReference(i), buttonHeight));
		buttonWidths.add(buttonHeight * 6.f);

	return buttonWidths;
}

Font VASTLookAndFeel::getTextButtonFont(TextButton& textButton, int buttonHeight) {
	Font newFont = myDefaultFontRegular;
	String text = textButton.getButtonText();
	float lwidth = textButton.getWidth();
	float lheight = textButton.getHeight();
	float newheight = jmin(lheight * 1.0f, lwidth * 0.20f);
	if (text.length() <= 2)
		newheight = textButton.getHeight() * 0.75f;
	newheight *= mFontSize;
	newFont.setSizeAndStyle(newheight, newFont.getStyleFlags(), 1.0f, 0.0f);
	vassert(newFont.getTypefacePtr() != nullptr);
	return newFont;
};

//==============================================================================
Slider::SliderLayout VASTLookAndFeel::getSliderLayout(Slider& slider)
{
	// 1. compute the actually visible textBox size from the slider textBox size and some additional constraints

	int minXSpace = 0;
	int minYSpace = 0;

	auto textBoxPos = slider.getTextBoxPosition();

	if (textBoxPos == Slider::TextBoxLeft || textBoxPos == Slider::TextBoxRight) {
		//CHVAST
		//minXSpace = 30;
		minXSpace = 20 * m_scaleFactorWidth;
	}
	else
		minYSpace = 15 * m_scaleFactorHeight;

	auto localBounds = slider.getLocalBounds();

	auto textBoxWidth = jmax(0, jmin(slider.getTextBoxWidth(), localBounds.getWidth() - minXSpace));
	auto textBoxHeight = jmax(0, jmin(slider.getTextBoxHeight(), localBounds.getHeight() - minYSpace));

	Slider::SliderLayout layout;

	// 2. set the textBox bounds

	if (textBoxPos != Slider::NoTextBox)
	{
		if (slider.isBar())
		{
			layout.textBoxBounds = localBounds;
		}
		else
		{
			layout.textBoxBounds.setWidth(textBoxWidth);
			layout.textBoxBounds.setHeight(textBoxHeight);

			if (textBoxPos == Slider::TextBoxLeft)           layout.textBoxBounds.setX(0);
			else if (textBoxPos == Slider::TextBoxRight)     layout.textBoxBounds.setX(localBounds.getWidth() - textBoxWidth);
			else /* above or below -> centre horizontally */ layout.textBoxBounds.setX((localBounds.getWidth() - textBoxWidth) / 2);

			if (textBoxPos == Slider::TextBoxAbove)          layout.textBoxBounds.setY(0);
			else if (textBoxPos == Slider::TextBoxBelow)     layout.textBoxBounds.setY(localBounds.getHeight() - textBoxHeight);
			else /* left or right -> centre vertically */    layout.textBoxBounds.setY((localBounds.getHeight() - textBoxHeight) / 2);
		}
	}

	// 3. set the slider bounds

	layout.sliderBounds = localBounds;

	if (slider.isBar())
	{
		layout.sliderBounds.reduce(1, 1);   // bar border
	}
	else
	{
		if (textBoxPos == Slider::TextBoxLeft)
			layout.sliderBounds.removeFromLeft(textBoxWidth);
		else if (textBoxPos == Slider::TextBoxRight) layout.sliderBounds.removeFromRight(textBoxWidth);
		else if (textBoxPos == Slider::TextBoxAbove) layout.sliderBounds.removeFromTop(textBoxHeight);
		else if (textBoxPos == Slider::TextBoxBelow) layout.sliderBounds.removeFromBottom(textBoxHeight);

		const int thumbIndent = getSliderThumbRadius(slider);

		if (slider.isHorizontal())    layout.sliderBounds.reduce(thumbIndent, 0);
		else if (slider.isVertical()) layout.sliderBounds.reduce(0, thumbIndent);
	}

	return layout;
}

// Callout Box

int VASTLookAndFeel::getCallOutBoxBorderSize(const CallOutBox &) {
	return 0;
}

float VASTLookAndFeel::getCallOutBoxCornerSize(const CallOutBox &) {
	return 0.f;
}

// SidePanel ==============================================================================================================

Font VASTLookAndFeel::getSidePanelTitleFont(SidePanel&) {	
	setColour(SidePanel::dismissButtonNormalColour,findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom).contrasting(.4f));
	setColour(SidePanel::dismissButtonOverColour, findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom).contrasting(.6f));
	setColour(SidePanel::dismissButtonDownColour, findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom).contrasting(.8f));
	setColour(SidePanel::backgroundColour, findVASTColour(VASTColours::colOscillatorComponentHeaderGradientFrom));

	Font f = getDefaultFontBold();
	f.setSizeAndStyle(6.f, f.getStyleFlags(), 1.f, 0.f);
	return f;
}

Justification VASTLookAndFeel::getSidePanelTitleJustification(SidePanel& panel) {
	return Justification::centredLeft;
}

Path VASTLookAndFeel::getSidePanelDismissButtonShape(SidePanel& panel) {
	return getCrossShape((float)panel.getTitleBarHeight() * 0.1f);
}
// SidePanel ==============================================================================================================

void VASTLookAndFeel::drawTabButton(TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown) {
	const juce::Rectangle<int> activeArea(button.getActiveArea());

	const TabbedButtonBar::Orientation o = button.getTabbedButtonBar().getOrientation();

	const Colour bkg(button.getTabBackgroundColour());

	if (button.getToggleState())
	{
		g.setColour(bkg.darker(0.8f));
	}
	else
	{
		Point<int> p1, p2;

		switch (o)
		{
		case TabbedButtonBar::TabsAtBottom:   p1 = activeArea.getBottomLeft(); p2 = activeArea.getTopLeft();    break;
		case TabbedButtonBar::TabsAtTop:      p1 = activeArea.getTopLeft();    p2 = activeArea.getBottomLeft(); break;
		case TabbedButtonBar::TabsAtRight:    p1 = activeArea.getTopRight();   p2 = activeArea.getTopLeft();    break;
		case TabbedButtonBar::TabsAtLeft:     p1 = activeArea.getTopLeft();    p2 = activeArea.getTopRight();   break;
		default:                              jassertfalse; break;
		}

		if (isMouseOver)
			g.setGradientFill(ColourGradient(bkg.brighter(0.7f), p1.toFloat(),
				bkg.brighter(0.2f), p2.toFloat(), false));
		else 
            g.setGradientFill(ColourGradient(bkg.brighter(0.5f), p1.toFloat(),
                bkg.darker(0.1f), p2.toFloat(), false));
            //g.setGradientFill(ColourGradient(bkg.brighter(0.2f), p1.toFloat(),
				//bkg.darker(0.1f), p2.toFloat(), false));
	}

	g.fillRect(activeArea);	
	/*g.setColour(button.findColour(TabbedButtonBar::tabOutlineColourId));
	juce::Rectangle<float> r(activeArea.toFloat());

	if (o != TabbedButtonBar::TabsAtBottom)   g.fillRect(r.removeFromTop(1.f * m_scaleFactorHeight));
	if (o != TabbedButtonBar::TabsAtTop)      g.fillRect(r.removeFromBottom(1.f * m_scaleFactorHeight));
	if (o != TabbedButtonBar::TabsAtRight)    g.fillRect(r.removeFromLeft(1.f * m_scaleFactorWidth));
	if (o != TabbedButtonBar::TabsAtLeft)     g.fillRect(r.removeFromRight(1.f * m_scaleFactorWidth));

	*/
	const float alpha = button.isEnabled() ? ((isMouseOver || isMouseDown) ? 1.0f : 0.8f) : 0.3f;

	Colour col(bkg.contrasting().withMultipliedAlpha(alpha));

	if (TabbedButtonBar* bar = button.findParentComponentOfClass<TabbedButtonBar>())
	{
		TabbedButtonBar::ColourIds colID = button.isFrontTab() ? TabbedButtonBar::frontTextColourId
			: TabbedButtonBar::tabTextColourId;

		if (bar->isColourSpecified(colID))
			col = bar->findColour(colID);
		else if (isColourSpecified(colID))
			col = findColour(colID);
	}

	const juce::Rectangle<float> area(button.getTextArea().toFloat());

	float length = area.getWidth();
	float depth = area.getHeight();

	if (button.getTabbedButtonBar().isVertical())
		std::swap(length, depth);

	TextLayout textLayout;
	createTabTextLayout(button, length, depth, col, textLayout);

	AffineTransform t;

	switch (o)
	{
	case TabbedButtonBar::TabsAtLeft:   t = t.rotated(MathConstants<float>::pi * -0.5f).translated(area.getX(), area.getBottom()); break;
	case TabbedButtonBar::TabsAtRight:  t = t.rotated(MathConstants<float>::pi *  0.5f).translated(area.getRight(), area.getY()); break;
	case TabbedButtonBar::TabsAtTop:
	case TabbedButtonBar::TabsAtBottom: t = t.translated(area.getX(), area.getY()); break;
	default:                            jassertfalse; break;
	}

	g.addTransform(t);
	textLayout.draw(g, juce::Rectangle<float>(length, depth));
}

void VASTLookAndFeel::createTabTextLayout(const TabBarButton& button, float length, float depth,
	Colour colour, TextLayout& textLayout)
{
	bool lF_ice = dynamic_cast<VASTLookAndFeelThemeIce*>(this);
	Font font = getDefaultFont();
	float newheight = depth * 0.52f;
	newheight *= mFontSize;
	font.setSizeAndStyle(newheight, font.getStyleFlags(), 1.0f, 0.0f);

	//Font font(depth * 0.5f);
	font.setUnderline(button.hasKeyboardFocus(false));

	AttributedString s;
	s.setJustification(Justification::centred);
	if (!lF_ice)
		s.append(button.getButtonText().trim(), font, colour);
	else 
		s.append(button.getButtonText().trim().toLowerCase(), font, colour); 

	textLayout.createLayout(s, length);
}

int VASTLookAndFeel::getTabButtonBestWidth(TabBarButton& button, int tabDepth)
{
	Font font = getDefaultFont();
	float newheight = tabDepth * 0.75f; //more than 52
	newheight *= mFontSize;
	font.setSizeAndStyle(newheight, font.getStyleFlags(), 1.0f, 0.0f);
	int width = font.getStringWidth(button.getButtonText().trim())
		+ getTabButtonOverlap(tabDepth) * 2;

	if (auto* extraComponent = button.getExtraComponent())
		width += button.getTabbedButtonBar().isVertical() ? extraComponent->getHeight()
		: extraComponent->getWidth();

	return jlimit(tabDepth * 2, tabDepth * 8, width);
}

void VASTLookAndFeel::drawComboBox(Graphics& g, int width, int height, const bool isButtonDown,
	int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& box)
{
	auto cornerSize = 0.0f; // box.findParentComponentOfClass<ChoicePropertyComponent>() != nullptr ? 0.0f : 3.0f;
	juce::Rectangle<int> boxBounds(0, 0, width, height);

	g.setColour(findVASTColour(VASTColours::colBoxTextBackground));
	g.fillRoundedRectangle(boxBounds.toFloat(), cornerSize);
	if (box.isMouseOver(true)) //including children
		g.setColour(findVASTColour(VASTColours::colBoxOutline).brighter(0.5f));
	else 
		g.setColour(findVASTColour(VASTColours::colBoxOutline));
	g.drawRoundedRectangle(boxBounds.toFloat().reduced(0.5f, 0.5f), cornerSize, 1.0f * m_scaleFactorHeight);

	//juce::Rectangle<int> arrowZone(width - 7, 0, 7, height);
	float arrowRectSize = height * 0.38f;
	float arrowRectInset = height * 0.31f;
	juce::Rectangle<int> arrowZone(width - arrowRectSize - arrowRectInset * 0.3f, arrowRectInset, arrowRectSize, arrowRectSize);
	Path path;
	//path.startNewSubPath(arrowZone.getX(), arrowZone.getCentreY() - 2.0f);
	path.startNewSubPath(arrowZone.getX(), arrowZone.getY());
	//path.lineTo(static_cast<float> (arrowZone.getCentreX() - 1.f), arrowZone.getCentreY() + 3.0f);
	path.lineTo(static_cast<float> (arrowZone.getCentreX()), arrowZone.getBottom());
	//path.lineTo(arrowZone.getRight() - 3.0f, arrowZone.getCentreY() - 2.0f);
	path.lineTo(arrowZone.getRight(), arrowZone.getY());
	path.closeSubPath();

	if (box.isMouseOver(true))  //including children
		g.setColour(findVASTColour(VASTColours::colBoxArrow).withAlpha((box.isEnabled() ? 1.0f : 0.2f)));
	else 
		g.setColour(findVASTColour(VASTColours::colBoxArrow).withAlpha((box.isEnabled() ? 0.5f : 0.1f)));
	
	//g.strokePath(path, PathStrokeType(strokeSize)); 
	g.fillPath(path); 

	box.setColour(ComboBox::textColourId, findVASTColour(VASTColours::colBoxText));
}

void VASTLookAndFeel::positionComboBoxText(ComboBox& box, Label& label)
{
	label.setBounds(2.f * m_scaleFactorWidth, 1.5f * m_scaleFactorHeight,
		box.getWidth() - 2.f * m_scaleFactorWidth,
		box.getHeight() - 2.f * m_scaleFactorHeight);

	Font font = getDefaultFontBold();

	label.setComponentID("ComboBox");
	label.setFont(font);
	label.setColour(Label::textColourId, findVASTColour(VASTColours::colBoxText));
	label.setColour(TextEditor::textColourId, findVASTColour(VASTColours::colBoxText));
	label.setColour(Label::textWhenEditingColourId, findVASTColour(VASTColours::colBoxText));	
}

//pitchbend and modwheel
void VASTLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
	float sliderPos, float minSliderPos, float maxSliderPos,
	const Slider::SliderStyle style, Slider& slider) 
{

	drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
	drawLinearSliderThumb(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
}

void VASTLookAndFeel::drawLinearSliderThumb(Graphics& g, int x, int y, int width, int height,
	float sliderPos, float minSliderPos, float maxSliderPos,
	const Slider::SliderStyle style, Slider& slider)
{
	auto sliderRadius = (float)getSliderThumbRadius(slider);
	auto grey = Colour(0xff2b2e33);
	g.setColour(Colour(grey).withMultipliedSaturation(0.8f));

	if (style == Slider::LinearHorizontal)
	{
		float kx, ky;
		kx = sliderPos;
		ky = x + height * 0.5f;
		g.fillRect(juce::Rectangle<float>(kx - 5.0f, ky - sliderRadius + 0.2f, 10.0f, sliderRadius - 0.2f));

		auto white = Colours::grey.withMultipliedAlpha(0.5f);
		g.setColour(white);
		auto calculatedOffset = (sliderPos - 12.0f) / 66.0f * 8.0f;
		g.fillRect(juce::Rectangle<float>(kx - 5.0f + calculatedOffset, ky - sliderRadius + 0.2f, 2.0f, sliderRadius - 0.2f));
	}
	else if (style == Slider::LinearVertical) //pitchbend and modhweel!
	{
		/*
		float ky;
		ky = sliderPos;
		g.fillRect(Rectangle<float>(2.f, ky - sliderRadius * 0.5f, width - 4.f, sliderRadius));

		auto white = Colours::grey.withMultipliedAlpha(0.5f);
		g.setColour(white);
		g.fillRect(Rectangle<float>(2.f , ky - 1.f, width - 4.f, 2.f));
		*/

		float ky;
		ky = sliderPos;
		auto grey = Colour(0xff444951);

		float sliderWidth = width * 0.7f; //smaller due to shadow;
		drawRoundShape(g,
			2.f,
			y - sliderRadius + 2.f,
			sliderWidth - 4.f, (height + y + 4.f) * 0.3f,
			grey, true, true);
		drawRoundShape(g,
			2.f,
			y - sliderRadius + 2.f + (height + y + 4.f) * 0.3f - 1.f,
			sliderWidth - 4.f, (height + y + 4.f) * 0.7f,
			grey, false, true);
		drawRoundShape(g,
			2.f,
			ky - sliderRadius * 0.5f,
			sliderWidth - 4.f, sliderRadius * 0.5f,
			grey.darker(0.5f), false, false);
		drawRoundShape(g,
			2.f,
			ky,
			sliderWidth - 4.f, sliderRadius * 0.5f,
			grey, true, false);			
	}
}

void VASTLookAndFeel::drawRoundShape(Graphics& g,
	const float x, const float y, const float width, const float height,
	const Colour& colour,
	const bool upperIsLower,
	const bool convex)
{
	juce::Rectangle<float> r(x, y, width, height);
	Colour c1 = colour;
	Colour c2 = colour;
	
	if (convex) {
		if (upperIsLower) {
			c1 = c1.darker(1.f).darker(1.f);
			c2 = c2.brighter(0.3f);
		}
		else {
			c1 = c1.brighter(0.3f);
			c2 = c2.darker(1.f).darker(1.f);
		}
	}
	else {
		if (upperIsLower)
			c2 = c2.brighter(0.3f);
		else {
			c1 = c1.darker(1.f);
			c2 = c2.darker(0.1f);
		}
	}
	g.setGradientFill({ c1, r.getX() + r.getWidth() * 0.5f, r.getY(), c2, r.getX() + r.getWidth() * 0.5f, r.getY() + r.getHeight(), false });
	g.fillRect(r);
}

void VASTLookAndFeel::drawLinearSliderBackground(Graphics& g, int x, int y, int width, int height,
	float sliderPos,
	float minSliderPos,
	float maxSliderPos,
	const Slider::SliderStyle style, Slider& slider) {
	Colour black(0xff2b2e33);

	g.setColour(black);
	//auto grey = Colour(0xff444951);
	auto sliderRadius = getSliderThumbRadius(slider) - 1.0f;
	float sliderWidth = width * 0.7f; //smaller due to shadow;
	if (sliderWidth <= 0) return;
	Path off;
	juce::Rectangle<float> backgroundR(0, y - sliderRadius, sliderWidth, height + y + 7.f); //black background
	juce::Rectangle<float> shadowR(sliderWidth * 0.4f, y - sliderRadius + height * 0.13f, sliderWidth * 0.2f, height * 0.6f + y + 7.f); //shadow
	off.addRectangle(shadowR);
	g.setColour(Colours::black.withMultipliedAlpha(0.8f));
	g.fillRect(backgroundR);
	DropShadow(findVASTColour(VASTColours::colRotarySliderKnobShadow), sliderWidth, { int(shadowR.getX()), int(shadowR.getY()) }).drawForPath(g, off);
	DropShadow(findVASTColour(VASTColours::colRotarySliderKnobShadow), sliderWidth, { int(shadowR.getX()), int(shadowR.getY()) }).drawForPath(g, off);
	DropShadow(findVASTColour(VASTColours::colRotarySliderKnobShadow), sliderWidth, { int(shadowR.getX()), int(shadowR.getY()) }).drawForPath(g, off);
}

class VASTLookAndFeel::SliderLabelComp : public Label
{
public:
	SliderLabelComp() : Label(String(), String()) {} //empty is deprecated

	void mouseWheelMove(const MouseEvent&, const MouseWheelDetails&) {}
};

Label* VASTLookAndFeel::createComboBoxTextBox(ComboBox& box) {
	Label* l = new Label(String(), String()); //empty is deprecated
	l->setLookAndFeel(this);
	l->setComponentID("ComboBox");
	return l;
}

Button* VASTLookAndFeel::createSliderButton(Slider&, bool isIncrement) {
	TextButton* textButton = new TextButton(isIncrement ? "+" : "-", String());
	return textButton;
}

void VASTLookAndFeel::drawButtonBackground(Graphics& g,
	Button& button,
	const Colour& backgroundColour,
	bool isMouseOverButton,
	bool isButtonDown)
{
	const int width = button.getWidth();
	const int height = button.getHeight();

	const float outlineThickness = button.isEnabled() ? ((isButtonDown || isMouseOverButton) ? 1.2f * m_scaleFactorHeight : 0.7f * m_scaleFactorHeight) : 0.4f * m_scaleFactorHeight;
	const float halfThickness = outlineThickness * 0.5f * m_scaleFactorHeight;

	const float indentL = button.isConnectedOnLeft() ? 0.1f * m_scaleFactorHeight : halfThickness;
	const float indentR = button.isConnectedOnRight() ? 0.1f * m_scaleFactorHeight : halfThickness;
	const float indentT = button.isConnectedOnTop() ? 0.1f * m_scaleFactorHeight : halfThickness;
	const float indentB = button.isConnectedOnBottom() ? 0.1f * m_scaleFactorHeight : halfThickness;
	
	//const Colour baseColour(createBaseColour(backgroundColour,
	
	Colour c1 = findVASTColour(VASTColours::colButtonBackground);
	if (button.getComponentID().equalsIgnoreCase("screenSize")) 
		c1 = Colour(0x00A45c94);
	const Colour baseColour(createBaseColour(c1,
		button.hasKeyboardFocus(true),
		isMouseOverButton, isButtonDown)
		.withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.2f));

	float l_cornerSize = 1.0f;
	drawGlassLozenge(g,
		indentL,
		indentT,
		width - indentL - indentR,
		height - indentT - indentB,
		baseColour, outlineThickness, l_cornerSize,
		button.isConnectedOnLeft(),
		button.isConnectedOnRight(),
		button.isConnectedOnTop(),
		button.isConnectedOnBottom());
}

Label* VASTLookAndFeel::createSliderTextBox(Slider & slider) {
	Label* const l = new SliderLabelComp();

	if ((slider.getSliderStyle() == Slider::LinearBarVertical) || (slider.getSliderStyle() == Slider::LinearVertical)) {
		l->setJustificationType(Justification::right);
	}
	else
		l->setJustificationType(Justification::centred);

	l->setColour(Label::textColourId, findVASTColour(VASTColours::colBoxText));

	l->setColour(Label::backgroundColourId, findVASTColour(VASTColours::colBoxTextBackground)); //CHTS

	l->setColour(Label::outlineColourId, slider.findColour(Slider::textBoxOutlineColourId).darker(1.f));

	//l->setColour(TextEditor::textColourId, slider.findColour(Slider::textBoxTextColourId));
	l->setColour(TextEditor::textColourId, findVASTColour(VASTColours::colBoxText)); //CHTS

	l->setColour(TextEditor::backgroundColourId,
		findVASTColour(VASTColours::colBoxTextBackground)
		.withAlpha((slider.getSliderStyle() == Slider::LinearBar || slider.getSliderStyle() == Slider::LinearBarVertical)
			? 0.7f : 1.0f));

	l->setColour(TextEditor::outlineColourId, slider.findColour(Slider::textBoxOutlineColourId).darker(1.f));

	l->setColour(TextEditor::highlightColourId, slider.findColour(Slider::textBoxHighlightColourId));

	/*
	Font newFont = getDefaultFont();
	float newheight = jmin(slider.getHeight() * 1.2f, slider.getWidth() * 0.19f);
	newFont.setSizeAndStyle(newheight, Font::bold, 1.0f, 0.0f);
	l->setFont(newFont);
	*/

	Font font = getDefaultFontBold();
	l->setFont(font);

	if (slider.getSliderStyle() == Slider::LinearVertical) { //CHTS
		l->setJustificationType(Justification::horizontallyCentred); //
	}

	if (slider.getSliderStyle() == Slider::IncDecButtons) { //CHTS
		l->setJustificationType(Justification::centredRight); //horizontallyCentred
		l->setColour(Label::textColourId, findVASTColour(VASTColours::colBoxText));
		l->setComponentID("IncDecButtons");
		//l->setEditable(true); //singleclick
		//slider.setTextBoxIsEditable(true);
	}

	if ((slider.getSliderStyle() == Slider::Rotary) ||
		(slider.getSliderStyle() == Slider::RotaryHorizontalDrag) ||
		(slider.getSliderStyle() == Slider::RotaryHorizontalVerticalDrag) ||
		(slider.getSliderStyle() == Slider::RotaryVerticalDrag)) { //CHTS
		if (slider.getTextBoxPosition() == Slider::TextBoxRight) {
			l->setJustificationType(Justification::left);
		}
	}
	// VAST adjustments

	return l;
}

void VASTLookAndFeel::drawGroupComponentOutline(Graphics& g, int width, int height,
	const String& text, const Justification& position,
	GroupComponent& group) {
	
	bool lF_ice = dynamic_cast<VASTLookAndFeelThemeIce*>(this);

	float textH = height * 0.16f;  ///15.0f;
	if (textH > 18.f) textH = 18.f;

	if (!group.getComponentID().equalsIgnoreCase("Voicing")) {

		const float indent = 3.0f * m_scaleFactorWidth;
		const float textEdgeGap = 4.0f * m_scaleFactorWidth;
		auto cs = 5.0f;

		Label l;
		l.setColour(Label::textColourId, findVASTColour(VASTColours::colGroupComponentText));
		l.setColour(Label::backgroundColourId, findVASTColour(VASTColours::colGroupComponentTextBackground));
		l.setText(text, NotificationType::dontSendNotification);
		l.setComponentID("GroupComponent");
		Font f = getLabelFont(l);
		//f.setSizeAndStyle(textH, f.getStyleFlags(), 1.0f, 0.0f);
		g.setFont(f);

		Path p;
		auto x = indent;
		auto y = f.getAscent() - 3.0f;
		auto w = jmax(0.0f, width - x * 2.0f);
		auto h = jmax(0.0f, height - y - indent);
		cs = jmin(cs, w * 0.5f, h * 0.5f);
		auto cs2 = 2.0f * cs;

		auto textW = text.isEmpty() ? 0 : jlimit(0.0f, jmax(0.0f, w - cs2 - textEdgeGap * 2), f.getStringWidth(text) + textEdgeGap * 2.0f);
		auto textX = cs + textEdgeGap;

		if (position.testFlags(Justification::horizontallyCentred))
			textX = cs + (w - cs2 - textW) * 0.5f;
		else if (position.testFlags(Justification::right))
			textX = w - cs - textW - textEdgeGap;

		p.startNewSubPath(x + textX + textW, y);
		p.lineTo(x + w - cs, y);

		p.addArc(x + w - cs2, y, cs2, cs2, 0, MathConstants<float>::halfPi);
		p.lineTo(x + w, y + h - cs);

		p.addArc(x + w - cs2, y + h - cs2, cs2, cs2, MathConstants<float>::halfPi, MathConstants<float>::pi);
		p.lineTo(x + cs, y + h);

p.addArc(x, y + h - cs2, cs2, cs2, MathConstants<float>::pi, MathConstants<float>::pi * 1.5f);
p.lineTo(x, y + cs);

p.addArc(x, y, cs2, cs2, MathConstants<float>::pi * 1.5f, MathConstants<float>::twoPi);
p.lineTo(x + textX, y);

auto alpha = group.isEnabled() ? 1.0f : 0.5f;

//g.setColour(group.findColour(GroupComponent::outlineColourId)
//	.withMultipliedAlpha(alpha));

g.setColour(findVASTColour(VASTColours::colGroupComponentOutline)
	.withMultipliedAlpha(alpha));
g.strokePath(p, PathStrokeType(2.0f * m_scaleFactorWidth));

//CHVAST
if (!text.equalsIgnoreCase("")) {
	l.setBounds(roundToInt(x + textX), 0, roundToInt(textW), roundToInt(textH));
	g.setColour(
		findVASTColour(VASTColours::colGroupComponentText).withMultipliedAlpha(alpha));
	g.setFont(f);

	if (!lF_ice)
		g.drawText(text,
			roundToInt(x + textX), 3 * m_scaleFactorHeight,
			roundToInt(textW),
			roundToInt(f.getHeight()),
			Justification::centred, false);
	else
		g.drawText(text.toLowerCase(),
			roundToInt(x + textX), 3 * m_scaleFactorHeight,
			roundToInt(textW),
			roundToInt(f.getHeight()),
			Justification::centred, false);
}
	}
}

void VASTLookAndFeel::drawButtonText(Graphics& g, TextButton& button, bool /*isMouseOverButton*/, bool /*isButtonDown*/)
{
	Font font(getTextButtonFont(button, button.getHeight()));
	g.setFont(font);

	/*g.setColour(button.findColour(button.getToggleState() ? TextButton::textColourOnId
		: TextButton::textColourOffId)
		.withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));
		*/
	g.setColour(
		findVASTColour(VASTColours::colLabelText).withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.3f));


	const int yIndent = jmin(4, button.proportionOfHeight(0.3f));
	const int cornerSize = jmin(button.getHeight(), button.getWidth()) / 2;

	const int fontHeight = roundToInt(font.getHeight() * 0.6f);
	const int leftIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
	const int rightIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
	const int textWidth = button.getWidth() - leftIndent - rightIndent;

	//CHVAST
	if (textWidth > 0)
		g.drawText(button.getButtonText(), leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2, Justification::centred, false);

	/*		g.drawFittedText(button.getButtonText(),
	leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
	Justification::centred, 2);*/
}

BorderSize<int> VASTLookAndFeel::getLabelBorderSize(Label& label) {
	return { int(1 * m_scaleFactorHeight), int(5 * m_scaleFactorWidth), int(1 * m_scaleFactorHeight), int(5 * m_scaleFactorWidth) };
}

void VASTLookAndFeel::drawLabel(Graphics& g, Label& label)
{

	label.setBorderSize(getLabelBorderSize(label));
	bool lF_ice = dynamic_cast<VASTLookAndFeelThemeIce*>(this);

	bool incdec = false;
	bool combo = false;
	bool voicing = false;
	bool headertext = false;
	bool customlabel = false;
	if (label.getComponentID().equalsIgnoreCase("IncDecButtons"))
		incdec = true;
	else if (label.getComponentID().equalsIgnoreCase("ComboBox"))
		combo = true;
	else if (label.getComponentID().equalsIgnoreCase("Voicing"))
		voicing = true;
	else if (label.getComponentID().equalsIgnoreCase("Headertext"))
		headertext = true;
	else if (label.getComponentID().equalsIgnoreCase("CustomLabel"))
		customlabel = true;

	if (incdec || voicing)
		g.fillAll(findVASTColour(VASTColours::colBoxTextBackground));
	else if (!combo)
		g.fillAll(findVASTColour(VASTColours::colLabelBackground));

	if (label.findColour(Label::textColourId) == Colours::transparentBlack) { //for the accessibleMessageLabel
		//dont draw it
	}
	else {

		if (!label.isBeingEdited())
		{
			auto alpha = label.isEnabled() ? 1.0f : 0.5f;
			if (combo) alpha = 1.f;
			const Font font(getLabelFont(label));

			if (combo || incdec)
				g.setColour(findVASTColour(VASTColours::colBoxText).withMultipliedAlpha(alpha));
			else if (headertext)
				g.setColour(findVASTColour(VASTColours::colHeaderLabelText).withMultipliedAlpha(alpha));
			else if (customlabel)
				g.setColour(findVASTColour(VASTColours::colLabelCustom).withMultipliedAlpha(alpha));
			else
				g.setColour(findVASTColour(VASTColours::colLabelText).withMultipliedAlpha(alpha));
			g.setFont(font);

			juce::Rectangle<int> textArea(label.getBorderSize().subtractedFrom(label.getLocalBounds()));

			if (!lF_ice)
				g.drawText(label.getText(), textArea, label.getJustificationType(), false);
			else {
				if ((!incdec) && (!combo) && (!voicing) && (label.isEnabled()))
					g.drawText(label.getText().toLowerCase(), textArea, label.getJustificationType(), false);
				else
					g.drawText(label.getText(), textArea, label.getJustificationType(), false);
			}

			if (!combo && !incdec) {
				g.setColour(findVASTColour(VASTColours::colLabelOutline).withMultipliedAlpha(alpha));
				g.drawRect(label.getLocalBounds().toFloat(), 1.f * m_scaleFactorHeight);
			}
			else
				if (incdec) {
					g.setColour(findVASTColour(VASTColours::colBoxOutline).withMultipliedAlpha(alpha));
					g.drawRect(label.getLocalBounds().toFloat(), 1.f * m_scaleFactorHeight);
				}
		}
		else if (label.isEnabled()) //custom parameter editable
		{
			if (!combo) {
				g.setColour(findVASTColour(VASTColours::colLabelOutlineEnabled));
				g.drawRect(label.getLocalBounds().toFloat(), 1.f * m_scaleFactorHeight);
			}
		}
	}
}

bool VASTLookAndFeel::shouldPopupMenuScaleWithTargetComponent(const PopupMenu::Options &options) {
	return true;
}

void VASTLookAndFeel::drawPopupMenuSectionHeader(Graphics& g, const juce::Rectangle<int>& area, const String& sectionName)
{
	auto font = myDefaultFontBold;
		
	font.setHeight(m_lastTextEditorFontHeight);
	g.setFont(font);
	g.setColour(findColour(PopupMenu::headerTextColourId));

	g.drawFittedText(sectionName,
		area.getX() + 12, area.getY(), area.getWidth() - 16, (int)(area.getHeight() * 0.8f),
		Justification::bottomLeft, 1);
}

void VASTLookAndFeel::getIdealPopupMenuItemSize(const String& text, const bool isSeparator,
	int standardMenuItemHeight, int& idealWidth, int& idealHeight)
{
	if (isSeparator)
	{
		idealWidth = 50;
		idealHeight = standardMenuItemHeight > 0 ? standardMenuItemHeight / 10 : 10;
	}
	else
	{
		auto font = myDefaultFontRegular;
		font.setHeight(m_lastTextEditorFontHeight * 0.9f); //see below drawPopupMenuItem
		//if (standardMenuItemHeight > 0 && font.getHeight() > standardMenuItemHeight / 1.3f)
			//font.setHeight(standardMenuItemHeight / 1.3f);

		//idealHeight = standardMenuItemHeight > 0 ? standardMenuItemHeight : roundToInt(font.getHeight() * 1.4f);
		idealHeight = roundToInt(font.getHeight() * 1.1f);
		idealWidth = font.getStringWidth(text) * 1.1f + idealHeight * 2 + 20;
	}
}

void VASTLookAndFeel::drawPopupMenuItem(Graphics& g, const juce::Rectangle<int>& area,
	const bool isSeparator, const bool isActive,
	const bool isHighlighted, const bool isTicked,
	const bool hasSubMenu, const String& text,
	const String& shortcutKeyText,
	const Drawable* icon, const Colour* const textColourToUse)
{
	if (isSeparator)
	{
		auto r = area.reduced(5, 0);
		r.removeFromTop(roundToInt((r.getHeight() * 0.5f) - 0.5f));

		g.setColour(findColour(PopupMenu::textColourId).withAlpha(0.3f));
		g.fillRect(r.removeFromTop(1));
	}
	else
	{
		auto textColour = (textColourToUse == nullptr ? findColour(PopupMenu::textColourId)
			: *textColourToUse);

		auto r = area.reduced(1);

		if (isHighlighted && isActive)
		{
			g.setColour(findColour(PopupMenu::highlightedBackgroundColourId));
			g.fillRect(r);

			g.setColour(findColour(PopupMenu::highlightedTextColourId));
		}
		else
		{
			g.setColour(textColour.withMultipliedAlpha(isActive ? 1.0f : 0.5f));
		}

		r.reduce(jmin(5, area.getWidth() / 20), 0);

		//auto font = getPopupMenuFont();
		//auto maxFontHeight = r.getHeight() / 1.3f;
		auto maxFontHeight = 20.f;
		auto font = myDefaultFontRegular;
		//

		//if (font.getHeight() > maxFontHeight)
			font.setHeight(area.getHeight()*0.9f);

		g.setFont(font);

		auto iconArea = r.removeFromLeft(roundToInt(maxFontHeight)).toFloat();

		if (icon != nullptr)
		{
			icon->drawWithin(g, iconArea, juce::RectanglePlacement::centred | juce::RectanglePlacement::onlyReduceInSize, 1.0f);
			r.removeFromLeft(roundToInt(maxFontHeight * 0.5f));
		}
		else if (isTicked)
		{
			auto tick = getTickShape(1.0f);
			g.fillPath(tick, tick.getTransformToScaleToFit(iconArea.reduced(iconArea.getWidth() / 5, 0).toFloat(), true));
		}

		if (hasSubMenu)
		{
			auto arrowH = 0.6f * getPopupMenuFont().getAscent();

			auto x = static_cast<float> (r.removeFromRight((int)arrowH).getX());
			auto halfH = static_cast<float> (r.getCentreY());

			Path path;
			path.startNewSubPath(x, halfH - arrowH * 0.5f);
			path.lineTo(x + arrowH * 0.6f, halfH);
			path.lineTo(x, halfH + arrowH * 0.5f);

			g.strokePath(path, PathStrokeType(2.0f));
		}

		r.removeFromRight(3);
		g.drawFittedText(text, r, Justification::centredLeft, 1);

		if (shortcutKeyText.isNotEmpty())
		{
			auto f2 = font;
			f2.setHeight(f2.getHeight() * 0.75f);
			f2.setHorizontalScale(0.95f);
			g.setFont(f2);

			g.drawText(shortcutKeyText, r, Justification::centredRight, true);
		}
	}
}

void VASTLookAndFeel::drawImageButton(Graphics& g, Image* image,
	int imageX, int imageY, int imageW, int imageH,
	const Colour& overlayColour,
	float imageOpacity,
	ImageButton& button) {

	if (!button.isEnabled())
		imageOpacity *= 0.3f;

	AffineTransform t = juce::RectanglePlacement(RectanglePlacement::stretchToFit)
		.getTransformToFit(image->getBounds().toFloat(),
			juce::Rectangle<int>(imageX, imageY, imageW, imageH).toFloat());


	//LookAndFeel_V2::drawImageButton(g, image, imageX, imageY, imageW, imageH, overlayColour, imageOpacity, button);

	if (!overlayColour.isOpaque())
	{
		g.setOpacity(imageOpacity);
		g.drawImageTransformed(*image, t, false);
	}

	if (!overlayColour.isTransparent())
	{
		g.setColour(overlayColour);
		g.drawImageTransformed(*image, t, true);
	}
	
	if (myProcessor != NULL) {
		if (myProcessor->m_iNowLearningMidiParameterVariableName != "")
			if (button.getComponentID().equalsIgnoreCase(myProcessor->m_iNowLearningMidiParameterVariableName)) { //MidiLearn
				g.fillAll(findVASTColour(VASTColours::colMidiLearn));
			}
	}
}

juce::Rectangle<int> VASTLookAndFeel::getTooltipBounds(const String& tipText, Point<int> screenPos, juce::Rectangle<int> parentArea) {

	const int maxToolTipWidth = 400;
	Font font = getDefaultFontBold();
	font.setSizeAndStyle(m_lastTextEditorFontHeight * 0.75f, font.getStyleFlags(), 1.0f, 0.0f); //0.8 is hack

	AttributedString s;
	s.setJustification(Justification::centred);
	s.append(tipText, font, Colours::white);

	TextLayout tl;
	tl.createLayoutWithBalancedLineLengths(s, (float)maxToolTipWidth);

	auto w = (int)(tl.getWidth() + 14.0f);
	auto h = (int)(tl.getHeight() + 6.0f);

	return juce::Rectangle<int>(screenPos.x > parentArea.getCentreX() ? screenPos.x - (w + 12) : screenPos.x + 24,
		screenPos.y > parentArea.getCentreY() ? screenPos.y - (h + 6) : screenPos.y + 6,
		w, h)
		.constrainedWithin(parentArea);
}

void VASTLookAndFeel::drawTooltip(Graphics& g, const String& text, int width, int height)
{
	g.fillAll(findColour(TooltipWindow::backgroundColourId));

#if ! JUCE_MAC // The mac windows already have a non-optional 1 pix outline, so don't double it here..
	g.setColour(findColour(TooltipWindow::outlineColourId));
	g.drawRect(0.f, 0.f, float(width), float(height), 1.f * m_scaleFactorHeight);
#endif

	Font font = getDefaultFontBold();
	font.setSizeAndStyle(m_lastTextEditorFontHeight * 0.75f, font.getStyleFlags(), 1.0f, 0.0f); //0.8 is hack
	g.setFont(font);

	const int maxToolTipWidth = 400;

	AttributedString s;
	s.setJustification(Justification::centred);
	s.append(text, font, Colours::white);

	TextLayout tl;
	tl.createLayoutWithBalancedLineLengths(s, (float)maxToolTipWidth);	
	tl.draw(g, juce::Rectangle<float>((float)width, (float)height));
}

void VASTLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button,
	bool isMouseOverButton, bool isButtonDown)
{
	/*
	if (button.hasKeyboardFocus(true))
	{
	g.setColour(button.findColour(TextEditor::focusedOutlineColourId));
	g.drawRect(0, 0, button.getWidth(), button.getHeight());
	}
	*/

	if (myProcessor != NULL) {
		if (myProcessor->m_iNowLearningMidiParameterVariableName != "")
			if (button.getComponentID().equalsIgnoreCase(myProcessor->m_iNowLearningMidiParameterVariableName)) { //MidiLearn
				g.fillAll(findVASTColour(VASTColours::colMidiLearn));
		}
	}

	float _ht = jmin(button.getHeight(), button.getWidth());
	float _wi = _ht; //make it square
	float _centerx = button.getWidth() * 0.5f;
	float _centery = button.getHeight() * 0.5f;

	float _x1 = _centerx - _wi * 0.25f;
	float _x2 = _centerx + _wi * 0.25f;
	float _y1 = _centery - _ht * 0.25f;
	float _y2 = _centery + _ht * 0.25f;

	float _x1_20perc = _centerx - _wi * 0.27f;
	float _x2_20perc = _centerx + _wi * 0.27f;
	float _y1_20perc = _centery - _ht * 0.27f;
	float _y2_20perc = _centery + _ht * 0.27f;

	float _x1_15perc = _centerx - _wi * 0.30f;
	float _x2_15perc = _centerx + _wi * 0.30f;
	float _y1_15perc = _centery - _ht * 0.30f;
	float _y2_15perc = _centery + _ht * 0.30f;

	float _x1_10perc = _centerx - _wi * 0.35f;
	float _x2_10perc = _centerx + _wi * 0.35f;
	float _y1_10perc = _centery - _ht * 0.35f;
	float _y2_10perc = _centery + _ht * 0.35f;


	juce::Colour col_shade;
	if (!isMouseOverButton)
		col_shade = findVASTColour(VASTColours::colToggleButtonShade);
	else
		col_shade = findVASTColour(VASTColours::colToggleButtonShadeMouseOver);
	juce::Colour col_inner = findVASTColour(VASTColours::colToggleButtonInner);
	juce::Colour col_light = findVASTColour(VASTColours::colToggleButtonLight);
	juce::Colour col_outer = findVASTColour(VASTColours::colToggleButtonOuter);
	if (button.getToggleState()) {
		if (!isMouseOverButton)
			col_shade = findVASTColour(VASTColours::colToggleButtonShadeOn);
		else
			col_shade = findVASTColour(VASTColours::colToggleButtonShadeMouseOverOn);
		col_inner = findVASTColour(VASTColours::colToggleButtonInnerOn);
		col_light = findVASTColour(VASTColours::colToggleButtonLightOn);
		col_outer = findVASTColour(VASTColours::colToggleButtonOuterOn);
	}

	float w4 = _y1_10perc;
	
	g.setOpacity(1.0f);
	g.setColour(col_shade);
	g.drawRoundedRectangle(_x1_10perc, _y1_10perc, _x2_10perc - _x1_10perc, _y2_10perc - _y1_10perc, w4, w4);

	g.setColour(findVASTColour(VASTColours::colToggleButtonBackground));

	g.drawRoundedRectangle(_x1_15perc, _y1_15perc, _x2_15perc - _x1_15perc, _y2_15perc - _y1_15perc, w4 / 2, w4 / 2);

	g.setColour(col_light);
	g.drawRoundedRectangle(_x1_20perc, _y1_20perc, _x2_20perc - _x1_20perc, _y2_20perc - _y1_20perc, w4 / 2, w4 / 4);

	Path pButtonOutline;
	pButtonOutline.addRoundedRectangle(_x1, _y1, _x2 - _x1, _y2 - _y1, w4 / 2);
	ColourGradient c_gradient(col_inner, _centerx, _centery, col_outer, _x2, _y2, true); //If israidal, the gradient should be filled circularly, centred around point1, with point2 defining a point on the circumference.
	g.setGradientFill(c_gradient);
	g.fillPath(pButtonOutline);

}

void VASTLookAndFeel::drawGlassLozenge(Graphics& g,
	const float x, const float y, const float width, const float height,
	const Colour& colour, const float outlineThickness, const float cornerSize,
	const bool flatOnLeft,
	const bool flatOnRight,
	const bool flatOnTop,
	const bool flatOnBottom) noexcept
{
	if (width <= outlineThickness || height <= outlineThickness)
		return;

	const int intX = (int)x;
	const int intY = (int)y;
	const int intW = (int)width;
	const int intH = (int)height;

	const float cs = cornerSize < 0 ? jmin(width * 0.5f, height * 0.5f) : cornerSize;
	const float edgeBlurRadius = height * 0.75f + (height - cs * 2.0f);
	const int intEdge = (int)edgeBlurRadius;

	Path outline;
	outline.addRoundedRectangle(x, y, width, height, cs, cs,
		!(flatOnLeft || flatOnTop),
		!(flatOnRight || flatOnTop),
		!(flatOnLeft || flatOnBottom),
		!(flatOnRight || flatOnBottom));

	{
		ColourGradient cg(colour.darker(0.2f), 0, y,
			colour.darker(0.2f), 0, y + height, false);

		cg.addColour(0.03, colour);
		cg.addColour(0.4, colour);
		cg.addColour(0.97, colour);

		g.setGradientFill(cg);
		g.fillPath(outline);
	}

	//	ColourGradient cg(findVASTColour(VASTColours::colGlassLozengeGradient), x + edgeBlurRadius, y + height * 0.5f,
	//		colour.darker(0.2f), x, y + height * 0.5f, true);

	//	cg.addColour(jlimit(0.0, 1.0, 1.0 - (cs * 0.5f) / edgeBlurRadius), findVASTColour(VASTColours::colGlassLozengeGradient));

	ColourGradient cg(colour, x + edgeBlurRadius, y + height * 0.5f,
		colour.darker(0.2f), x, y + height * 0.5f, true);

	cg.addColour(jlimit(0.0, 1.0, 1.0 - (cs * 0.5f) / edgeBlurRadius), colour);

	cg.addColour(jlimit(0.0, 1.0, 1.0 - (cs * 0.25f) / edgeBlurRadius), colour.darker(0.2f).withMultipliedAlpha(0.3f));
	//cg.addColour(jlimit(0.0, 1.0, 1.0 - (cs * 0.5f) / edgeBlurRadius), VASTColours::black);
	//cg.addColour(jlimit(0.0, 1.0, 1.0 - (cs * 0.25f) / edgeBlurRadius), VASTColours::black);

	if (!(flatOnLeft || flatOnTop || flatOnBottom))
	{
		g.saveState();
		g.setGradientFill(cg);
		g.reduceClipRegion(intX, intY, intEdge, intH);
		g.fillPath(outline);
		g.restoreState();
	}

	if (!(flatOnRight || flatOnTop || flatOnBottom))
	{
		cg.point1.setX(x + width - edgeBlurRadius);
		cg.point2.setX(x + width);

		g.saveState();
		g.setGradientFill(cg);
		g.reduceClipRegion(intX + intW - intEdge, intY, 2 + intEdge, intH);
		g.fillPath(outline);
		g.restoreState();
	}

	{
		const float leftIndent = flatOnTop || flatOnLeft ? 0.0f : cs * 0.4f;
		const float rightIndent = flatOnTop || flatOnRight ? 0.0f : cs * 0.4f;

		Path highlight;
		highlight.addRoundedRectangle(x + leftIndent,
			y + cs * 0.1f,
			width - (leftIndent + rightIndent),
			height * 0.4f,
			cs * 0.4f,
			cs * 0.4f,
			!(flatOnLeft || flatOnTop),
			!(flatOnRight || flatOnTop),
			!(flatOnLeft || flatOnBottom),
			!(flatOnRight || flatOnBottom));

		g.setGradientFill(ColourGradient(colour.brighter(0.3f), 0, y + height * 0.06f,
			Colours::transparentWhite, 0, y + height * 0.4f, false));
		//g.setGradientFill(ColourGradient(VASTColours::white, 0, y + height * 0.06f,
		//Colours::white, 0, y + height * 0.4f, false));
		g.fillPath(highlight);
	}

	//	g.setColour(colour.darker().withMultipliedAlpha(1.5f));
	g.setColour(colour.darker().withMultipliedAlpha(10.f));
	g.strokePath(outline, PathStrokeType(outlineThickness));
}

void VASTLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
	const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
	/*
	const float radius = jmin(width / 2, height / 2) - 2.0f;
	const float centreX = x + width * 0.5f;
	const float centreY = y + height * 0.5f;
	const float rx = centreX - radius;
	const float ry = centreY - radius;
	const float rw = radius * 2.0f;
	const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
	*/

	float radius = jmin(width / 2, height / 2) * 0.60f; // 40% smaller
	if (radius < 1.f) radius = 1.f;
	const float centreX = x + width * 0.5f;
	const float centreY = y + height * 0.5f;
	const float rx = centreX - radius;
	const float ry = centreY - radius;
	const float rw = radius * 2.0f;
	const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();

	bool hasModMatrixSource = false;
	bool isDDTarget = false;
	double modStartPercentage = 0.0;
	double modEndPercentage = 0.0;
	double modStartPercentageUI = 0.0;
	double modEndPercentageUI = 0.0;
	double valStartPercentage = 0.0;
	double valEndPercentage = 0.0;
	float lastSrceVals[C_MAX_POLY] {};

	if (slider.getValue() != slider.getDoubleClickReturnValue()) {
		//not the default value
		juce::Range<double> range = (juce::Range<double>)slider.getRange();
		double defaultVal = slider.getDoubleClickReturnValue();
		double curVal = slider.getValue();
		double skew = slider.getSkewFactor();

		double curValPercentage = (curVal - range.getStart()) / (range.getEnd() - range.getStart());
		if (curValPercentage < 0) curValPercentage = 0.f; //not in range
		double curValPercentageUI = std::pow(curValPercentage, skew);
		double defaultValPercentage = (defaultVal - range.getStart()) / (range.getEnd() - range.getStart());
		if (defaultValPercentage < 0) defaultValPercentage = 0.f; //not in range
		double defaultValPercentageUI = std::pow(defaultValPercentage, skew);

		if (curVal > defaultVal) {
			valStartPercentage = defaultValPercentageUI;
			valEndPercentage = curValPercentageUI;
		}
		else {
			valStartPercentage = curValPercentageUI;
			valEndPercentage = defaultValPercentageUI;
		}
	}

	bool isParameterSlider = false;
    bool isHighlighted = false;
    bool isDragAndDropInterested = false;
	VASTParameterSlider* _parameterslider = nullptr;
    if ((_parameterslider = dynamic_cast<VASTParameterSlider*>(&slider))) {
		isParameterSlider = true;
		if (myProcessor != NULL) {
			String cid = _parameterslider->getComponentID();
            isHighlighted = _parameterslider->getHighlighted();
			/*
			if (cid.endsWith("_bus2") || cid.endsWith("_bus3")) {
				cid = cid.dropLastCharacters(5);
			}
			*/
            isDragAndDropInterested = _parameterslider->m_draganddropinterested;
			int modmatdest = myProcessor->autoParamGetDestination(cid);
			if (modmatdest > 0) { //-1 and 0 are not wanted
				isDDTarget = true;
				if (myProcessor->m_pVASTXperience.m_Set.modMatrixDestinationSetFast(modmatdest)) {
					hasModMatrixSource = true;

					int slot = myProcessor->m_pVASTXperience.m_Set.modMatrixGetFirstSlotWithDestination(modmatdest);					
					float modVal = 0.f;
					double curvy = 0.f;
					int polarity = 0;
					if (slot >= 0)
						myProcessor->m_pVASTXperience.m_Set.modMatrixSlotGetValues(slot, modVal, curvy, polarity, lastSrceVals);

					modVal *= 0.01f;
					juce::Range<double> range = (juce::Range<double>)_parameterslider->getRange();
					double curVal = _parameterslider->getValue();
					double skew = _parameterslider->getSkewFactor();
					double curValPercentage = (curVal - range.getStart()) / (range.getEnd() - range.getStart());
					double curValPercentageUI = std::pow(curValPercentage, skew);

					modStartPercentage = curValPercentage - modVal;
					if (modStartPercentage > 1.f) modStartPercentage = 1.f;
					if (modStartPercentage < 0.f) modStartPercentage = 0.f;
					modEndPercentage = curValPercentage + modVal;
					if (modEndPercentage > 1.f) modEndPercentage = 1.f;
					if (modEndPercentage < 0.f) modEndPercentage = 0.f;

					modStartPercentageUI = std::pow(modStartPercentage, skew);
					modEndPercentageUI = std::pow(modEndPercentage, skew);

					if (polarity == POLARITY::Unipolar) {
						modStartPercentageUI = curValPercentageUI;
						modStartPercentage = curValPercentage; 
					}
				}
			}
			else
				isDDTarget = false;
		}
	}

	if (myProcessor != NULL) {
		if (myProcessor->m_iNowLearningMidiParameterVariableName != "")
			if (slider.getComponentID().equalsIgnoreCase(myProcessor->m_iNowLearningMidiParameterVariableName)) { //MidiLearn
				g.fillAll(findVASTColour(VASTColours::colMidiLearn));
			}
	}

	//draw rotary self
	// fill
	const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
	float radius20perc = radius * 0.2f;
	if (radius20perc <= 1.f) radius20perc = 1;
	float radius30perc = radius * 0.3f;
	if (radius30perc <= 1.f) radius30perc = 1;
	float radius40perc = radius * 0.4f;
	if (radius40perc <= 1.f) radius40perc = 1;
	int radius70perc = radius * 0.7f;
	if (radius70perc <= 1.f) radius70perc = 1;

	float w1 = radius / 12.f; 
	// outer arc	
	if ((isParameterSlider) && (isDDTarget)) {
        if (isHighlighted) {
            g.setColour(findVASTColour(colParameterBindingHighlight));
            g.fillAll();
        }
		Path pArc;
		pArc.addCentredArc(centreX, centreY, radius*1.3f, radius*1.3f, 0.0f, 1.2f * float(M_PI), 2.8f  * float(M_PI), true);
		g.setColour(findVASTColour(VASTColours::colRotarySliderOuterArcBackground));
        if (isDragAndDropInterested) {
            g.setColour(findVASTColour(VASTColours::colParameterBindingHighlight));
        }
        g.strokePath(pArc, PathStrokeType(radius30perc));

		if (hasModMatrixSource == false) {
			Path pArc2;
			pArc2.addCentredArc(centreX, centreY, radius*1.3f, radius*1.3f, 0.0f, (1.2f * float(M_PI)) + valStartPercentage * (2.8f - 1.2f) * float(M_PI), (1.2f * float(M_PI)) + valEndPercentage *  (2.8f - 1.2f) * float(M_PI), true);
			g.setColour(findVASTColour(VASTColours::colRotarySliderOuterArcValueIndicator));
			g.strokePath(pArc2, PathStrokeType(radius40perc));
			g.setColour(findVASTColour(VASTColours::colRotarySliderOuterArcValueIndicatorInner));
			g.strokePath(pArc2, PathStrokeType(radius20perc));

		}
		else {
			//yellow arc here
			Path pArc2;
			pArc2.addCentredArc(centreX, centreY, radius*1.3f, radius*1.3f, 0.0f, (1.2f * M_PI) + modStartPercentageUI * (2.8f - 1.2f) * M_PI, (1.2f * M_PI) + modEndPercentageUI * (2.8f - 1.2f) * M_PI, true);
			g.setColour(findVASTColour(VASTColours::colRotarySliderOuterArcModulationIndicator));
			g.strokePath(pArc2, PathStrokeType(radius40perc));
			g.setColour(findVASTColour(VASTColours::colRotarySliderOuterArcModulationIndicatorInner));
			g.strokePath(pArc2, PathStrokeType(radius20perc));

			//actual indicator
			if (!myProcessor->m_disableOpenGLGFX) {				
                /*
				for (int voice = 0; voice < C_MAX_POLY; voice++) {
					if (myProcessor->m_pVASTXperience.m_Poly.isVoicePlaying(voice)) {
						float lastSrceValPercentage = modStartPercentage + ((lastSrceVals[voice] + 1.f) * 0.5f)  * (modEndPercentage - modStartPercentage);
						float lastSrceValPercentageUI = std::pow(lastSrceValPercentage, _parameterslider->getSkewFactor());
						//store for animation
						float lastDrawnSrceValPercentageUI = _parameterslider->m_lastDrawnValPercentageUI[voice];
                        
                        if (abs(lastDrawnSrceValPercentageUI - lastSrceValPercentageUI) < 0.4f) {
                            _parameterslider->m_lastDrawnValPercentageUI[voice] = lastSrceValPercentageUI;
                            Path pArcIndicator;
                            pArcIndicator.addCentredArc(centreX, centreY, radius*1.3f, radius*1.3f, 0.0f, (1.2f * M_PI) + jlimit<float>(0.f, 1.f, lastSrceValPercentageUI) * (2.8f - 1.2f) * M_PI, (1.2f * M_PI) + jlimit<float>(0.f, 1.f, lastDrawnSrceValPercentageUI) * (2.8f - 1.2f) * M_PI, true);
                            
                            g.setGradientFill(ColourGradient(Colour(Colours::magenta).withAlpha(0.5f), pArcIndicator.getPointAlongPath(0.f), Colour(Colours::magenta).withAlpha(0.5f), pArcIndicator.getPointAlongPath(pArcIndicator.getLength()), false));
                            //g.setColour(Colours::white);
                            //g.strokePath(pArcIndicator, PathStrokeType(radius40perc));
                        }
					}
				}
                 */
                for (int voice = 0; voice < C_MAX_POLY; voice++) { //white indicators
                    if (myProcessor->m_pVASTXperience.m_Poly.isVoicePlaying(voice)) {
                        float lastSrceValPercentage = modStartPercentage + ((lastSrceVals[voice] + 1.f) * 0.5f)  * (modEndPercentage - modStartPercentage);
                        float lastSrceValPercentageUI = std::pow(lastSrceValPercentage, _parameterslider->getSkewFactor());
                        g.setColour(Colours::white);
                        Path pInid;
                        pInid.addCentredArc(centreX, centreY, radius*1.3f, radius*1.3f, 0.0f, -0.2f + (1.2f * M_PI) + jlimit<float>(0.f, 1.f, lastSrceValPercentageUI) * (2.8f - 1.2f) * M_PI, 0.2f + (1.2f * M_PI) + jlimit<float>(0.f, 1.f, lastSrceValPercentageUI) * (2.8f - 1.2f) * M_PI, true);
                        g.strokePath(pInid, PathStrokeType(radius40perc));
                    }
                }
			}
		}
	}
	else {
		Path pArc2;
		pArc2.addCentredArc(centreX, centreY, radius*1.3f, radius*1.3f, 0.0f, (1.2f * M_PI) + valStartPercentage * (2.8f - 1.2f) * M_PI, (1.2f * M_PI) + valEndPercentage * (2.8f - 1.2f) * M_PI, true);
		g.setColour(findVASTColour(VASTColours::colRotarySliderOuterArcNonParameter)); 
		g.strokePath(pArc2, PathStrokeType(radius40perc));
		g.setColour(findVASTColour(VASTColours::colRotarySliderOuterArcNonParameterInner)); 
		g.strokePath(pArc2, PathStrokeType(radius20perc));
	}

	// Draw to Image
	Image knobBuffer;
	if (isMouseOver)
		for (int i = 0; i < m_knobBufferArrayMouseOver.size(); i++) {
			if ((m_knobBufferArrayMouseOver[i].getWidth() == width) && (m_knobBufferArrayMouseOver[i].getHeight() == height)) {
				knobBuffer = m_knobBufferArrayMouseOver[i];
				break;
			}
		}
	else 
		for (int i = 0; i < m_knobBufferArray.size(); i++) {
			if ((m_knobBufferArray[i].getWidth() == width) && (m_knobBufferArray[i].getHeight() == height)) {
				knobBuffer = m_knobBufferArray[i];
				break;
			}
		}

	if (knobBuffer.isNull()) {

		knobBuffer = Image(Image::ARGB, width, height, false);
		Image knobBufferMouseOver = Image(Image::ARGB, width, height, false);
		knobBuffer.clear(knobBuffer.getBounds(), Colours::transparentBlack);
		knobBufferMouseOver.clear(knobBufferMouseOver.getBounds(), Colours::transparentBlack);
		Graphics gKnob(knobBuffer);
		Graphics gKnobMouseOver(knobBufferMouseOver);

		Path pKnob;
		pKnob.addEllipse(rx + radius20perc - 1, ry + radius20perc - 0.5f, rw - radius20perc, rw - radius20perc);

		//very expensive
		DropShadow(findVASTColour(VASTColours::colRotarySliderKnobShadow), int(radius * 1.2f), { int(radius20perc), int(radius40perc) }).drawForPath(gKnob, pKnob);
		DropShadow(findVASTColour(VASTColours::colRotarySliderKnobShadow), int(radius * 1.2f), { int(radius20perc), int(radius40perc) }).drawForPath(gKnob, pKnob);
		DropShadow(findVASTColour(VASTColours::colRotarySliderKnobShadow), int(radius * 1.2f), { int(radius20perc), int(radius40perc) }).drawForPath(gKnob, pKnob);
		
		DropShadow(findVASTColour(VASTColours::colRotarySliderKnobShadow), int(radius * 1.2f), { int(radius20perc), int(radius40perc) }).drawForPath(gKnobMouseOver, pKnob);
		DropShadow(findVASTColour(VASTColours::colRotarySliderKnobShadow), int(radius * 1.2f), { int(radius20perc), int(radius40perc) }).drawForPath(gKnobMouseOver, pKnob);
		//very expensive

		Path pKnobHighlight;
		pKnobHighlight.addEllipse(rx + radius20perc - 2 * w1, ry + radius20perc - 1 * w1, rw - radius20perc + 2 * w1, rw - radius20perc + 2 * w1);
		const ColourGradient c_gradient2 = ColourGradient(findVASTColour(VASTColours::colRotarySliderKnobHighlightGradientFrom), rx, ry, findVASTColour(VASTColours::colRotarySliderKnobHighlightGradientTo), rx + rw, ry + rw, false);
		gKnob.setGradientFill(c_gradient2);
		gKnob.fillPath(pKnobHighlight);

		gKnobMouseOver.setGradientFill(c_gradient2);
		gKnobMouseOver.fillPath(pKnobHighlight);

		/*
		// outline of knob dark
		const ColourGradient c_gradient = ColourGradient(findVASTColour(VASTColours::colRotarySliderOuterOutlineGradientFrom), rx, ry, findVASTColour(VASTColours::colRotarySliderOuterOutlineGradientTo), rx + rw, ry + rw, false);
		gKnob.setGradientFill(c_gradient);
		gKnob.drawEllipse(rx, ry, rw, rw + 1, radius30perc);
		*/

		//inner circle line - knob
		gKnobMouseOver.setGradientFill(ColourGradient(findVASTColour(VASTColours::colRotarySliderKnobCenterMouseOverGradientFrom), rx, ry, findVASTColour(VASTColours::colRotarySliderKnobCenterMouseOverGradientTo), rx + rw, ry + rw, false));
		gKnobMouseOver.fillPath(pKnob);
		m_knobBufferArrayMouseOver.add(knobBufferMouseOver);

		gKnob.setGradientFill(ColourGradient(findVASTColour(VASTColours::colRotarySliderKnobCenterGradientFrom), rx, ry, findVASTColour(VASTColours::colRotarySliderKnobCenterGradientTo), rx + rw, ry + rw, true));
		gKnob.fillPath(pKnob);
		m_knobBufferArray.add(knobBuffer);

		if (!isMouseOver)
			knobBuffer = m_knobBufferArray[m_knobBufferArray.size() - 1];
		else 
			knobBuffer = m_knobBufferArrayMouseOver[m_knobBufferArrayMouseOver.size() - 1];
	}

	g.drawImageAt(knobBuffer, 0, 0);
	// Draw to Image

	Path p2;
	float pointerLength = radius70perc;
	float pointerThickness = radius20perc;
	p2.addRectangle(-pointerThickness * 0.5f + 1, -radius + 1, pointerThickness, pointerLength);
	p2.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
	g.setColour(findVASTColour(VASTColours::colRotarySliderMarkerShadow));
	g.fillPath(p2);

	Path p;
	//const float pointerLength = radius * 0.33f;
	pointerLength = radius70perc;
	pointerThickness = radius20perc;
	p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
	p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
	g.setColour(findVASTColour(VASTColours::colRotarySliderMarker));
	g.fillPath(p);

	Path p1;
	pointerLength = radius20perc;
	pointerThickness = radius20perc;
	p1.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
	p1.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
	p1.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
	g.setColour(findVASTColour(VASTColours::colRotarySliderMarkerTip));
	g.fillPath(p1);
}

void VASTLookAndFeel::drawTableHeaderColumn(Graphics& g, TableHeaderComponent& header,
	const String& columnName, int /*columnId*/,
	int width, int height, bool isMouseOver, bool isMouseDown,
	int columnFlags)
{
	//auto highlightColour = header.findColour(TableHeaderComponent::highlightColourId);
	auto highlightColour = findVASTColour(VASTColours::colOscillatorComponentBackgroundGradientFrom);
	
	if (isMouseDown)
		g.fillAll(highlightColour.withMultipliedAlpha(0.625f));
	else if (isMouseOver)
		g.fillAll(highlightColour.contrasting(0.625f));

	juce::Rectangle<int> area(width, height);
	area.reduce(4, 0);

	if ((columnFlags & (TableHeaderComponent::sortedForwards | TableHeaderComponent::sortedBackwards)) != 0)
	{
		Path sortArrow;
		sortArrow.addTriangle(0.0f, 0.0f,
			0.5f, (columnFlags & TableHeaderComponent::sortedForwards) != 0 ? -0.8f : 0.8f,
			1.0f, 0.0f);

		g.setColour(Colour(0x99000000));
		g.fillPath(sortArrow, sortArrow.getTransformToScaleToFit(area.removeFromRight(height / 2).reduced(2).toFloat(), true));
	}

	//g.setColour(header.findColour(TableHeaderComponent::textColourId));
	g.setColour(findVASTColour(VASTColours::colLabelText));
	
	Font font = getDefaultFontBold();
	font.setSizeAndStyle(m_lastTextEditorFontHeight * 0.75f, font.getStyleFlags(), 1.0f, 0.0f); //0.8 is hack
	g.setFont(font);
	//g.drawFittedText(columnName, area, Justification::centredLeft, 1);
	g.drawText(columnName, area, Justification::centredLeft, 1);
}

void VASTLookAndFeel::drawTableHeaderBackground(Graphics& g, TableHeaderComponent& header)
{
	//auto r = header.getLocalBounds();
	//auto outlineColour = header.findColour(TableHeaderComponent::outlineColourId);
	auto outlineColour = Colours::darkgrey;

	//g.setColour(outlineColour);
	//g.fillRect(r.removeFromBottom(1));

	//g.setColour(header.findColour(TableHeaderComponent::backgroundColourId));
	//g.setColour(findVASTColour(VASTColours::colOscillatorComponentBackgroundGradientFrom));
	
	auto highlightColour = findVASTColour(VASTColours::colOscillatorComponentBackgroundGradientFrom);
	g.fillAll(highlightColour);

	g.setColour(outlineColour);

	for (int i = header.getNumColumns(true); --i >= 0;) {
		juce::Rectangle<float> rec = header.getColumnPosition(i).reduced(1, 0).toFloat();
		rec.removeFromBottom(1.f * m_scaleFactorHeight);
		g.drawRect(rec, 1.f * m_scaleFactorHeight);
	}
}

void VASTLookAndFeel::fillTextEditorBackground(Graphics& g, int width, int height, TextEditor& textEditor)
{
	textEditor.setIndents(1.f + 3.f * m_scaleFactorWidth, 2.5f + 1.f * m_scaleFactorHeight);
	textEditor.setColour(TextEditor::outlineColourId, findVASTColour(VASTColours::colBoxOutline));
	textEditor.setColour(TextEditor::focusedOutlineColourId, findVASTColour(VASTColours::colBoxOutline).contrasting(0.4f));

	textEditor.setColour(TextEditor::backgroundColourId, findVASTColour(VASTColours::colBoxTextBackground).withAlpha((textEditor.isEnabled() ? 1.0f : 0.2f)));

	juce::LookAndFeel_V4::fillTextEditorBackground(g, width, height, textEditor);

	//textEditor.applyFontToAllText(getTextEditorFont(textEditor)); //performance
	Font f = getTextEditorFont(textEditor);
	textEditor.setFont(f);
	textEditor.setColour(TextEditor::textColourId, findVASTColour(VASTColours::colBoxText));	
}

void VASTLookAndFeel::drawTextEditorOutline(Graphics& g, int width, int height, TextEditor& textEditor)
{
	if (textEditor.isMouseOver(true)) { //including children
		textEditor.setColour(TextEditor::outlineColourId, findVASTColour(VASTColours::colBoxOutline).brighter(0.5f));
		textEditor.setColour(TextEditor::focusedOutlineColourId, findVASTColour(VASTColours::colBoxOutline).contrasting(0.4f).brighter(0.5f));
	}
	else {
		textEditor.setColour(TextEditor::outlineColourId, findVASTColour(VASTColours::colBoxOutline));
		textEditor.setColour(TextEditor::focusedOutlineColourId, findVASTColour(VASTColours::colBoxOutline).contrasting(0.4f));
	}
	
	//juce::LookAndFeel_V4::drawTextEditorOutline(g, width, height, textEditor);

	if (dynamic_cast<AlertWindow*> (textEditor.getParentComponent()) == nullptr)
	{
		if (textEditor.isEnabled())
		{
			if (textEditor.hasKeyboardFocus(true) && !textEditor.isReadOnly())
			{
				g.setColour(textEditor.findColour(TextEditor::focusedOutlineColourId));
				g.drawRect(0.f, 0.f, float(width), float(height), 2.f * m_scaleFactorHeight);
			}
			else
			{
				g.setColour(textEditor.findColour(TextEditor::outlineColourId));
				g.drawRect(0.f, 0.f, float(width), float(height), 1.f * m_scaleFactorHeight);
			}
		}
	}

	//textEditor.applyFontToAllText(getTextEditorFont(textEditor)); //performance
	Font f = getTextEditorFont(textEditor);
	textEditor.setFont(f);
	textEditor.setColour(TextEditor::textColourId, findVASTColour(VASTColours::colBoxText));
	textEditor.setColour(TextEditor::backgroundColourId, findVASTColour(VASTColours::colBoxTextBackground));
}

int VASTLookAndFeel::getDefaultScrollbarWidth() {
	return 12.f; //also used in Viewoports > c_viewportFreqDomain
}

void VASTLookAndFeel::drawScrollbarButton(Graphics& g, ScrollBar& scrollbar,
	int width, int height, int buttonDirection,
	bool /*isScrollbarVertical*/,
	bool /*isMouseOverButton*/,
	bool isButtonDown)
{
	Path p;

	if (buttonDirection == 0)
		p.addTriangle(width * 0.5f, height * 0.2f,
			width * 0.1f, height * 0.7f,
			width * 0.9f, height * 0.7f);
	else if (buttonDirection == 1)
		p.addTriangle(width * 0.8f, height * 0.5f,
			width * 0.3f, height * 0.1f,
			width * 0.3f, height * 0.9f);
	else if (buttonDirection == 2)
		p.addTriangle(width * 0.5f, height * 0.8f,
			width * 0.1f, height * 0.3f,
			width * 0.9f, height * 0.3f);
	else if (buttonDirection == 3)
		p.addTriangle(width * 0.2f, height * 0.5f,
			width * 0.7f, height * 0.1f,
			width * 0.7f, height * 0.9f);

	if (isButtonDown)
		g.setColour(findVASTColour(VASTColours::colScrollbarButtonDown));
	else
		g.setColour(findVASTColour(VASTColours::colScrollbarButton));

	g.fillPath(p);

	g.setColour(Colour(0x80000000));
	g.strokePath(p, PathStrokeType(0.5f));
}

void VASTLookAndFeel::drawScrollbar(Graphics& g,
	ScrollBar& scrollbar,
	int x, int y,
	int width, int height,
	bool isScrollbarVertical,
	int thumbStartPosition,
	int thumbSize,
	bool /*isMouseOver*/,
	bool /*isMouseDown*/)
{
	g.fillAll(scrollbar.findColour(ScrollBar::backgroundColourId));

	Path slotPath, thumbPath;

	const float slotIndent = jmin(width, height) > 15 ? 1.0f : 0.0f;
	const float slotIndentx2 = slotIndent * 2.0f;
	const float thumbIndent = slotIndent + 1.0f;
	const float thumbIndentx2 = thumbIndent * 2.0f;

	float gx1 = 0.0f, gy1 = 0.0f, gx2 = 0.0f, gy2 = 0.0f;

	if (isScrollbarVertical)
	{
		slotPath.addRoundedRectangle(x + slotIndent,
			y + slotIndent,
			width - slotIndentx2,
			height - slotIndentx2,
			(width - slotIndentx2) * 0.5f);

		if (thumbSize > 0)
			thumbPath.addRoundedRectangle(x + thumbIndent,
				thumbStartPosition + thumbIndent,
				width - thumbIndentx2,
				thumbSize - thumbIndentx2,
				(width - thumbIndentx2) * 0.5f);
		gx1 = (float)x;
		gx2 = x + width * 0.7f;
	}
	else
	{
		slotPath.addRoundedRectangle(x + slotIndent,
			y + slotIndent,
			width - slotIndentx2,
			height - slotIndentx2,
			(height - slotIndentx2) * 0.5f);

		if (thumbSize > 0)
			thumbPath.addRoundedRectangle(thumbStartPosition + thumbIndent,
				y + thumbIndent,
				thumbSize - thumbIndentx2,
				height - thumbIndentx2,
				(height - thumbIndentx2) * 0.5f);
		gy1 = (float)y;
		gy2 = y + height * 0.7f;
	}

	const Colour thumbColour(Colour(0x66EEEEEE));
	Colour trackColour1, trackColour2;

	trackColour1 = Colour(0x66000000);
	trackColour2 = Colour(0x22222222);

	g.setGradientFill(ColourGradient(trackColour1, gx1, gy1,
		trackColour2, gx2, gy2, false));
	g.fillPath(slotPath);

	if (isScrollbarVertical)
	{
		gx1 = x + width * 0.6f;
		gx2 = (float)x + width;
	}
	else
	{
		gy1 = y + height * 0.6f;
		gy2 = (float)y + height;
	}

	g.setGradientFill(ColourGradient(findVASTColour(colScrollBarGradient), gx1, gy1,
		Colour(0x19000000), gx2, gy2, false));
	g.fillPath(slotPath);

	g.setColour(thumbColour);
	g.fillPath(thumbPath);

	g.setGradientFill(ColourGradient(Colour(0x10000000), gx1, gy1,
		Colours::transparentBlack, gx2, gy2, false));

	g.saveState();

	if (isScrollbarVertical)
		g.reduceClipRegion(x + width / 2, y, width, height);
	else
		g.reduceClipRegion(x, y + height / 2, width, height);

	g.fillPath(thumbPath);
	g.restoreState();

	g.setColour(Colour(0x4c000000));
	g.strokePath(thumbPath, PathStrokeType(0.4f));
}


void VASTLookAndFeel::drawFileBrowserRow(Graphics& g, int width, int height,
	const String& filename, Image* icon,
	const String& fileSizeDescription,
	const String& fileTimeDescription,
	const bool isDirectory, const bool isItemSelected,
	const int /*itemIndex*/, DirectoryContentsDisplayComponent& dcc)
{
	Component* const fileListComp = dynamic_cast<Component*> (&dcc);

	if (isItemSelected)
		g.fillAll(fileListComp != nullptr ? fileListComp->findColour(DirectoryContentsDisplayComponent::highlightColourId)
			: findColour(DirectoryContentsDisplayComponent::highlightColourId));
	else
		g.fillAll(findVASTColour(VASTColours::colBoxTextBackground));

	const int x = 32;
	g.setColour(findVASTColour(VASTColours::colFileBrowserIcon));

	if (icon != nullptr && icon->isValid())
	{
		g.drawImageWithin(*icon, 2, 2, x - 4, height - 4,
			RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize,
			false);
	}
	else
	{
		if (const Drawable* d = isDirectory ? getDefaultFolderImage()
			: getDefaultDocumentFileImage())
			d->drawWithin(g, juce::Rectangle<float>(2.0f, 2.0f, x - 4.0f, height - 4.0f),
				RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
	}

	g.setColour(findVASTColour(VASTColours::colBoxText));
	g.setFont(getDefaultFont());
	g.setFont(height * 0.7f);

	if (width > 450 && !isDirectory)
	{
		const int sizeX = roundToInt(width * 0.7f);
		const int dateX = roundToInt(width * 0.8f);

		g.drawFittedText(filename,
			x, 0, sizeX - x, height,
			Justification::centredLeft, 1);

		g.setFont(getDefaultFont());
		g.setFont(height * 0.5f);
		g.setColour(findVASTColour(VASTColours::colBoxText));

		if (!isDirectory)
		{
			g.drawFittedText(fileSizeDescription,
				sizeX, 0, dateX - sizeX - 8, height,
				Justification::centredRight, 1);

			g.drawFittedText(fileTimeDescription,
				dateX, 0, width - 8 - dateX, height,
				Justification::centredRight, 1);
		}
	}
	else
	{
		g.drawFittedText(filename,
			x, 0, width - x, height,
			Justification::centredLeft, 1);

	}
}

void VASTLookAndFeel::layoutFileBrowserComponent(FileBrowserComponent& browserComp,
	DirectoryContentsDisplayComponent* fileListComponent,
	FilePreviewComponent* previewComp,
	ComboBox* currentPathBox,
	TextEditor* filenameBox,
	Button* goUpButton)
{
	const int x = 8;
	int w = browserComp.getWidth() - x - x;

	if (previewComp != nullptr)
	{
		const int previewWidth = w / 3;
		previewComp->setBounds(x + w - previewWidth, 0, previewWidth, browserComp.getHeight());

		w -= previewWidth + 4;
	}

	int y = 4;

	const int controlsHeight = 22;
	const int bottomSectionHeight = controlsHeight + 8;
	const int upButtonWidth = 50;

	currentPathBox->setBounds(x, y, w - upButtonWidth - 6, controlsHeight);
	goUpButton->setBounds(x + w - upButtonWidth, y, upButtonWidth, controlsHeight);

	y += controlsHeight + 4;

	if (Component* const listAsComp = dynamic_cast <Component*> (fileListComponent))
	{
		listAsComp->setBounds(x, y, w, browserComp.getHeight() - y - bottomSectionHeight);
		y = listAsComp->getBottom() + 4;
	}

	filenameBox->setBounds(x + 50, y, w - 50, controlsHeight);
}


//=====================================================================================================
//VU Meter stuff

void VASTLookAndFeel::updateMeterGradients()
{
	horizontalGradient.clearColours();
	verticalGradient.clearColours();
}

juce::Rectangle<float> VASTLookAndFeel::getMeterInnerBounds(const juce::Rectangle<float> bounds,
	const VASTVUMeter::MeterFlags meterType) const
{
	if (meterType & VASTVUMeter::HasBorder) {
		const float corner = std::min(bounds.getWidth(), bounds.getHeight()) * 0.01;
		return bounds.reduced(3 + corner);
	}
	return bounds;
}

juce::Rectangle<float> VASTLookAndFeel::getMeterBounds(const juce::Rectangle<float> bounds,
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

juce::Rectangle<float> VASTLookAndFeel::getMeterBarBounds(const juce::Rectangle<float> bounds,
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

juce::Rectangle<float> VASTLookAndFeel::getMeterTickmarksBounds(const juce::Rectangle<float> bounds,
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

juce::Rectangle<float> VASTLookAndFeel::getMeterClipIndicatorBounds(const juce::Rectangle<float> bounds,
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

juce::Rectangle<float> VASTLookAndFeel::getMeterMaxNumberBounds(const juce::Rectangle<float> bounds,
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

juce::Rectangle<float> VASTLookAndFeel::drawBackground(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds)
{
	g.setColour(findColour(VASTVUMeter::lmBackgroundColour));
	if (meterType & VASTVUMeter::HasBorder) {
		const float corner = std::min(bounds.getWidth(), bounds.getHeight()) * 0.01;
		g.fillRoundedRectangle(bounds, corner);
		g.setColour(findColour(VASTVUMeter::lmOutlineColour));
		g.drawRoundedRectangle(bounds.reduced(3), corner, 2 * m_scaleFactorWidth);
		return bounds.reduced(3 * m_scaleFactorWidth + corner);
	}
	else {
		g.fillRect(bounds);
		return bounds;
	}
}

void VASTLookAndFeel::drawMeterBars(juce::Graphics& g,
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
				juce::Rectangle<float> meter = innerBounds.withHeight(height);
				for (int channel = 0; channel < numChannels; ++channel) {
					meter.setY(height * channel * 2);
					{
						juce::Rectangle<float> meterBarBounds = getMeterBarBounds(meter, meterType);
						drawMeterBar(g, meterType, meterBarBounds,
							source->getRMSLevel(channel),
							source->getMaxLevel(channel));
						const float reduction = source->getReductionLevel(channel);
						if (reduction < 1.0)
							drawMeterReduction(g, meterType,
								meterBarBounds.withBottom(meterBarBounds.getCentreY()),
								reduction);
					}
					juce::Rectangle<float> clip = getMeterClipIndicatorBounds(meter, meterType);
					if (!clip.isEmpty())
						drawClipIndicator(g, meterType, clip, source->getClipFlag(channel));
					juce::Rectangle<float> maxNum = getMeterMaxNumberBounds(meter, meterType);
					if (!maxNum.isEmpty())
						drawMaxNumber(g, meterType, maxNum, source->getMaxOverallLevel(channel));
					if (channel < numChannels - 1) {
						meter.setY(height * (channel * 2 + 1));
						juce::Rectangle<float> ticks = getMeterTickmarksBounds(meter, meterType);
						if (!ticks.isEmpty())
							drawTickMarks(g, meterType, ticks);
					}
				}
			}
			else {
				const float width = innerBounds.getWidth() / (2 * numChannels - 1);
				juce::Rectangle<float> meter = innerBounds.withWidth(width);
				for (int channel = 0; channel < numChannels; ++channel) {
					meter.setX(width * channel * 2);
					{
						juce::Rectangle<float> meterBarBounds = getMeterBarBounds(meter, meterType);
						drawMeterBar(g, meterType, getMeterBarBounds(meter, meterType),
							source->getRMSLevel(channel),
							source->getMaxLevel(channel));
						const float reduction = source->getReductionLevel(channel);
						if (reduction < 1.0)
							drawMeterReduction(g, meterType,
								meterBarBounds.withLeft(meterBarBounds.getCentreX()),
								reduction);
					}
					juce::Rectangle<float> clip = getMeterClipIndicatorBounds(meter, meterType);
					if (!clip.isEmpty())
						drawClipIndicator(g, meterType, clip, source->getClipFlag(channel));
					juce::Rectangle<float> maxNum = getMeterMaxNumberBounds(innerBounds.withWidth(innerBounds.getWidth() / numChannels).withX(innerBounds.getX() + channel * (innerBounds.getWidth() / numChannels)), meterType);
					if (!maxNum.isEmpty())
						drawMaxNumber(g, meterType, maxNum, source->getMaxOverallLevel(channel));
					if (channel < numChannels - 1) {
						meter.setX(width * (channel * 2 + 1));
						juce::Rectangle<float> ticks = getMeterTickmarksBounds(meter, meterType);
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

void VASTLookAndFeel::drawMeterBarsBackground(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds,
	const int numChannels,
	const int fixedNumChannels)
{
	const juce::Rectangle<float> innerBounds = getMeterInnerBounds(bounds, meterType);
	if (meterType & VASTVUMeter::Minimal) {
		if (meterType & VASTVUMeter::Horizontal) {
			const float height = innerBounds.getHeight() / (2 * numChannels - 1);
			juce::Rectangle<float> meter = innerBounds.withHeight(height);
			for (int channel = 0; channel < numChannels; ++channel) {
				meter.setY(height * channel * 2);
				drawMeterBarBackground(g, meterType, getMeterBarBounds(meter, meterType));
				juce::Rectangle<float> clip = getMeterClipIndicatorBounds(meter, meterType);
				if (!clip.isEmpty())
					drawClipIndicatorBackground(g, meterType, clip);
				if (channel < numChannels - 1) {
					meter.setY(height * (channel * 2 + 1));
					juce::Rectangle<float> ticks = getMeterTickmarksBounds(meter, meterType);
					if (!ticks.isEmpty())
						drawTickMarks(g, meterType, ticks);
				}
			}
		}
		else {
			const float width = innerBounds.getWidth() / (2 * numChannels - 1);
			juce::Rectangle<float> meter = innerBounds.withWidth(width);
			for (int channel = 0; channel < numChannels; ++channel) {
				meter.setX(width * channel * 2);
				drawMeterBarBackground(g, meterType, getMeterBarBounds(meter, meterType));
				juce::Rectangle<float> clip = getMeterClipIndicatorBounds(meter, meterType);
				if (!clip.isEmpty())
					drawClipIndicatorBackground(g, meterType, clip);
				if (channel < numChannels - 1) {
					meter.setX(width * (channel * 2 + 1));
					juce::Rectangle<float> ticks = getMeterTickmarksBounds(meter, meterType);
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


void VASTLookAndFeel::drawMeterChannel(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds,
	const VASTVUMeterSource* source,
	const int selectedChannel)
{
	if (source) {
		juce::Rectangle<float> meter = getMeterBarBounds(bounds, meterType);
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
			juce::Rectangle<float> clip = getMeterClipIndicatorBounds(bounds, meterType);
			if (!clip.isEmpty())
				drawClipIndicator(g, meterType, clip, true);
		}
		juce::Rectangle<float> maxes = getMeterMaxNumberBounds(bounds, meterType);
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

void VASTLookAndFeel::drawMeterChannelBackground(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds)
{
	juce::Rectangle<float> meter = getMeterBarBounds(bounds, meterType);
	if (!meter.isEmpty()) {
		drawMeterBarBackground(g, meterType, meter);
	}
	juce::Rectangle<float> clip = getMeterClipIndicatorBounds(bounds, meterType);
	if (!clip.isEmpty())
		drawClipIndicatorBackground(g, meterType, clip);
	juce::Rectangle<float> ticks = getMeterTickmarksBounds(bounds, meterType);
	if (!ticks.isEmpty())
		drawTickMarks(g, meterType, ticks);
	juce::Rectangle<float> maxes = getMeterMaxNumberBounds(bounds, meterType);
	if (!maxes.isEmpty()) {
		drawMaxNumberBackground(g, meterType, maxes);
	}
}

void VASTLookAndFeel::drawMeterBar(juce::Graphics& g,
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

void VASTLookAndFeel::drawMeterReduction(juce::Graphics& g,
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

void VASTLookAndFeel::drawMeterBarBackground(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds)
{
	g.setColour(findColour(VASTVUMeter::lmMeterBackgroundColour));
	g.fillRect(bounds);

	g.setColour(findColour(VASTVUMeter::lmMeterOutlineColour));
	g.drawRect(bounds, 1.f * m_scaleFactorHeight);
}

void VASTLookAndFeel::drawTickMarks(juce::Graphics& g,
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
				g.setFont(getDefaultFont());
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
			g.setFont(getDefaultFont());
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

void VASTLookAndFeel::drawClipIndicator(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds,
	const bool hasClipped)
{
	g.setColour(findColour(hasClipped ? VASTVUMeter::lmBackgroundClipColour : VASTVUMeter::lmMeterBackgroundColour));
	g.fillRect(bounds);
	g.setColour(findColour(VASTVUMeter::lmMeterOutlineColour));
	g.drawRect(bounds, 1.f * m_scaleFactorHeight);
}

void VASTLookAndFeel::drawClipIndicatorBackground(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds)
{
	g.setColour(findColour(VASTVUMeter::lmMeterBackgroundColour));
	g.fillRect(bounds);
	g.setColour(findColour(VASTVUMeter::lmMeterOutlineColour));
	g.drawRect(bounds, 1.f * m_scaleFactorHeight);
}

void VASTLookAndFeel::drawMaxNumber(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds,
	const float maxGain)
{
	g.setColour(findColour(VASTVUMeter::lmMeterBackgroundColour));
	g.fillRect(bounds);
	const float maxDb = juce::Decibels::gainToDecibels(maxGain, -80.0f);
	g.setColour(findColour(maxDb > 0.0 ? VASTVUMeter::lmTextClipColour : VASTVUMeter::lmTextColour));
	g.setFont(getDefaultFont());
	g.setFont(bounds.getHeight() * 0.5f);
	g.drawFittedText(juce::String(maxDb, 1) + " dB",
		bounds.reduced(2.0).toNearestInt(),
		juce::Justification::centred, 1);
	g.setColour(findColour(VASTVUMeter::lmMeterOutlineColour));
	g.drawRect(bounds, 1.f * m_scaleFactorHeight);
}

void VASTLookAndFeel::drawMaxNumberBackground(juce::Graphics& g,
	const VASTVUMeter::MeterFlags meterType,
	const juce::Rectangle<float> bounds)
{
	g.setColour(findColour(VASTVUMeter::lmMeterBackgroundColour));
	g.fillRect(bounds);
}

int VASTLookAndFeel::hitTestClipIndicator(const juce::Point<int> position,
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

int VASTLookAndFeel::hitTestMaxNumber(const juce::Point<int> position,
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
