/*
VAST Dynamics

How to calculate quadratic beziers
x from 0 to 1: 
y = (1-x)^2  * startval  + 2x(1-x) * intermediateval + x^2 * endval

http://slideplayer.com/slide/6554987/

*/

#include "VASTStepSeqEditor.h"
#include "../../Engine/VASTEngineHeader.h"
#include "../VASTAudioProcessor.h"
#include "../../Engine/VASTMSEGData.h"
#include "../VASTControls/VASTPositionEditor.h"

VASTStepSeqEditor::VASTStepSeqEditor(AudioProcessor* processor, VASTMSEGData* data, VASTMSEGData* datalive)
	: myProcessor((VASTAudioProcessor*)processor), myData(data), myDataLive(datalive)
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

VASTStepSeqEditor::~VASTStepSeqEditor() {
	this->setLookAndFeel(nullptr);
	stopTimer();
}

void VASTStepSeqEditor::resized() {
	m_needsRescale = true;
	m_dirty = true;
}

void VASTStepSeqEditor::lookAndFeelChanged() {
	if (myProcessor->isCurrentEditorInitialized())
		updateContent(true);
}


void VASTStepSeqEditor::paint(Graphics& g)
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

float VASTStepSeqEditor::screenToValX(float screenX) {
	float xVal = (screenX - m_xbounds / m_screenWidthScale) / (m_drawwidth / m_screenWidthScale);
	return jlimit(0.f, 1.f, xVal);
}

float VASTStepSeqEditor::screenToValY(float screenY) {
	float yVal = 1.0f - (screenY - m_ybounds / m_screenHeightScale) / (m_drawheight / m_screenHeightScale);
	return jlimit(0.f, 1.f, yVal);
}

float VASTStepSeqEditor::valToScreenX(float xVal) {
	float xPos = xVal * (m_drawwidth / m_screenWidthScale) + m_xbounds / m_screenWidthScale;
	return xPos;
}

float VASTStepSeqEditor::valToScreenY(float yVal) {
	float yPos = (1.0f - yVal) *  (m_drawheight / m_screenHeightScale) + m_ybounds / m_screenHeightScale;
	return yPos;
}

void VASTStepSeqEditor::handleBorderDisplay() {
	//selection
	if (myDataLive == nullptr)
		return;
	if (waveformImage.isNull())
		return;
	if (waveformImageWithBorder.isNull())
		waveformImageWithBorder = waveformImage.createCopy();
	Graphics g(waveformImageWithBorder);
	g.drawImageAt(waveformImage, 0, 0);

	if (m_drawwidth <= 0) 
		return;

	//current pos marker
	int voiceNo = 0; //stepSeq Enveloped is dtored as voice 0 
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

		float pointSize = 8.f * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale;
		PathFlatteningIterator i(m_lastLinePath, AffineTransform(), Path::defaultToleranceForTesting);
		Point<float> intersection;
		while (i.next())
			if (lline.intersects(Line<float>(i.x1, i.y1, i.x2, i.y2), intersection)) {
				g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colMSEGEditorPosMarker).withAlpha(0.7f));
				g.fillEllipse(intersection.getX() - pointSize * 0.5f, intersection.getY() - pointSize * 0.5f, pointSize, pointSize);
				break;
			}
	}	
}

inline VASTMSEGData& VASTStepSeqEditor::getData() { return *myData; }

