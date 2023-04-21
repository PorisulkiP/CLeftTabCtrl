#pragma once
#include "resource.h"		// основные символы
#include <vector>
#include <unordered_map>

constexpr double M_PI = 3.14159265358979323846;

struct sRGB 
{
    int r;  // 0 ... 255
    int g;  // 0 ... 255
    int b;  // 0 ... 255
};

class CTestDialog : public CDialog 
{
public:
    CTestDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = nullptr)
        : CDialog(lpszTemplateName, pParentWnd) {}

    virtual ~CTestDialog() = default;
};

class MainDlg : public CDialog {
    DECLARE_DYNAMIC(MainDlg)

public:
    MainDlg(CWnd* pParentWnd = nullptr);
    virtual ~MainDlg();

    void AddTab(CWnd* pWnd, const CString& strTabName, const UINT uiImageId = 0);
    void InsertTab(CWnd* pWnd, const CString& strTabName, UINT pos, const UINT uiImageId = 0);

protected:
    class CLeftTabCtrl : public CMFCTabCtrl {
    public:
        CLeftTabCtrl();

        static constexpr int height = 30;
        static constexpr int length = 140;
        static constexpr COLORREF m_bgColor = RGB(255, 0, 255);
        COLORREF color_off = 0xb2b2b2;
        COLORREF color_active = 0x2270ff;

    protected:
        CPen pen_off, pen_active;
        CBrush brush_off, brush_active, brush_outline, brush_black;
        sRGB rgb{};
        XFORM xform{};

        afx_msg void OnPaint();
        afx_msg BOOL OnEraseBkgnd(CDC*) { return TRUE; }
        DECLARE_MESSAGE_MAP()
    } mainTab;

    CSize m_szMinimum{};

    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT message, UINT nRepCnt, UINT nFlags);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMove(int x, int y);

    DECLARE_MESSAGE_MAP()
};
