
// Point Cloud ProcessorDlg.h : header file
//

#pragma once


// CPointCloudProcessorDlg dialog
class CPointCloudProcessorDlg : public CDialogEx
{
// Construction
public:
	CPointCloudProcessorDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POINTCLOUDPROCESSOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_edit_browse_control_string;
	afx_msg void OnBnClickedButton2();
	CButton m_load_file_button_control;
};
