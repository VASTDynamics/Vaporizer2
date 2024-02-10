/*
VAST Dynamics Audio Software (TM)
*/

#ifndef VASTAUDIOPROCESSOR_H_INCLUDED
#define VASTAUDIOPROCESSOR_H_INCLUDED

#include "../Engine/VASTEngineHeader.h"
#include "../Engine/VASTXperience.h"
#if defined(VASTCOMMERCIAL) || defined(VASTBUILD)
	#include "VASTCommercial/VASTLicense.h"
#endif
#include "VASTPreset/VASTPresetElement.h"
#include "VASTPreset/VASTPresetData.h"
#include "VASTVUMeterSource.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <unordered_map>
#include "VASTUtils/VASTLookAndFeel.h"

#define MAX_LEN 63
#define MAX_TRIAL_SECONDS 1200
#define CONST_USER_PATCH_INDEX 9999

#ifdef JUCE_WINDOWS
	#ifdef _WIN64	
		#define CONST_DLL_NAME "VASTvaporizer64.dll"
	#else
		#define CONST_DLL_NAME "VASTvaporizer.dll"
	#endif

#else
	#ifdef JUCE_MAC	
		#define CONST_DLL_NAME "VASTvaporizer.component"
	#endif
#endif

class VASTParameterSlider; //forward declaration

//==============================================================================
/**
*/
class VASTAudioProcessor  : public AudioProcessor
#ifdef VASTLOG
	, public Logger