void VASTStepSeqEditor::updateContent(bool force)
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

	m_xbounds = (float)getScreenBounds().getWidth() * 0.02f;
	m_ybounds = (float)getScreenBounds().getHeight() * 0.05f;
	m_drawheight = (float)getScreenBounds().getHeight() - 2 * m_ybounds;
	m_drawwidth = (float)getScreenBounds().getWidth() - 2 * m_xbounds; //check was ybounds before

	waveformImage = Image(Image::RGB, jmax(1, getScreenBounds().getWidth()), jmax(1, getScreenBounds().getHeight()), false);
	waveformImage.clear(waveformImage.getBounds(), myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeBackground));
	waveformImageWithBorder = Image(Image::RGB, waveformImage.getWidth(), waveformImage.getHeight(), true);
	waveformImageWithBorder.clear(waveformImage.getBounds(), myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeBackground));
	Graphics g(waveformImage);

	float fontSize = (m_drawwidth / 1000.f) * 16.f;
	Font myFont = myProcessor->getCurrentVASTLookAndFeel()->getDefaultFont();
	myFont.setSizeAndStyle(fontSize, Font::plain, 1.0f, 0.0f); // no squashing, no kerning
	g.setFont(myFont);
	   
	bool sync = false;
	int beats = 1;
	float speed = 1.f;
	if (myData->m_stepSeqNo == 0) {
		sync = *myProcessor->m_pVASTXperience.m_Set.m_State->m_bStepSeqSynch_STEPSEQ1;
		beats = *myProcessor->m_pVASTXperience.m_Set.m_State->m_uStepSeqTimeBeats_STEPSEQ1;
		speed = *myProcessor->m_pVASTXperience.m_Set.m_State->m_fStepSeqSpeed_STEPSEQ1;
	}
	else if (myData->m_stepSeqNo == 1) {
		sync = *myProcessor->m_pVASTXperience.m_Set.m_State->m_bStepSeqSynch_STEPSEQ2;
		beats = *myProcessor->m_pVASTXperience.m_Set.m_State->m_uStepSeqTimeBeats_STEPSEQ2;
		speed = *myProcessor->m_pVASTXperience.m_Set.m_State->m_fStepSeqSpeed_STEPSEQ2;
	}
	else if (myData->m_stepSeqNo == 2) {
		sync = *myProcessor->m_pVASTXperience.m_Set.m_State->m_bStepSeqSynch_STEPSEQ3;
		beats = *myProcessor->m_pVASTXperience.m_Set.m_State->m_uStepSeqTimeBeats_STEPSEQ3;
		speed = *myProcessor->m_pVASTXperience.m_Set.m_State->m_fStepSeqSpeed_STEPSEQ3;
	}

	float lIntervalTime = speed / 1000.f;
	float displayPeriod = myData->getStepSeqSteps() * lIntervalTime;
	if (sync) {
		lIntervalTime = myProcessor->m_pVASTXperience.m_Set.getIntervalTimeFromDAWBeats(beats) / 1000.f;
		displayPeriod = myData->getStepSeqSteps() * myProcessor->m_pVASTXperience.m_Set.getIntervalRatio(beats) * lIntervalTime;
	}
	float beatsPerDisplay = displayPeriod / lIntervalTime;
	
	//gridlines
	if (sync) {
		Colour cGridlines = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFilterDisplayGrid).withAlpha(0.6f);
		g.setColour(cGridlines);
		float fGridWidth = m_drawwidth / beatsPerDisplay;
		for (int verti = 0; verti <= int(beatsPerDisplay); verti++) {
			float xpoint = verti;
			(verti % 4 == 0.f) ? g.setColour(cGridlines.withAlpha(1.0f)) : g.setColour(cGridlines.withAlpha(0.4f));
			g.drawLine(m_xbounds + xpoint * fGridWidth, m_ybounds, m_xbounds + xpoint * fGridWidth, m_drawheight, 1.f);
		}
	}
	else {
		Colour cGridlines = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colFilterDisplayGrid).withAlpha(0.6f);
		g.setColour(cGridlines);
		float fGridWidth = m_drawwidth / displayPeriod;
		for (int verti = 0; verti <= int(displayPeriod); verti++) {
			float xpoint = verti;
			g.drawLine(m_xbounds + xpoint * fGridWidth, m_ybounds, m_xbounds + xpoint * fGridWidth, m_drawheight, 1.f);
		}
	}


	float prevxPos = 0.0f;
	float prevyPos = 0.0f;
	int numPoints = int(myData->controlPoints.size());	
	Path parea;

	for (int i = 0; i < numPoints; i++) {
		if (myData->controlPoints[i].isDecay || myData->controlPoints[i].isLoopStart || myData->controlPoints[i].isSustain) {
			//edges
			g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colStepSeqEditorEdges)); 
			float xPos = myData->controlPoints[i].xVal * m_drawwidth;
			g.drawLine(xPos + m_xbounds, m_ybounds, xPos + m_xbounds, m_drawheight + m_ybounds, 2.0f * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale);

			//markers
			float markersize = 18.0f * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale;
			Path p;
			p.startNewSubPath(xPos + m_xbounds, m_ybounds);
			p.lineTo(xPos + m_xbounds, m_ybounds + markersize);
			p.lineTo(xPos + m_xbounds + markersize, m_ybounds + markersize * 0.5f);
			p.lineTo(xPos + m_xbounds, m_ybounds);
			g.fillPath(p);
			juce::String markerChar = "";
			if (myData->controlPoints[i].isLoopStart) markerChar = "L"; else 
				if (myData->controlPoints[i].isSustain) markerChar = "S"; else
					if (myData->controlPoints[i].isDecay) markerChar = "D"; 
			g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colStepSeqEditorText));
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

			//g.setColour(Colours::lightcyan);
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
			g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colStepSeqEditorLine));
			g.strokePath(pline, PathStrokeType(1.0f * myProcessor->getPluginScaleWidthFactor() * m_screenWidthScale));

			/*
			//draw curvepoint			
			float middleX = (prevxPos + xPos) * 0.5f; // single line
			juce::Point<float> pointonPath;
			pline.getNearestPoint(juce::Point<float>(middleX - m_xbounds, middleY - m_ybounds), pointonPath);
			parea.getNearestPoint(juce::Point<float>(middleX - m_xbounds, middleY - m_ybounds), pointonPath);
			//			g.fillEllipse(weightedX - m_xbounds, middleY - m_ybounds, m_pointSize, m_pointSize);			
			//g.fillEllipse(pointonPath.getX() - m_xbounds, pointonPath.getY() - m_ybounds, m_pointSize, m_pointSize);
			Path curvepoint;
			curvepoint.addEllipse(pointonPath.getX() - m_xbounds, pointonPath.getY() - m_ybounds, m_pointSize, m_pointSize);
			g.setColour(Colour(0xff002a32)); //background color
			g.fillPath(curvepoint);
			g.setColour(Colours::lightcyan);
			g.strokePath(curvepoint, PathStrokeType(1.0f));
			*/
		}

		/*
		//draw controlpoint
		g.setColour(Colours::yellow.withAlpha(0.8f));
		g.fillEllipse(xPos - m_pointSize * 0.5f, yPos - m_pointSize * 0.5f, m_pointSize, m_pointSize); 
		*/

		prevxPos = xPos;
		prevyPos = yPos;
	}

	// Shade under line
	const Colour c1 = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colStepSeqEditorLine).withAlpha(0.5f);
	const Colour c2 = myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colStepSeqEditorLine).withAlpha(0.0f);
	if (myData->env_mode == VASTMSEGData::unipolar) {
		g.setGradientFill(ColourGradient(c2, m_xbounds, m_ybounds + m_drawheight, c1, m_xbounds, m_ybounds, false));
		parea.lineTo(m_xbounds + m_drawwidth, m_drawheight + m_ybounds);
	}
	else { //plus minus
		g.setGradientFill(ColourGradient(c2, m_xbounds, m_ybounds + m_drawheight, c1, m_xbounds, m_ybounds, false));
		parea.lineTo(m_xbounds + m_drawwidth, m_drawheight *0.5f + m_ybounds);
	}	
	parea.closeSubPath();
	g.fillPath(parea);
	m_lastLinePath = parea;

	// Text
	int fontHeight = myFont.getHeightInPoints();
	g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colLabelText));
	if (sync)
		g.drawText(String(int(beatsPerDisplay + 0.5f)) + " beats", juce::Rectangle<int>(waveformImage.getWidth() * 0.8f, waveformImage.getHeight() - fontHeight - 2.f, waveformImage.getWidth() * 0.2f - m_xbounds, fontHeight + 2.f), Justification::centredRight, false);
	else
		g.drawText(String(int(displayPeriod * 1000.f)) + " ms", juce::Rectangle<int>(waveformImage.getWidth() * 0.8f, waveformImage.getHeight() - fontHeight - 2.f, waveformImage.getWidth() * 0.2f - m_xbounds, fontHeight + 2.f), Justification::centredRight, false);

	handleBorderDisplay();
	m_needsRescale = false;
	m_dirty = false;
	repaint();
}

