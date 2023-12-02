/*
VAST Dynamics
*/

#include "VASTConcertinaPanel.h"
#include "../../Engine/VASTEngineHeader.h"
#include "../VASTPreset/VASTPresetComponent.h"

struct VASTConcertinaPanel::PanelSizes
{
	struct Panel
	{
		Panel() noexcept {}

		Panel(const int sz, const int mn, const int mx) noexcept
			: size(sz), minSize(mn), maxSize(mx) {}

		int setSize(const int newSize) noexcept
		{
			jassert(minSize <= maxSize);
			const int oldSize = size;
			size = jlimit(minSize, maxSize, newSize);
			return size - oldSize;
		}

		int expand(int amount) noexcept
		{
			amount = jmin(amount, maxSize - size);
			size += amount;
			return amount;
		}

		int reduce(int amount) noexcept
		{
			amount = jmin(amount, size - minSize);
			size -= amount;
			return amount;
		}

		bool canExpand() const noexcept { return size < maxSize; }
		bool isMinimised() const noexcept { return size <= minSize; }

		int size, minSize, maxSize;
	};

	Array<Panel> sizes;

	Panel& get(int index) noexcept { return sizes.getReference(index); }
	const Panel& get(int index) const noexcept { return sizes.getReference(index); }

	PanelSizes withMovedPanel(int index, int targetPosition, int totalSpace) const
	{
		const int num = sizes.size();
		totalSpace = jmax(totalSpace, getMinimumSize(0, num));
		targetPosition = jmax(targetPosition, totalSpace - getMaximumSize(index, num));

		PanelSizes newSizes(*this);
		newSizes.stretchRange(0, index, targetPosition - newSizes.getTotalSize(0, index), stretchLast);
		newSizes.stretchRange(index, num, totalSpace - newSizes.getTotalSize(0, index) - newSizes.getTotalSize(index, num), stretchFirst);
		return newSizes;
	}

	PanelSizes fittedInto(int totalSpace) const
	{
		PanelSizes newSizes(*this);
		const int num = newSizes.sizes.size();
		totalSpace = jmax(totalSpace, getMinimumSize(0, num));
		newSizes.stretchRange(0, num, totalSpace - newSizes.getTotalSize(0, num), stretchAll);
		return newSizes;
	}

	PanelSizes withResizedPanel(int index, int panelHeight, int totalSpace) const
	{
		PanelSizes newSizes(*this);

		if (totalSpace <= 0)
		{
			newSizes.get(index).size = panelHeight;
		}
		else
		{
			const int num = sizes.size();
			const int minSize = getMinimumSize(0, num);
			totalSpace = jmax(totalSpace, minSize);

			newSizes.get(index).setSize(panelHeight);
			newSizes.stretchRange(0, index, totalSpace - newSizes.getTotalSize(0, num), stretchLast);
			newSizes.stretchRange(index, num, totalSpace - newSizes.getTotalSize(0, num), stretchLast);
			newSizes = newSizes.fittedInto(totalSpace);
		}

		return newSizes;
	}

private:
	enum ExpandMode
	{
		stretchAll,
		stretchFirst,
		stretchLast
	};

	void growRangeFirst(const int start, const int end, int spaceDiff) noexcept
	{
		for (int attempts = 4; --attempts >= 0 && spaceDiff > 0;)
			for (int i = start; i < end && spaceDiff > 0; ++i)
				spaceDiff -= get(i).expand(spaceDiff);
	}

	void growRangeLast(const int start, const int end, int spaceDiff) noexcept
	{
		for (int attempts = 4; --attempts >= 0 && spaceDiff > 0;)
			for (int i = end; --i >= start && spaceDiff > 0;)
				spaceDiff -= get(i).expand(spaceDiff);
	}

	void growRangeAll(const int start, const int end, int spaceDiff) noexcept
	{
		Array<Panel*> expandableItems;

		for (int i = start; i < end; ++i)
			if (get(i).canExpand() && !get(i).isMinimised())
				expandableItems.add(&get(i));

		for (int attempts = 4; --attempts >= 0 && spaceDiff > 0;)
			for (int i = expandableItems.size(); --i >= 0 && spaceDiff > 0;)
				spaceDiff -= expandableItems.getUnchecked(i)->expand(spaceDiff / (i + 1));

		growRangeLast(start, end, spaceDiff);
	}

