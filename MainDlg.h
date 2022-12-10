#pragma once
#include "resource.h"		// основные символы
#include <vector>
#include <unordered_map>

constexpr double M_PI = 3.14159265358979323846;

// Диалоговое окно MainDlg
class MainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MainDlg)
public:
	MainDlg(CWnd* pParent = nullptr);	// стандартный конструктор
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };

	//CTabCtrl, у которого вкладки находятся слева и текст на вкладках горизонтален
	class CLeftTabCtrl : public CTabCtrl
	{
		public: 
			CLeftTabCtrl() : CTabCtrl() {}
			const int height = 25;
			const int length = 140;
		protected:
			afx_msg void OnPaint();
			afx_msg void OnSize(UINT nType, int cx, int cy);
			afx_msg BOOL OnEraseBkgnd(CDC*){ return TRUE; }
			DECLARE_MESSAGE_MAP()
	} mainTab;

protected:
	CSize           m_szMinimum;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV
	afx_msg void OnSize(UINT nType, int cx, int cy);	// при масштабировании
	afx_msg void OnPaint();								// при отрисовке
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);// Ограничевает уменьшение окна
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT message, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()
};
