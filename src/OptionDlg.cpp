// OptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OptionDlg.h"

//////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
//////////////////////////////////////////////////////////////////


COptionDlg::COptionDlg(CWnd* pParent) : CDialog(COptionDlg::IDD)
{
	
}

void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_VERSION, m_ctrlText);

}

BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
END_MESSAGE_MAP()

BOOL COptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//SetDlgItemText(IDC_VERSION, m_FileVersion);

	return TRUE;  // return TRUE  unless you set the focus to a control
}