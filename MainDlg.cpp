#include "framework.h"
#include "MainDlg.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(MainDlg, CDialog);

BEGIN_MESSAGE_MAP(MainDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_GETMINMAXINFO()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(MainDlg::CLeftTabCtrl, CMFCTabCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

MainDlg::MainDlg(CWnd* pParent) : CDialog(IDD_MFCAPPLICATION1_DIALOG, pParent){}

MainDlg::~MainDlg()
{
	for (int i = 0; i < mainTab.GetTabsNum(); ++i) 
	{
		CTestDialog* pTest = static_cast<CTestDialog*>(mainTab.GetTabWnd(i));
		delete pTest;
	}
}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_DB_GLOB_SETTINGS, mainTab);
}

BOOL MainDlg::OnInitDialog()
{
	__super::OnInitDialog();

	mainTab.SetParent(this);

	// Получаем размер окна, чтобы в дальнейшем нельзя было сделать окно меньше первоначального
	if ((m_szMinimum.cx == 0) && (m_szMinimum.cy == 0))
	{
		CRect rcWindow;
		GetWindowRect(rcWindow);
		m_szMinimum = rcWindow.Size();
	}

	for (int i = 0; i < 10; ++i)
	{
		CString str;
		str.Format(L"%d. Пустая вкладка", i);

		CTestDialog* pTest = new CTestDialog(str, this);
		VERIFY(pTest->Create(IDD_SPC_TAB_TCI, this));

		// Изменяем стили диалогового окна
		pTest->ModifyStyle(WS_POPUP, WS_CHILD);

		InsertTab(pTest, str, i);
	}


	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void MainDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	__super::OnGetMinMaxInfo(lpMMI);

	if (m_szMinimum.cx > 0 && m_szMinimum.cy > 0)
	{
		lpMMI->ptMinTrackSize.x = m_szMinimum.cx;
		lpMMI->ptMinTrackSize.y = m_szMinimum.cy;
		lpMMI->ptMaxTrackSize.x = m_szMinimum.cx;
		lpMMI->ptMaxTrackSize.y = m_szMinimum.cy;
	}
}

void MainDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Получаем координаты нажатия
	GetCursorPos(&point);

	// Получаем вкладки
	std::unordered_map<int, CRect> rects;
	CRect tabCtrlRect, rcitem;

	rcitem.left = 0;
	rcitem.right = mainTab.length;
	for (int i = 0, h = 0; i < mainTab.GetTabsNum(); ++i)
	{
		rcitem.top = h;
		rcitem.bottom = h += mainTab.height;
		// value_type на 2 операции быстрее, чем make_pair
		rects.insert(std::unordered_map<int, CRect>::value_type(i, rcitem));
	}

	mainTab.GetWindowRect(&tabCtrlRect);
	// Вычитаем из нынешних координат, координаты CLeftTabCtrl
	point.SetPoint(point.x - tabCtrlRect.left, point.y - tabCtrlRect.top);
	// Проверяем попадание курсора в прямоугольники вкладок
	for (auto&& i : rects)
	{
		if (i.second.PtInRect(point))
		{
			mainTab.SetActiveTab(i.first);
			if (auto pTestDialog = dynamic_cast<CTestDialog*>(mainTab.GetTabWnd(i.first)))
			{
				CRect rcClient;
				GetClientRect(&rcClient);
				ClientToScreen(&rcClient);

				// Установка границ диалога со всех сторон на 100 пикселей
				rcClient.DeflateRect(200, 100, 100, 100);

				pTestDialog->SetWindowPos(NULL, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
			}
			break;
		}
	}
}

void MainDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnLButtonDown(nFlags, point); // Для удобства
}

void MainDlg::OnKeyDown(UINT message, UINT nRepCnt, UINT nFlags)
{
	switch (message)
	{
	case VK_TAB:
		// Выделение следующей вкладки по нажатию Tab
		if (mainTab.GetActiveTab() >= mainTab.GetTabsNum())
		{
			mainTab.SetActiveTab(0);
		}
		else
		{
			mainTab.SetActiveTab(mainTab.GetActiveTab() + 1);
		}
		break;
	}
}

void MainDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if (mainTab.GetSafeHwnd())
	{
		CRect mainRect;
		mainTab.GetClientRect(&mainRect);

		mainRect.DeflateRect(200, 100, 100, 100);

		auto pWnd = mainTab.GetTabWnd(mainTab.GetActiveTab());
		if (pWnd)
		{
			::SetWindowPos(pWnd->m_hWnd, HWND_TOP, 0, 0, mainRect.Width(), mainRect.Height(), SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
		}
	}
}

void MainDlg::OnMove(int x, int y)
{
	__super::OnMove(x, y);
	if (mainTab.GetSafeHwnd())
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		ClientToScreen(&rcClient);

		// Установка границ диалога со всех сторон на 100 пикселей
		rcClient.DeflateRect(200, 100, 100, 100);

		for (int i = 0; i < mainTab.GetTabsNum(); i++)
		{
			if (auto pDialog = static_cast<CTestDialog*>(mainTab.GetTabWnd(i)))
			{
				pDialog->SetWindowPos(NULL, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
			}
		}
	}
}

