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
	CEdit x_edit_control;
	CEdit y_edit_control;
	CEdit z_edit_control;
	float x_edit_control_value;
	float y_edit_control_value;
	float z_edit_control_value;
	BOOL x_check_box_value;
	BOOL y_check_box_value;
	BOOL z_check_box_value;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedButton1();
	CButton m_get_points_button_control;
	CRichEditCtrl m_rich_edit_control;
	CString m_rich_edit_control_value;
	afx_msg void OnBnClickedMfcbutton2();
	afx_msg void OnBnClickedMfcbutton3();
	CMFCButton increase_point_size_button_control;
	CMFCButton decrease_point_size_button_control;
	afx_msg void OnBnClickedMfcbutton4();
	afx_msg void OnBnClickedMfcbutton6();
	afx_msg void OnBnClickedMfcbutton5();
};
