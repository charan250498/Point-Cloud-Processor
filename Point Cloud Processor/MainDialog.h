#pragma once

//*************Newlly Added header file*****************
#include "OpenGLControl.h"
//******************************************************


// MainDialog dialog

class MainDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MainDialog)
	
private:
	OpenGLControl m_oglWindow;

public:
	MainDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MainDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MainDialog };
#endif

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