	void shrinkRangeFirst(const int start, const int end, int spaceDiff) noexcept
	{
		for (int i = start; i < end && spaceDiff > 0; ++i)
			spaceDiff -= get(i).reduce(spaceDiff);
	}

	void shrinkRangeLast(const int start, const int end, int spaceDiff) noexcept
	{
		for (int i = end; --i >= start && spaceDiff > 0;)
			spaceDiff -= get(i).reduce(spaceDiff);
	}

	void stretchRange(const int start, const int end, const int amountToAdd,
		const ExpandMode expandMode) noexcept
	{
		if (end > start)
		{
			if (amountToAdd > 0)
			{
				if (expandMode == stretchAll)        growRangeAll(start, end, amountToAdd);
				else if (expandMode == stretchFirst) growRangeFirst(start, end, amountToAdd);
				else if (expandMode == stretchLast)  growRangeLast(start, end, amountToAdd);
			}
			else
			{
				if (expandMode == stretchFirst)  shrinkRangeFirst(start, end, -amountToAdd);
				else                             shrinkRangeLast(start, end, -amountToAdd);
			}
		}
	}

	int getTotalSize(int start, const int end) const noexcept
	{
		int tot = 0;
		while (start < end)  tot += get(start++).size;
		return tot;
	}

	int getMinimumSize(int start, const int end) const noexcept
	{
		int tot = 0;
		while (start < end)  tot += get(start++).minSize;
		return tot;
	}

	int getMaximumSize(int start, const int end) const noexcept
	{
		int tot = 0;
		while (start < end)
		{
			const int mx = get(start++).maxSize;
			if (mx > 0x100000)
				return mx;

			tot += mx;
		}

		return tot;
	}
};

//==============================================================================
class VASTConcertinaPanel::PanelHolder : public Component
{
public:
	PanelHolder(Component* const comp, bool takeOwnership)
		: component(comp, takeOwnership)
	{
		setRepaintsOnMouseActivity(true);
		setWantsKeyboardFocus(false);
		addAndMakeVisible(comp);
	}

