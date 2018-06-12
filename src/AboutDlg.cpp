// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AboutDlg.h"

//////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
//////////////////////////////////////////////////////////////////


CAboutDlg::CAboutDlg(CWnd* pParent, CString csVer) : CDialog(CAboutDlg::IDD)
{
	m_FileVersion = csVer;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VERSION, m_ctrlText);

}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetDlgItemText(IDC_VERSION, m_FileVersion);

	return TRUE;  // return TRUE  unless you set the focus to a control
}