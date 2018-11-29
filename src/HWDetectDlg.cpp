// HWDetectDlg.cpp : implementation file
//

#include "stdafx.h"
#include <Dbt.h>
#include <setupapi.h>
#include "HWDetect.h"
#include "HWDetectDlg.h"
#include "AboutDlg.h"
#include "GlobalFunctions.h"
#include "OptionDlg.h"
#include "const.h"

//#include <windows.h>
#include <iostream>
using std::cout; using std::endl;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
//const std::wstring currentDateTime() {
const CString currentDateTime() {
	CString t = CTime::GetCurrentTime().Format("%Y-%m-%d.%X");
	return t;
}
//### register read/write
bool SetVal(HKEY hKey, LPCTSTR lpKey, DWORD data)
{
	LONG nError = RegSetValueEx(hKey, lpKey, NULL, REG_DWORD, (LPBYTE)&data, sizeof(DWORD));

	if (nError) {
		cout << "Error: " << nError << " Could not set registry value: " << (char*)lpKey << endl;
		return false;
	}
	else {
		return true;
	}
}

DWORD GetVal(HKEY hKey, LPCTSTR lpKey)
{
	DWORD data;		
	DWORD size = sizeof(data);	
	DWORD type = REG_DWORD;
	LONG nError = RegQueryValueEx(hKey, lpKey, NULL, &type, (LPBYTE)&data, &size);

	if (nError == ERROR_FILE_NOT_FOUND)
		data = 0; // The value will be created and set to data next time SetVal() is called.
	else if (nError)
		cout << "Error: " << nError << " Could not get registry value " << (char*)lpKey << endl;

	return data;
}

LONG WINAPI RegDeleteKeyRecursive(HKEY hKey, LPCTSTR lpSubKey)
{
	LONG lResult;
	HKEY hSubKey = NULL;
	DWORD dwIndex, cbName;
	char szSubKey[512];
	FILETIME ft;
	lResult = RegOpenKeyEx(hKey, lpSubKey, 0, KEY_ALL_ACCESS, &hSubKey);
	if (lResult == ERROR_SUCCESS)
	{
		for (dwIndex = 0;; dwIndex = 0)
		{
			cbName = sizeof(szSubKey) / sizeof(szSubKey[0]);
			lResult = RegEnumKeyEx(hSubKey, dwIndex, (LPWSTR)szSubKey, &cbName, NULL, NULL, NULL, &ft);
			if (lResult == ERROR_SUCCESS) RegDeleteKeyRecursive(hSubKey, (LPCTSTR)szSubKey);
			else break;
		}
		RegCloseKey(hSubKey);
	}
	int re = RegDeleteKey(hKey, lpSubKey);
	return re;
}
//////////////////////////////////////////////////////////////////
// CHWDetectDlg dialog
//////////////////////////////////////////////////////////////////
// ------------------------------------------------
// The following code declares our AnchorMap
// and defines how each control should be handled
// ------------------------------------------------
BEGIN_ANCHOR_MAP(CHWDetectDlg)
	ANCHOR_MAP_ENTRY(IDC_HW_TREE, ANF_TOP | ANF_LEFT | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_LOG, ANF_TOP | ANF_BOTTOM | ANF_LEFT | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_CLEAR, ANF_BOTTOM | ANF_RIGHT)
//	ANCHOR_MAP_ENTRY(IDOK, ANF_BOTTOM | ANF_RIGHT)
//	ANCHOR_MAP_ENTRY(IDCANCEL, ANF_BOTTOM | ANF_RIGHT)
END_ANCHOR_MAP()

CHWDetectDlg::CHWDetectDlg(CWnd* pParent)
	: CDialog(CHWDetectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bInitVisible = true;
	loadSetting();
}

void CHWDetectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HW_TREE, m_ctrlTree);
	DDX_Control(pDX, IDC_EDIT_LOG, m_ctrlEdit);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_ctrlButtonClear);
}

