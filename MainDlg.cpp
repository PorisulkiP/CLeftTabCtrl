#include "framework.h"
#include "MainDlg.h"

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(MainDlg, CDialogEx);

BEGIN_MESSAGE_MAP(MainDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(MainDlg::CLeftTabCtrl, CTabCtrl)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


MainDlg::MainDlg(CWnd* pParent) : CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent), m_szMinimum(0, 0){}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_DB_GLOB_SETTINGS, mainTab);
}

BOOL MainDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// Получаем размер окна, чтобы в дальнейшем нельзя было сделать окно меньше первоначального
	if ((m_szMinimum.cx == 0) && (m_szMinimum.cy == 0))
	{
		CRect rcWindow;
		GetWindowRect(rcWindow);
		m_szMinimum = rcWindow.Size();
	}

	mainTab.InsertItem(0, L"Установки ИТС");
	for (int i = 0; i < 10; ++i)
	{
		CString str; str.Format(L"%d. Пустая вкладка", i);
		mainTab.InsertItem(i, str);
	}

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void MainDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	__super::OnGetMinMaxInfo(lpMMI);

	if (lpMMI->ptMinTrackSize.x < m_szMinimum.cx)
		lpMMI->ptMinTrackSize.x = m_szMinimum.cx;
	if (lpMMI->ptMinTrackSize.y < m_szMinimum.cy)
		lpMMI->ptMinTrackSize.y = m_szMinimum.cy;
}

void MainDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	if (mainTab.GetSafeHwnd())
		mainTab.SetWindowPos(this, 0, 0, cx - mainTab.height, cy - 50, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void MainDlg::OnPaint()
{
	__super::OnPaint();
}

void MainDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Получаем координаыт нажатия
	GetCursorPos(&point);

	// Получаем вкладки
	std::unordered_map<int, CRect> rects;
	CRect tabCtrlRect, rcitem;

	rcitem.left = 0;
	rcitem.right = mainTab.length;
	for (int i = 0, h = 0; i < mainTab.GetItemCount(); ++i)
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
			mainTab.SetCurFocus(i.first);
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
			if (mainTab.GetCurSel() >= mainTab.GetItemCount()) mainTab.SetCurSel(0);
			else mainTab.SetCurSel(mainTab.GetCurSel() + 1);
			break;
		default:
			MessageBox(L"Whatever");
	}
}

//////////////////////////////////////////////////////////////
/// CLeftTabCtrl
void MainDlg::CLeftTabCtrl::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
}

void MainDlg::CLeftTabCtrl::OnPaint()
{
	Invalidate(TRUE); // Очистка перед отрисовкой
	XFORM xform;
	POINT centerPt; // точка, отоносительно которой происходит вращение
	HDC hDc = GetDC()->GetSafeHdc();
	double m_iAngle = 0; // Выравнивание ориентации теста
	double fangle = m_iAngle / 180. * M_PI;

	CPaintDC dc(this);
	dc.SelectObject(GetFont());

	// Переменные для смены цвета объектов
	CPen pen, pen_active;
	COLORREF color_off = RGB(240, 240, 240);
	COLORREF color_active = RGB(200, 240, 240);
	CBrush brush_off, brush_active;
	brush_off.CreateSolidBrush(color_off);
	brush_active.CreateSolidBrush(color_active);
	pen.CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	pen_active.CreatePen(PS_SOLID, 1, color_active);

	CRect rc, rcitem;
	rcitem.right = length; // Устанавливается длинна
	
	GetClientRect(&rc);
	// Должен немного заезжать, чтобы прямоугольники при выделении сливались
	rc.left = length - 1; 
	rc.bottom -= 5; // Чтобы на кнопки не заезжал
	rc.top = 0;

	dc.SelectObject(&pen_active);	// Установка обводки в цвет фона
	dc.SelectObject(&brush_active);	// Установка цвета фона
	dc.Rectangle(&rc);				// Отрисовка прямоугольнка под место настроек

	for (auto i = 0, h = 0; i < GetItemCount(); ++i)
	{
		auto tabCurSel = GetCurSel();

		dc.SelectObject(&pen);
		if (i == tabCurSel)
		{
			dc.SelectObject(&brush_active);
			dc.SetBkColor(color_active);
			dc.SelectObject(pen_active);
			dc.MoveTo(rcitem.left  + 1, rcitem.bottom - 1);
			dc.LineTo(rcitem.right - 1, rcitem.bottom - 1);
		}
		else
		{
			dc.SelectObject(&brush_off);
			dc.SetBkColor(color_off);
		}

		// Всегда находятся на одном и том же уровне по оси Х
		rcitem.left = 0;
		rcitem.top = h;
		rcitem.bottom = h += height;

		dc.Rectangle(&rcitem);

		// Устанавливается небольшой отступ для лучшей читаемости текста
		rcitem.left = 10;

		// Получения данных о текущей вкладки
		TCITEM tcItem{ 0 };
		tcItem.mask = TCIF_TEXT;
		const int len = 256;
		tcItem.cchTextMax = len;
		TCHAR buf[len] = { 0 };
		tcItem.pszText = buf;
		GetItem(i, &tcItem);		

		// Определение точки, относительно которой будет производится поворот
		centerPt.x = (rcitem.right - rcitem.left) / 2;
		centerPt.y = (rcitem.top - rcitem.bottom) / 2;

		// Поворот текста через XFORM
		xform.eM11 = (FLOAT) cos(fangle);
		xform.eM12 = (FLOAT) sin(fangle);
		xform.eM21 = (FLOAT)-sin(fangle);
		xform.eM22 = (FLOAT) cos(fangle);
		xform.eDx  = (FLOAT)(centerPt.x - xform.eM11 * centerPt.x + sin(fangle) * centerPt.y);
		xform.eDy  = (FLOAT)(centerPt.y - xform.eM11 * centerPt.y - sin(fangle) * centerPt.x);

		SetWorldTransform(hDc, &xform);
		SetBkMode(hDc, TRANSPARENT);	// Убирает фон при отрисовке DrawTextEx
		DrawTextEx(hDc, tcItem.pszText, lstrlenW(tcItem.pszText), rcitem, DT_LEFT | DT_VCENTER, NULL);
	}
}