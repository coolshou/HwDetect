// GlobalFunctions.cpp: Implementierung der Klasse CGlobalFunctions.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobalFunctions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

struct LANGANDCODEPAGE {
	WORD wLanguage;
	WORD wCodePage;
} *lpTranslate;

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CString CGlobalFunctions::m_csFileVersion;
CString CGlobalFunctions::m_csProductVersion;

CGlobalFunctions::CGlobalFunctions()
{

}

CGlobalFunctions::~CGlobalFunctions()
{

}

/***********************************************************************************/
/*                                                                                 */
/* Class:   CGlobalFunctions                                                       */
/* Method:  GetVersionInfo                                                         */
/*                                                                                 */
/* Parameters:                                                                     */
/* -----------                                                                     */
/*   HMODULE hLib                                                                  */
/*                Handle to the module that contains the resource (EXE or DLL)     */            
/*                A value of NULL specifies the current applications resources     */ 
/*                                                                                 */
/*   CString csEntry                                                               */
/*                Specifies the name of the resource. For more information,        */
/*                see the Remarks section.                                         */
/*                                                                                 */
/* Return Values:                                                                  */
/* --------------                                                                  */
/* If the function succeeds, the return value is a string containing the value     */
/* of the specified resource.                                                      */ 
/* If the function fails, the returned string is empty. To get extended error      */
/* information, call GetLastError.                                                 */
/*                                                                                 */
/* Remarks:                                                                        */
/* --------                                                                        */
/* Since the Win32 API resource information is encoded in Unicode, this method     */
/* also strips the strings from Unicode.                                           */
/*                                                                                 */
/* The following valid values for csEntry, as specified by Microsoft are:          */
/*   CompanyName, FileDescription, FileVersion, InternalName, LegalCopyright,      */
/*   OriginalFilename, ProductName, ProductVersion, Comments, LegalTrademarks,     */
/*   PrivateBuild, SpecialBuild                                                    */
/*                                                                                 */
/* Opening the rc-file as "text" or with a text-editor allows you to add further   */
/* entries to your version information structure and it is retrievable using       */
/* this same method.                                                               */
/*                                                                                 */
/***********************************************************************************/

CString CGlobalFunctions::GetVersionInfo(HMODULE hLib, CString csEntry)
{
  CString csRet;

  if (hLib == NULL)
    hLib = AfxGetResourceHandle();
  
  HRSRC hVersion = FindResource( hLib, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION );
  if (hVersion != NULL)
  {
    HGLOBAL hGlobal = LoadResource( hLib, hVersion ); 
    if ( hGlobal != NULL)  
    {  
  
      LPVOID versionInfo  = LockResource(hGlobal);  
      if (versionInfo != NULL)
      {
		  DWORD vLen;
		  //DWORD langD;
        BOOL retVal;    
    
        LPVOID retbuf=NULL;

		const int iSize = 256;
		WCHAR czEntry[iSize];
		wsprintf(czEntry, (L"%s"), csEntry);

		static wchar_t fileEntry[iSize];

		swprintf_s(fileEntry, iSize, L"\\VarFileInfo\\Translation");
		// Read the list of languages and code pages.
        retVal = VerQueryValue(versionInfo,fileEntry, (LPVOID*)&lpTranslate,(UINT *)&vLen);
        if (retVal && vLen==4) 
        {
		  swprintf_s(fileEntry, iSize, L"\\StringFileInfo\\%04x%04x\\%s",
			  lpTranslate[0].wLanguage, lpTranslate[0].wCodePage
			  , czEntry);
        }
        else 
			swprintf_s(fileEntry, iSize, L"\\StringFileInfo\\%04X04B0\\%s",  
				GetUserDefaultLangID(), czEntry);

		// Retrieve czEntry for language and code page. 
        if (VerQueryValue(versionInfo,fileEntry,&retbuf,(UINT *)&vLen)) 
          csRet = (wchar_t*)retbuf;
      }
    }

    UnlockResource( hGlobal );  
    FreeResource( hGlobal );  
  }

  return csRet;
}

/***********************************************************************************/
/*                                                                                 */
/* Class:   CGlobalFunctions                                                       */
/* Method:  FormatVersion                                                          */
/*                                                                                 */
/* Parameters:                                                                     */
/* -----------                                                                     */
/*   CString cs                                                                    */
/*                Specifies a version number such as "FileVersion" or              */
/*                "ProductVersion" in the format "m, n, o, p"                      */
/*                (e.g. "1, 2, 3, a")                                              */
/*                                                                                 */
/* Return Values:                                                                  */
/* --------------                                                                  */
/* If the function succeeds, the return value is a string containing the version   */
/* in the format "m.nop" (e.g. "1.23a")                                            */ 
/*                                                                                 */
/* If the function fails, the returned string is empty.                            */
/*                                                                                 */
/***********************************************************************************/
CString CGlobalFunctions::FormatVersion(CString cs)
{
  CString csRet;
  if (!cs.IsEmpty())
  {
    cs.TrimRight();
    int iPos = cs.Find(',');
    if (iPos == -1)
      return L"";
    cs.TrimLeft();
    cs.TrimRight();
    csRet.Format(L"%s.", cs.Left(iPos));

    while (1)
    {
      cs = cs.Mid(iPos + 1);
      cs.TrimLeft();
      iPos = cs.Find(',');
      if (iPos == -1)
      {
        csRet +=cs;
        break;
      }
      csRet += cs.Left(iPos);
    }
  }

  return csRet;
}

/***********************************************************************************/
/*                                                                                 */
/* Class:   CGlobalFunctions                                                       */
/* Method:  GetFileVersionX                                                        */
/*                                                                                 */
/* Parameters:                                                                     */
/* -----------                                                                     */
/*                                                                                 */
/* Return Values:                                                                  */
/* --------------                                                                  */
/* If the function succeeds, the return value is a string containing the           */
/* "FileVersion" in the format "m.nop" (e.g. "1.23a")                              */ 
/*                                                                                 */
/* If the function fails, the returned string is empty.                            */
/*                                                                                 */
/***********************************************************************************/
CString CGlobalFunctions::GetFileVersionX()
{
  if (m_csFileVersion.IsEmpty())
  {
    //CString csVersion = FormatVersion(GetVersionInfo(NULL, L"FileVersion"));
	  CString csVersion = GetVersionInfo(NULL, L"FileVersion");
	//  m_csFileVersion.Format(L"Version %s (Build %s)", csVersion, GetVersionInfo(NULL, L"SpecialBuild"));
	  m_csFileVersion.Format(L"%s", csVersion);
  }

  return m_csFileVersion;
}

/***********************************************************************************/
/*                                                                                 */
/* Class:   CGlobalFunctions                                                       */
/* Method:  GetFileVersionX                                                        */
/*                                                                                 */
/* Parameters:                                                                     */
/* -----------                                                                     */
/*                                                                                 */
/* Return Values:                                                                  */
/* --------------                                                                  */
/* If the function succeeds, the return value is a string containing the           */
/* "ProductVersion" in the format "m.nop" (e.g. "1.23a")                           */ 
/*                                                                                 */
/* If the function fails, the returned string is empty.                            */
/*                                                                                 */
/***********************************************************************************/
CString CGlobalFunctions::GetProductVersionX()
{
  if (m_csProductVersion.IsEmpty())
    m_csProductVersion = FormatVersion(GetVersionInfo(NULL, L"ProductVersion"));

  return m_csProductVersion;
}