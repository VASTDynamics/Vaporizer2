/*
VAST Dynamics
*/

#include "VASTGUIRuntimeModel.h"
#include "../Engine/VASTEngineHeader.h"
#include "VASTAudioProcessor.h"
#include "VASTAudioProcessorEditor.h"
#include <functional>

VASTGUIRuntimeModel::VASTGUIRuntimeModel(VASTAudioProcessor* processor, VASTAudioProcessorEditor* editor) : myProcessor(processor), myEditor(editor) {
}

void VASTGUIRuntimeModel::registerDragSource(VASTDragSource* dragSource, int modMatrixSource) {
    sGUISidePanel ls_GUISidePanel;
    ls_GUISidePanel.modMatrixSource = modMatrixSource;
    ls_GUISidePanel.dragSource = dragSource;
    m_GUISidePanel.push_back(ls_GUISidePanel);
    m_GUISidePanel_HashMapByModMatrixSource.insert(std::pair<int,int>(ls_GUISidePanel.modMatrixSource, m_numGUISidePanel));
    m_numGUISidePanel++;
}

VASTDragSource* VASTGUIRuntimeModel::getDragSourceByModMatrxiSource(int modMatrixSource) {
    auto pos = m_GUISidePanel_HashMapByModMatrixSource.find(modMatrixSource); //fast binary search
    if (pos != m_GUISidePanel_HashMapByModMatrixSource.end()) {
        sGUISidePanel ls_GUISidePanel = m_GUISidePanel[pos->second];
        if (ls_GUISidePanel.modMatrixSource == modMatrixSource)
            return ls_GUISidePanel.dragSource;
    }
    vassertfalse;
    return nullptr;
}

void VASTGUIRuntimeModel::registerParameterSlider(VASTParameterSlider* slider, String componentID, VASTGUIRuntimeModel::GUIComponents guiComponent, int tabNo) {
    AudioProcessorParameterWithID* param = myProcessor->m_parameterState.getParameter(componentID);
    vassert(param != nullptr);
    
    sGUIParameterSliders ls_GUIParameterSliders;
    ls_GUIParameterSliders.slider = slider;
    ls_GUIParameterSliders.componentID = componentID;
    ls_GUIParameterSliders.parameterName = componentID; //parameterName == componentID
    ls_GUIParameterSliders.modMatrixDestination = myProcessor->autoParamGetDestination(componentID);
    ls_GUIParameterSliders.parameter = param;
    ls_GUIParameterSliders.parameterId = param->paramID;
    ls_GUIParameterSliders.parent = slider->getParentComponent();
    ls_GUIParameterSliders.guiComponent = guiComponent; //slider is on which tab panel
    ls_GUIParameterSliders.tabNo = tabNo; //slider is on which sub tab panel

    m_GUIParameterSliders.push_back(ls_GUIParameterSliders);
    m_GUIParameterSliders_HashMapByParameterName.insert(std::pair<String,int>(ls_GUIParameterSliders.parameterName, m_numParameterSliders));
    m_numParameterSliders++;
}

VASTGUIRuntimeModel::sGUIParameterSliders VASTGUIRuntimeModel::getParameterSliderByParameterName(String parameterName) {
    sGUIParameterSliders ls_GUIParameterSliders;
    ls_GUIParameterSliders.slider = nullptr;
    auto pos = m_GUIParameterSliders_HashMapByParameterName.find(parameterName); //fast binary search
    if (pos != m_GUIParameterSliders_HashMapByParameterName.end()) {
        ls_GUIParameterSliders = m_GUIParameterSliders[pos->second];
        if (ls_GUIParameterSliders.parameterName.equalsIgnoreCase(parameterName))
            return ls_GUIParameterSliders;
    }
    vassertfalse;
    return ls_GUIParameterSliders;
}

void VASTGUIRuntimeModel::repaintAllSliders() {
    sGUIParameterSliders ls_GUIParameterSliders;
    for (int i=0; i< m_GUIParameterSliders.size(); i++) {
        ls_GUIParameterSliders = m_GUIParameterSliders[i];
        VASTParameterSlider* lslider = ls_GUIParameterSliders.slider;
        if (lslider != nullptr) {
            if (lslider->isShowing())
                lslider->repaint();
        }
    }
}

void VASTGUIRuntimeModel::clearAllHighlights() {
    sGUIParameterSliders ls_GUIParameterSliders;
    for (int i = 0; i < m_GUIParameterSliders.size(); i++) {
        ls_GUIParameterSliders = m_GUIParameterSliders[i];
        VASTParameterSlider* lslider = ls_GUIParameterSliders.slider;
        if (lslider != nullptr) {
            lslider->clearHighlighted();
        }
    }
}
