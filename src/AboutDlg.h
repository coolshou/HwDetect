// AboutDlg.h : header file
//

#pragma once
#include <Dbt.h>
//#include <setupapi.h>
#include "afxcmn.h"
#include "afxwin.h"
#include "resource.h"		// main symbols

class CAboutDlg : public CDialog
{
public:
	CAboutDlg(CWnd* pParent = NULL);

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
														// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	//	afx_msg void OnPopupDisable();
	//	afx_msg void OnUpdatePopupEnable(CCmdUI *pCmdUI);
	CStatic m_ctrlText;
};