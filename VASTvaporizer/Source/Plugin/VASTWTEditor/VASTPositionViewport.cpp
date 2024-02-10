/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTPositionViewport.h"
#include "../VASTAudioProcessorEditor.h"
#include "../VASTAudioProcessor.h"
#include "../VASTWaveTableEditorComponent.h"
#include "../../Engine/VASTSingleNote.h"

VASTPositionViewport::VASTPositionViewport()
{
	myProcessor = nullptr;
	myEditor = nullptr;
	myWtEditor = nullptr;
	setOpaque(true);

	setComponentID("freqDomainViewport");
	
	setPaintingIsUnclipped(true);

	setViewportIgnoreDragFlag(true);

	setSize(1500, 38);

	m_scaling = 8;
	m_wtImageWidth = 60;
	m_wtImageHeight = 38;
	m_Offset = 1;
	m_ImageTotalWidth = m_wtImageWidth + 2 * m_Offset;
	m_mouseover_arrayidx = -1;

	//myFont.setTypefaceName("Open Sans"); //bold 1-2, regular 2
	//myFont.setDefaultMinimumHorizontalScaleFactor(1.0);
	//myFont.setSizeAndStyle(14.f, Font::bold, 1.0f, 0.0f); // no squashing, no kerning

	//this->addKeyListener(this);
}

VASTPositionViewport::~VASTPositionViewport() {
	this->setLookAndFeel(nullptr);
	//this->removeKeyListener(this);
	stopTimer();
}

void VASTPositionViewport::setZoomFactor(int zoom) {
	m_scaling = zoom;
	m_dirty = true;

    m_screenWidthScale = 1.f;
    m_screenHeightScale = 1.f;
    if (myWtEditor->getEditorView()->c_viewportPositions->getWidth() > 0)
        m_screenWidthScale = float(myWtEditor->getEditorView()->c_viewportPositions->getScreenBounds().getWidth()) / float(myWtEditor->getEditorView()->c_viewportPositions->getWidth());
    if (myWtEditor->getEditorView()->c_viewportPositions->getHeight() > 0)
        m_screenHeightScale = float(myWtEditor->getEditorView()->c_viewportPositions->getScreenBounds().getHeight()) / float(myWtEditor->getEditorView()->c_viewportPositions->getHeight());

	juce::Rectangle<int> lVisibleArea = myWtEditor->getEditorView()->c_viewportPositions->getViewArea();
	m_wtImageWidth = (lVisibleArea.getHeight() * m_screenHeightScale - 2.0f) / 6.f * m_scaling;
	m_wtImageHeight = lVisibleArea.getHeight() * m_screenHeightScale;
	m_ImageTotalWidth = m_wtImageWidth + 2 * m_Offset;
	int width = (m_ImageTotalWidth + m_Offset) * (C_MAX_NUM_POSITIONS + 5);
	setSize(width / m_screenWidthScale, lVisibleArea.getHeight());
}

void VASTPositionViewport::setEditor(VASTAudioProcessorEditor* editor) { myEditor = editor; }

void VASTPositionViewport::setProcessor(VASTAudioProcessor* processor) { myProcessor = processor; }

void VASTPositionViewport::setWTEditor(VASTWaveTableEditorComponent* wtEditor) { myWtEditor = wtEditor; }

void VASTPositionViewport::resized()
{	
	if (myWtEditor == nullptr) return;
	if (!myWtEditor->c_waveTableEditorView) return;
	if (!myWtEditor->c_waveTableEditorView->c_viewportPositions) return;

	m_screenWidthScale = float(myWtEditor->getEditorView()->c_viewportPositions->getScreenBounds().getWidth()) / float(myWtEditor->getEditorView()->c_viewportPositions->getWidth());
	m_screenHeightScale = float(myWtEditor->getEditorView()->c_viewportPositions->getScreenBounds().getHeight()) / float(myWtEditor->getEditorView()->c_viewportPositions->getHeight());

	juce::Rectangle<int> lVisibleArea = myWtEditor->getEditorView()->c_viewportPositions->getViewArea();
	m_wtImageWidth = (lVisibleArea.getHeight() * m_screenHeightScale - 2.0f) / 6.f * m_scaling;
	m_wtImageHeight = lVisibleArea.getHeight() * m_screenHeightScale;
	m_ImageTotalWidth = m_wtImageWidth + 2 * m_Offset;

	//waveformImage = waveformImage.rescaled(jmax(1, getWidth()), jmax(1, getHeight()), juce::Graphics::ResamplingQuality::lowResamplingQuality);

	m_dirty = true;
}