#endif
{
public:
    //==============================================================================
	VASTAudioProcessor();
    ~VASTAudioProcessor();

	enum VstStringConstants
	{
		//-------------------------------------------------------------------------------------------------------
		kMaxProgNameLen = 24,	///< used for #effGetProgramName, #effSetProgramName, #effGetProgramNameIndexed
		kMaxParamStrLen = 8,	///< used for #effGetParamLabel, #effGetParamDisplay, #effGetParamName
		kMaxVendorStrLen = 64,	///< used for #effGetVendorString, #audioMasterGetVendorString
		kMaxProductStrLen = 64,	///< used for #effGetProductString, #audioMasterGetProductString
		kMaxEffectNameLen = 32	///< used for #effGetEffectName
		//-------------------------------------------------------------------------------------------------------
	};

	enum vastErrorState { 
		noError,
		errorState1,
		errorState2_invalidMSEGData,
		errorState3_invalidStepSeqData,
		errorState4_invalidArpData,
		errorState5_couldNotOpenFileForWritingPreset,
		errorState6_loadPresetFromFileXMLParsingFailed,
		errorState7_unknownPresetFormatVersion,
		errorState8_loadPresetOscillatorTreeInvalid,
		errorState9_loadPresetWavetableDataInvalid,
		errorState10_loadPresetChunkDataInvalid,
		errorState11_loadPresetUnsuccessful,
		errorState12_writeSettingsToFileFailed,
		errorState13_readSettingsFromFileFailed,
		errorState14_loadDefaultMidiMappingFailed,
		errorState15_couldNotCreateSymlink,
		errorState16_loadPresetLockUnsuccessful,
		errorState17_internalWavetableEngineError,
		errorState18_prepareToPlayFailed,
		errorState20_loadPresetInvalidTree,
		errorState24_invalidFXBusData,
		errorState25_maxBufferSizeExceeded,
		errorState26_maxPolyNotSet,
		errorState30_invalidLicense,
		errorState31_blockedLicense
	};

	const String vastErrorStateToString(vastErrorState e) noexcept
	{
		switch (e)
		{
		case vastErrorState::noError: return (TRANS("No error."));
		case vastErrorState::errorState1: return (TRANS("Unknown error."));
		case vastErrorState::errorState2_invalidMSEGData: return (TRANS("The MSEG data is invalid. Internal error. Reload plugin."));
		case vastErrorState::errorState3_invalidStepSeqData: return (TRANS("The Step Sequence data is invalid. Internal error. Reload plugin."));
		case vastErrorState::errorState4_invalidArpData: return (TRANS("The ARP data is invalid. Internal error. Reload plugin."));
		case vastErrorState::errorState5_couldNotOpenFileForWritingPreset: return (TRANS("Could not open file for writing preset. Please check access rights to folders."));
		case vastErrorState::errorState6_loadPresetFromFileXMLParsingFailed: return (TRANS("The XML structure of the loaded preset is invalid."));
		case vastErrorState::errorState7_unknownPresetFormatVersion: return (TRANS("The version of the preset format is unknown."));
		case vastErrorState::errorState8_loadPresetOscillatorTreeInvalid: return (TRANS("The loaded preset data for oscillator banks is invalid."));
		case vastErrorState::errorState9_loadPresetWavetableDataInvalid: return (TRANS("The loaded preset data for wavetables is invalid."));
		case vastErrorState::errorState10_loadPresetChunkDataInvalid: return (TRANS("The loaded preset chunk data is invalid."));
		case vastErrorState::errorState11_loadPresetUnsuccessful: return (TRANS("Loading the preset failed."));
		case vastErrorState::errorState12_writeSettingsToFileFailed: return (TRANS("Writing the settings file failed. Please check access rights to folders."));
		case vastErrorState::errorState13_readSettingsFromFileFailed: return (TRANS("Reading the settings file failed. Please check access rights to folders."));
		case vastErrorState::errorState14_loadDefaultMidiMappingFailed: return (TRANS("Loading of default MIDI mapping failed."));
		case vastErrorState::errorState15_couldNotCreateSymlink: return (TRANS("Could not create symlink in the selected folder. Check write access rights."));
		case vastErrorState::errorState16_loadPresetLockUnsuccessful: return(TRANS("Loading the preset failed. Process lock failed."));
		case vastErrorState::errorState17_internalWavetableEngineError: return(TRANS("An internal wavetable engine error occured. Please reload the plugin."));
		case vastErrorState::errorState18_prepareToPlayFailed: return(TRANS("The audio thread could not be prepared to play. Please reload the plugin."));
		case vastErrorState::errorState20_loadPresetInvalidTree: return (TRANS("Loading the preset resulted in an invalid parameter tree. Please reload the plugin."));
		case vastErrorState::errorState24_invalidFXBusData: return (TRANS("The FX bus data is invalid."));
		case vastErrorState::errorState25_maxBufferSizeExceeded: return (TRANS("The maximum supported buffer size is exceeded."));
		case vastErrorState::errorState26_maxPolyNotSet: return (TRANS("The maximum polyphony value could not be set. Please reload the plugin."));
		case vastErrorState::errorState30_invalidLicense: return (TRANS("Invalid license issue.Contact support@vast-dynamics.com."));
		case vastErrorState::errorState31_blockedLicense: return (TRANS("License issue.Contact support@vast-dynamics.com."));		
		default: return (TRANS("Error state - reload plugin"));
		}
	}

	typedef struct {
		float rangeStart;
		float rangeEnd;
		float rangeSkew;
		StringRef paramID;
		AudioProcessorParameterWithID* param;
	} sModMatrixLookup;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
	void processBlockBypassed(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;

    //==============================================================================
	AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
	bool initNotCompleted() { return !m_initCompleted; };
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

	bool isInErrorState() { return bIsInErrorState; };
	void clearErrorState();
	void setErrorState(vastErrorState state);
	VASTAudioProcessor::vastErrorState getErrorState() const;
	bool wantsUIAlert() const;
	void clearUIAlertFlag();
	void requestUIAlert();
	void requestUIPresetUpdate();
	bool needsUIPresetUpdate() const;
	void clearUIPresetFlag();

	void requestUIPresetReloadUpdate();
	bool needsUIPresetReloadUpdate() const;
	void clearUIPresetReloadFlag();

	bool needsUIInit() const;
	bool needsUIInitAfterPresetLoad() const;
	void clearUIInitFlagAfterPresetLoad();
	void clearUIInitFlag();
	void requestUIInit();
	void requestUIInitAfterPrestLoad();
	bool isCurrentEditorInitialized();
	void resetCurrentEditorInitialized();
	void setCurrentEditorInitialized();
	bool needsUIUpdate() const;
	bool needsUIUpdate_tabs() const;
	bool needsUIUpdate_matrix() const;
	bool needsUIUpdate_sliders() const;
	int needsUIUpdate_slider1dest() const;
	int needsUIUpdate_slider2dest() const;
	void clearUIUpdateFlag();
	void requestUIUpdate(bool tabs = true, bool matrix = true, bool sliders = true, int slider1dest = -1, int slider2dest = -1);
	void requestUILoadAlert();
	static thread_local bool m_threadLocalIsAudioThread;
	
#ifdef VASTLOG
	//logger
	void logMessage(const String& message) override;
	String m_last_message{};
	static void* sendlogMessage(void* userData);
	CodeDocument logCodeDocument{};	
	std::chrono::high_resolution_clock::time_point m_processorStartTime = std::chrono::high_resolution_clock::now();
#endif

	// 
    //==============================================================================
    const String getName() const override;
  
	void setParameterText(StringRef parName, StringRef textVal, bool bSilent);
	void setParameterFloat01(StringRef parName, float nvalue, bool bSilent);
	AudioProcessorValueTreeState& getParameterTree();
	VASTVUMeterSource* getMeterSource();
	char* _strncpy(char* dst, const char* src, size_t maxLen);

    /*
	const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;
	*/

	static bool isAudioThread();

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;

	int getNumFactoryPresets() const;
	int getNumUserPresets() const;

    int getCurrentProgram() override;
	int getCurrentPresetProgram() const;
	void setCurrentProgram (int index) override;
	void initializeToDefaults();
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override; //bank
	void getCurrentProgramStateInformation(juce::MemoryBlock &destData) override; //preset
    void setStateInformation (const void* data, int sizeInBytes) override; //bank
	void setCurrentProgramStateInformation(const void *data, int sizeInBytes) override;
	void addChunkTreeState(ValueTree* treeState);

	bool isLicensed();
	String getLicenseText();
	bool isUserPatch();
	String getUserPatchName();
	void savePatchXML(File *selectedFile);
	bool loadPatchXML(XmlDocument* xmlDoc, bool bNameOnly, const VASTPresetElement* preset, int index, VASTPresetElement& resultPresetData);
	static String getVSTPath();
	static String getVSTPathAlternative();
	static String getSettingsFilePath(bool read, bool& migrate_legacy);
    bool loadUserPatchMetaData(File file, VASTPresetElement& lPreset);

	String getVersionString();
	static void crashHandler(void*);

	void loadDefaultMidiMapping();
	void loadZeroMidiMapping();
	void midiLearnParameter(int internalIndex, String compID);
	void midiForgetParameter(String componentName);
	int midiMappingGetParameterIndex(int iCC);
	int parameterIndexGetMidiMapping(int internalIndex);
	String midiMappingComponentVariableName(int iCC);
	int m_iNowLearningMidiParameter = -1;
	String m_iNowLearningMidiParameterVariableName = "";
	bool writeSettingsToFile();
	void writeSettingsToFileAsync();

	void addModMatrixLookupTable(int modMatrixDestination, float rangeStart, float rangeEnd, float rangeSkew, StringRef paramID, AudioProcessorParameterWithID* param);
	sModMatrixLookup m_modMatrixLookupTable[M_MODMATRIX_MAX_DESTINATIONS];
	std::unordered_multimap<int, String> m_mapModdestToParameterName; //fast: hashed //declare before vastxperience
	std::unordered_map<String, int> m_mapParameterNameToModdest; //declare before vastxperience	
	Array<VASTParameterSlider*> m_mapParameterNameToControl; //declare before vastxperience

    UndoManager m_undoManager {3000, 30}; //declare before parameterState
	AudioProcessorValueTreeState m_parameterState; //declare before vastxperience
	
	//==============================================================================
	CVASTXperience m_pVASTXperience;
	//==============================================================================

	bool nonThreadsafeIsBlockedProcessingInfo();
	VASTPresetData m_presetData{ this };
	VASTPresetElement getCurPatchData();

	int m_curPatchDataLoadRequestedIndex = 0;

	//--------------------------------------------------------------------------------------------------------------------
	//from settings files
	String m_UserPresetRootFolder; // root folder for user presets - read from config file, set in preset component
	String m_UserWavetableRootFolder; // root folder for user presets - read from config file, set in preset component
	String m_UserWavRootFolder; // root folder for user presets - read from config file, set in preset component
	String m_UserTuningFile; // AnaMArk .tun tuning file 
	bool m_disableOpenGLGFX = false; // settings

	int m_iUserTargetPluginWidth = 0;
	int m_iUserTargetPluginHeight = 0;
	
	float getPluginScaleWidthFactor();
	float getPluginScaleHeightFactor();
	void togglePerspectiveDisplay(int lOscillatorBank);
	bool m_bTogglePerspectiveDisplay[4] = { false, false, false, false }; //per oscbank

	void setWTmode(int wtMode);
	int getWTmode() const;
	int getMPEmode() const;
	void setMPEmode(int mode);
	bool isMPEenabled();
	int getUIFontSize() const;
	void setUIFontSize(int size);
	
	void setModWheelPermaLink(int permalink);
	int getModWheelPermaLink() const;
	void setUserTuningFile(String filename);
	
	void setBendRange(int bendRange);
	int getBendRange() const;

	int m_iWTEditorDrawMode = OscillatorEditMode::SelectMode;
	int m_iWTEditorGridMode = OscillatorGridMode::NoGrid;
	int m_iWTEditorBinMode = BinMode::ClipBin;
	int m_iWTEditorBinEditMode = FreqEditMode::SingleBin;
	int getDrawMode() const;
	int getGridMode() const;
	int getBinMode() const;
	int getBinEditMode() const;

	String getMidiKeyboardCharLayout();
	int getMidiKeyboardBaseOctave();
	void setMidiKeyboardCharLayout(String charLayout);
	void setMidiKeyboardBaseOctave(int baseOctave);

	//--------------------------------------------------------------------------------------------------------------------

	const int m_iDefaultPluginWidth = 1420; //default size from projucer
	const int m_iDefaultPluginHeight = 820; //default size from projucer
	const double m_dPluginRatio = double(m_iDefaultPluginWidth) / double(m_iDefaultPluginHeight);

	int autoParamGetDestination(String parametername);
	String autoDestinationGetParam(int modmatdest);
	
	static void passTreeToAudioThread(ValueTree tree, bool externalRepresentation, VASTPresetElement preset, int index, VASTAudioProcessor* processor, bool isSeparateThread, bool initOnly);
	std::atomic<bool> m_bAudioThreadStarted = false;
	std::atomic<bool> m_bAudioThreadCurrentlyRunning = false;
	//std::atomic<bool> m_bCreateCachedVASTEditorDelayed = false;

	bool lockedAndSafeToDoDeallocatios();

	void registerThread();
	void unregisterThread();
	bool readSettingsFromFile();
	int m_loadedPresetIndexCount = 0;
	int m_activeLookAndFeel = 3; //new default: dark
	int m_uiFontSize = 0; //default 100%
	void initLookAndFeels();
	OwnedArray<VASTLookAndFeel> vastLookAndFeels; 
	VASTLookAndFeel* getCurrentVASTLookAndFeel();

	void dumpBuffers();
	void dumpBuffersFlush();
	bool isDumping();

	bool doUndo();
	std::atomic<bool> m_initCompleted = false;

	//license information from regfile
	struct {
		String mProduct_id;
		String mUserID;
		String mActivationString;
		String mReg_name;
		String mEmail;
		String mPurchase_id;
		String mPurchase_date;
		String mLegacyLicenseString;
		bool m_bIsLicensed = false;
		bool m_bExpired = false;
		bool m_bIsLegacyLicense = true;
		bool m_bIsFreeVersion = false;
	} m_sLicenseInformation;

	String getLocalMachineID();
	String getEncodedIDString(const String& input);
	char getPlatformPrefix();

	bool m_showNewerVersionPopup = false;
	String m_newerVersionThatIsAvailble = "No newer version";
	bool readLicense();

	const String shiftHexEncryptString(const String& str);

	static void* deleteComponent(void* userData);

	MidiKeyboardState m_midiKeyboardState{};
	void midiParameterLearned(int iCC);

	bool isEditorCurrentlyProbablyVisible();
	atomic<bool> m_editorIsProbablyVisible = false;
	void registerComponentValueUpdate(String uiComponentName, float lValue);

	atomic<bool> m_bShallComponentValueUpdate = false;
	String m_shallComponentUpdate = "";
	atomic<float> m_shallComponentUpdateValue = 0.f;
    void toggleKeyboardHoldMode();
    std::atomic<bool> m_keyboardHoldMode = false;
    
#if defined(VASTCOMMERCIAL) || defined(VASTBUILD)
	VASTLicense mLicense;
#endif

private:
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VASTAudioProcessor)

    void loadPreset(int index);
    void registerLoadPreset(int index);
    int m_presetToLoad = -1;
    
	juce::uint32 m_tSetChunkCalled = 0;

	std::string  XOREncrypt(std::string  a_sValue, std::string  a_sKey);
	std::string  XORDecrypt(std::string  a_sValue, std::string  a_sKey);
	void convertToASCIIhex(std::string  & output, std::string  & input);
	void convertASCIIhexToString(std::string  & output, std::string  & input);
  
	void checkForNewerVersion(String resultString);

	String FloatArrayToString(float* fData, int numFloat);
	int StringToFloatArray(String sFloatCSV, float* fData, int maxNumFloat);
	String StringArrayToString(String* sData, int numFloat);
	int StringToStringArray(String sStringCSV, String* sData, int maxNumFloat);    

	XmlElement createPatchXML(bool externalRepresentation);
	void xmlParseToPatch(XmlElement *pRoot, bool bNameOnly, const VASTPresetElement* lPreset, int index, bool externalRepresentation, bool isFromState, VASTPresetElement& resultPresetData);

	std::atomic<bool> mUIUpdateFlag = true;
    std::atomic<bool> mUIUpdateFlag_tabs = true;
    std::atomic<bool> mUIUpdateFlag_matrix = true;
    std::atomic<bool> mUIUpdateFlag_sliders = true;
    std::atomic<int> mUIUpdateFlag_slider1dest = -1;
    std::atomic<int> mUIUpdateFlag_slider2dest = -1;
    std::atomic<bool> mUIPresetUpdate = false;
    std::atomic<bool> mUIPresetReloadUpdate = false;
	std::atomic<bool> mCurrentEditorInitialized = false;

	int m_MPEmode = 0; // settings
	int m_ModWheelPermaLink = 0;

	String m_MidiKeyboardCharLayout = "ysxdcvgbhnjmq2w3er5t6z7"; //FL Studio setup			
	int m_iMidiKeyboardBaseOctave = 4; //FL Studio setup

    std::atomic<bool> m_wasBypassed = false;
    std::atomic<bool> bIsInErrorState = false;
    std::atomic<vastErrorState> iErrorState = vastErrorState::noError;

	std::atomic<bool> mUIInitFlag;
	std::atomic<bool> mUIInitFlagAfterPrestLoad;
    std::atomic<bool> mUIAlert;

	int mIntPpq = 0;
	String m_sLicenseString = " n/a ";
	float m_fTrialSeconds = 0.0;

	void initSettings();

	typedef struct {
		bool isParam;
		int paramID;
		String componentVariableName;
	} sMidiMap;
	sMidiMap m_MidiMapping[128]; // there are only 128 MIDI CCs
	//void writeMappingForDocumentation();
	
	int m_iDumpFileSampleCount = 0;
	std::unique_ptr<FileOutputStream> m_DumpOutStream = nullptr;
	bool m_bDumpFileCreated = false;
	AudioSampleBuffer* m_WAVDumpBuffer_MasterOutBuffer;
	AudioSampleBuffer* m_WAVDumpBuffer_FilterBuffer1;
	AudioSampleBuffer* m_WAVDumpBuffer_FilterBuffer2;
	AudioSampleBuffer* m_WAVDumpBuffer_FilterBuffer3;
	AudioSampleBuffer* m_WAVDumpBuffer_FilterVoices00;
	AudioSampleBuffer* m_WAVDumpBuffer_FilterVoices01;
	AudioSampleBuffer* m_WAVDumpBuffer_FilterVoices10;
	AudioSampleBuffer* m_WAVDumpBuffer_FilterVoices11;

	VASTVUMeterSource m_meterSource;

	int m_iNumPassTreeThreads = 0;
	bool getTreeThreadLock();
    Label safePointerLabel; //for safePointerOnly

	//std::unique_ptr<VASTAudioProcessorEditor> cachedVASTEditor = nullptr;
};

#endif  