BEGIN_MESSAGE_MAP(CHWDetectDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_DEVICECHANGE, OnMyDeviceChange)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_POPUP_DISABLE, OnPopupDisable)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnClearButtonClick)
	ON_COMMAND(ID_EXIT, &CHWDetectDlg::OnExit)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()


// CHWDetectDlg message handlers

BOOL CHWDetectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// minimize at startup
	if (m_bStartInTray)
	{
		PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		// this seems to be a workaround to let the statement above to get working
		//CRect rect;
		//GetWindowRect(rect);
		//SetWindowPos(&CWnd::wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE);
	}
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strOptionMenu;
		strOptionMenu.LoadString(IDS_OPTION_DIALOG);
		// Add "Option..." menu item to system menu.
		if (!strOptionMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_OPTION_DIALOG, strOptionMenu);
		}
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			//pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_muMain.LoadMenu(IDR_POPUP_MENU);
	m_muPopup = m_muMain.GetSubMenu(0);
	hSelectedDev = NULL;

	// TODO: Add extra initialization here
    ZeroMemory(&m_imgList, sizeof(SP_CLASSIMAGELIST_DATA));
    m_imgList.cbSize = sizeof(SP_CLASSIMAGELIST_DATA);
	BOOL b = SetupDiGetClassImageList(&m_imgList);
	ASSERT(b);
	int nRootImg;
	b = SetupDiGetClassImageIndex(&m_imgList, &GUID_DEVCLASS_COMPUTER, &nRootImg);
	ASSERT(b);
	m_ctrlTree.SetImageList(CImageList::FromHandle(m_imgList.ImageList), TVSIL_NORMAL);
	int n = GetClassImgIndex(&GUID_DEVCLASS_COMPUTER);
	TCHAR szName[MAX_PATH];
	DWORD nSize = sizeof(szName);
	b = GetComputerName(szName, &nSize);
	ASSERT(b);
	hRoot = m_ctrlTree.InsertItem(szName, n, n);

	HDEVNOTIFY hDevNotify;
    DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
    ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
    NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	for(int i=0; i<sizeof(GUID_DEVINTERFACE_LIST)/sizeof(GUID); i++) {
		NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];
		hDevNotify = RegisterDeviceNotification(this->GetSafeHwnd(), &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
		if( !hDevNotify ) {
			AfxMessageBox(CString("Can't register device notification: ") 
				+ _com_error(GetLastError()).ErrorMessage(), MB_ICONEXCLAMATION);
			return FALSE;
		}
	}
	m_FileVersion = CGlobalFunctions::GetFileVersionX();
	m_ProductVersion = CGlobalFunctions::GetProductVersionX();

	// --------------------------------------
	// At this point, we need set everything
	// up for our dialog except the 
	// anchoring/docking. We will do this now
	// with a call to InitAnchors()
	// --------------------------------------
	InitAnchors(ANIF_SIZEGRIP);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHWDetectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	switch (nID)
	{
		case SC_CLOSE:
			if (m_bCloseToTray) {
				ShowWindow(SW_HIDE); //最小化隐藏窗口
			}
			else {
				CDialog::OnSysCommand(nID, lParam);
			}
			break;
		case SC_MINIMIZE:
			// do not minimize to the taskbar
			ShowWindow(SW_HIDE); //最小化隐藏窗口
			break;
		default:
			if ((nID & 0xFFF0) == IDM_OPTION_DIALOG)
			{
				COptionDlg dlgOption(this, m_bStartInTray, m_bCloseToTray, m_bLaunchOnBoot);
				if (dlgOption.DoModal() == IDOK) {
					m_bStartInTray = dlgOption.m_bStartInTray;
					m_bCloseToTray = dlgOption.m_bCloseToTray;
					m_bLaunchOnBoot = dlgOption.m_bLaunchOnBoot;
					
					applySetting();
				}
			} else if ((nID & 0xFFF0) == IDM_ABOUTBOX)
			{
				CAboutDlg dlgAbout(this, m_FileVersion);
				dlgAbout.DoModal();
			}
			else
			{
				CDialog::OnSysCommand(nID, lParam);
			}
	}
}
void CHWDetectDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
	//ths can remove splich issue but tray icon double click to show main dialog will not work
	if (m_bInitVisible)
	{
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
		//m_bVisible = true;
	}
	
	CDialog::OnWindowPosChanging(lpwndpos);
}
void CHWDetectDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) {
	if (!m_bInitVisible) {
		m_bInitVisible = true;
	}
}
void CHWDetectDlg::OnClose()
{
	PostQuitMessage(0);
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHWDetectDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHWDetectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CHWDetectDlg::OnMyDeviceChange(WPARAM wParam, LPARAM lParam)
{
	if ( DBT_DEVICEARRIVAL == wParam || DBT_DEVICEREMOVECOMPLETE == wParam ) {
		PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
		PDEV_BROADCAST_DEVICEINTERFACE pDevInf;
		PDEV_BROADCAST_HANDLE pDevHnd;
		PDEV_BROADCAST_OEM pDevOem;
		PDEV_BROADCAST_PORT pDevPort;
		PDEV_BROADCAST_VOLUME pDevVolume;
		switch( pHdr->dbch_devicetype ) {
			case DBT_DEVTYP_DEVICEINTERFACE:
				pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)pHdr;
				UpdateDevice(pDevInf, wParam);
				break;

			case DBT_DEVTYP_HANDLE:
				pDevHnd = (PDEV_BROADCAST_HANDLE)pHdr;
				break;

			case DBT_DEVTYP_OEM:
				pDevOem = (PDEV_BROADCAST_OEM)pHdr;
				break;

			case DBT_DEVTYP_PORT:
				pDevPort = (PDEV_BROADCAST_PORT)pHdr;
				break;

			case DBT_DEVTYP_VOLUME:
				pDevVolume = (PDEV_BROADCAST_VOLUME)pHdr;
				break;
		}
	}
	return 0;
}

