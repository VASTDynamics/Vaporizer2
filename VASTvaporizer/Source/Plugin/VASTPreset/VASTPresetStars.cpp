/*
VAST Dynamics
*/

#include "../../Engine/VASTEngineHeader.h"
#include "VASTPresetStars.h"
#include "VASTPresetComponent.h"
#include <ciso646>

VASTPresetStars::VASTPresetStars(VASTPresetComponent* presetComponent, String internalid, int ranking) : _presetComponent(presetComponent), m_internalid(internalid), m_ranking(ranking) {
}

VASTPresetStars::~VASTPresetStars() {
	//this->setLookAndFeel(nullptr);
}

void VASTPresetStars::paint(Graphics& g) {
	juce::Rectangle<int> rec = getLocalBounds();
	Path p1;
	Path p2;
	float dist = rec.getWidth() / 6.f;

	for (int i = 0; i < m_ranking; i++) { //5 stars		
		float maxi = (dist * 0.55f > getHeight() * 0.5f ? getHeight() * 0.5f : dist * 0.55f);
		p1.addStar(Point<float>(((i + 1) * dist), getHeight() * 0.5f), 5, maxi * 0.40f, maxi, 0.f);
	}
	g.setColour(Colours::yellow.withAlpha(0.8f)); //TODO
	g.fillPath(p1);

	for (int i = m_ranking; i < 5; i++) { //5 stars		
		float maxi = (dist * 0.55f > getHeight() * 0.5f ? getHeight() * 0.5f : dist * 0.55f);
		p2.addStar(Point<float>(((i + 1) * dist), getHeight() * 0.5f), 5, maxi * 0.40f, maxi, 0.f);
	}
	g.setColour(Colours::darkgrey.withAlpha(0.4f)); //TODO
	g.fillPath(p2);
}

void VASTPresetStars::resized() {
	
}

void VASTPresetStars::mouseDown(const MouseEvent &e) {
	juce::Rectangle<int> rec = getLocalBounds();
	float dist = rec.getWidth() / 6.f;
	int rank = e.getMouseDownX() / dist;
	if ((rank >= 0) && (rank <= 5))
		setRanking(rank);
	repaint();
}

void VASTPresetStars::mouseDrag(const MouseEvent &e) {
	juce::Rectangle<int> rec = getLocalBounds();
	float dist = rec.getWidth() / 6.f;
	int rank = (e.getMouseDownX() + e.getDistanceFromDragStartX()) / dist;
	if ((rank >= 0) && (rank <= 5))
		setRanking(rank);
	repaint();
}

void VASTPresetStars::setRanking(int rank) {
	m_ranking = rank;
	_presetComponent->myProcessor->m_presetData.setStars(m_internalid, m_ranking);
}

void VASTPresetStars::update(String internalid, int ranking) {
	m_ranking = ranking;
	m_internalid = internalid;
}