/*
VAST Dynamics

How to calculate quadratic beziers
x from 0 to 1: 
y = (1-x)^2  * startval  + 2x(1-x) * intermediateval + x^2 * endval

http://slideplayer.com/slide/6554987/

*/

#include "VASTMSEGEditor.h"
#include "../../Engine/VASTEngineHeader.h"
#include "../../Engine/VASTParamState.h"
#include "../VASTAudioProcessor.h"
#include "../VASTAudioProcessorEditor.h"
#include "../../Engine/VASTMSEGData.h"
#include "../VASTControls/VASTParameterSlider.h"
#include "../VASTControls/VASTPositionEditor.h"

VASTMSEGEditor::VASTMSEGEditor(AudioProcessor* processor, VASTMSEGData* data, VASTMSEGData* datalive, int msegNo)
	: myProcessor((VASTAudioProcessor*)processor), myData(data), myDataLive(datalive), myMSEGNo(msegNo)
{
	//myFont.setTypefaceName("Open Sans"); //bold 1-2, regular 2
	//myFont.setDefaultMinimumHorizontalScaleFactor(1.0);
	//myFont.setSizeAndStyle(14.f, Font::bold, 1.0f, 0.0f); // no squashing, no kerning
	m_isDragged = -1;
	m_isDraggedCurve = false;
	
	setOpaque(true); //avoid repaints of parents

	startAutoUpdate();
	resized();
	//updateContent(true);
}

VASTMSEGEditor::~VASTMSEGEditor() {
	this->setLookAndFeel(nullptr);
	stopTimer();
}

void VASTMSEGEditor::resized() {
	m_needsRescale = true;
	m_dirty = true;
}

float VASTMSEGEditor::screenToValX(float screenX) {
	float xVal = (screenX - m_xbounds / m_screenWidthScale) / (m_drawwidth / m_screenWidthScale);
	return jlimit(0.f, 1.f, xVal);
}

float VASTMSEGEditor::screenToValY(float screenY) {
	float yVal = 1.0f - (screenY - m_ybounds / m_screenHeightScale) / (m_drawheight / m_screenHeightScale);
	return jlimit(0.f, 1.f, yVal);	
}

float VASTMSEGEditor::valToScreenX(float xVal) {
	float xPos = xVal * (m_drawwidth / m_screenWidthScale) + m_xbounds / m_screenWidthScale;
	return xPos;
}

float VASTMSEGEditor::valToScreenY(float yVal) {
	float yPos = (1.0f - yVal) *  (m_drawheight / m_screenHeightScale) + m_ybounds / m_screenHeightScale;
	return yPos;
}

void VASTMSEGEditor::lookAndFeelChanged() {
	if (myProcessor->isCurrentEditorInitialized())
		updateContent(true);
}

void VASTMSEGEditor::paint(Graphics& g)
{
	if (waveformImageWithBorder.isNull())
		waveformImageWithBorder = waveformImage.createCopy();

	if (m_needsRescale) {
		waveformImage = waveformImage.rescaled(jmax(1, getWidth()), jmax(1, getHeight()), juce::Graphics::ResamplingQuality::highResamplingQuality);
		waveformImageWithBorder = waveformImage.createCopy();
		handleBorderDisplay();
	}
	else
		if (m_needsPositionUpdate) {
			m_needsPositionUpdate = false;
			handleBorderDisplay();
		}

	g.drawImageWithin(waveformImageWithBorder, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
}

void VASTMSEGEditor::handleBorderDisplay() {
	//selection
	if (myDataLive == nullptr)
		return;
	if (waveformImage.isNull())
		return;
	if (waveformImageWithBorder.isNull())
		waveformImageWithBorder = waveformImage.createCopy();
	Graphics g(waveformImageWithBorder);
	g.drawImageAt(waveformImage, 0, 0);
	if (m_drawwidth <= 0) return;

	//current pos marker	
	for (int voiceNo = 0; voiceNo < myProcessor->m_pVASTXperience.m_Set.m_uMaxPoly; voiceNo++) {
		if (myData->dispVoicePlaying(voiceNo)) {
			if ((myData->controlPoints.size() - 1) > myData->getDispActiveSegment(voiceNo)) {
					float markerPos = 0.f;
					//array access with myData!
					if (myDataLive->getDispSegmentLengthInSamples(voiceNo) <= 0)
						markerPos = (myData->controlPoints[myData->getDispActiveSegment(voiceNo)].xVal + (myData->controlPoints[myData->getDispActiveSegment(voiceNo) + 1].xVal - myData->controlPoints[myData->getDispActiveSegment(voiceNo)].xVal)) * m_drawwidth;
					else
						markerPos = (myData->controlPoints[myData->getDispActiveSegment(voiceNo)].xVal + (myData->controlPoints[myData->getDispActiveSegment(voiceNo) + 1].xVal - myData->controlPoints[myData->getDispActiveSegment(voiceNo)].xVal) * (float(myDataLive->getDispSamplesSinceSegmentStart(voiceNo)) / float(myDataLive->getDispSegmentLengthInSamples(voiceNo)))) * m_drawwidth;
					if (markerPos == markerPos) { //why can it be NaN?? 
						g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMSEGEditorPosMarker).darker(0.5f).withAlpha(0.4f));
						g.drawLine(markerPos + m_xbounds, m_ybounds, markerPos + m_xbounds, m_drawheight + m_ybounds, 1.0f * m_screenWidthScale * myProcessor->getPluginScaleWidthFactor());
					}

				Line<float> lline = Line<float>(markerPos + m_xbounds, m_ybounds, markerPos + m_xbounds, m_drawheight + m_ybounds);

				PathFlatteningIterator i(m_lastLinePath, AffineTransform(), Path::defaultToleranceForTesting);
				Point<float> intersection;
				while (i.next())
					if (lline.intersects(Line<float>(i.x1, i.y1, i.x2, i.y2), intersection)) {
						g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMSEGEditorPosMarker).withAlpha(0.7f));
						g.fillEllipse(intersection.getX() - m_pointSize * 0.5f, intersection.getY() - m_pointSize * 0.5f, m_pointSize, m_pointSize);
						break;
					}
			}
		}
	}
}