void CHWDetectDlg::UpdateDevice(PDEV_BROADCAST_DEVICEINTERFACE pDevInf, WPARAM wParam)
{
	// pDevInf->dbcc_name: 
	// \\?\USB#Vid_04e8&Pid_503b#0002F9A9828E0F06#{a5dcbf10-6530-11d2-901f-00c04fb951ed}
	// szDevId: USB\Vid_04e8&Pid_503b\0002F9A9828E0F06
	// szClass: USB
	ASSERT(lstrlen(pDevInf->dbcc_name) > 4);
	CString szDevId = pDevInf->dbcc_name+4;
	int idx = szDevId.ReverseFind(_T('#'));
	ASSERT( -1 != idx );
	szDevId.Truncate(idx);
	szDevId.Replace(_T('#'), _T('\\'));
	szDevId.MakeUpper();

	CString szClass;
	idx = szDevId.Find(_T('\\'));
	ASSERT(-1 != idx );
	szClass = szDevId.Left(idx);
	CString szLog;
	m_ctrlEdit.GetWindowText(szLog);
	CString szTime;
	szTime = currentDateTime();
	CString szTmp;
	if ( DBT_DEVICEARRIVAL == wParam ) {
		szTmp.Format(_T("%s Adding %s\r\n"), szTime, szDevId.GetString());
		// TRACE("Adding %s\n", szDevId.GetBuffer());
	} else {
		szTmp.Format(_T("%s Removing %s\r\n"), szTime, szDevId.GetString());
		// TRACE("Removing %s\n", szDevId.GetBuffer());
	}
	szLog.Append(szTmp);
	m_ctrlEdit.SetWindowText(szLog);

	// seems we should ignore "ROOT" type....
	if ( _T("ROOT") == szClass ) {
		return;
	}

	DWORD dwFlag = DBT_DEVICEARRIVAL != wParam ? 
		DIGCF_ALLCLASSES : (DIGCF_ALLCLASSES | DIGCF_PRESENT);
	HDEVINFO hDevInfo = SetupDiGetClassDevs(NULL,szClass,NULL,dwFlag);
	if( INVALID_HANDLE_VALUE == hDevInfo ) {
		AfxMessageBox(CString("SetupDiGetClassDevs(): ") 
			+ _com_error(GetLastError()).ErrorMessage(), MB_ICONEXCLAMATION);
		return;
	}

	SP_DEVINFO_DATA spDevInfoData;
	if ( FindDevice(hDevInfo, szDevId, spDevInfoData) ) {
		// OK, device found
		DWORD DataT ;
		TCHAR buf[MAX_PATH];
		DWORD nSize = 0;

		// get Friendly Name or Device Description
		if ( SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData, 
			SPDRP_FRIENDLYNAME, &DataT, (PBYTE)buf, sizeof(buf), &nSize) ) {
		} else if ( SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData, 
			SPDRP_DEVICEDESC, &DataT, (PBYTE)buf, sizeof(buf), &nSize) ) {
		} else {
			lstrcpy(buf, _T("Unknown"));
		}

		HTREEITEM hClass = GetClassItem(&(spDevInfoData.ClassGuid), wParam);
		// hClass can only be NULL for remove device and class node is not found
		if ( NULL == hClass ) return;
		// remove device
		HTREEITEM hChildItem = m_ctrlTree.GetChildItem(hClass);
		BOOL bFound = FALSE;
		while (hChildItem != NULL) {
			CString* pszData = (CString*)m_ctrlTree.GetItemData(hChildItem);
			if ( szDevId == *pszData  ) {
				bFound = TRUE;
				if ( DBT_DEVICEARRIVAL == wParam ) {
					// add device, but seems the device already exists
					break;
				} else {
					// remove device
					delete pszData;
					m_ctrlTree.DeleteItem(hChildItem);
					if ( !m_ctrlTree.ItemHasChildren(hClass) ) {
						HeapFree(GetProcessHeap(), 0, (LPVOID)m_ctrlTree.GetItemData(hClass));
						m_ctrlTree.DeleteItem(hClass);
					}
					break;
				}
			} else {
				hChildItem = m_ctrlTree.GetNextItem(hChildItem, TVGN_NEXT);
			}
		}
		if ( DBT_DEVICEARRIVAL == wParam && !bFound ) {
			// Add device
			int n = GetClassImgIndex(&(spDevInfoData.ClassGuid));
			HTREEITEM hItem = m_ctrlTree.InsertItem(buf, n, n, hClass);
			m_ctrlTree.SetItemData(hItem, (DWORD_PTR)(new CString(szDevId)));
			// set pspDevInfoData to NULL, otherwise it will be HeapFree
			m_ctrlTree.Expand(hClass, TVE_EXPAND);
		}
	}

	SetupDiDestroyDeviceInfoList(hDevInfo);
}

