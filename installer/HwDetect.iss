; Script generated by the Inno Script Studio Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "HwDetect"
#define MyAppVersion "1.0.0.4"
#define MyAppPublisher "Coonshou.idv, Inc."
#define MyAppURL "https://github.com/coolshou/HwDetect"
#define MyAppExeName "HWDetect.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{E56F6374-1FD2-48F5-84E1-29A0CF5A1247}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
;file version
VersionInfoVersion={#MyAppVersion}  
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}/releases
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputBaseFilename={#MyAppName}-{#MyAppVersion}-setup
Compression=lzma
SolidCompression=yes
ArchitecturesInstallIn64BitMode=x64 
AppCopyright={#MyAppPublisher}
AppMutex={#MyAppName}
SetupIconFile=..\src\res\HWDetect.ico
UninstallDisplayIcon={app}\HWDetect.exe
CloseApplications=force

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\x64\Release\HWDetect.exe"; DestDir: "{app}"; DestName: "HWDetect.exe"; Flags: 64bit; Check: Is64BitInstallMode
Source: "..\Win32\Release\HWDetect.exe"; DestDir: "{app}"; DestName: "HWDetect.exe"; Flags: 32bit; Check: not Is64BitInstallMode
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