	void paint(Graphics& g) override
	{
			//TODO LandF
			/*
			getLookAndFeel().drawVASTConcertinaPanelHeader(g, area, isMouseOver(), isMouseButtonDown(),
			getPanel(), *component);
			*/

		if (customHeaderComponent == nullptr)
		{
			VASTConcertinaPanel* concertina = (VASTConcertinaPanel*)getParentComponent();
			if (concertina->mbIsVertical) {
				const juce::Rectangle<int> area(getWidth(), getHeaderSize());
				g.reduceClipRegion(area);

				g.setColour(Colours::black);
				g.fillAll();

				juce::Rectangle<float> bounds = area.toFloat().reduced(0.5f);
				auto cornerSize = 4.0f;
			
				auto isTopPanel = (concertina->getPanel(0) == &getPanel());

				Path p;
				p.addRoundedRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(),
					cornerSize, cornerSize, isTopPanel, isTopPanel, false, false);

				//auto bkg = Colours::grey;

				g.setGradientFill(ColourGradient::vertical(Colours::white.withAlpha(isMouseOver() ? 0.4f : 0.2f), static_cast<float> (area.getY()),
					Colours::darkgrey.withAlpha(0.1f), static_cast<float> (area.getBottom())));
				g.fillPath(p);

				// arrows
				Path parr;
				int concertinaStatus = concertina->getConcertinaStatus();
				if ((concertinaStatus == ConcertinaStatus::firstOpen) || ((!concertina->mbSecondCollapsable) && (concertinaStatus == ConcertinaStatus::bothOpen2))) {
					parr.addTriangle(0.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f); //up - bar is down
				}
				else if ((concertinaStatus == ConcertinaStatus::secondOpen) || ((!concertina->mbFirstCollapsable) && (concertinaStatus == ConcertinaStatus::bothOpen))) {					
					parr.addTriangle(0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f); //down - bar is up
				}
				else if ((concertinaStatus == ConcertinaStatus::bothOpen) || (concertinaStatus == ConcertinaStatus::bothOpen2)) {
					parr.addTriangle(0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f); //down
					parr.addTriangle(2.0f, 1.0f, 3.0f, 1.0f, 2.5f, 0.0f); //up + 2
				}

				//g.setColour(Colours::lightgrey.contrasting().withAlpha(isMouseOver() ? 0.5f : 0.3f));
				g.setColour(Colours::white);
				g.fillPath(parr, parr.getTransformToScaleToFit(bounds.reduced(2, bounds.getHeight() / 4), true));
			}
			else {
				const juce::Rectangle<int> area(getHeaderSize(), getHeight());
				g.reduceClipRegion(area);

				g.setColour(Colours::black);
				g.fillAll();

				juce::Rectangle<float> bounds = area.toFloat().reduced(0.5f);
				auto cornerSize = 4.0f;

				auto isTopPanel = (concertina->getPanel(0) == &getPanel());

				Path p;
				p.addRoundedRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(),
					cornerSize, cornerSize, isTopPanel, isTopPanel, false, false);

				//auto bkg = Colours::grey;

				g.setGradientFill(ColourGradient::vertical(Colours::white.withAlpha(isMouseOver() ? 0.4f : 0.2f), static_cast<float> (area.getY()),
					Colours::darkgrey.withAlpha(0.1f), static_cast<float> (area.getBottom())));
				g.fillPath(p);

				// arrows
				Path parr;
				int concertinaStatus = concertina->getConcertinaStatus();			

				if ((concertinaStatus == ConcertinaStatus::firstOpen) || ((!concertina->mbSecondCollapsable) && (concertinaStatus == ConcertinaStatus::bothOpen2))) {
					parr.addTriangle(0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.5f); //right - bar is left
				}
				else if ((concertinaStatus == ConcertinaStatus::secondOpen) || ((!concertina->mbFirstCollapsable) && (concertinaStatus == ConcertinaStatus::bothOpen))) {
					parr.addTriangle(0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f);  //left - bar is right
				}
				else if ((concertinaStatus == ConcertinaStatus::bothOpen) || (concertinaStatus == ConcertinaStatus::bothOpen2)) {
					parr.addTriangle(0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.5f);  //right
					parr.addTriangle(0.0f, 2.0f, 0.0f, 3.0f, 0.5f, 2.5f);  //left + 2 down
				}

				//g.setColour(Colours::lightgrey.contrasting().withAlpha(isMouseOver() ? 0.5f : 0.3f));
				g.setColour(Colours::white);
				g.fillPath(parr, parr.getTransformToScaleToFit(bounds.reduced(2, bounds.getHeight() / 4), true));
			}
		}
	}

	void resized() override
	{
		auto bounds = getLocalBounds();		
		juce::Rectangle<int> headerBounds;
		VASTConcertinaPanel* concertina = (VASTConcertinaPanel*)getParentComponent();
		if (concertina->mbIsVertical)
			headerBounds = bounds.removeFromTop(getHeaderSize());
		else 
			headerBounds = bounds.removeFromLeft(getHeaderSize());

		if (customHeaderComponent != nullptr)
			customHeaderComponent->setBounds(headerBounds);

		component->setBounds(bounds);	
	}

	void mouseDown(const MouseEvent&) override
	{
		mouseDownY = getY();
		mouseDownX = getX();
		dragStartSizes = getPanel().getFittedSizes();
	}

	void mouseDrag(const MouseEvent& e) override
	{
		VASTConcertinaPanel& panel = getPanel();

		if (!panel.mbAllowInbetweenSizes) {
			if ((panel.concertinaStatus == ConcertinaStatus::bothOpen) || (panel.concertinaStatus == ConcertinaStatus::bothOpen2)) {
				if ((panel.mbIsVertical) && (e.getDistanceFromDragStartY() > 0))
					if (panel.mbSecondCollapsable) {
						panel.concertinaStatus = ConcertinaStatus::firstOpen;
						panel.setLayoutAccordingToStatus(true);
					}
				if ((panel.mbIsVertical) && (e.getDistanceFromDragStartY() < 0))
					if (panel.mbFirstCollapsable) {
						panel.concertinaStatus = ConcertinaStatus::secondOpen;
						panel.setLayoutAccordingToStatus(true);
					}
				if ((!panel.mbIsVertical) && (e.getDistanceFromDragStartX() > 0))
					if (panel.mbSecondCollapsable) {
						panel.concertinaStatus = ConcertinaStatus::firstOpen;
						panel.setLayoutAccordingToStatus(true);
					}
				if ((!panel.mbIsVertical) && (e.getDistanceFromDragStartX() < 0))
					if (panel.mbFirstCollapsable) {
						panel.concertinaStatus = ConcertinaStatus::secondOpen;
						panel.setLayoutAccordingToStatus(true);
					}
			}
			else if (panel.concertinaStatus == ConcertinaStatus::firstOpen) {
				if ((panel.mbIsVertical) && (e.getDistanceFromDragStartY() < 0)) {
					panel.concertinaStatus = ConcertinaStatus::bothOpen;
					panel.setLayoutAccordingToStatus(true);
				}
				if ((!panel.mbIsVertical) && (e.getDistanceFromDragStartX() < 0)) {
					panel.concertinaStatus = ConcertinaStatus::bothOpen;
					panel.setLayoutAccordingToStatus(true);
				}
			}
			else if (panel.concertinaStatus == ConcertinaStatus::secondOpen) {
				if ((panel.mbIsVertical) && (e.getDistanceFromDragStartY() > 0)) {
					panel.concertinaStatus = ConcertinaStatus::bothOpen2;
					panel.setLayoutAccordingToStatus(true);
				}
				if ((!panel.mbIsVertical) && (e.getDistanceFromDragStartX() > 0)) {
					panel.concertinaStatus = ConcertinaStatus::bothOpen2;
					panel.setLayoutAccordingToStatus(true);
				}
			}
			return;
		}
				
		if (panel.mbIsVertical) {
			panel.setLayout(dragStartSizes.withMovedPanel(panel.holders.indexOf(this),
				mouseDownY + e.getDistanceFromDragStartY(),
				panel.getHeight()), false);

			if (panel.holders[1]->component->getHeight() <= 1) //dragged VAST Panel
				panel.concertinaStatus = ConcertinaStatus::firstOpen;
			else if (panel.holders[0]->component->getHeight() <= 1)
				panel.concertinaStatus = ConcertinaStatus::secondOpen;
			else
				panel.concertinaStatus = ConcertinaStatus::bothOpen;
		}
		else {
			panel.setLayout(dragStartSizes.withMovedPanel(panel.holders.indexOf(this),
				mouseDownX + e.getDistanceFromDragStartX(),
				panel.getWidth()), false);

			if (panel.holders[1]->component->getWidth() <= 1) //dragged VAST Panel
					panel.concertinaStatus = ConcertinaStatus::firstOpen;
			else if (panel.holders[0]->component->getWidth() <= 1)
					panel.concertinaStatus = ConcertinaStatus::secondOpen;
			else 
				panel.concertinaStatus = ConcertinaStatus::bothOpen;
		}
	}

	void mouseDoubleClick(const MouseEvent&) override
	{
		getPanel().panelHeaderDoubleClicked(component);
	}

	void setCustomHeaderComponent(Component* headerComponent, bool shouldTakeOwnership)
	{
		customHeaderComponent.set(headerComponent, shouldTakeOwnership);

		if (headerComponent != nullptr)
		{
			addAndMakeVisible(customHeaderComponent);
			customHeaderComponent->addMouseListener(this, false);
		}
	}

	OptionalScopedPointer<Component> component;