void VASTPositionViewport::paint(Graphics& g)
{
	//g.drawImageAt(waveformImage, 0, 0);
	juce::Rectangle<int> lVisibleArea = myWtEditor->getEditorView()->c_viewportPositions->getViewArea();
	g.drawImageWithin(waveformImage, lVisibleArea.getX(), 0, lVisibleArea.getWidth(), lVisibleArea.getHeight(), juce::RectanglePlacement::stretchToFit);
}

void VASTPositionViewport::updateContentAsync() {
	m_dirty = true;
}

void VASTPositionViewport::updateContent(bool force) {
	if (!force) {
		if (this->isShowing() == false)
			return;
	}
	if ((getWidth() <= 1) || (getHeight() <= 1))
		return;

	if (m_ImageTotalWidth <= 0) return;
	if (myProcessor == nullptr) return;
	if (myWtEditor == nullptr) return;
	if (!myWtEditor->c_waveTableEditorView) return;
	if (!myWtEditor->getEditorView()->c_viewportPositions) return;

	if (!(force || m_dirty))
		//if (!(myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->getOscBank()].getAndClearSoftChangedFlagPos() || myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->getOscBank()].isChanged())) 
		if (!(myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->getOscBank()].getAndClearSoftChangedFlagPos()) )
			return;
	m_dirty = false;
	
	//VDBG("VASTPositionViewport::updateContent");
	if (!approximatelyEqual(m_screenWidthScale, float(myWtEditor->getEditorView()->c_viewportPositions->getScreenBounds().getWidth()) / float(myWtEditor->getEditorView()->c_viewportPositions->getWidth())))
		setZoomFactor(m_scaling);

	myWtEditor->updateHeaderSelectionLabel(); //right place here?

	std::shared_ptr<CVASTWaveTable> wavetable = myWtEditor->getBankWavetable();
    if (wavetable->m_isBeingUpdated.load() == true) //safety
        return;
    
	//int newWidth = (m_ImageTotalWidth + m_Offset) * (wavetable->getNumPositions() + 1);
	//setSize(newWidth, getHeight());

	juce::Rectangle<int> lVisibleArea = myWtEditor->getEditorView()->c_viewportPositions->getViewArea();
	AffineTransform af;
	lVisibleArea = lVisibleArea.transformedBy(af.scaled(m_screenWidthScale, m_screenHeightScale));

	waveformImage = Image(Image::RGB, jmax(1, lVisibleArea.getWidth()), jmax(1, lVisibleArea.getHeight()), false);
	waveformImage.clear(waveformImage.getBounds(), myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeOff));

	float iViewPortwidth = getScreenBounds().getWidth();
	float iImageTop = 0;
	float iImageBottom = waveformImage.getHeight();
	float iImageHalf = waveformImage.getHeight() / 2.0f;

	Graphics g(waveformImage);	

	int visiStart = int(lVisibleArea.getBottomLeft().getX() / (m_ImageTotalWidth + m_Offset)) * (m_ImageTotalWidth + m_Offset);
	int visiEnd = (iViewPortwidth < lVisibleArea.getTopRight().getX()) ? iViewPortwidth : lVisibleArea.getTopRight().getX();
	int posStart = visiStart / (m_ImageTotalWidth + m_Offset);
	int posEnd = visiEnd / (m_ImageTotalWidth + m_Offset) + 2;
	if (posEnd > wavetable->getNumPositions() + 1) posEnd = wavetable->getNumPositions() + 1;

	float fontSize = (lVisibleArea.getWidth() / 1000.f) * 16.f;
	Font myFont = myProcessor->getCurrentVASTLookAndFeel()->getDefaultFont();
	myFont.setSizeAndStyle(fontSize, Font::plain, 1.0f, 0.0f); // no squashing, no kerning
	g.setFont(myFont);

	bool l_soloMode = false;
	l_soloMode = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoloMode();	

	for (int wtPos = posStart; wtPos < posEnd; wtPos++) { //+1 due to add pos area					
		
		if (m_mouseover_arrayidx == (wtPos))
			g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeBackground).brighter(0.15f));
		else 
			g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeBackground));

		//g.fillRect(float(m_ImageTotalWidth + m_Offset) * wtPos, iImageTop, m_ImageTotalWidth, iImageBottom);
		g.fillRect(float(m_ImageTotalWidth + m_Offset) *  (wtPos - posStart), iImageTop, m_ImageTotalWidth, iImageBottom);

		//int picStartsAtX = ((m_ImageTotalWidth + m_Offset) * wtPos) + 1;
		float picStartsAtX = ((m_ImageTotalWidth + m_Offset) * (wtPos - posStart)) + 1;
		
		float step = C_WAVE_TABLE_SIZE / m_wtImageWidth;
		if (wtPos >= wavetable->getNumPositions()) { //+ sign
			g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colPositionViewportPlusSign));
			g.drawLine(picStartsAtX + m_wtImageWidth/4, iImageHalf, picStartsAtX + m_wtImageWidth * 3 / 4, iImageHalf, 1.3f);
			g.drawLine(picStartsAtX + m_wtImageWidth * 0.5f, iImageTop + m_wtImageHeight / 4, picStartsAtX + m_wtImageWidth * 0.5f, iImageBottom - m_wtImageHeight/4, 1.3f);
		}
		else {
			Colour c1, c2;
			g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeLine));
			c1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeArea);
			if (l_soloMode) {
				if ((wtPos >= wavetable->getMultiSelectBegin()) && (wtPos <= wavetable->getMultiSelectEnd()))
					g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeLineSolo));
				c1 = myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeArea);
			}
			c2 = c1.withAlpha(0.0f);
			
			Path myWave;
			Path myArea;
			myArea.startNewSubPath(picStartsAtX, iImageHalf);
			for (int i = 0; i<m_wtImageWidth; i++) {				
				int arrayidx = step * i;
				float yval = (*wavetable->getNaiveTable(wtPos))[arrayidx];
				if (i == 0) myWave.startNewSubPath(picStartsAtX + i, iImageHalf - yval * iImageHalf);
				myWave.lineTo(picStartsAtX + i, iImageHalf - yval * (iImageHalf - 2.f));
				myArea.lineTo(picStartsAtX + i, iImageHalf - yval * (iImageHalf - 2.f));
			}
			g.strokePath(myWave, PathStrokeType(1.5f * myProcessor->getPluginScaleHeightFactor())); //was 2.3

			// Shade under line			
			myArea.lineTo(picStartsAtX + m_wtImageWidth, iImageHalf);
			myArea.closeSubPath();
			ColourGradient cg = ColourGradient(c2, juce::Point<float>(0, waveformImage.getHeight()), c1, juce::Point<float>(0, 0), false);
			cg.clearColours();
			cg.addColour(0.0, c1);
			cg.addColour(0.5, c2);
			cg.addColour(1.0, c1);
			g.setGradientFill(cg);
			g.fillPath(myArea);
			
			//g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colOscilloscopeBackground).withAlpha(0.9f));
			//g.fillRoundedRectangle(float(picStartsAtX + 3), 3.f, fontSize, fontSize, 2.f);
			g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colPositionViewportText).brighter(0.4f));
			//float fontSize = (myWtEditor->getPositionViewport()->getViewArea().getWidth() / 1000.f) * 16.f;
			g.drawText(juce::String(wtPos + 1), juce::Rectangle<float>(picStartsAtX + 2, 2 + 2 * myProcessor->getPluginScaleHeightFactor(), m_wtImageWidth, 14.0f * myProcessor->getPluginScaleHeightFactor()), juce::Justification::centredLeft, false);
		}

		if ((m_dragging) && (m_arrayidxdragfrom == wtPos)) //drag begin - red
			g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colPositionViewportDragBegin));
		else if ((m_dragging) && (m_arrayidxdragto == wtPos)) //drag begin - green
			g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colPositionViewportDragEnd));
		else if (wtPos == wavetable->getSelectedWtPos()) //selected
			g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colPositionViewportSelected));
		else if (wavetable->isMultiSelected() && ((wtPos >= wavetable->getMultiSelectBegin()) && (wtPos <= wavetable->getMultiSelectEnd())))   //multiselected
			g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colPositionViewportMultiSelected));
		else
			g.setColour(myEditor->getCurrentVASTLookAndFeel()->findVASTColour(VASTColours::colPositionViewportFrame));

		//g.drawRect(float(m_ImageTotalWidth + m_Offset) * wtPos, iImageTop, m_ImageTotalWidth, iImageBottom, 1.0f);
		g.drawRect(float(m_ImageTotalWidth + m_Offset) * (wtPos - posStart), iImageTop, m_ImageTotalWidth, iImageBottom, 1.0f * myProcessor->getPluginScaleHeightFactor());
	}

	repaint();
}

