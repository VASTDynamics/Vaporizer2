/*
VAST Dynamics
*/

#ifndef VASTGUIRUNTIMEMODEL_H_INCLUDED
#define VASTGUIRUNTIMEMODEL_H_INCLUDED

#include "../Engine/VASTEngineHeader.h"
#include <unordered_map>
#include <vector>

class VASTAudioProcessor; //forward declaration
class VASTAudioProcessorEditor; //forward declaration
class VASTParameterSlider; //forward declaration
class VASTDragSource; //forward declaration
class VASTGUIRuntimeModel
{
public:
    enum GUIComponents {VaporizerComponent, MasterVoicingComponent, SidePanelComponent, HeaderComponent, KeyboardComponent, FilterComponent, LFOMSEGComponent, MSEGEditorPane, LFOEditorPane, StepSeqEditorPane, ARPComponent, ARPEditorPane, MatrixComponent, DragMatrix, OscillatorComponent, SamplerEditorComponent, SamplerEditorComponentHeader, SamplerNoiseComponent, FXComponent, FXSelectionPane, DragFX, WavetableEditorComponent, WavetableEditorView, WavetableEditorViewHeader, GenericEditor};

    typedef struct {
        // vector with structure is created once for each control once displayed first time
        // but only for visible controls (lazy loading)
        // opening side panel shall load all tabs to fill DB
        VASTParameterSlider* slider;
        String componentID;
        String parameterName;
        int modMatrixDestination;
        AudioProcessorParameterWithID* parameter;
        ParameterID parameterId;
        Component* parent;
        VASTGUIRuntimeModel::GUIComponents guiComponent; //slider is on which tab panel
        int tabNo; //slider is on which sub tab panel
    } sGUIParameterSliders;
    
    typedef struct { //structure is created once when side panel opened
        int modMatrixSource;
        VASTDragSource* dragSource;
    } sGUISidePanel;
    
    VASTGUIRuntimeModel(VASTAudioProcessor* processor, VASTAudioProcessorEditor* editor);
    void registerParameterSlider(VASTParameterSlider* slider, String componentID, VASTGUIRuntimeModel::GUIComponents guiComponent, int tabNo);
    void registerDragSource(VASTDragSource* dragSource, int modMatrixSource);

    VASTGUIRuntimeModel::sGUIParameterSliders getParameterSliderByParameterName(String parameterName);
    VASTDragSource* getDragSourceByModMatrxiSource(int modMatrixSource);
    void repaintAllSliders();
    void clearAllHighlights();
        
private:
    VASTAudioProcessor* myProcessor;
    VASTAudioProcessorEditor* myEditor;
    std::vector<sGUIParameterSliders> m_GUIParameterSliders;
    std::vector<sGUISidePanel> m_GUISidePanel;
    std::map<String, int> m_GUIParameterSliders_HashMapByParameterName; //binary search index
    std::map<int,int> m_GUISidePanel_HashMapByModMatrixSource; //binary search index
    int m_numParameterSliders = 0;
    int m_numGUISidePanel = 0;
};

#endif /* VASTGUIRUNTIMEMODEL_H_INCLUDED */