private:
	PanelSizes dragStartSizes;
	int mouseDownX;
	int mouseDownY;
	OptionalScopedPointer<Component> customHeaderComponent;

	int getHeaderSize() const noexcept
	{
		VASTConcertinaPanel& panel = getPanel();
		auto ourIndex = panel.holders.indexOf(this);
		return panel.currentSizes->get(ourIndex).minSize;
	}

	VASTConcertinaPanel& getPanel() const
	{
		VASTConcertinaPanel* const panel = dynamic_cast<VASTConcertinaPanel*> (getParentComponent());
		jassert(panel != nullptr);
		return *panel;
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PanelHolder)
};

//==============================================================================
VASTConcertinaPanel::VASTConcertinaPanel(VASTAudioProcessor* _processor, bool vertical)
	: myProcessor(_processor),
    currentSizes(new PanelSizes()),
	headerHeight(10) //check
{
	mbIsVertical = vertical;
}

VASTConcertinaPanel::~VASTConcertinaPanel() {}

void VASTConcertinaPanel::addVASTPanels(Component* firstComponent, Component* secondComponent, int maxSizeFirst, int maxSizeSecond, float firstPercentage, bool firstCollapsable, bool secondCollapsable, bool allowInbetweenSizes) {
	
	//VASTConcertinaPanel is a two component panel!
	mbFirstCollapsable = firstCollapsable;
	mbSecondCollapsable = secondCollapsable;
	mbAllowInbetweenSizes = allowInbetweenSizes;

	concertinaStatus = ConcertinaStatus::bothOpen;
	if (!mbSecondCollapsable) concertinaStatus = ConcertinaStatus::bothOpen2;

	screenPercentageForFirst = firstPercentage;
	addPanel(-1, firstComponent, false); //not owned
	setMaximumPanelSize(firstComponent, maxSizeFirst);
	setPanelHeaderSize(firstComponent, 0);
	addPanel(-1, secondComponent, false); //not owned 
	setMaximumPanelSize(secondComponent, maxSizeSecond);
}