BOOL CHWDetectDlg::FindDevice(HDEVINFO& hDevInfo, 
							  CString& szDevId, 
							  SP_DEVINFO_DATA& spDevInfoData)
{
	spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for(int i=0; SetupDiEnumDeviceInfo(hDevInfo, i, &spDevInfoData); i++) {
		DWORD nSize=0 ;
		TCHAR buf[MAX_PATH];

		if ( !SetupDiGetDeviceInstanceId(hDevInfo, &spDevInfoData, buf, sizeof(buf), &nSize) ) {
			TRACE(CString("SetupDiGetDeviceInstanceId(): ") 
				+ _com_error(GetLastError()).ErrorMessage());
			return FALSE;
		} 
		if ( szDevId == buf ) {
			// OK, device found
			return TRUE;
		}
	}
	return FALSE;
}

CString CHWDetectDlg::GetClassDesc(const GUID* pGuid)
{
	TCHAR buf[MAX_PATH];
	DWORD size;
	if ( SetupDiGetClassDescription(pGuid, buf, sizeof(buf), &size) ) {
		return CString(buf);
	} else {
		TRACE("Can't get class description: %s", _com_error(GetLastError()).ErrorMessage());
		return _T("");
	}
}

int CHWDetectDlg::GetClassImgIndex(const GUID* pGuid)
{
	int nIdx;
	if ( SetupDiGetClassImageIndex(&m_imgList, pGuid, &nIdx) ) {
		return nIdx;
	} else {
		TRACE("Can't get class image index: %s", _com_error(GetLastError()).ErrorMessage());
		return -1;
	}
}