void VASTPositionViewport::visibleAreaChanged(const juce::Rectangle<int>& newVisibleArea) {
	if (this->isVisible()) { //skip on close
		//updateContent(false); //CHECK was true
		updateContentAsync();
	}
}

void VASTPositionViewport::startAutoUpdate() {
	//startTimer(150);     // orig value: repaint every 50 milliseconds
	startTimer(50);     // orig value: repaint every 50 milliseconds
						//startTimer(400);     // repaint every 0.4 seconds
}

void VASTPositionViewport::stopAutoUpdate() {
	stopTimer(); 				
}

void VASTPositionViewport::timerCallback() {
	if (!myProcessor->m_initCompleted)
		return;
	updateContent(false);
}

void VASTPositionViewport::mouseDrag(const MouseEvent &e) { // show value
	m_dragging = true;
	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
	std::shared_ptr<CVASTWaveTable> wavetable = myWtEditor->getBankWavetable();
	int x = e.getMouseDownX();
	//int y = e.getMouseDownY();
	int dragx = e.getDistanceFromDragStartX();
	//int dragy = e.getDistanceFromDragStartY();


	if (modifiers.isLeftButtonDown()) { 
		if (m_dragging_addnew) { //add new multi
			myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
			std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable(true, true); //need a copy as wtpos structure will be changed - selection only - keep freqs
			juce::Rectangle<int> lVisibleArea = myWtEditor->getEditorView()->c_viewportPositions->getViewArea();
			int targetWTPosNum = int((x + dragx) / ((m_ImageTotalWidth + m_Offset) / m_screenWidthScale));
			int wTPosNum = wavetable->getNumPositions();
			vassert(wTPosNum > 0);
			int visiStart = lVisibleArea.getBottomLeft().getX();
			int posStart = visiStart / (m_ImageTotalWidth + m_Offset);
			if (targetWTPosNum > wTPosNum) {
				wavetable->addPosition();
				myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wavetable);
				myWtEditor->getPositionViewport()->setViewPosition((wTPosNum - posStart)* ((m_ImageTotalWidth + m_Offset) / m_screenWidthScale) - lVisibleArea.getWidth() * 0.5f, 0.f);
			} 
			else if (targetWTPosNum < wTPosNum) {
				if (wTPosNum > 1) {
					wavetable->deletePosition(wTPosNum - 1);
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wavetable);
					myWtEditor->getPositionViewport()->setViewPosition((wTPosNum - posStart) * ((m_ImageTotalWidth  + m_Offset) / m_screenWidthScale) - lVisibleArea.getWidth() * 0.5f, 0.f);
				}
			}
			wavetable->setSelectedWtPos(wavetable->getNumPositions() - 1);
			myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
			//myWtEditor->setCurWavetable(wavetable); //make a copy
			//myWtEditor->setWtPos(wavetable->getNumPositions() - 1);
			//myWtEditor->setWtPos(arrayidx);
		}
		else { //select it
			//m_arrayidxdragfrom = int(x / ((m_ImageTotalWidth + m_Offset) / m_screenWidthScale));
			m_arrayidxdragfrom = getArrayIdx(x);
			if (m_arrayidxdragfrom >= wavetable->getNumPositions()) m_arrayidxdragfrom = wavetable->getNumPositions() - 1;
			//m_arrayidxdragto = int((x + dragx) / ((m_ImageTotalWidth + m_Offset) / m_screenWidthScale));
			m_arrayidxdragto = getArrayIdx(x + dragx);
			if (m_arrayidxdragto >= wavetable->getNumPositions()) m_arrayidxdragto = wavetable->getNumPositions() - 1;
			updateContent(true);
		}
	}
}