void VASTStepSeqEditor::mouseDown(const MouseEvent & e)
{
	if (myData == nullptr) return;

	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
	// check the mod keys ..
	int numSteps = myData->getStepSeqSteps();
	float mouseY = e.getMouseDownY();
	float mouseX = e.getMouseDownX();

	int numIsClicked = -1;
	
	float stepWidth = (m_drawwidth / m_screenWidthScale) / numSteps;
	for (int i = 0; i < numSteps; i++) {
		if ((i * stepWidth < mouseX) && (mouseX < (i + 1) * stepWidth)) {
			float fBar = screenToValY(mouseY);
			if (modifiers.isLeftButtonDown())
				myData->stepSeqChangeBar(i, fBar);
			numIsClicked = i;
			repaint();
			break;
		}
	}
	
	if (modifiers.isLeftButtonDown() || modifiers.isRightButtonDown() || modifiers.isCtrlDown()) {
		m_isDragged = numIsClicked;
	}
	if (modifiers.isPopupMenu()) {
		PopupMenu mainMenu;
		mainMenu.setLookAndFeel(&this->getLookAndFeel());
		mainMenu.addItem(1, TRANS("Type in y value"));

		mainMenu.showMenuAsync(PopupMenu::Options().withTargetComponent(this).withTargetScreenArea(juce::Rectangle<int>{}.withPosition(Desktop::getMousePosition())),
			juce::ModalCallbackFunction::create([this, numIsClicked, mouseX, mouseY](int result) {
			if (result == 1) {
				//y value
				float yval = myData->getStepSeqBar(numIsClicked);
				std::unique_ptr<VASTPositionEditor> l_veditor = std::make_unique<VASTPositionEditor>(myProcessor, yval, this, false, numIsClicked); //OK will be owned by the cb
				l_veditor->setLookAndFeel(myProcessor->getCurrentVASTLookAndFeel());
				l_veditor->setSize(300.f * myProcessor->getPluginScaleWidthFactor(), 30.f * myProcessor->getPluginScaleHeightFactor());
				l_veditor->setOpaque(true);

				juce::Rectangle<int> targetrect = juce::Rectangle<int>(mouseX, mouseY, 1, 1);

				l_veditor->setText(String(yval));
				juce::CallOutBox::launchAsynchronously(std::move(l_veditor), targetrect, this);

				updateContent(false);
			}
		}));
	}
}