HTREEITEM CHWDetectDlg::GetClassItem(const GUID* pGuid, WPARAM wParam)
{
	if ( m_ctrlTree.ItemHasChildren(hRoot) ) {
		HTREEITEM hChildItem = m_ctrlTree.GetChildItem(hRoot);
		while (hChildItem != NULL) {
			GUID* tmp = (GUID*)m_ctrlTree.GetItemData(hChildItem);
			if ( IsEqualGUID(*tmp, *pGuid) ) {
				return hChildItem;
			} else {
				hChildItem = m_ctrlTree.GetNextItem(hChildItem, TVGN_NEXT);
			}
		}
	}
	
	if ( DBT_DEVICEARRIVAL == wParam ) {
		// no child or not found
		int n = GetClassImgIndex(pGuid);
		GUID* tmp = (GUID*)HeapAlloc(GetProcessHeap(), 0, sizeof(GUID));
		CopyMemory(tmp, pGuid, sizeof(GUID));
		HTREEITEM hItem = m_ctrlTree.InsertItem(GetClassDesc(pGuid), n, n, hRoot);
		m_ctrlTree.SetItemData(hItem, (DWORD_PTR)tmp);
		m_ctrlTree.Expand(hRoot, TVE_EXPAND);
		return hItem;
	} else {
		return NULL;
	}
}

void CHWDetectDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	hSelectedDev = NULL;
	CPoint scPoint = point;
	m_ctrlTree.ScreenToClient(&scPoint);
	HTREEITEM hItem = m_ctrlTree.HitTest(scPoint);
	if ( NULL != hItem && hRoot != hItem ) {
		HTREEITEM hParent = m_ctrlTree.GetParentItem(hItem);
		if ( hParent != hRoot ) {
			m_ctrlTree.SelectItem(hItem);
			hSelectedDev = hItem;
			m_muPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, 
				point.x, point.y, this);
		}
	}
}

void CHWDetectDlg::OnPopupDisable()
{
	// TODO: Add your command handler code here
	if ( NULL != hSelectedDev ) {
		HTREEITEM hClass = m_ctrlTree.GetParentItem(hSelectedDev);
		GUID* pGuid = (GUID*)m_ctrlTree.GetItemData(hClass);
		ASSERT(pGuid);

		HDEVINFO hDevInfo = SetupDiGetClassDevs(NULL,NULL,NULL,DIGCF_ALLCLASSES);
		if( INVALID_HANDLE_VALUE == hDevInfo ) {
			AfxMessageBox(CString("SetupDiGetClassDevs(): ") 
				+ _com_error(GetLastError()).ErrorMessage(), MB_ICONEXCLAMATION);
			return;
		}

		SP_PROPCHANGE_PARAMS spPropChangeParams;
		spPropChangeParams.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
		spPropChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
		spPropChangeParams.Scope = DICS_FLAG_GLOBAL;
		spPropChangeParams.StateChange = DICS_DISABLE;
		spPropChangeParams.HwProfile = 0;

		CString* pszDevId = (CString*)m_ctrlTree.GetItemData(hSelectedDev);
		SP_DEVINFO_DATA spDevInfoData;
		if ( FindDevice(hDevInfo, *pszDevId, spDevInfoData) ) {
			if ( !SetupDiSetClassInstallParams(hDevInfo, &spDevInfoData, 
				(SP_CLASSINSTALL_HEADER*)&spPropChangeParams, sizeof(spPropChangeParams)) ) {
				AfxMessageBox(CString("SetupDiSetClassInstallParams(): ") 
					+ _com_error(GetLastError()).ErrorMessage(), MB_ICONEXCLAMATION);
			} else if ( !SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, hDevInfo, &spDevInfoData) ) {
				AfxMessageBox(CString("SetupDiClassInstaller(): ") 
					+ _com_error(GetLastError()).ErrorMessage(), MB_ICONEXCLAMATION);
			} else {
				AfxMessageBox(_T("Dislabe OK"), MB_OK);
			}
		}

		SetupDiDestroyDeviceInfoList(hDevInfo);
	}
}
void CHWDetectDlg::OnClearButtonClick() {
	m_ctrlEdit.SetWindowText(L"");
}
// ==============================================================================
// Within this message-handler we must call the HandleAnchors() function
// to get the controls moved
// ==============================================================================
void CHWDetectDlg::OnSize(UINT nType, int cx, int cy) {

	CDialog::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);

	HandleAnchors(&rcWnd);     // you can alternatively pass NULL for &rcWnd
	Invalidate(false);         // this is for ensuring the controls to be redrawn correctly 


}