int VASTConcertinaPanel::getNumPanels() const noexcept
{
	return holders.size();
}

Component* VASTConcertinaPanel::getPanel(int index) const noexcept
{
	if (PanelHolder* h = holders[index])
		return h->component;

	return nullptr;
}

void VASTConcertinaPanel::addPanel(int insertIndex, Component* component, bool takeOwnership)
{
	jassert(component != nullptr); // can't use a null pointer here!
	jassert(indexOfComp(component) < 0); // You can't add the same component more than once!

	PanelHolder* const holder = new PanelHolder(component, takeOwnership);
	holders.insert(insertIndex, holder);
	currentSizes->sizes.insert(insertIndex, PanelSizes::Panel(headerHeight, headerHeight, std::numeric_limits<int>::max()));
	addAndMakeVisible(holder);
	resized();
}

void VASTConcertinaPanel::removePanel(Component* component)
{
	const int index = indexOfComp(component);

	if (index >= 0)
	{
		currentSizes->sizes.remove(index);
		holders.remove(index);
		resized();
	}
}

bool VASTConcertinaPanel::setPanelSize(Component* panelComponent, int size, const bool animate)
{
	const int index = indexOfComp(panelComponent);

	jassert(index >= 0); // The specified component doesn't seem to have been added!

	size += currentSizes->get(index).minSize;
	const int oldSize = currentSizes->get(index).size;
	if (mbIsVertical)
		setLayout(currentSizes->withResizedPanel(index, size, getHeight()), animate);
	else 
		setLayout(currentSizes->withResizedPanel(index, size, getWidth()), animate);

	//TODO Check performance
	Array<Component*> allChildren = panelComponent->getChildren();
	for (int i = 0; i < allChildren.size(); i++) {
		if (dynamic_cast<VASTPresetComponent*>(allChildren[i]) != 0) {
			if (mbIsVertical)
				((VASTPresetComponent*)allChildren[i])->setSize(panelComponent->getWidth(), currentSizes->get(index).size);
			else
				((VASTPresetComponent*)allChildren[i])->setSize(currentSizes->get(index).size, getHeight());
			break;
		}
	}
	/*
	if (_presetComponent != nullptr) {
		if (mbIsVertical)
			_presetComponent->setSize(panelComponent->getWidth(), currentSizes->get(index).size);
		else
			_presetComponent->setSize(currentSizes->get(index).size, getHeight());
	}
	*/

	return oldSize != currentSizes->get(index).size;
}