void VASTMSEGEditor::updateContent(bool force)
{
	if (myData == nullptr) return;
	if (myDataLive == nullptr) return;
	if (!force) {
		if ((this->isShowing() == false)) {
			return;
		}
		if (!m_dirty) {
			return;
		}
	}
	if ((getWidth() <= 1) || (getHeight() <= 1))
		return;

	m_screenWidthScale = float(getScreenBounds().getWidth()) / float(getWidth());
	m_screenHeightScale = float(getScreenBounds().getHeight()) / float(getHeight());

	m_pointSize = 8.f * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale;
	m_xbounds = (float)getScreenBounds().getWidth() * 0.02f;
	m_ybounds = (float)getScreenBounds().getHeight() * 0.05f;
	m_drawheight = (float)getScreenBounds().getHeight() - 2 * m_ybounds;
	m_drawwidth = (float)getScreenBounds().getWidth() - 2 * m_xbounds; //check was ybounds here

	waveformImage = Image(Image::RGB, jmax(1, getScreenBounds().getWidth()), jmax(1, getScreenBounds().getHeight()), false);
	waveformImage.clear(waveformImage.getBounds(), myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeBackground));
	waveformImageWithBorder = Image(Image::RGB, waveformImage.getWidth(), waveformImage.getHeight(), true);
	waveformImageWithBorder.clear(waveformImage.getBounds(), myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeBackground));
	Graphics g(waveformImage);

	float fontSize = (m_drawwidth / 1000.f) * 16.f;
	Font myFont = myProcessor->getCurrentVASTLookAndFeel()->getDefaultFont();
	myFont.setSizeAndStyle(fontSize, Font::plain, 1.0f, 0.0f); // no squashing, no kerning
	g.setFont(myFont);

	float prevxPos = 0.0f;
	float prevyPos = 0.0f;
	int numPoints = int(myData->controlPoints.size());	
	Path parea;

	//sync grid
	if (myData->getSynch()) {
		int beats = myData->getTimeBeats();

		float displayPeriod = myData->calcTotalDuration();
		if (displayPeriod > 0.f) {
			float millisPerBeat = myProcessor->m_pVASTXperience.m_Set.getMillisecondsPerBeat();
			float intRatio = myProcessor->m_pVASTXperience.m_Set.getIntervalRatio(beats);
			float stepsPerDisplay = (displayPeriod / (millisPerBeat * intRatio));

			Colour cGridlines = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFilterDisplayGrid).withAlpha(0.6f);
			g.setColour(cGridlines);
			float fGridWidth = m_drawwidth / stepsPerDisplay;
			for (int verti = 0; verti <= int(stepsPerDisplay); verti++) {
				float xpoint = verti;
				int mod = 1 / intRatio;
				if (mod < 1)
					mod = 1;
				(verti % mod == 0.f) ? g.setColour(cGridlines.withAlpha(1.0f)) : g.setColour(cGridlines.withAlpha(0.4f));
				g.drawLine(m_xbounds + xpoint * fGridWidth, 0.f, m_xbounds + xpoint * fGridWidth, m_drawheight, 1.f * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale);
			}
		}
	}

	for (int i = 0; i < numPoints; i++) {
		if (myData->controlPoints[i].isDecay || myData->controlPoints[i].isLoopStart || myData->controlPoints[i].isSustain || myData->controlPoints[i].isMPELift) {
			//edges
			g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMSEGEditorEdge)); 
			float xPos = myData->controlPoints[i].xVal * m_drawwidth;
			g.drawLine(xPos + m_xbounds, 0.0f + m_ybounds, xPos + m_xbounds, m_drawheight + m_ybounds, 2.0f * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale);

			//markers
			float markersize = 18.0f * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale;
			Path p;
			p.startNewSubPath(xPos + m_xbounds, 0.0f + m_ybounds);
			p.lineTo(xPos + m_xbounds, 0.0f + m_ybounds + markersize);
			p.lineTo(xPos + m_xbounds + markersize, 0.0f + m_ybounds + markersize * 0.5f);
			p.lineTo(xPos + m_xbounds, 0.0f + m_ybounds);
			g.fillPath(p);
			juce::String markerChar = "";
			if (myData->controlPoints[i].isLoopStart) markerChar = "L"; else
				if (myData->controlPoints[i].isSustain) markerChar = "S"; else
					if (myData->controlPoints[i].isDecay) markerChar = "D"; else
						if (myData->controlPoints[i].isMPELift) markerChar = "M";
			g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLabelText));
			g.drawText(markerChar, juce::Rectangle<float>(xPos + m_xbounds + 2.0f, 0.0f + m_ybounds + 2.0f, 14.0f * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale, 14.0f * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale), juce::Justification::centredLeft, false);
		}
		
		float xPos = valToScreenX(myData->controlPoints[i].xVal) * m_screenWidthScale;
		float yPos = valToScreenY(myData->controlPoints[i].yVal) * m_screenHeightScale;

		//draw solid curve line
		if (i == 0) { 			
			if (myData->env_mode == VASTMSEGData::unipolar) {
				parea.startNewSubPath(m_xbounds, m_drawheight + m_ybounds);
			}
			else { //plus minus
				parea.startNewSubPath(m_xbounds, m_drawheight *0.5f + m_ybounds); 
			}
			parea.lineTo(xPos, yPos);
		}
		else { // i>0
			Path pline;
			pline.startNewSubPath(prevxPos, prevyPos);

			//g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMSEGEditorEdge)); Colours::lightcyan);

			float xDist = xPos - prevxPos; //sharp line			
			float weightedX = 0.0f;
			float middleY = 0.0f;
			if (prevyPos > yPos) {
				weightedX = prevxPos + xDist * (1.0f - myData->controlPoints[i].curvy);
				middleY = (prevyPos - yPos) * (1.0f - myData->controlPoints[i].curvy) + jmin(prevyPos, yPos);
			}
			else {
				weightedX = xPos - xDist * (1.0f - myData->controlPoints[i].curvy);
				middleY = (yPos - prevyPos) * (1.0f - myData->controlPoints[i].curvy) + jmin(prevyPos, yPos);
			}

			if (myData->controlPoints[i].curvy <= 0.000001) {
				if (prevyPos > yPos) {
					pline.lineTo(xPos, prevyPos);
					parea.lineTo(xPos, prevyPos);
					pline.lineTo(xPos, yPos);
					parea.lineTo(xPos, yPos);
				}
				else {
					pline.lineTo(prevxPos, yPos);
					parea.lineTo(prevxPos, yPos);
					pline.lineTo(xPos, yPos);
					parea.lineTo(xPos, yPos);
				}
			}
			else if (myData->controlPoints[i].curvy >= 0.999999) {
				if (prevyPos > yPos) {
					pline.lineTo(prevxPos, yPos);
					parea.lineTo(prevxPos, yPos);
					pline.lineTo(xPos, yPos);
					parea.lineTo(xPos, yPos);
				}
				else {
					pline.lineTo(xPos, prevyPos);
					parea.lineTo(xPos, prevyPos);
					pline.lineTo(xPos, yPos);
					parea.lineTo(xPos, yPos);
				}
			}
			else {				
				//pline.quadraticTo(weightedX, middleY, xPos, yPos);
				//parea.quadraticTo(weightedX, middleY, xPos, yPos);
				pline.cubicTo(weightedX, middleY, weightedX, middleY, xPos, yPos);
				parea.cubicTo(weightedX, middleY, weightedX, middleY, xPos, yPos);
			}

			//draw solid curve line
			g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMSEGEditorLine)); 
			g.strokePath(pline, PathStrokeType(1.0f * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale));

			//draw curvepoint			
			float middleX = (prevxPos + xPos) * 0.5f; // single line
			juce::Point<float> pointonPath;
			pline.getNearestPoint(juce::Point<float>(middleX - m_xbounds, middleY - m_ybounds), pointonPath);
			Path curvepoint;
			curvepoint.addEllipse(pointonPath.getX() - m_pointSize*.5f, pointonPath.getY() - m_pointSize * .5f, m_pointSize, m_pointSize);

			if (m_numIsOver == i)
				g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMSEGEditorCurvepoint).contrasting(0.3f)); 
			else 
				g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMSEGEditorCurvepoint));
			g.fillPath(curvepoint);

			g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMSEGEditorCurvepointOutline));
			g.strokePath(curvepoint, PathStrokeType(1.0f * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale));
		}

		//draw controlpoint
		if (m_numIsOver == i)
			g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMSEGEditorControlpoint).contrasting(0.3f));
		else 
			g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMSEGEditorControlpoint));
		g.fillEllipse(xPos - m_pointSize * 0.5f, yPos - m_pointSize * 0.5f, m_pointSize, m_pointSize); 

		prevxPos = xPos;
		prevyPos = yPos;
	}

	// Shade under line
	const Colour c1 = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMSEGEditorLine).withAlpha(0.4f);
	const Colour c2 = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMSEGEditorLine).withAlpha(0.0f);
	ColourGradient cg = ColourGradient(c2, juce::Point<float>(m_xbounds, m_drawheight), c1, juce::Point<float>(m_xbounds , m_ybounds), false);

	if (myData->env_mode == VASTMSEGData::unipolar) { //close by returning to starting point
		g.setGradientFill(cg);
		parea.lineTo(m_xbounds + m_drawwidth, m_drawheight + m_ybounds);
	}
	else { //plus minus
		cg.clearColours();
		cg.addColour(0.0, c1);
		cg.addColour(0.5, c2);
		cg.addColour(1.0, c1);
		g.setGradientFill(cg);
		parea.lineTo(m_xbounds + m_drawwidth, m_drawheight *0.5f + m_ybounds);
	}

	parea.closeSubPath();
	g.fillPath(parea);
	m_lastLinePath = parea;

	// Text
	int fontHeight = myFont.getHeightInPoints();
	float x1 = waveformImage.getWidth() / 4 * 0;
	float x2 = waveformImage.getWidth() / 4 * 1;
	float x3 = waveformImage.getWidth() / 4 * 2;
	float x4 = waveformImage.getWidth() / 4 * 3;
	float wi = waveformImage.getWidth() / 4 ;
	if (myData->getSynch()) {
		int beats = myData->getTimeBeats();
		Colour col1 = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLabelText);
		g.setColour(col1.withAlpha(1.f));
		g.drawText(TRANS("Total   ") + String(round(myData->calcTotalDuration())) + String(" ms in ") + String(CVASTParamState::comboBoxValueToTextFunction_TIMEBEATS(beats) + " beats"), juce::Rectangle<int>(x1, waveformImage.getHeight() - fontHeight - 2.f, wi, fontHeight + 2.f), Justification::centred, false);
		g.drawText(TRANS("Decay   ") + String(round(myData->getDecaySteps())), juce::Rectangle<int>(x3, waveformImage.getHeight() - fontHeight - 2.f, wi, fontHeight + 2.f), Justification::centred, false);
		if (!myData->hasAttackPhase())
			g.setColour(col1.withAlpha(0.3f));
		else
			g.setColour(col1.withAlpha(1.f));
		g.drawText(TRANS("Attack  ") + String(round(myData->getAttackSteps())), juce::Rectangle<int>(x2, waveformImage.getHeight() - fontHeight - 2.f, wi, fontHeight + 2.f), Justification::centred, false);
		if (!myData->hasReleasePhase())
			g.setColour(col1.withAlpha(0.3f));
		else
			g.setColour(col1.withAlpha(1.f));
		g.drawText(TRANS("Release ") + String(round(myData->getReleaseSteps())), juce::Rectangle<int>(x4, waveformImage.getHeight() - fontHeight - 2.f, wi, fontHeight + 2.f), Justification::centred, false);
	} else {
		Colour col1 = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLabelText);
		g.setColour(col1.withAlpha(1.f));
		g.drawText(TRANS("Total   ") + String(round(myData->calcTotalDuration())) + String(" ms"), juce::Rectangle<int>(x1, waveformImage.getHeight() - fontHeight - 2.f, wi, fontHeight + 2.f), Justification::centred, false);
		g.drawText(TRANS("Decay   ") + String(round(myData->getDecayTime())) + String(" ms"), juce::Rectangle<int>(x3, waveformImage.getHeight() - fontHeight - 2.f, wi, fontHeight + 2.f), Justification::centred, false);
		if (!myData->hasAttackPhase())
			g.setColour(col1.withAlpha(0.3f));
		else
			g.setColour(col1.withAlpha(1.f));
		g.drawText(TRANS("Attack  ") + String(round(myData->getAttackTime())) + String(" ms"), juce::Rectangle<int>(x2, waveformImage.getHeight() - fontHeight - 2.f, wi, fontHeight + 2.f), Justification::centred, false);
		if (!myData->hasReleasePhase())
			g.setColour(col1.withAlpha(0.3f));
		else
			g.setColour(col1.withAlpha(1.f));
		g.drawText(TRANS("Release ") + String(round(myData->getReleaseTime())) + String(" ms"), juce::Rectangle<int>(x4, waveformImage.getHeight() - fontHeight - 2.f, wi, fontHeight + 2.f), Justification::centred, false);
	}

	handleBorderDisplay();
	m_needsRescale = false;
	m_dirty = false;
	repaint();
}