// ==============================================================================
// ==============================================================================
BOOL CHWDetectDlg::OnEraseBkgnd(CDC* pDC) {

	// Here we call the EraseBackground-Handler from the
	// anchor-map which will reduce the flicker.  

	return(m_bpfxAnchorMap.EraseBackground(pDC->m_hDC));

}

void CHWDetectDlg::SetVisibal(BOOL visibal)
{
	m_bInitVisible = visibal;
}
void CHWDetectDlg::OnExit()
{
	m_bCloseToTray = false;
	exit(0);
}

void CHWDetectDlg::applySetting()
{
	HKEY hkey = NULL;
	long lReturn;
	lReturn = RegCreateKey(HKEY_CURRENT_USER, APP_REG_PATH, &hkey);
	if (lReturn == ERROR_SUCCESS)
	{
		SetVal(hkey, L"bStartInTray", m_bStartInTray);
		SetVal(hkey, L"bCloseToTray", m_bCloseToTray);
		SetVal(hkey, L"bLaunchOnBoot", m_bLaunchOnBoot);
	}
	RegCloseKey(hkey);

	if (m_bLaunchOnBoot)
	{
		TCHAR szPath[_MAX_PATH];
		VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szPath, _MAX_PATH));
		CString csPath(szPath);
		//HKEY_LOCAL_MACHINE : all user
		//HKEY_CURRENT_USER  : current user
		if (ERROR_SUCCESS != RegCreateKey(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey))
		{
			AfxMessageBox(_T("Create regkey for Launch fail"));
			return;
		}
		if (RegSetValueEx(hkey, _T(APP_NAME),
			0, REG_SZ, (BYTE*)csPath.GetBuffer(), 
			csPath.GetLength() * sizeof(TCHAR))!=ERROR_SUCCESS)
		{
			AfxMessageBox(_T("Unable to set registry key"));
			return;
		}
		RegCloseKey(hkey);

	}
	else {
		lReturn = RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
									0L, KEY_ALL_ACCESS, &hkey);
		if (lReturn == ERROR_SUCCESS)
		{
			//remove key
			//TODO: why this will fail
			lReturn = RegDeleteKeyRecursive(hkey, _T(APP_NAME));
			TRACE("%s", lReturn);
		}
		else {
			AfxMessageBox(_T("Unable to remove registry key"));
		}
		RegCloseKey(hkey);
	}

}

void CHWDetectDlg::loadSetting()
{
	HKEY hkey = NULL;
	long lReturn;
	lReturn = RegCreateKey(HKEY_CURRENT_USER, APP_REG_PATH, &hkey);
	if (lReturn == ERROR_SUCCESS)
	{
		m_bStartInTray = GetVal(hkey, L"bStartInTray");
		m_bCloseToTray = GetVal(hkey, L"bCloseToTray");
		m_bLaunchOnBoot = GetVal(hkey, L"bLaunchOnBoot");
		m_bInitVisible = !m_bStartInTray;
	}

}