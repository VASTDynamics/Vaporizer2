; https://www.kvraudio.com/forum/viewtopic.php?t=501615

#define PluginBaseName = "Vaporizer2"
#ifndef AppVer
#define AppVer "3.4.2"
#endif

[Setup]
ArchitecturesInstallIn64BitMode=x64
AppName=VAST Dynamics Vaporizer2
AppPublisher=VAST Dynamics Software
AppPublisherURL=https://www.vast-dynamics.com/
AppSupportURL=https://www.vast-dynamics.com/?q=contacts
AppUpdatesURL=https://www.vast-dynamics.com/?q=products
AppVerName=VAST Dynamics Vaporizer2 {#AppVer}
AppVersion={#AppVer}
AppCopyright=Copyright (C) VAST Dynamics Software
Compression=lzma2/ultra64
DefaultDirName={commonpf}\Vaporizer2\
DefaultGroupName={#PluginBaseName}
DisableReadyPage=no
AlwaysShowDirOnReadyPage=yes
CreateAppDir=yes
DisableDirPage=no
DisableWelcomePage=no
LanguageDetectionMethod=uilanguage
LicenseFile=license.txt
OutputBaseFilename=Vaporizer2_installer_win_{#AppVer}
SetupIconFile=logokreisvdalpha.ico
ShowLanguageDialog=no
VersionInfoCompany=VAST Dynamics Software
VersionInfoCopyright=VAST Dynamics Software
VersionInfoDescription=VAST Dynamics Vaporizer 2 {#AppVer}
VersionInfoProductName={#PluginBaseName}
VersionInfoProductVersion={#AppVer}
VersionInfoVersion={#AppVer}
WizardImageFile=vasttitle_410x797.bmp,vasttitle_164x314.bmp,vasttitle_273x556.bmp
WizardSmallImageFile=logokreisvdalpha_83x80.bmp
WizardImageStretch=false
WizardSizePercent=150
;SignTool=signtool
PrivilegesRequired=admin

[Types]
Name: "compact"; Description: "Compact installation"
Name: "full"; Description: "Full installation"
Name: "compatibility"; Description: "Compatibility installation (older CPUs, legacy names)"
Name: "custom"; Description: "Custom installation"; Flags: iscustom

[Components]
Name: "vst_win64"; Description: "VST2 Plugin (64-bit)"; Types: full; Check: IsWin64
;Name: "vst_win32"; Description: "32-bit VST2"; Types: full
;Name: "vst_win64_SSE2"; Description: "64-bit VST2 (SSE2 compatibility version for older systems)"; Types: full; Check: IsWin64 
;Name: "vst_win32_SSE2"; Description: "32-bit VST2 (SSE2 compatibility version for older systems)"; Types: full
Name: "vst3_win64"; Description: "VST3 Plugin (64-bit)"; Types: full compact; Check: IsWin64
;Name: "vst3_win32"; Description: "32-bit VST3"; Types: full
;Name: "vst3_win64_SSE2"; Description: "64-bit VST3 (SSE2 compatibility version for older systems)"; Types: full; Check: IsWin64
;Name: "vst3_win32_SSE2"; Description: "32-bit VST3 (SSE2 compatibility version for older systems)"; Types: full
Name: "standalone_win64"; Description: "Standalone (64-bit)"; Types: full compact; Check: IsWin64 
;Name: "standalone_win64_SSE2"; Description: "64-bit Standalone (SSE2 compatibility version for older systems)"; Types: full; Check: IsWin64
;Name: "standalone_win32_SSE2"; Description: "32-bit Standalone (SSE2 compatibility version for older systems)"; Types: full
Name: "aax_win64"; Description: "AAX Plugin (ProTools, 64-bit)"; Types: full; Check: IsWin64 
Name: "lv2_win64"; Description: "LV2 Plugin (64-bit)"; Types: full; Check: IsWin64
Name: "factorypresets"; Description: "Factory Presets"; Types: full compact compatibility
Name: "wavetables"; Description: "Wavetables"; Types: full compact compatibility
Name: "noises"; Description: "Noises"; Types: full compact compatibility
Name: "documentation"; Description: "Documentation File"; Types: full compact compatibility
Name: "license"; Description: "License Text"; Types: full compact; Flags: fixed
Name: "old_cpu"; Description: "SSE2 versions for systems / hosts that do not support AVX"; Types: compatibility
Name: "compatibility"; Description: "Install old plugin names for compatibility (_64./_64_SSE.)"; Types: compatibility

[Dirs]
;Name: "{app}"; Permissions: everyone-full                     
Name: "{app}"; Permissions: users-readexec
;Name: "{code:GetPluginDir|2}\VASTvaporizer2_64.aaxplugin"; Check: Is64BitInstallMode; Components:aax_win64; Attribs: system;
//;user presets
Name: "{code:GetPluginDir|4}"; Permissions: everyone-full 
//;user tables
Name: "{code:GetPluginDir|5}"; Permissions: everyone-full 
//;user noises
Name: "{code:GetPluginDir|6}"; Permissions: everyone-full 

[InstallDelete]
Type: files; Name: "{group}\uninstall Vaporizer2*"
Type: files; Name: "{group}\Vaporizer2*"
Type: files; Name: "{app}\unins*.dat"
Type: files; Name: "{app}\unins*.exe"   
;delete legacy files that are replaced by folder bundles
Type: filesandordirs; Name: "{app}\VASTvaporizer2_64.vst3" 
Type: filesandordirs; Name: "{code:GetPluginDir|1}\VASTvaporizer2_64.vst3" 
Type: filesandordirs; Name: "{app}\VASTvaporizer2.vst3"
Type: filesandordirs; Name: "{code:GetPluginDir|1}\VASTvaporizer2.vst3"
Type: filesandordirs; Name: "{app}\VASTvaporizer2_SSE_64.vst3"
Type: filesandordirs; Name: "{code:GetPluginDir|1}\VASTvaporizer2_64_SSE2.vst3"
Type: filesandordirs; Name: "{app}\VASTvaporizer2_SSE.vst3"
Type: filesandordirs; Name: "{code:GetPluginDir|1}\VASTvaporizer2_SSE2.vst3"

[Files]
;Intrinsics Detector
Source: "..\InstallerFiles\DetectIntrinsicsWin32.exe"; DestDir: "{app}";

;*STANDARD*
;VST
Source: "..\..\cmake-build\x64\VASTvaporizer2_artefacts\Release\VST\VASTvaporizer2.dll"; DestDir: "{code:GetPluginDir|0}"; Components: vst_win64; Flags: ignoreversion overwritereadonly
;VST3
;Dont deliver mousleinfo.json for now! ;Source: "..\..\cmake-build\x64\VASTvaporizer2_artefacts\Release\VST3\VASTvaporizer2.vst3\*"; DestDir: "{code:GetPluginDir|1}\VASTvaporizer2.vst3"; Components: vst3_win64; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly
Source: "..\..\cmake-build\x64\VASTvaporizer2_artefacts\Release\VST3\VASTvaporizer2.vst3\desktop.ini"; DestDir: "{code:GetPluginDir|1}\VASTvaporizer2.vst3"; Components: vst3_win64; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly
Source: "..\..\cmake-build\x64\VASTvaporizer2_artefacts\Release\VST3\VASTvaporizer2.vst3\Plugin.ico"; DestDir: "{code:GetPluginDir|1}\VASTvaporizer2.vst3"; Components: vst3_win64; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly
Source: "..\..\cmake-build\x64\VASTvaporizer2_artefacts\Release\VST3\VASTvaporizer2.vst3\Contents\x86_64-win\*"; DestDir: "{code:GetPluginDir|1}\VASTvaporizer2.vst3\Contents\x86_64-win"; Components: vst3_win64; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly

;STANDALONE
Source: "..\..\cmake-build\x64\VASTvaporizer2_artefacts\Release\Standalone\VASTvaporizer2.exe"; DestDir: "{app}"; Components: standalone_win64; Flags: ignoreversion overwritereadonly
;AAX
Source: "..\..\cmake-build\x64\VASTvaporizer2_artefacts\Release\AAX\VASTvaporizer2.aaxplugin\*"; DestDir: "{code:GetPluginDir|2}\VASTvaporizer2.aaxplugin"; Components: aax_win64; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly 
;LV2
Source: "..\..\cmake-build\x64\VASTvaporizer2_artefacts\Release\LV2\VASTvaporizer2.lv2\*"; DestDir: "{code:GetPluginDir|3}\VASTvaporizer2.lv2"; Components: lv2_win64; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly

;*COMPATIBILITY* and !*OLDCPU*
;VST
Source: "..\..\cmake-build\x64\VASTvaporizer2_artefacts\Release\VST\VASTvaporizer2.dll"; DestDir: "{code:GetPluginDir|0}"; DestName: "VASTvaporizer2_64.dll"; Components: compatibility and not old_cpu; Flags: ignoreversion overwritereadonly
;VST3
;Dont deliver mousleinfo.json for now! Source: "..\..\cmake-build\x64\VASTvaporizer2_artefacts\Release\VST3\VASTvaporizer2.vst3\*"; DestDir: "{code:GetPluginDir|1}\VASTvaporizer2_64.vst3"; Components: compatibility and not old_cpu; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly
Source: "..\..\cmake-build\x64\VASTvaporizer2_artefacts\Release\VST3\VASTvaporizer2.vst3\desktop.ini"; DestDir: "{code:GetPluginDir|1}\VASTvaporizer2_64.vst3"; Components: compatibility and not old_cpu; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly
Source: "..\..\cmake-build\x64\VASTvaporizer2_artefacts\Release\VST3\VASTvaporizer2.vst3\Plugin.ico"; DestDir: "{code:GetPluginDir|1}\VASTvaporizer2_64.vst3"; Components: compatibility and not old_cpu; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly
Source: "..\..\cmake-build\x64\VASTvaporizer2_artefacts\Release\VST3\VASTvaporizer2.vst3\Contents\x86_64-win\*"; DestDir: "{code:GetPluginDir|1}\VASTvaporizer2_64.vst3\Contents\x86_64-win"; Components: compatibility and not old_cpu; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly

;STANDALONE
Source: "..\..\cmake-build\x64\VASTvaporizer2_artefacts\Release\Standalone\VASTvaporizer2.exe"; DestDir: "{app}"; DestName: "VASTvaporizer2_64.exe"; Components: compatibility and not old_cpu; Flags: ignoreversion overwritereadonly
;AAX
Source: "..\..\cmake-build\x64\VASTvaporizer2_artefacts\Release\AAX\VASTvaporizer2.aaxplugin\*"; DestDir: "{code:GetPluginDir|2}\VASTvaporizer2_64.aaxplugin"; Components: compatibility and not old_cpu; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly 

;*COMPATIBILITY* and *OLDCPU*
;VST
Source: "..\..\cmake-build\x64SSE2\VASTvaporizer2_artefacts\Release\VST\VASTvaporizer2.dll"; DestDir: "{code:GetPluginDir|0}"; DestName: "VASTvaporizer2_SSE2.dll"; Components: compatibility and old_cpu; Flags: ignoreversion overwritereadonly
;VST3
;Dont deliver mousleinfo.json for now! Source: "..\..\cmake-build\x64SSE2\VASTvaporizer2_artefacts\Release\VST3\VASTvaporizer2.vst3\*"; DestDir: "{code:GetPluginDir|1}\VASTvaporizer2_SSE2.vst3"; Components: compatibility and old_cpu; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly
Source: "..\..\cmake-build\x64SSE2\VASTvaporizer2_artefacts\Release\VST3\VASTvaporizer2.vst3\desktop.ini"; DestDir: "{code:GetPluginDir|1}\VASTvaporizer2_SSE2.vst3"; Components: compatibility and old_cpu; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly
Source: "..\..\cmake-build\x64SSE2\VASTvaporizer2_artefacts\Release\VST3\VASTvaporizer2.vst3\Plugin.ico"; DestDir: "{code:GetPluginDir|1}\VASTvaporizer2_SSE2.vst3"; Components: compatibility and old_cpu; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly
Source: "..\..\cmake-build\x64SSE2\VASTvaporizer2_artefacts\Release\VST3\VASTvaporizer2.vst3\Contents\x86_64-win\*"; DestDir: "{code:GetPluginDir|1}\VASTvaporizer2_SSE2.vst3\Contents\x86_64-win"; Components: compatibility and old_cpu; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly
;STANDALONE
Source: "..\..\cmake-build\x64SSE2\VASTvaporizer2_artefacts\Release\Standalone\VASTvaporizer2.exe"; DestDir: "{app}"; DestName: "VASTvaporizer2_SSE2.exe"; Components: compatibility and old_cpu; Flags: ignoreversion overwritereadonly
;AAX
Source: "..\..\cmake-build\x64SSE2\VASTvaporizer2_artefacts\Release\AAX\VASTvaporizer2.aaxplugin\*"; DestDir: "{code:GetPluginDir|2}\VASTvaporizer2_SSE2.aaxplugin"; Components: compatibility and old_cpu; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly 

;*OLDCPU* and !*COMPATIBILITY*
;VST
Source: "..\..\cmake-build\x64SSE2\VASTvaporizer2_artefacts\Release\VST\VASTvaporizer2.dll"; DestDir: "{code:GetPluginDir|0}"; Components: old_cpu and not compatibility; Flags: ignoreversion overwritereadonly
;VST3
;Dont deliver mousleinfo.json for now! Source: "..\..\cmake-build\x64SSE2\VASTvaporizer2_artefacts\Release\VST3\VASTvaporizer2.vst3\*"; DestDir: "{code:GetPluginDir|1}\VASTvaporizer2.vst3"; Components: old_cpu and not compatibility; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly
Source: "..\..\cmake-build\x64SSE2\VASTvaporizer2_artefacts\Release\VST3\VASTvaporizer2.vst3\desktop.ini"; DestDir: "{code:GetPluginDir|1}\VASTvaporizer2.vst3"; Components: old_cpu and not compatibility; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly
Source: "..\..\cmake-build\x64SSE2\VASTvaporizer2_artefacts\Release\VST3\VASTvaporizer2.vst3\Plugin.ico"; DestDir: "{code:GetPluginDir|1}\VASTvaporizer2.vst3"; Components: old_cpu and not compatibility; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly
Source: "..\..\cmake-build\x64SSE2\VASTvaporizer2_artefacts\Release\VST3\VASTvaporizer2.vst3\Contents\x86_64-win\*"; DestDir: "{code:GetPluginDir|1}\VASTvaporizer2.vst3\Contents\x86_64-win"; Components: old_cpu and not compatibility; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly
;STANDALONE
Source: "..\..\cmake-build\x64SSE2\VASTvaporizer2_artefacts\Release\Standalone\VASTvaporizer2.exe"; DestDir: "{app}"; Components: old_cpu and not compatibility; Flags: ignoreversion overwritereadonly
;AAX
Source: "..\..\cmake-build\x64SSE2\VASTvaporizer2_artefacts\Release\AAX\VASTvaporizer2.aaxplugin\*"; DestDir: "{code:GetPluginDir|2}\VASTvaporizer2.aaxplugin"; Components: old_cpu and not compatibility; Flags: ignoreversion recursesubdirs createallsubdirs overwritereadonly 


;Old Win32 that are nor longer part of installer but can be build via Cmake
;VST2
;Source: "..\..\cmake-build\Win32\VASTvaporizer2_artefacts\Release\VST\VASTvaporizer2.dll"; DestDir: {code:GetPluginDir|1}; Components: vst_win32; Flags: ignoreversion
;Source: "..\..\cmake-build\Win32SSE2\VASTvaporizer2_artefacts\Release\VST\VASTvaporizer2.dll"; DestDir: {code:GetPluginDir|0}; Components: vst_win32_SSE2; Flags: ignoreversion
;VST3
;Source: "..\..\cmake-build\Win32\VASTvaporizer2_artefacts\Release\VST3\VASTvaporizer2.vst3"; DestDir: "{code:GetPluginDir|3}\VASTvaporizer2.vst3"; Components: vst3_win32; Flags: ignoreversion recursesubdirs createallsubdirs
;Source: "..\..\cmake-build\Win32SSE2\VASTvaporizer2_artefacts\Release\VST3\VASTvaporizer2.vst3"; DestDir: "{code:GetPluginDir|3}\VASTvaporizer2_SSE2.vst3"; Components: vst3_win32_SSE2; Flags: ignoreversion recursesubdirs createallsubdirs
;STANDALONE
;Source: "..\..\cmake-build\Win32SSE2\VASTvaporizer2_artefacts\Release\Standalone\VASTvaporizer2.exe"; DestDir: "{app}"; Components: standalone_win32_SSE2; Flags: ignoreversion

;OTHER STUFF
;Source: "..\Presets\*"; DestDir: "{code:GetPluginDir|6}"; Components: factorypresets; Flags: recursesubdirs
;Source: "..\Tables\*"; DestDir: "{code:GetPluginDir|7}"; Components: wavetables; Flags: recursesubdirs
;Source: "..\Noises\*"; DestDir: "{code:GetPluginDir|8}"; Components: noises; Flags: recursesubdirs
;new logic - always in appfolder
Source: "..\Presets\*"; DestDir: "{app}\Presets"; Components: factorypresets; Flags: recursesubdirs
Source: "..\Tables\*"; DestDir: "{app}\Tables"; Components: wavetables; Flags: recursesubdirs
Source: "..\Tables\*"; DestDir: "{code:GetPluginDir|5}\Factory"; Components: wavetables; Flags: recursesubdirs
Source: "..\Noises\*"; DestDir: "{app}\Noises"; Components: noises; Flags: recursesubdirs
Source: "..\Noises\*"; DestDir: "{code:GetPluginDir|6}\Factory"; Components: noises; Flags: recursesubdirs
Source: "VaporizerUserManual.url"; DestDir: "{app}"; Components: documentation
Source: "license.txt"; DestDir: "{app}"; Components: license

[Icons]
Name: {group}\Vaporizer2 {#AppVer} Standalone; Filename: "{app}\VASTvaporizer2.exe"; WorkingDir: "{app}";
Name: {group}\Vaporizer2 Users Manual; Filename: {app}\VaporizerUserManual.url; Flags: runmaximized
Name: {group}\Uninstall Vaporizer2 {#AppVer}; Filename: {uninstallexe}

[Registry]
Root: HKCU; Subkey: "Software\VAST Dynamics"; Flags: uninsdeletekeyifempty
Root: HKCU; Subkey: "Software\VAST Dynamics\Vaporizer2"; Flags: uninsdeletekey
Root: HKLM; Subkey: "Software\VAST Dynamics"; Flags: uninsdeletekeyifempty
Root: HKLM; Subkey: "Software\VAST Dynamics\Vaporizer2"; Flags: uninsdeletekey
Root: HKLM; Subkey: "Software\VAST Dynamics\Vaporizer2\Settings"; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"
Root: HKLM; Subkey: "Software\VAST Dynamics\Vaporizer2\Settings"; ValueType: string; ValueName: "UserPresetFolder"; ValueData: "{code:GetPluginDir|4}"
Root: HKLM; Subkey: "Software\VAST Dynamics\Vaporizer2\Settings"; ValueType: string; ValueName: "UserTableFolder"; ValueData: "{code:GetPluginDir|5}"
Root: HKLM; Subkey: "Software\VAST Dynamics\Vaporizer2\Settings"; ValueType: string; ValueName: "UserNoisesFolder"; ValueData: "{code:GetPluginDir|6}"
  
[Run]
Filename: "{app}\VASTvaporizer2.exe"; WorkingDir: "{app}"; Description: "Run Vaporizer2 {#AppVer} Standalone"; Components: standalone_win64; Flags: postinstall runascurrentuser nowait

[Code]
var
  PluginDirPage: TInputDirWizardPage;
  TypesComboOnChangePrev: TNotifyEvent;

//delete old uninstall registry
procedure RegSearch(RootKey: Integer; KeyName: string);
var
  I: Integer;
  Names: TArrayOfString;
  Name: string;
  VersionString: String;
begin
  if RegGetSubkeyNames(RootKey, KeyName, Names) then
  begin
    for I := 0 to GetArrayLength(Names) - 1 do
    begin
      Name := KeyName + '\' + Names[I];

      if Pos('VAST Dynamics Vaporizer2', Name) > 0 then
      begin   
        VersionString := '';
        Log(Name);                          
        if(RegValueExists(HKLM64, Name, 'DisplayVersion')) Then
          RegQueryStringValue(HKLM64, Name, 'DisplayVersion', VersionString);
        Log(VersionString);
        if (VersionString <> '{#AppVer}') then begin
          if RegDeleteKeyIncludingSubkeys(RootKey, Name) then begin
            Log(Format('Removed VAST Dynamics key %s', [Name]));
          end else begin
            Log(Format('Could not remove VAST Dynamics key %s', [Name]));
          end;
        end;
      end;
    end;
  end;
end;

procedure ComponentsListCheckChanges;
begin
  WizardForm.NextButton.Enabled := (WizardSelectedComponents(False) <> '');
end;                                        

procedure ComponentsListClickCheck(Sender: TObject);
begin
  ComponentsListCheckChanges;
end;

procedure TypesComboOnChange(Sender: TObject);
begin
  TypesComboOnChangePrev(Sender);
  ComponentsListCheckChanges;
end;

procedure ReducePromptSpacing(Page: TInputDirWizardPage; Count: Integer; Delta: Integer);
var
  I: Integer;
begin
  for I := 1 to Count - 1 do
  begin
    Page.Edits[I].Top := Page.Edits[I].Top - Delta * I;
    Page.PromptLabels[I].Top := Page.PromptLabels[I].Top - Delta * I;
    Page.Buttons[I].Top := Page.Buttons[I].Top - Delta * I;
  end;
end;


//function PrepareToInstall(var NeedsRestart: Boolean): String;
//var
//  ExitCode: integer;
//begin
//  //check for AVX or AVX2
//  Exec(ExpandConstant('{app}\DetectIntrinsicsWin32.exe'), '', '', SW_SHOW, ewWaitUntilTerminated, ExitCode);
//  
//  Result := '';
//  //MsgBox(IntToStr(ExitCode), mbInformation, MB_OK);
//  if ExitCode = 2 then 
//    begin
//      if (WizardIsComponentSelected('vst_win64_SSE2') or WizardIsComponentSelected('vst_win32_SSE2') or WizardIsComponentSelected('vst3_win64_SSE2') or WizardIsComponentSelected('vst3_win32_SSE2') or WizardIsComponentSelected('standalone_win64_SSE2') or WizardIsComponentSelected('standalone_win32_SSE2')) then
//        Result:= 'We have detected that your CPU has full AVX vector extensions support. You do not need to install the versions for SSE2 compatibility. Please go back and deselect.';
//    end
//  else if ExitCode = 1 then 
//    begin
//      if (not (WizardIsComponentSelected('vst_win64_SSE2') or WizardIsComponentSelected('vst_win32_SSE2') or WizardIsComponentSelected('vst3_win64_SSE2') or WizardIsComponentSelected('vst3_win32_SSE2') or WizardIsComponentSelected('standalone_win64_SSE2') or WizardIsComponentSelected('standalone_win32_SSE2'))) then
//        Result:= 'We have detected that your CPU does not support AVX vector extensions but has SSE2 support. Please install the SSE2 compatibility versions and deselect the others in the custom installation options.';
//    end
//  else 
//    begin;
//      Result:= 'We have detected that your CPU neither supports AVX nor SSE2 vector extensions. The plugin will unfortunately not run on this computer.';
//    end;  
//end;

function GetOriginalUserDocumentsPath(): string;
var
  OriginalUserDocumentsPath: string;
  TempFileName: string;
  Cmd: string;
  Params: string;
  ResultCode: Integer;
  Buf: AnsiString;
begin
  OriginalUserDocumentsPath := ExpandConstant('{userdocs}');
  TempFileName := ExpandConstant('{tmp}\test.txt');
  Params := Format('/C echo %%userprofile%%\Documents > %s', [TempFileName]);
  Cmd := ExpandConstant('{cmd}');
  if ExecAsOriginalUser(Cmd, Params, '', SW_HIDE, ewWaitUntilTerminated, ResultCode) and
     (ResultCode = 0) then
  begin
    if LoadStringFromFile(TempFileName, Buf) then
    begin
      OriginalUserDocumentsPath := Trim(Buf);      
    end;
  end;
  DeleteFile(TempFileName);
  Log(Format('Original user documents path: %s', [OriginalUserDocumentsPath]));
  Result := OriginalUserDocumentsPath;
end;  

procedure InitializeWizard;
begin    
  WizardForm.ComponentsList.OnClickCheck := @ComponentsListClickCheck;
  TypesComboOnChangePrev := WizardForm.TypesCombo.OnChange;
  WizardForm.TypesCombo.OnChange := @TypesComboOnChange;  

  PluginDirPage := CreateInputDirPage(wpSelectComponents, 
  'Confirm Plugin Directories', '',
  'Select the folders in which setup should install the plugins, then click Next.',
  False, '');
  
  PluginDirPage.Add('Folder VST2');
  PluginDirPage.Values[0] := GetPreviousData('VST64', ExpandConstant('{reg:HKLM\SOFTWARE\VST,VSTPluginsPath|{commonpf}\Steinberg\VSTPlugins}\Vaporizer2'));
  //PluginDirPage.Add('32-bit folder VST2');
  //PluginDirPage.Values[1] := GetPreviousData('VST32', ExpandConstant('{reg:HKLM\SOFTWARE\WOW6432NODE\VST,VSTPluginsPath|{commonpf32}\Steinberg\VSTPlugins}\Vaporizer2'));

  PluginDirPage.Add('Folder VST3');
  PluginDirPage.Values[1] := GetPreviousData('VST3_64', ExpandConstant('{commoncf64}\VST3\'));

  //PluginDirPage.Add('32-bit folder VST3');
  //PluginDirPage.Values[3] := GetPreviousData('VST3_32', ExpandConstant('{commoncf32}\VST3\'));
  
  PluginDirPage.Add('Folder AAX');
  PluginDirPage.Values[2] := GetPreviousData('AAX64', ExpandConstant('{commoncf}\Avid\Audio\Plug-Ins\'));

  PluginDirPage.Add('Folder LV2');
  PluginDirPage.Values[3] := GetPreviousData('LV264', ExpandConstant('{commoncf}\LV2\'));

  //If not Is64BitInstallMode then
  //begin
    //PluginDirPage.Values[1] := GetPreviousData('VST32', ExpandConstant('{reg:HKLM\SOFTWARE\VSTPluginsPath\VST,VSTPluginsPath|{commonpf}\Steinberg\VSTPlugins}\Vaporizer2'));
    //PluginDirPage.Buttons[0].Enabled := False;
    //PluginDirPage.PromptLabels[0].Enabled := PluginDirPage.Buttons[0].Enabled;
    //PluginDirPage.Edits[0].Enabled := PluginDirPage.Buttons[0].Enabled;
  //end;

  //new logic: factory presets are always stored in app folder, user folder can be selected
  PluginDirPage.Add('User Presets Folder');
  PluginDirPage.Values[4] := GetPreviousData('Vaporizer2PresetFolder', ExpandConstant('{code:GetOriginalUserDocumentsPath}\Vaporizer2\Presets')); //second is default value
  PluginDirPage.Add('User Wavetables');                                                       
  PluginDirPage.Values[5] := GetPreviousData('Vaporizer2TablesFolder', ExpandConstant('{code:GetOriginalUserDocumentsPath}\Vaporizer2\Tables')); //second is default value 
  PluginDirPage.Add('User Noises');
  PluginDirPage.Values[6] := GetPreviousData('Vaporizer2NoisesFolder', ExpandConstant('{code:GetOriginalUserDocumentsPath}\Vaporizer2\Noises')); //second is default value;

  //ReducePromptSpacing(PluginDirPage, 9, ScaleY(20));

end;

procedure CurPageChanged(CurPageID: Integer);
begin
  if CurPageID = PluginDirPage.ID then
  begin
    PluginDirPage.Buttons[0].Enabled := WizardIsComponentSelected('vst_win64') or WizardIsComponentSelected('old_cpu') or WizardIsComponentSelected('compatibility') 
    PluginDirPage.Buttons[0].Visible := PluginDirPage.Buttons[0].Enabled;
    PluginDirPage.PromptLabels[0].Enabled := PluginDirPage.Buttons[0].Enabled;
    PluginDirPage.PromptLabels[0].Visible := PluginDirPage.Buttons[0].Enabled;
    PluginDirPage.Edits[0].Enabled := PluginDirPage.Buttons[0].Enabled;
    PluginDirPage.Edits[0].Visible := PluginDirPage.Buttons[0].Enabled;

    //PluginDirPage.Buttons[1].Enabled := WizardIsComponentSelected('vst_win32') or WizardIsComponentSelected('old_cpu') 
    //PluginDirPage.Buttons[1].Visible := PluginDirPage.Buttons[1].Enabled;
    //PluginDirPage.PromptLabels[1].Enabled := PluginDirPage.Buttons[1].Enabled;
    //PluginDirPage.PromptLabels[1].Visible := PluginDirPage.Buttons[1].Enabled;
    //PluginDirPage.Edits[1].Enabled := PluginDirPage.Buttons[1].Enabled;
    //PluginDirPage.Edits[1].Visible := PluginDirPage.Buttons[1].Enabled;

    PluginDirPage.Buttons[1].Enabled := WizardIsComponentSelected('vst3_win64') or WizardIsComponentSelected('old_cpu') or WizardIsComponentSelected('compatibility') 
    PluginDirPage.Buttons[1].Visible := PluginDirPage.Buttons[1].Enabled;
    PluginDirPage.PromptLabels[1].Enabled := PluginDirPage.Buttons[1].Enabled;
    PluginDirPage.PromptLabels[1].Visible := PluginDirPage.Buttons[1].Enabled;
    PluginDirPage.Edits[1].Enabled := PluginDirPage.Buttons[1].Enabled;
    PluginDirPage.Edits[1].Visible := PluginDirPage.Buttons[1].Enabled;                                                                       

    //PluginDirPage.Buttons[3].Enabled := WizardIsComponentSelected('vst3_win32') or WizardIsComponentSelected('old_cpu') 
    //PluginDirPage.Buttons[3].Visible := PluginDirPage.Buttons[3].Enabled;
    //PluginDirPage.PromptLabels[3].Enabled := PluginDirPage.Buttons[3].Enabled;
    //PluginDirPage.PromptLabels[3].Visible := PluginDirPage.Buttons[3].Enabled;
    //PluginDirPage.Edits[3].Enabled := PluginDirPage.Buttons[3].Enabled;
    //PluginDirPage.Edits[3].Visible := PluginDirPage.Buttons[3].Enabled;

    PluginDirPage.Buttons[2].Enabled := WizardIsComponentSelected('aax_win64') or WizardIsComponentSelected('old_cpu') or WizardIsComponentSelected('compatibility') 
    PluginDirPage.Buttons[2].Visible := PluginDirPage.Buttons[2].Enabled;
    PluginDirPage.PromptLabels[2].Enabled := PluginDirPage.Buttons[2].Enabled;
    PluginDirPage.PromptLabels[2].Visible := PluginDirPage.Buttons[2].Enabled;
    PluginDirPage.Edits[2].Enabled := PluginDirPage.Buttons[2].Enabled;
    PluginDirPage.Edits[2].Visible := PluginDirPage.Buttons[2].Enabled;

    PluginDirPage.Buttons[3].Enabled := WizardIsComponentSelected('lv2_win64') 
    PluginDirPage.Buttons[3].Visible := PluginDirPage.Buttons[3].Enabled;
    PluginDirPage.PromptLabels[3].Enabled := PluginDirPage.Buttons[3].Enabled;
    PluginDirPage.PromptLabels[3].Visible := PluginDirPage.Buttons[3].Enabled;
    PluginDirPage.Edits[3].Enabled := PluginDirPage.Buttons[3].Enabled;
    PluginDirPage.Edits[3].Visible := PluginDirPage.Buttons[3].Enabled;

    PluginDirPage.Buttons[4].Enabled := WizardIsComponentSelected('factorypresets');
    PluginDirPage.Buttons[4].Visible := PluginDirPage.Buttons[4].Enabled;
    PluginDirPage.PromptLabels[4].Enabled := PluginDirPage.Buttons[4].Enabled;
    PluginDirPage.PromptLabels[4].Visible := PluginDirPage.Buttons[4].Enabled;
    PluginDirPage.Edits[4].Enabled := PluginDirPage.Buttons[4].Enabled;
    PluginDirPage.Edits[4].Visible := PluginDirPage.Buttons[4].Enabled;

    PluginDirPage.Buttons[5].Enabled := WizardIsComponentSelected('wavetables');
    PluginDirPage.Buttons[5].Visible := PluginDirPage.Buttons[5].Enabled;
    PluginDirPage.PromptLabels[5].Enabled := PluginDirPage.Buttons[5].Enabled;
    PluginDirPage.PromptLabels[5].Visible := PluginDirPage.Buttons[5].Enabled;
    PluginDirPage.Edits[5].Enabled := PluginDirPage.Buttons[5].Enabled;
    PluginDirPage.Edits[5].Visible := PluginDirPage.Buttons[5].Enabled;

    PluginDirPage.Buttons[6].Enabled := WizardIsComponentSelected('noises');
    PluginDirPage.Buttons[6].Visible := PluginDirPage.Buttons[6].Enabled;
    PluginDirPage.PromptLabels[6].Enabled := PluginDirPage.Buttons[6].Enabled;
    PluginDirPage.PromptLabels[6].Visible := PluginDirPage.Buttons[6].Enabled;
    PluginDirPage.Edits[6].Enabled := PluginDirPage.Buttons[6].Enabled;
    PluginDirPage.Edits[6].Visible := PluginDirPage.Buttons[6].Enabled;

  end;

  if CurPageID = wpSelectComponents then
  begin
    ComponentsListCheckChanges;
  end;
end;

function ShouldSkipPage(PageID: Integer): Boolean;
begin
  if PageID = PluginDirPage.ID then
  begin
    If ((not WizardIsComponentSelected('aax_win64')) and (not WizardIsComponentSelected('lv2_win64')) and (not WizardIsComponentSelected('vst_win32')) and (not WizardIsComponentSelected('vst_win64')) and (not WizardIsComponentSelected('vst_win32_SSE2')) and (not WizardIsComponentSelected('vst_win64_SSE2')) and (not WizardIsComponentSelected('vst3_win64')) and (not WizardIsComponentSelected('vst3_win32')) and (not WizardIsComponentSelected('vst3_win64_SSE2')) and (not WizardIsComponentSelected('vst3_win32_SSE2')) and (not WizardIsComponentSelected('old_cpu')) and (not WizardIsComponentSelected('compatibility'))) then
      begin
        Result := True
      end;
  end;
end;

function GetPluginDir(Param: string): string;
begin
    Result := PluginDirPage.Values[StrToInt(Param)];
end;

procedure RegisterPreviousData(PreviousDataKey: Integer);
begin
  //delete old uninstall registry
  RegSearch(HKEY_LOCAL_MACHINE, 'Software\Microsoft\Windows\CurrentVersion\Uninstall');

  If (PluginDirPage.Buttons[0].Enabled) Then
    SetPreviousData(PreviousDataKey, 'VST64', PluginDirPage.Values[0]);
  //If (PluginDirPage.Buttons[1].Enabled) Then
    //SetPreviousData(PreviousDataKey, 'VST32', PluginDirPage.Values[1]);
  If (PluginDirPage.Buttons[1].Enabled) Then
    SetPreviousData(PreviousDataKey, 'VST3_64', PluginDirPage.Values[1]);
  //If (PluginDirPage.Buttons[3].Enabled) Then
    //SetPreviousData(PreviousDataKey, 'VST3_32', PluginDirPage.Values[3]);
  If (PluginDirPage.Buttons[2].Enabled) Then
    SetPreviousData(PreviousDataKey, 'AAX64', PluginDirPage.Values[2]);
  If (PluginDirPage.Buttons[3].Enabled) Then
    SetPreviousData(PreviousDataKey, 'LV264', PluginDirPage.Values[3]);
  If (PluginDirPage.Buttons[4].Enabled) Then
    SetPreviousData(PreviousDataKey, 'Vaporizer2PresetFolder', PluginDirPage.Values[4]);
  If (PluginDirPage.Buttons[5].Enabled) Then
    SetPreviousData(PreviousDataKey, 'Vaporizer2TablesFolder', PluginDirPage.Values[5]);
  If (PluginDirPage.Buttons[6].Enabled) Then
    SetPreviousData(PreviousDataKey, 'Vaporizer2NoisesFolder', PluginDirPage.Values[6]);
end;

//procedure WriteInitialSettingsFile;
//var
//    FileData: String;
//begin
//	if not FileExists(ExpandConstant('{commonappdata}\Vaporizer2\VASTvaporizerSettings.xml')) then begin
//		FileData := '<?xml version="1.0" encoding="UTF-8"?><VASTVaporizerSettingsV1.000><Settings PresetRootFolder="XXXXXPRESETXXXXX" WavetableRootFolder="XXXXXTABLESXXXXX" WavRootFolder="XXXXXNOISESXXXXX"/></VASTVaporizerSettingsV1.000>';
//		StringChange(FileData, 'XXXXXPRESETXXXXX', GetPreviousData('Vaporizer2PresetFolder', WizardDirValue + '\Presets'));
//		StringChange(FileData, 'XXXXXTABLESXXXXX', GetPreviousData('Vaporizer2TablesFolder', WizardDirValue + '\Tables'));
//		StringChange(FileData, 'XXXXXNOISESXXXXX', GetPreviousData('Vaporizer2NoisesFolder', WizardDirValue + '\Noises'));
//		SaveStringToFile(ExpandConstant('{commonappdata}\Vaporizer2\VASTvaporizerSettings.xml'), FileData, False);
//	end;           
//end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
  begin
    //Log('Post install - write initial settings file');
    //WriteInitialSettingsFile;
  end;
end;