void MainDlg::AddTab(CWnd* pWnd, const CString& strTabName, const UINT uiImageId)
{
	// Изменяем стили диалогового окна
	DWORD dwStyle = pWnd->GetStyle();
	dwStyle &= ~WS_POPUP;      // Удаляем стиль WS_POPUP
	dwStyle |= WS_CHILD;       // Добавляем стиль WS_CHILD
	pWnd->ModifyStyle(0, dwStyle); // Применяем изменения
	mainTab.AddTab(pWnd, strTabName, uiImageId);
}

void MainDlg::InsertTab(CWnd* pWnd, const CString& strTabName, UINT pos, const UINT uiImageId)
{
	// Изменяем стили диалогового окна
	DWORD dwStyle = pWnd->GetStyle();
	dwStyle &= ~WS_POPUP;      // Удаляем стиль WS_POPUP
	dwStyle |= WS_CHILD;       // Добавляем стиль WS_CHILD
	pWnd->ModifyStyle(0, dwStyle); // Применяем изменения
	mainTab.InsertTab(pWnd, strTabName, pos, uiImageId);
}

//////////////////////////////////////////////////////////////
/// CLeftTabCtrl
MainDlg::CLeftTabCtrl::CLeftTabCtrl() : CMFCTabCtrl()
{
	brush_off.CreateSolidBrush(color_off);
	brush_active.CreateSolidBrush(color_active);
	brush_black.CreateSolidBrush(RGB(0, 0, 0));
}

/// <summary>
/// Обработчик сообщения WM_PAINT. Отвечает за отрисовку вкладок. 
/// Вызывается автоматически системой каждый раз, когда требуется обновление содержимого окна.
/// </summary>
void MainDlg::CLeftTabCtrl::OnPaint()
{
	CPaintDC dc(this);
	int save = dc.SaveDC();

	CDC memDC;
	CBitmap memBitmap;

	CRect rc, rcitem, rcClient;
	rcitem.right = length; // Устанавливается длина

	GetClientRect(&rc);
	rcClient = rc;

	memDC.CreateCompatibleDC(&dc);

	memBitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());

	memDC.SelectObject(&memBitmap);
	memDC.FillSolidRect(&rc, m_bgColor);

	// Должен немного заезжать, чтобы прямоугольники при выделении сливались
	rc.left = length - 1;
	rc.bottom -= 5; // Чтобы на кнопки не заезжал
	rc.top = 0;

	memDC.SelectObject(&pen_active);	// Установка обводки в цвет фона
	memDC.SelectObject(&brush_active);	// Установка цвета фона
	memDC.Rectangle(&rc);				// Отрисовка прямоугольника под место настроек

	int tabsNum = GetTabsNum();
	int activeTab = GetActiveTab();

	for (int i = 0; i < tabsNum; ++i)
	{
		bool isActivetab = (i == activeTab);

		// Всегда находятся на одном и том же уровне по оси Х
		rcitem.left = 0;
		rcitem.top = i * height;
		rcitem.bottom = rcitem.top + height;

		memDC.SelectObject(brush_outline);
		memDC.Rectangle(&rcitem);	// Отрисовка обводки

		if (isActivetab)
		{
			memDC.SelectObject(brush_active);
			memDC.SetBkColor(color_active);
			memDC.SelectObject(pen_active);
			//memDC.SelectObject(activeFont);
			//memDC.SetTextColor(color_active);
			rcitem.top += 2;
			rcitem.bottom -= 2;
			rcitem.left += 2;
		}
		else
		{
			memDC.SelectObject(brush_off);
			memDC.SetBkColor(color_off);
			memDC.SelectObject(pen_off);
			//memDC.SelectObject(inactiveFont);
			//memDC.SetTextColor(color_off);
		}
		memDC.Rectangle(&rcitem);	// Отрисовка фона, поверх обводки

		// Устанавливается небольшой отступ для лучшей читаемости текста
		rcitem.left = 5;

		// Получения данных о текущей вкладки
		CString str;
		GetTabLabel(i, str);

		// Определение точки, относительно которой будет производится поворот и
		// поворот текста через XFORM
		xform.eDx = static_cast<FLOAT>(rcitem.Width() / 2.);
		xform.eDy = static_cast<FLOAT>(rcitem.Height() / 2.);

		memDC.SetWorldTransform(&xform);
		memDC.DrawText(str, rcitem, DT_LEFT | DT_VCENTER);
		memDC.SetWorldTransform(NULL);
	}
	TransparentBlt(dc, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), memDC, 0, 0, rcClient.Width(), rcClient.Height(), m_bgColor);

	memDC.DeleteDC();

	dc.RestoreDC(save);
}