void VASTMSEGEditor::mouseDown(const MouseEvent & e)
{
	if (myData == nullptr) return;

	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
	// check the mod keys ..
	int numPoints = int(myData->controlPoints.size());
	float mouseY = e.getMouseDownY();
	float mouseX = e.getMouseDownX();

	int numIsClicked = -1;
	enum Clicked {nothing, controlpoint, curvepoint};
	int clicked = Clicked::nothing;
	for (int i = 0; i < numPoints; i++) {
		float pointX = myData->controlPoints[i].xVal;
		float pointY = myData->controlPoints[i].yVal;
		if ((abs(valToScreenX(pointX) - mouseX) <= m_pointSize / m_screenWidthScale) && //clicked on control point
			(abs(valToScreenY(pointY) - mouseY) <= m_pointSize / m_screenWidthScale)) {
			clicked = Clicked::controlpoint;
			numIsClicked = i;
			m_isDraggedCurve = false;
		}
		else {
			if (i > 0) {
				//float middleX = valToScreenX(myData->controlPoints[i - 1].xVal) + (valToScreenX(pointX) - valToScreenX(myData->controlPoints[i - 1].xVal)) * 0.5f;
				//if ((abs(middleX - mouseX) <= m_pointSize)) {//clicked on curve point
				if (((valToScreenX(myData->controlPoints[i - 1].xVal) + m_pointSize / m_screenWidthScale) < mouseX)
					&& (mouseX < (valToScreenX(myData->controlPoints[i].xVal) - m_pointSize / m_screenWidthScale))) {
					clicked = Clicked::curvepoint;
					numIsClicked = i;
					m_isDraggedCurve = true;
					m_isDraggedCurveVal = myData->controlPoints[i].curvy;
				}
			}
		}
	}

	if (e.getNumberOfClicks() > 1) { //double click
		if (modifiers.isLeftButtonDown() || modifiers.isCtrlDown()) { //CTRL?
			if (numIsClicked >= 0) {
				if (m_isDraggedCurve) { //curve point
					double newVal = 0.5f;
					m_isDraggedCurveVal = newVal;
					myData->setCurveValues(numIsClicked, newVal);
					myProcessor->m_parameterState.undoManager->beginNewTransaction();
					updateContent(false);
				}
				else { //control point
					double newxVal = myData->controlPoints[numIsClicked].xVal; //TODO getter?
					double newyVal = 0.0f; //biploar
					if (myData->env_mode == VASTMSEGData::biploar) {
						newyVal = 0.5f;
					}
					myData->setXYValues(numIsClicked, newxVal, newyVal);
					myProcessor->m_parameterState.undoManager->beginNewTransaction(); 
					updateContent(false);
				}
			}
		}
	}

	if (modifiers.isPopupMenu()) {
		PopupMenu mainMenu;
		mainMenu.setLookAndFeel(&this->getLookAndFeel());
		if (clicked == Clicked::curvepoint) { //add controlpoint
			mainMenu.addSectionHeader(TRANS("Control point"));
			mainMenu.addItem(1, TRANS("Add control point"));
			//mainMenu.addItem(2, "Delete control point");			
			//mainMenu.addSeparator();
			//mainMenu.addSectionHeader(TRANS("Curve style"));
			//mainMenu.addItem(8, TRANS("Single curve"));
			//mainMenu.addItem(9, TRANS("Half sine"));
			//mainMenu.addSeparator();
			//mainMenu.addSectionHeader("ADSR and loop section");
			//mainMenu.addItem(20, "Decay point");
			//mainMenu.addItem(21, "Loop start");
			//mainMenu.addItem(21, "Sustain (loop end)");
		}
		else if (clicked == Clicked::controlpoint) { // controlpoint clicked
			mainMenu.addSectionHeader(TRANS("Control point"));
			//mainMenu.addItem(1, "Add control point");
			mainMenu.addItem(2, TRANS("Delete control point"));
			mainMenu.addSeparator();
			//mainMenu.addSectionHeader(TRANS("Curve style"));
			//mainMenu.addItem(8, TRANS("Single curve"));
			//mainMenu.addItem(9, "Half sine");
			mainMenu.addSeparator();
			mainMenu.addItem(12, TRANS("Type in x value"));
			mainMenu.addItem(13, TRANS("Type in y value"));
			mainMenu.addSeparator();
			mainMenu.addSectionHeader(TRANS("ADSR and loop section"));
			mainMenu.addItem(20, TRANS("Toggle decay point (attack phase)"));
			mainMenu.addItem(21, TRANS("Toggle loop start"));
			mainMenu.addItem(22, TRANS("Toggle sustain (loop end / release phase)"));
			mainMenu.addItem(23, TRANS("Toggle MPE lift (release velocity)"));
			mainMenu.addItem(24, TRANS("Clear loop points"));
		}
		mainMenu.addSeparator();
		mainMenu.addSectionHeader(TRANS("Copy & Paste"));
		mainMenu.addItem(30, TRANS("Copy MSEG"));
		mainMenu.addItem(31, TRANS("Paste MSEG"));

		mainMenu.showMenuAsync(PopupMenu::Options().withTargetComponent(this).withTargetScreenArea(juce::Rectangle<int>{}.withPosition(Desktop::getMousePosition())),
			juce::ModalCallbackFunction::create([this, numPoints, mouseX, mouseY, numIsClicked](int result) {
			if (result == 1) {
				int i = 0;
				while ((i < numPoints) && (valToScreenX(myData->controlPoints[i].xVal) < mouseX)) {
					i++;
				}
				if (i < numPoints) {
					VASTMSEGData::ControlPoint newpoint;
					myData->insertPointUI(i, newpoint);
					myData->setXYValues(i, screenToValX(mouseX), screenToValY(mouseY));
					updateContent(false);
				}
			}
			else if (result == 2) {
				myData->removePointUI(numIsClicked);
				updateContent(false);
			}
			else if (result == 8) {
				myData->setCurveStyle(numIsClicked, VASTMSEGData::CurveStyle::SingleCurve);
				updateContent(false);
			}
			else if (result == 9) {
				myData->setCurveStyle(numIsClicked, VASTMSEGData::CurveStyle::HalfSine);
				updateContent(false);
			}
			else if (result == 12) {
				//x value
				float xval = myData->controlPoints[numIsClicked].xVal;
				std::unique_ptr<VASTPositionEditor> l_veditor = std::make_unique<VASTPositionEditor>(myProcessor, xval, this, true, numIsClicked); //OK will be owned by the cb
				VASTAudioProcessorEditor* myEditor = (VASTAudioProcessorEditor*)myProcessor->getActiveEditor();

				l_veditor->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
				l_veditor->setSize(300.f * myProcessor->getPluginScaleWidthFactor(), 30.f * myProcessor->getPluginScaleHeightFactor());
				l_veditor->setOpaque(true);

				juce::Rectangle<int> targetrect = juce::Rectangle<int>(m_xbounds + (this->getBounds().getWidth() - 2 * m_xbounds) * myData->controlPoints[numIsClicked].xVal, m_ybounds + (this->getBounds().getHeight() - 2 * m_ybounds) * (1.0 - myData->controlPoints[numIsClicked].yVal), 1, 1);

				l_veditor->setText(String(xval));

				juce::CallOutBox::launchAsynchronously(std::move(l_veditor), targetrect, this);

				updateContent(false);
			}
			else if (result == 13) {
				//y value
				float yval = myData->controlPoints[numIsClicked].yVal;
				std::unique_ptr<VASTPositionEditor> l_veditor = std::make_unique<VASTPositionEditor>(myProcessor, yval, this, false, numIsClicked); //OK will be owned by the cb
				VASTAudioProcessorEditor* myEditor = (VASTAudioProcessorEditor*)myProcessor->getActiveEditor();

				l_veditor->setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());
				l_veditor->setSize(300.f * myProcessor->getPluginScaleWidthFactor(), 30.f * myProcessor->getPluginScaleHeightFactor());
				l_veditor->setOpaque(true);

				juce::Rectangle<int> targetrect = juce::Rectangle<int>(m_xbounds + (this->getBounds().getWidth() - 2 * m_xbounds) * myData->controlPoints[numIsClicked].xVal, m_ybounds + (this->getBounds().getHeight() - 2 * m_ybounds) * (1.0 - myData->controlPoints[numIsClicked].yVal), 1, 1);

				l_veditor->setText(String(yval));
				juce::CallOutBox::launchAsynchronously(std::move(l_veditor), targetrect, this);

				updateContent(false);
			}
			else if (result == 20) {
				myData->toggleDecayPoint(numIsClicked);
				updateContent(false);
			}
			else if (result == 21) {
				myData->toggleLoopStart(numIsClicked);
				updateContent(false);
			}
			else if (result == 22) {
				myData->toggleSustainPoint(numIsClicked);
				updateContent(false);
			}
			else if (result == 23) {
				myData->toggleMPELift(numIsClicked);
				updateContent(false);
			}
			else if (result == 24) {
				myData->clearLoopPoints();
				updateContent(false);
			}
			else if (result == 30) {
				//copy MSEG
				myProcessor->m_pVASTXperience.m_Set.m_MSEGCopyBuffer.copyDataFrom(*myData);
			}
			else if (result == 31) {
				//pate MSEG
				myData->copyDataFrom(myProcessor->m_pVASTXperience.m_Set.m_MSEGCopyBuffer);
				updateContent(false);
			}
		}));
	}		
	else if (modifiers.isLeftButtonDown() || modifiers.isCtrlDown()) {
		if (clicked != Clicked::nothing) {
			myProcessor->m_parameterState.undoManager->beginNewTransaction();
			m_isDragged = numIsClicked;
			m_isDraggedCurve = clicked == Clicked::controlpoint ? false : true;
		}
	}
}