void VASTPositionViewport::mouseDoubleClick(const MouseEvent &e) {
	
}

int VASTPositionViewport::getArrayIdx(float logicalX) const {
	juce::Rectangle<int> lVisibleArea = myWtEditor->getEditorView()->c_viewportPositions->getViewArea();
	float x = logicalX - lVisibleArea.getX();

	AffineTransform af;
	lVisibleArea = lVisibleArea.transformedBy(af.scaled(m_screenWidthScale, m_screenHeightScale));
	int visiStart = int(lVisibleArea.getBottomLeft().getX() / (m_ImageTotalWidth + m_Offset)) * (m_ImageTotalWidth + m_Offset);
	int startPos = (visiStart) / (m_ImageTotalWidth + m_Offset);
	int pos = startPos + (x * m_screenWidthScale) / (m_ImageTotalWidth + m_Offset);
	return pos;
}

void VASTPositionViewport::mouseMove(const MouseEvent &e) {
	if (myWtEditor == nullptr) return;
	std::shared_ptr<CVASTWaveTable> wavetable = myWtEditor->getBankWavetable();
	int x = e.getMouseDownX();
	int arrayidx = getArrayIdx(x);
	if (arrayidx > wavetable->getNumPositions()) arrayidx = wavetable->getNumPositions(); //+ sign
	if (arrayidx < 0) arrayidx = 0;
	m_dirty = true;
	m_mouseover_arrayidx = arrayidx;
}

