﻿// HWDetect.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "HWDetect.h"
#include "HWDetectDlg.h"
#include "const.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHWDetectApp

BEGIN_MESSAGE_MAP(CHWDetectApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHWDetectApp construction

CHWDetectApp::CHWDetectApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	//https://msdn.microsoft.com/en-us/library/dd465232.aspx
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
}


// The one and only CHWDetectApp object

CHWDetectApp theApp;


// CHWDetectApp initialization

BOOL CHWDetectApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T(APP_ORG));

	// Create the tray icon
	if (!m_TrayIcon.Create(WM_ICON_NOTIFY, // Icon notify message to use
		_T(APP_NAME), // tooltip
		LoadIcon(IDR_MAINFRAME), // ID of tray icon
		IDR_TRAY_MENU)) // ID of PopUp Menu
	{
		return FALSE;
	}
	CHWDetectDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	/*
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	*/
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