bool VASTConcertinaPanel::expandPanelFully(Component* component, const bool animate)
{
	if (mbIsVertical)
		return setPanelSize(component, getHeight(), animate);
	else 
		return setPanelSize(component, getWidth(), animate);
}

void VASTConcertinaPanel::setMaximumPanelSize(Component* component, int maximumSize)
{
	const int index = indexOfComp(component);
	jassert(index >= 0); // The specified component doesn't seem to have been added!

	if (index >= 0)
	{
		currentSizes->get(index).maxSize = currentSizes->get(index).minSize + maximumSize;
		resized();
	}
}

void VASTConcertinaPanel::setPanelHeaderSize(Component* component, int headerSize)
{
	const auto index = indexOfComp(component);
	jassert(index >= 0); // The specified component doesn't seem to have been added!

	if (index >= 0)
	{
		auto oldMin = currentSizes->get(index).minSize;

		currentSizes->get(index).minSize = headerSize;
		currentSizes->get(index).size += headerSize - oldMin;
		resized();
	}
}

void VASTConcertinaPanel::setCustomPanelHeader(Component* component, Component* customComponent, bool takeOwnership)
{
	OptionalScopedPointer<Component> optional(customComponent, takeOwnership);

	const auto index = indexOfComp(component);
	jassert(index >= 0); // The specified component doesn't seem to have been added!

	if (index >= 0)
		holders.getUnchecked(index)->setCustomHeaderComponent(optional.release(), takeOwnership);
}

void VASTConcertinaPanel::resized()
{	
	applyLayout(getFittedSizes(), false);

	if (getNumPanels() == 2) {//this is VAST
		setLayoutAccordingToStatus(false);
	}
}

int VASTConcertinaPanel::indexOfComp(Component* comp) const noexcept
{
	for (int i = 0; i < holders.size(); ++i)
		if (holders.getUnchecked(i)->component == comp)
			return i;

	return -1;
}

VASTConcertinaPanel::PanelSizes VASTConcertinaPanel::getFittedSizes() const
{
	if (mbIsVertical)
		return currentSizes->fittedInto(getHeight());
	else 
		return currentSizes->fittedInto(getWidth()); //CHECK
}

void VASTConcertinaPanel::applyLayout(const PanelSizes& sizes, const bool animate)
{
	if (!animate)
		animator.cancelAllAnimations(false);
	const int animationDuration = 150;

	if (mbIsVertical) {
		const int w = getWidth();
		int y = 0;

		for (int i = 0; i < holders.size(); ++i)
		{
			PanelHolder& p = *holders.getUnchecked(i);

			const int h = sizes.get(i).size;
			const juce::Rectangle<int> pos(0, y, w, h);

			if (animate)
				animator.animateComponent(&p, pos, 1.0f, animationDuration, false, 1.0, 1.0);
			else
				p.setBounds(pos);

			y += h;
		}
	}
	else {
		const int h = getHeight();
		int x = 0;

		for (int i = 0; i < holders.size(); ++i)
		{
			PanelHolder& p = *holders.getUnchecked(i);

			const int w = sizes.get(i).size;
			const juce::Rectangle<int> pos(x, 0, w, h);

			if (animate)
				animator.animateComponent(&p, pos, 1.0f, animationDuration, false, 1.0, 1.0);
			else
				p.setBounds(pos);

			x += w;
		}
	}
	
}

