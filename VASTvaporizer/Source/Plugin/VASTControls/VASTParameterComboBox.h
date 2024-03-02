/*
VAST Dynamics
*/

#ifndef VASTPARAMETERCOMBOBOX_H_INCLUDED
#define VASTPARAMETERCOMBOBOX_H_INCLUDED

#include "../../Engine/VASTEngineHeader.h"
#include "VASTPopupHandler.h"
#include "../VASTGUIRuntimeModel.h"

struct VASTAttachedControlBase : public AudioProcessorValueTreeState::Listener,
	public AsyncUpdater
{
	VASTAttachedControlBase(AudioProcessorValueTreeState& s, const String& p)
		: state(s), paramID(p), lastValue(0)
	{
		state.addParameterListener(paramID, this);
	}

	void removeListener()
	{
		state.removeParameterListener(paramID, this);
	}

	void setNewDenormalisedValue(float newDenormalisedValue)
	{
		if (auto* p = state.getParameter(paramID))
		{
			const float newValue = state.getParameterRange(paramID)
				.convertTo0to1(newDenormalisedValue);

			if (p->getValue() != newValue)
				p->setValueNotifyingHost(newValue);
		}
	}

	void sendInitialUpdate()
	{
		if (auto* v = state.getRawParameterValue(paramID))
			parameterChanged(paramID, *v);
	}

	void parameterChanged(const String&, float newValue) override
	{
		lastValue = newValue;

		if (MessageManager::getInstance()->isThisTheMessageThread())
		{
			cancelPendingUpdate();
			setValue(newValue);
		}
		else
		{
			triggerAsyncUpdate();
		}
	}

	void beginParameterChange()
	{
		if (auto* p = state.getParameter(paramID))
		{
			if (state.undoManager != nullptr)
				state.undoManager->beginNewTransaction();

			p->beginChangeGesture();
		}
	}

	void endParameterChange()
	{
		if (AudioProcessorParameter* p = state.getParameter(paramID))
			p->endChangeGesture();
	}

	void handleAsyncUpdate() override
	{
		setValue(lastValue);
	}

	virtual void setValue(float) = 0;

	AudioProcessorValueTreeState& state;
	String paramID;
	float lastValue;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTAttachedControlBase)
};

struct VASTPimpl :
	private VASTAttachedControlBase,
	private ComboBox::Listener
{
	VASTPimpl(AudioProcessorValueTreeState& s, const String& p, ComboBox& c)
		: VASTAttachedControlBase(s, p), combo(c), ignoreCallbacks(false)
	{
		sendInitialUpdate();
		combo.addListener(this);
	}

	~VASTPimpl() override
	{
		combo.removeListener(this);
		removeListener();
	}

	void setValue(float newValue) override
	{
		const ScopedLock selfCallbackLock(selfCallbackMutex);

		if (state.getParameter(paramID) != nullptr)
		{
			auto normValue = state.getParameterRange(paramID)
				.convertTo0to1(newValue);
			//auto index = roundToInt(normValue * (combo.getNumItems() - 1));
			auto itemID = 1 + roundToInt(normValue * (combo.getNumItems() - 1.f)); //CHTS

			//if (index != combo.getSelectedItemIndex())
			if (itemID != combo.getSelectedId())
			{
				ScopedValueSetter<bool> svs(ignoreCallbacks, true);
				//combo.setSelectedItemIndex(index, sendNotificationSync);

				combo.setSelectedId(itemID, sendNotificationSync); //CHTS
			}
		}
	}

	void comboBoxChanged(ComboBox*) override
	{
		const ScopedLock selfCallbackLock(selfCallbackMutex);

		if (!ignoreCallbacks)
		{
			if (auto* p = state.getParameter(paramID))
			{
				//auto newValue = (float)combo.getSelectedItemIndex() / (combo.getNumItems() - 1);
				float newValue = (float)(combo.getSelectedId() - 1) / (combo.getNumItems() - 1.f) ; //CHTS

				if (p->getValue() != newValue)
				{
					beginParameterChange();
					p->setValueNotifyingHost(newValue);
					endParameterChange();
				}
			}
		}
	}

	ComboBox& combo;
	bool ignoreCallbacks;
	CriticalSection selfCallbackMutex;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTPimpl)
};

class VASTComboBoxAttachment
{
	public:
		VASTComboBoxAttachment(AudioProcessorValueTreeState& s, const String& p, ComboBox& c) : pimpl(new VASTPimpl(s, p, c)) {};

	private:
		struct Pimpl;
		std::unique_ptr<VASTPimpl> pimpl;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTComboBoxAttachment)
};


class VASTParameterComboBox : public ComboBox, public LookAndFeel_V2
{
public:
	typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

	VASTParameterComboBox(const juce::String &componentName);
	~VASTParameterComboBox();
	//void resized() override;
	//Label *briefLabel;
	VASTAudioProcessor* getAudioProcessor();
	void setAudioProcessor(VASTAudioProcessor &processor);
	void bindParameter(VASTAudioProcessorEditor* editor, const String& newID, VASTGUIRuntimeModel::GUIComponents guiComponent, int tabNo);
	
	void showPopup() override;

private:
	VASTPopupHandler vastPopupHandler;
	VASTAudioProcessor *m_processor;

	std::unique_ptr<VASTComboBoxAttachment> comboBoxAttachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTParameterComboBox)
};

#endif