void VASTMSEGEditor::mouseMove(const MouseEvent& e) {	

	int numPoints = int(myData->controlPoints.size());
	float mouseY = e.getMouseDownY();
	float mouseX = e.getMouseDownX();

	for (int i = 0; i < numPoints; i++) {
		float pointX = myData->controlPoints[i].xVal;
		float pointY = myData->controlPoints[i].yVal;
		if ((abs(valToScreenX(pointX) - mouseX) <= m_pointSize) && //clicked on control point
			(abs(valToScreenY(pointY) - mouseY) <= m_pointSize)) {
			m_numIsOver = i;
		}
		else {
			if (i > 0) {
				if (((valToScreenX(myData->controlPoints[i - 1].xVal) + m_pointSize) < mouseX)
					&& (mouseX < (valToScreenX(myData->controlPoints[i].xVal) - m_pointSize))) {
					m_numIsOver = i;
				}
			}
		}
	}

	if (m_numIsOver != -1) {
		m_dirty = true;
		updateContent(false);
	}
}

void VASTMSEGEditor::mouseExit(const MouseEvent& e) {
	m_numIsOver = -1;
	m_dirty = true;
	updateContent(false);
}

void VASTMSEGEditor::passBackPopupResult(float resultVal, bool isX, int numIsClicked) {
	if (isX)
		myData->setXYValues(numIsClicked, resultVal, myData->controlPoints[numIsClicked].yVal);
	else 
		myData->setXYValues(numIsClicked, myData->controlPoints[numIsClicked].xVal, resultVal);
}