void VASTConcertinaPanel::setLayout(const PanelSizes& sizes, const bool animate)
{
	*currentSizes = sizes;
	applyLayout(getFittedSizes(), animate);

	/* //set invisble for performance??
	if (mbIsVertical) {
		if (holders[0]->component->getHeight() <= 1)
			holders[0]->component->setVisible(false);
		else
			holders[0]->component->setVisible(true);
		if (holders[1]->component->getHeight() <= 1)
			holders[1]->component->setVisible(false);
		else
			holders[1]->component->setVisible(true);
	}
	else {
		if (holders[0]->component->getWidth() <= 1)
			holders[0]->component->setVisible(false);
		else
			holders[0]->component->setVisible(true);
		if (holders[1]->component->getWidth() <= 1)
			holders[1]->component->setVisible(false);
		else
			holders[1]->component->setVisible(true);
	}
	*/
}

bool VASTConcertinaPanel::isPanelMaximized(int panel)
{
	if (panel == 0) {
		if (concertinaStatus == ConcertinaStatus::firstOpen)
			return true;
	}
	else
		if (concertinaStatus == ConcertinaStatus::secondOpen)
			return true;
	return false;
}

void VASTConcertinaPanel::maximizeBothPanels()
{
	concertinaStatus = ConcertinaStatus::bothOpen;
	setLayoutAccordingToStatus(true);
}

void VASTConcertinaPanel::maximizePanel(int panel)
{
	if (panel == 0)
		concertinaStatus = ConcertinaStatus::firstOpen;
	else 
		concertinaStatus = ConcertinaStatus::secondOpen;
	setLayoutAccordingToStatus(true);
}

void VASTConcertinaPanel::panelHeaderDoubleClicked(Component* component)
{
	if (holders[0]->component->isShowing()) VDBG("1 is showing"); else VDBG("1 is NOT showing");
	if (holders[1]->component->isShowing()) VDBG("2 is showing"); else VDBG("2 is NOT showing");
	//holders[0]->component->setVisible(true);
	//holders[1]->component->setVisible(true);
	
	concertinaStatus++;
	concertinaStatus = concertinaStatus % 4;
	if ((!mbFirstCollapsable) && (concertinaStatus == ConcertinaStatus::secondOpen)) concertinaStatus += 2;
	if ((!mbSecondCollapsable) && (concertinaStatus == ConcertinaStatus::firstOpen)) concertinaStatus += 2;
	concertinaStatus = concertinaStatus % 4;

	setLayoutAccordingToStatus(true);
	//holders[0]->component->resized();
	//holders[1]->component->resized();
	//setLayout(*currentSizes, true);

//	if (!expandPanelFully(component, true))
//		setPanelSize(component, 0, true);
}

void VASTConcertinaPanel::setLayoutAccordingToStatus(bool animate) {
	//CHECK TODO	
	if (mbIsVertical)
		headerHeight = 10 * myProcessor->getPluginScaleHeightFactor();
	else 
		headerHeight = 8 * myProcessor->getPluginScaleWidthFactor();

	currentSizes->get(1).minSize = headerHeight;

	if (concertinaStatus == ConcertinaStatus::firstOpen) {
		expandPanelFully(getPanel(0), animate);
		setPanelSize(getPanel(1), 0, animate);
	}
	else if (concertinaStatus == ConcertinaStatus::secondOpen) {
		expandPanelFully(getPanel(1), animate);
		setPanelSize(getPanel(0), 0, animate);
	}
	else if ((concertinaStatus == ConcertinaStatus::bothOpen) || (concertinaStatus == ConcertinaStatus::bothOpen2)) {
		if (mbIsVertical) {
			setPanelSize(getPanel(0), getHeight() * screenPercentageForFirst, animate);
			setPanelSize(getPanel(1), getHeight() * (1.f - screenPercentageForFirst), animate);
		}
		else {
			setPanelSize(getPanel(0), getWidth() * screenPercentageForFirst, animate);
			setPanelSize(getPanel(1), getWidth() * (1.f - screenPercentageForFirst), animate);
		}
	}
}

Component* VASTConcertinaPanel::getFirstComponent() {
	if (holders.size() > 0)
		return holders[0]->component;
	else
		return nullptr;
};

Component* VASTConcertinaPanel::getSecondComponent() {
	if (holders.size() > 1)
		return holders[1]->component;
	else
		return nullptr;

};
