// OptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OptionDlg.h"

//////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
//////////////////////////////////////////////////////////////////


COptionDlg::COptionDlg(CWnd* pParent, BOOL bStartInTray, BOOL bCloseToTray, BOOL bLaunchOnBoot) : CDialog(COptionDlg::IDD)
{
	m_bStartInTray = bStartInTray;
	m_bCloseToTray = bCloseToTray;
	m_bLaunchOnBoot = bLaunchOnBoot;
}

void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_VERSION, m_ctrlText);
	DDX_Check(pDX, IDC_CHECK_START_TO_TRAY, m_bStartInTray);
	DDX_Check(pDX, IDC_CHECK_CLOSE_TO_TRAY, m_bCloseToTray);
	DDX_Check(pDX, IDC_CHECK_LAUNCH_ON_BOOT, m_bLaunchOnBoot);
}

BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
	ON_BN_CLICKED(IDOK, &COptionDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL COptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData(false);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COptionDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDialog::OnOK();
}