void VASTMSEGEditor::mouseUp(const MouseEvent & e) {
	m_isDragged = -1;
	m_isDraggedCurve = false;
	m_isDraggedCurveVal = 0.5f;
}

void VASTMSEGEditor::mouseDrag(const MouseEvent & e)
{
	if (myData == nullptr) return;

	// check the mod keys ..
	if (m_isDragged>=0)	{
		float mouseY = e.getMouseDownY();
		float mouseX = e.getMouseDownX();

		if (!m_isDraggedCurve) { // control point
			double newxVal = screenToValX(mouseX + e.getDistanceFromDragStartX());
			double newyVal = screenToValY(mouseY + e.getDistanceFromDragStartY());
			myData->setXYValues(m_isDragged, newxVal, newyVal);
		}
		else { // curve point
			double newVal = m_isDraggedCurveVal - 2.f * e.getDistanceFromDragStartY() / getHeight();
			myData->setCurveValues(m_isDragged, newVal);
		}
	}
	updateContent(false);
}

void VASTMSEGEditor::startAutoUpdate() {
	startTimer(35);
}
void VASTMSEGEditor::stopAutoUpdate() {
	stopTimer();
}

void VASTMSEGEditor::timerCallback() {
	if (myDataLive == nullptr) return;

	if (myData->getADSRUpdated()) {
		VASTAudioProcessor* processor = ((VASTAudioProcessor*)myProcessor);

		for (int i = 0; i < processor->m_mapParameterNameToControl.size(); i++) {
			VASTParameterSlider* lslider = dynamic_cast<VASTParameterSlider*>(processor->m_mapParameterNameToControl[i]);
            if (lslider == nullptr)
                continue;
			if (lslider->getComponentID().equalsIgnoreCase("m_fSustainLevel_MSEG" + String(myData->m_msegNo + 1)))
				lslider->setValue(myData->getSustainLevel() * 100.f, juce::NotificationType::dontSendNotification);

			if (myData->getSynch() == false) {
				if (lslider->getComponentID().equalsIgnoreCase("m_fAttackTime_MSEG" + String(myData->m_msegNo + 1)))
					lslider->setValue(myData->getAttackTime(), juce::NotificationType::dontSendNotification);
				else if (lslider->getComponentID().equalsIgnoreCase("m_fDecayTime_MSEG" + String(myData->m_msegNo + 1)))
					lslider->setValue(myData->getDecayTime(), juce::NotificationType::dontSendNotification);
				else if (lslider->getComponentID().equalsIgnoreCase("m_fReleaseTime_MSEG" + String(myData->m_msegNo + 1)))
					lslider->setValue(myData->getReleaseTime(), juce::NotificationType::dontSendNotification);
			}
			else {
				if (lslider->getComponentID().equalsIgnoreCase("m_fAttackSteps_MSEG" + String(myData->m_msegNo + 1)))
					lslider->setValue(myData->calcStepsFromTime(myData->getAttackTime(), &myProcessor->m_pVASTXperience.m_Set) , juce::NotificationType::dontSendNotification);
				else if (lslider->getComponentID().equalsIgnoreCase("m_fDecaySteps_MSEG" + String(myData->m_msegNo + 1)))
					lslider->setValue(myData->calcStepsFromTime(myData->getDecayTime(), &myProcessor->m_pVASTXperience.m_Set), juce::NotificationType::dontSendNotification);
				else if (lslider->getComponentID().equalsIgnoreCase("m_fReleaseSteps_MSEG" + String(myData->m_msegNo + 1)))
					lslider->setValue(myData->calcStepsFromTime(myData->getReleaseTime(), &myProcessor->m_pVASTXperience.m_Set), juce::NotificationType::dontSendNotification);

				//CHECK
				myData->m_fAttackSteps = myData->calcStepsFromTime(myData->getAttackTime(), &myProcessor->m_pVASTXperience.m_Set);
				myData->m_fDecaySteps = myData->calcStepsFromTime(myData->getDecayTime(), &myProcessor->m_pVASTXperience.m_Set);
				myData->m_fReleaseSteps = myData->calcStepsFromTime(myData->getReleaseTime(), &myProcessor->m_pVASTXperience.m_Set);
			}
		}

		myData->resetADSRUpdated();
		m_dirty = true;
	}

	if (myDataLive->getAndClearUIUpdateFlag()) {
		m_dirty = true;
	}

	if (myDataLive->getAndClearPositionUpdateFlag()) {
		m_needsPositionUpdate = true;
		repaint();

		/*
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t1).count();
		t1 = std::chrono::high_resolution_clock::now();
		VDBG("Last update ms " + String(duration));
		*/

	}

	if (m_dirty)
		updateContent(false);
}
