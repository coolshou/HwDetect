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
	COptionDlg(CWnd* pParent = NULL);

	// Dialog Data
	enum { IDD = IDD_OPTION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
														// Implementation
	virtual BOOL OnInitDialog();
protected:
	DECLARE_MESSAGE_MAP()

public:
	//	afx_msg void OnPopupDisable();
	//	afx_msg void OnUpdatePopupEnable(CCmdUI *pCmdUI);
	CStatic m_ctrlText;
private:
	CString m_FileVersion;

};