void VASTStepSeqEditor::passBackPopupResult(float resultVal, int numIsClicked) {
	myData->stepSeqChangeBar(numIsClicked, resultVal);
}

void VASTStepSeqEditor::mouseMove(const MouseEvent& e) {
	updateContent(true);

	int numSteps = myData->getStepSeqSteps();
	float mouseX = e.getMouseDownX();
	float stepWidth = (m_drawwidth / m_screenWidthScale) / numSteps;
	Graphics g(waveformImage);
	for (int i = 0; i < numSteps; i++) {
		if ((i * stepWidth < mouseX) && (mouseX < (i + 1) * stepWidth)) {
			g.setColour(myProcessor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeSelection).darker(0.7f));
			g.drawRect(juce::Rectangle<float>(m_xbounds + i * stepWidth * m_screenWidthScale, m_ybounds, stepWidth * m_screenWidthScale, m_drawheight));
		}
	}
	repaint();
}

void VASTStepSeqEditor::mouseExit(const MouseEvent& e) {
	updateContent(true);
}

void VASTStepSeqEditor::mouseUp(const MouseEvent & e) {
	m_isDragged = -1;
	m_isDraggedCurve = false;
}

void VASTStepSeqEditor::mouseDrag(const MouseEvent & e)
{
	if (myData == nullptr) return;

	//ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
	// check the mod keys ..
	int numSteps = myData->getStepSeqSteps();
	if (m_isDragged>=0)	{
		float mouseY = e.getMouseDownY();
		float mouseX = e.getMouseDownX();
		float pointX = mouseX + e.getDistanceFromDragStartX();;
		float pointY = mouseY + e.getDistanceFromDragStartY();;

		float stepWidth = (m_drawwidth / m_screenWidthScale) / numSteps;
		for (int i = 0; i < numSteps; i++) {
			if ((i * stepWidth < pointX) && (pointX < (i + 1) * stepWidth)) {
				float fBar = screenToValY(pointY);
				myData->stepSeqChangeBar(i, fBar);
				break;
			}
		}
	}
	repaint();
}

void VASTStepSeqEditor::startAutoUpdate() {
	startTimer(35); //50 milliseconds
}

void VASTStepSeqEditor::stopAutoUpdate() {
	stopTimer();
}

void VASTStepSeqEditor::timerCallback() {
	if (myDataLive == nullptr) return;
	if (myDataLive->getAndClearUIUpdateFlag()) {
		m_dirty = true;
	}

	if (myDataLive->getAndClearPositionUpdateFlag()) {
		m_needsPositionUpdate = true;
		repaint();
	}

	if (m_dirty)
		updateContent(false);
}

String VASTStepSeqEditor::getTooltip()
{
    return TRANS("Left mouse drag up/down: choose step height / velocity. Right click: enter value manually.");
}
