#include "framework.h"
#include "resource.h"		// основные символы
#include "MainDlg.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

class CMFCApplication1App : public CWinApp
{
public:
	CMFCApplication1App(){}
	virtual BOOL InitInstance();
};

CMFCApplication1App theApp;

BOOL CMFCApplication1App::InitInstance()
{
	__super::InitInstance();

	MainDlg dlg;
	dlg.DoModal();
	return FALSE;
}

