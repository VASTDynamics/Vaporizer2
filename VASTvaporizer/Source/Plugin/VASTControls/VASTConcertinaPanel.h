/*
VAST Dynamics
*/

#ifndef VASTCONCERTINAPANEL_H_INCLUDED
#define VASTCONCERTINAPANEL_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "../VASTUtils/VASTLookAndFeel.h"
#include "../VASTPreset/VASTPresetComponent.h"

	//==============================================================================
	/**
	A panel which holds a vertical stack of components which can be expanded
	and contracted.
	Each section has its own header bar which can be dragged up and down
	to resize it, or double-clicked to fully expand that section.
	@tags{GUI}
	*/

class VASTConcertinaPanel : public Component
{
public:
	/** Creates an empty concertina panel.
	You can call addPanel() to add some components to it.
	*/
	VASTConcertinaPanel(VASTAudioProcessor* _processor, bool vertical);

	/** Destructor. */
	~VASTConcertinaPanel();

	void paint(Graphics& ) override {		
	};

	void addVASTPanels(Component* firstComponent, Component* secondComponent, int maxSizeFirst, int maxSizeSecond, float firstPercentage, bool firstCollapsable, bool secondCollapsable, bool allowInbetweenSizes);

	VASTPresetComponent* getPresetOverlay() {
		return _presetComponent;
	}

	void addPresetBroswerOverlay(VASTPresetComponent* presetComponent) {
		_presetComponent = presetComponent;
		getFirstComponent()->addAndMakeVisible(*_presetComponent);	
	}

	void removePresetBroswerOverlay() {
		getFirstComponent()->removeChildComponent(_presetComponent);
		_presetComponent = nullptr;
	}

	/** Returns the number of panels.
	@see getPanel
	*/
	int getNumPanels() const noexcept;

	/** Returns one of the panels.
	@see getNumPanels()
	*/
	Component* getPanel(int index) const noexcept;

	/** Resizes one of the panels.
	The panelComponent must point to  a valid panel component.
	If animate is true, the panels will be animated into their new positions;
	if false, they will just be immediately resized.
	*/
	bool setPanelSize(Component* panelComponent, int newHeight, bool animate);

	/** Attempts to make one of the panels full-height.
	The panelComponent must point to  a valid panel component.
	If this component has had a maximum size set, then it will be
	expanded to that size. Otherwise, it'll fill as much of the total
	space as possible.
	*/
	bool expandPanelFully(Component* panelComponent, bool animate);

	/** Sets a maximum size for one of the panels. */
	void setMaximumPanelSize(Component* panelComponent, int maximumSize);

	/** Sets the height of the header section for one of the panels. */
	void setPanelHeaderSize(Component* panelComponent, int headerSize);

	/** Sets a custom header Component for one of the panels.
	@param panelComponent           the panel component to add the custom header to.
	@param customHeaderComponent    the custom component to use for the panel header.
	This can be nullptr to clear the custom header component
	and just use the standard LookAndFeel panel.
	@param takeOwnership            if true, then the PanelHolder will take ownership
	of the custom header component, and will delete it later when
	it's no longer needed. If false, it won't delete it, and
	you must make sure it doesn't get deleted while in use.
	*/
	void setCustomPanelHeader(Component* panelComponent, Component* customHeaderComponent, bool takeOwnership);

	//==============================================================================
	/** This abstract base class is implemented by LookAndFeel classes. */

	/*
	struct JUCE_API  LookAndFeelMethods
	{
		virtual ~LookAndFeelMethods() {}

		virtual void drawVASTConcertinaPanelHeader(Graphics&, const juce::Rectangle<int>& area,
			bool isMouseOver, bool isMouseDown,
			VASTConcertinaPanel&, Component&) = 0;
	};
	*/

	int getConcertinaStatus() const {
		return concertinaStatus;
	};

	Component* getFirstComponent();

	Component* getSecondComponent();

	void maximizePanel(int panel);
	void maximizeBothPanels();
	bool isPanelMaximized(int panel);

private:
	void resized() override;

	VASTAudioProcessor* myProcessor = nullptr;
	VASTPresetComponent* _presetComponent = nullptr;

	/** Adds a component to the panel.
	@param insertIndex          the index at which this component will be inserted, or
	-1 to append it to the end of the list.
	@param component            the component that will be shown
	@param takeOwnership        if true, then the VASTConcertinaPanel will take ownership
	of the content component, and will delete it later when
	it's no longer needed. If false, it won't delete it, and
	you must make sure it doesn't get deleted while in use.
	*/
	void addPanel(int insertIndex, Component* component, bool takeOwnership);

	/** Removes one of the panels.
	If the takeOwnership flag was set when the panel was added, then
	this will also delete the component.
	*/
	void removePanel(Component* panelComponent);

	class PanelHolder;
	struct PanelSizes;
	friend class PanelHolder;
	friend struct PanelSizes;
	friend struct ContainerDeletePolicy<PanelSizes>;
	friend struct ContainerDeletePolicy<PanelHolder>;

	std::unique_ptr<PanelSizes> currentSizes;
	OwnedArray<PanelHolder> holders;
	ComponentAnimator animator;
	int headerHeight;

	int indexOfComp(Component*) const noexcept;
	PanelSizes getFittedSizes() const;
	void applyLayout(const PanelSizes&, bool animate);
	void setLayout(const PanelSizes&, bool animate);	
	void panelHeaderDoubleClicked(Component*);

	//VAST additions
	enum ConcertinaStatus {firstOpen, bothOpen, secondOpen, bothOpen2};
	int concertinaStatus = ConcertinaStatus::bothOpen2;
	float screenPercentageForFirst = 0.4f; //testval
	bool mbIsVertical = false;
	bool mbFirstCollapsable = false;
	bool mbSecondCollapsable = false;
	bool mbAllowInbetweenSizes = true;
	void setLayoutAccordingToStatus(bool animate);
		
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTConcertinaPanel)
};

#endif
