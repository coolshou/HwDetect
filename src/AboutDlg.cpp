// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AboutDlg.h"

//////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
//////////////////////////////////////////////////////////////////


CAboutDlg::CAboutDlg(CWnd* pParent) : CDialog(CAboutDlg::IDD)
{
	
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VERSION, m_ctrlText);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()
