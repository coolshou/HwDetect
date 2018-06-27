// OptionDlg.h : header file
//

#pragma once
#include <Dbt.h>
#include "afxcmn.h"
#include "afxwin.h"
#include "resource.h"		// main symbols

class COptionDlg : public CDialog
{
public:
	COptionDlg(CWnd* pParent = NULL, 
		BOOL bStartInTray= true, BOOL bCloseToTray = true, BOOL bLaunchOnBoot = true);

	// Dialog Data
	enum { IDD = IDD_OPTION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
														// Implementation
	virtual BOOL OnInitDialog();
protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bStartInTray;
	BOOL m_bCloseToTray;
	BOOL m_bLaunchOnBoot;
	afx_msg void OnBnClickedOk();
};