void VASTPositionViewport::mouseExit(const MouseEvent &e) {
	m_mouseover_arrayidx = -1;
}

void VASTPositionViewport::mouseUp(const MouseEvent &e) {
	if ((m_dragging) && (m_arrayidxdragfrom >= 0) && (m_arrayidxdragto >= 0) && (m_arrayidxdragfrom != m_arrayidxdragto)) {
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
		std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable();			
		int frompos = m_arrayidxdragfrom;
		int topos = m_arrayidxdragto;
		if (frompos > topos) {
			frompos = m_arrayidxdragto;
			topos = m_arrayidxdragfrom;
		}
		wavetable->copyPositionToOtherWavetable(frompos, topos + 1, wavetable.get());
		wavetable->deletePosition(frompos);
		wavetable->copyPositionToOtherWavetable(topos - 1, frompos, wavetable.get());
		wavetable->deletePosition(topos);

		m_dragging = false;
		m_dragging_addnew = false;
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wavetable);
		myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
		myWtEditor->updateAll(false);
	}

	m_dragging = false;
	m_dragging_addnew = false;
	m_arrayidxdragfrom = -1;
	m_arrayidxdragto = -1;
}

void VASTPositionViewport::mouseDown(const MouseEvent &e) {
	ModifierKeys modifiers = ModifierKeys::getCurrentModifiersRealtime();
	int x = e.getMouseDownX();
	//int y = e.getMouseDownY();

	int arrayidx = getArrayIdx(x);

	if (myWtEditor == nullptr) return;
	std::shared_ptr<CVASTWaveTable> wavetable = myWtEditor->getBankWavetable();
	if (arrayidx > wavetable->getNumPositions()) arrayidx = wavetable->getNumPositions(); //+ sign
	if (arrayidx < 0) arrayidx = 0;
	if (modifiers.isPopupMenu()) { //context menu

		PopupMenu mainMenu;
		mainMenu.setLookAndFeel(myEditor->getCurrentVASTLookAndFeel());

		mainMenu.addSectionHeader(TRANS("Select"));
		mainMenu.addItem(1, TRANS("Select all (Ctrl + a)"), true);
		mainMenu.addItem(2, TRANS("Unselect all"), true);
		mainMenu.addSeparator();
		mainMenu.addSectionHeader(TRANS("Insert & delete"));
		mainMenu.addItem(3, TRANS("Insert empty wavetable cycle (INS)"), true);
		mainMenu.addItem(4, TRANS("Delete selected wavetable cycles (DEL)"), true);

		mainMenu.addSeparator();
		PopupMenu subMenuCopyReplace;
		mainMenu.addSectionHeader(TRANS("Copy & replace"));
		subMenuCopyReplace.addItem(8, TRANS("Copy (Ctrl + c)"), true);
		subMenuCopyReplace.addItem(9, TRANS("Cut (Ctrl + x)"), true);
		subMenuCopyReplace.addItem(10, TRANS("Replace with copied / cutted wavetable cycles (Ctrl + v)"), true);
		subMenuCopyReplace.addItem(11, TRANS("Insert copied / cutted wavetable cycles (Ctrl + insert)"), true);
		subMenuCopyReplace.addItem(12, TRANS("Delete all but selection"), true);
		subMenuCopyReplace.addItem(22, TRANS("Duplicate (Ctrl + d)"), true);
		mainMenu.addSubMenu(TRANS("Copy and replace selected wavetable cycles"), subMenuCopyReplace, true);

		mainMenu.addSeparator();
		PopupMenu subMenuInterpolate;
		mainMenu.addSectionHeader(TRANS("Thin-out & interpolate"));
		subMenuInterpolate.addItem(23, TRANS("Thin-out selected cycles (every 2nd) (Ctrl + t)"), true);
		subMenuInterpolate.addItem(24, TRANS("Stretch and interpolate selected cycles (Ctrl + s)"), true);
		subMenuInterpolate.addItem(25, TRANS("Reverse order of selected cycles (Ctrl + r)"), true);
		mainMenu.addSubMenu(TRANS("Thin-out, interpolate and reverse"), subMenuInterpolate, true);

		mainMenu.addSeparator();
		PopupMenu subMenuExportImport;
		mainMenu.addSectionHeader(TRANS("Export & import"));
		subMenuExportImport.addItem(36, TRANS("Export single cycle waveform as .wav"), true);
		subMenuExportImport.addItem(37, TRANS("Export selected wavetable positions as single cycle .wav"), true);
		subMenuExportImport.addItem(38, TRANS("Import and resample wav / aiff files as single cycles at position"), true);
		mainMenu.addSubMenu(TRANS("Export and import wavetables & samples"), subMenuExportImport, true);

		mainMenu.addSeparator();
		PopupMenu subMenuNormalize;
		mainMenu.addSectionHeader(TRANS("Normalize"));
		subMenuNormalize.addItem(40, TRANS("Normalize to global gain maximum (Ctrl + n)"), true);
		subMenuNormalize.addItem(41, TRANS("Normalize inidividually"), true);
		mainMenu.addSubMenu(TRANS("Normalize selected cycles"), subMenuNormalize, true);

		mainMenu.showMenuAsync(PopupMenu::Options().withTargetComponent(this).withTargetScreenArea(juce::Rectangle<int>{}.withPosition(Desktop::getMousePosition())),
			juce::ModalCallbackFunction::create([this, wavetable, arrayidx](int result) {
			if (result == 0) {
				// user dismissed the menu without picking anything
			}
			else if (result == 1) {
				//select all
				myWtEditor->multiSelectAll();
				myWtEditor->updateAll(false);
			}
			else if (result == 2) {
				//unselect all
				myWtEditor->clearMultiSelect();
				myWtEditor->updateAll(false);
			}
			else if (result == 3) {
				//Insert empty wavetable position		
				myWtEditor->insertPosition();
				//myWtEditor->setWtPos(arrayidx);
				myWtEditor->updateAll(false);
			}
			else if (result == 4) {
				//Delete selected wavetable positions 		
				myWtEditor->deletePositions(false);
				myWtEditor->updateAll(false);
			}
			//-----
			else if (result == 8) {
				//Copy selected wavetable positions		
				myWtEditor->copyPositions();
			}
			else if (result == 9) {
				//Cut selected wavetable positions		
				myWtEditor->cutPositions();
				myWtEditor->updateAll(false);
			}
			else if (result == 10) {
				//Replace wavetable positions
				myWtEditor->replaceWithCutCopiedPositions();
				myWtEditor->updateAll(false);
			}
			else if (result == 11) {
				//Insert copied / cutted wavetable positions
				myWtEditor->insertCutCopiedPositions(false, false);
				myWtEditor->updateAll(false);
			}
			else if (result == 12) {
				//Insert copied / cutted wavetable positions
				myWtEditor->deleteAllButSelectedPositions();
				myWtEditor->updateAll(false);
			}
			else if (result == 22) {
				//Duplicate selected cycles
				myWtEditor->duplicateSelectedPositions();
				myWtEditor->updateAll(false);
			}
			else if (result == 23) {
				//Thin-out
				myWtEditor->thinOutSelectedPositions();
				myWtEditor->updateAll(false);
			}
			else if (result == 24) {
				//Stretch and interpolate selected cycles
				myWtEditor->stretchAndInterpolateSelectedPositions();
				myWtEditor->updateAll(false);
			}
			else if (result == 25) {
				//Reverse order of selected cycles
				myWtEditor->reverseOrderSelectedPositions();
				myWtEditor->updateAll(false);
			}

			else if (result == 36) {
				//Export single cycle waveform as .wav

				String m_UserWTTablesFolder = myProcessor->m_UserWavetableRootFolder;
				File initialFile = File(m_UserWTTablesFolder);
				/*
				FileChooser myChooser(TRANS("Please specify name and location of the WAV wavetable file you want to save ..."), initialFile, "*.wav");
				if (myChooser.browseForFileToSave(true))
				{
					File wtFile(myChooser.getResult());
					*/

				myChooser = std::make_unique<FileChooser>(TRANS("Please specify name and location of the WAV wavetable file you want to save ..."), initialFile, "*.wav");
				myChooser->launchAsync(FileBrowserComponent::saveMode | FileBrowserComponent::canSelectFiles | FileBrowserComponent::warnAboutOverwriting,
					[this, arrayidx, wavetable](const FileChooser& fileChooser) {

					File wtFile(fileChooser.getResult());

					WavAudioFormat wavFormat;
					wtFile.deleteFile();
					std::unique_ptr<FileOutputStream> outputStream(wtFile.createOutputStream());
					StringPairArray sarray;
                    std::unique_ptr<AudioFormatWriter> writer(wavFormat.createWriterFor(outputStream.get(), 44100.0, 1, 32, sarray, 0)); //check params 32 bit?, no metadata, quality options ignored, mono (1 channel)
					if (writer != nullptr)
					{
						outputStream.release();
						int wtPos = arrayidx;

						std::unique_ptr<AudioSampleBuffer> buffer(new AudioSampleBuffer(1, C_WAV_FORMAT_WT_SIZE));
						for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
							buffer->setSample(0, i, (*wavetable->getNaiveTable(wtPos))[i]);
							//if C_WAVE_TABLE_SIZE != C_WAV_FORMAT_WT_SIZE scale here?
						}
						writer->writeFromAudioSampleBuffer(*buffer, 0, C_WAV_FORMAT_WT_SIZE);
					}
					AlertWindow::showMessageBoxAsync(MessageBoxIconType::InfoIcon, TRANS("Files exported"), TRANS("Wavetable saved."), TRANS("Continue"), this);
					myWtEditor->updateAll(false);
				});
			}
			else if (result == 37) {
				//Export whole wavetable as single cycle .wav

				String m_UserWTTablesFolder = myProcessor->m_UserWavetableRootFolder;
				File initialFile = File(m_UserWTTablesFolder);

				/*
				FileChooser myChooser(TRANS("Please specify name and location of the WAV wavetable files you want to save ..."), initialFile, "*.wav");
				if (myChooser.browseForFileToSave(true))
				{
				*/
				myChooser = std::make_unique<FileChooser>(TRANS("Please specify name and location of the WAV wavetable file you want to save ..."), initialFile, "*.wav");
				myChooser->launchAsync(FileBrowserComponent::saveMode | FileBrowserComponent::canSelectFiles | FileBrowserComponent::warnAboutOverwriting,
					[this, arrayidx, wavetable](const FileChooser& fileChooser) {

					File wtFile(fileChooser.getResult());
					if (!wtFile.getFileExtension().equalsIgnoreCase("")) {
						std::shared_ptr<CVASTWaveTable> wavetable = myWtEditor->getBankWavetable();
						int wtPosStart = wavetable->getSelectedWtPos();
						int wtPosEnd = wavetable->getSelectedWtPos();
						if (wavetable->isMultiSelected()) {
							wtPosStart = wavetable->getMultiSelectBegin();
							wtPosEnd = wavetable->getMultiSelectEnd();
						}

						int counter = 0;
						for (int wtPos = wtPosStart; wtPos <= wtPosEnd; wtPos++) {
							counter++;
							String newName = "";
#ifdef JUCE_WINDOWS
							newName = wtFile.getParentDirectory().getFullPathName() + "\\" + wtFile.getFileNameWithoutExtension() + String(wtPos + 1) + wtFile.getFileExtension();
#else
							newName = myChooser->getResult().getParentDirectory().getFullPathName() + "/" + myChooser->getResult().getFileNameWithoutExtension() + String(wtPos + 1) + myChooser->getResult().getFileExtension();
#endif									
							File wtFileNew(newName);
							WavAudioFormat wavFormat;
							wtFileNew.deleteFile();
							std::unique_ptr<FileOutputStream> outputStream(wtFileNew.createOutputStream());
							StringPairArray sarray;
							std::unique_ptr<AudioFormatWriter> writer(wavFormat.createWriterFor(outputStream.get(), 44100.0, 1, 32, sarray, 0)); //check params 32 bit?, no metadata, quality options ignored, mono (1 channel)
							if (writer != nullptr)
							{
								outputStream.release();
								std::unique_ptr<AudioSampleBuffer> buffer(new AudioSampleBuffer(1, C_WAV_FORMAT_WT_SIZE));
								for (int i = 0; i < C_WAVE_TABLE_SIZE; i++) {
									buffer->setSample(0, i, (*wavetable->getNaiveTable(wtPos))[i]);
									//if C_WAVE_TABLE_SIZE != C_WAV_FORMAT_WT_SIZE scale here?
								}
								writer->writeFromAudioSampleBuffer(*buffer, 0, C_WAV_FORMAT_WT_SIZE);
							}
						}
						AlertWindow::showMessageBoxAsync(MessageBoxIconType::InfoIcon, TRANS("Files exported"), TRANS(String(counter) + " files created."), TRANS("Continue"), this);
					}
					myWtEditor->updateAll(false);
				});
			}
			else if (result == 38) {
				//Import and resample wav / aiff files as single cycles at position

				String m_UserWTTablesFolder = myProcessor->m_UserWavetableRootFolder;
				File initialFile = File(m_UserWTTablesFolder);
				//FileChooser myChooser(TRANS("Select wav / aiff files to import and resample as single cycles ..."), initialFile, "*.wav");

				myChooser = std::make_unique<FileChooser>(TRANS("Select wav / aiff files to import and resample as single cycles ..."), initialFile, "*.wav");
				myChooser->launchAsync(FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles | FileBrowserComponent::canSelectMultipleItems,
					[this, arrayidx, wavetable](const FileChooser& fileChooser) {

					//if (myChooser.browseForMultipleFilesToOpen())

					Array<File> fileArray = fileChooser.getResults();
					StringArray files;
					for (int i = 0; i < fileArray.size(); i++)
						files.add(fileArray[i].getFullPathName());
					//int arrayidx = int(x / (m_ImageTotalWidth + m_Offset));
					myWtEditor->importFilesAsCycles(files, myWtEditor->getWtPos());
					myWtEditor->updateAll(false);
				});
			}
			else if (result == 40) {
				// Normalize selected cycles globally
				std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::Normalize, 0.f, /*wtPosStart, wtPosEnd,*/ 0, 0, myWtEditor, 0.f, 0.f, nullptr, true);
				edit_thread.detach();

				//myWtEditor->normalizePositions(wtPosStart, wtPosEnd, wavetable.get(), myWtEditor->getBankWavetable());
				//myWtEditor->updateAll(false);
			}
			else if (result == 41) {
				// Normalize selected cycles locally
				std::thread edit_thread(&VASTWaveTableEditorComponent::threadedEditorFunction, EditorFunction::NormalizePerCycle, 0.f, /*wtPosStart, wtPosEnd,*/ 0, 0, myWtEditor, 0.f, 0.f, nullptr, true);
				edit_thread.detach();

				//myWtEditor->normalizePositions(i, i, wavetable.get(), myWtEditor->getBankWavetable());
				myWtEditor->updateAll(false);
			}
		}));
	}
	else if (modifiers.isLeftButtonDown()) { //select it or add
		if (arrayidx == wavetable->getNumPositions()) { //add new
			myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
			std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable(true, true); //need a copy as wtpos structure will be changed - selection only - keep freqs
			wavetable->addPosition();
			myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wavetable);
			myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
			//myWtEditor->setCurWavetable(wavetable); //make a copy
			wavetable->setSelectedWtPos(arrayidx);
			//myWtEditor->setWtPos(arrayidx);
			m_dragging_addnew = true;
		}
		else
			if (modifiers.isLeftButtonDown() && !modifiers.isShiftDown()) { //singleselect
				if (arrayidx < wavetable->getNumPositions()) {
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
					std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable(true, true); //need a copy as wtpos structure will be changed - selection only - keep freqs
					wavetable->setSelectedWtPos(arrayidx);
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wavetable);
					myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
				}
			}
			else
				if (modifiers.isLeftButtonDown() && modifiers.isShiftDown()) { //multiselect
					if (arrayidx < wavetable->getNumPositions()) {
						myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].addSoftFadeEditor();
						std::shared_ptr<CVASTWaveTable> wavetable = myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].getSoftOrCopyWavetable(true, true); //need a copy as wtpos structure will be changed - selection only - keep freqs
						wavetable->setMultiSelect(arrayidx);
						myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].setWavetableSoftFade(wavetable);
						myProcessor->m_pVASTXperience.m_Poly.m_OscBank[myWtEditor->m_bank].removeSoftFadeEditor();
					}
				}

		myWtEditor->initSliders();
		myWtEditor->updateAll(false);
	}
}

void VASTPositionViewport::filesDropped(const StringArray& files, int x, int ) {	
	int arrayidx = getArrayIdx(x);
	int numPos = myWtEditor->getBankWavetable()->getNumPositions();
	if (arrayidx > numPos) arrayidx = numPos;
	if (arrayidx < 0) arrayidx = 0;
	myWtEditor->importFilesAsCycles(files, arrayidx);
}

String VASTPositionViewport::getTooltip()
{
	return TRANS("Hold shift key to multiselect. Click and drag to rearrange sequence. Click and drag plus symbol to add cycles. Rightclick for menu.");
}