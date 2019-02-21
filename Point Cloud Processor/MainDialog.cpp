// MainDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Point Cloud Processor.h"
#include "MainDialog.h"
#include "afxdialogex.h"

//******************Newly Added Header Files***********************
#include "CustomAVLTree.h"
//*****************************************************************

//******************Global variables*******************************
extern CustomAVLTree *x_tree;
extern CustomAVLTree *z_tree;
extern CustomAVLTree *y_tree;
struct LinkedListNode *highlighted_points = NULL;
//*****************************************************************

// MainDialog dialog

IMPLEMENT_DYNAMIC(MainDialog, CDialogEx)

MainDialog::MainDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MainDialog, pParent)
	, x_edit_control_value(0)
	, y_edit_control_value(0)
	, z_edit_control_value(0)
	, x_check_box_value(FALSE)
	, y_check_box_value(FALSE)
	, z_check_box_value(FALSE)
{

}

MainDialog::~MainDialog()
{
}

void MainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, x_edit_control);
	DDX_Control(pDX, IDC_EDIT2, y_edit_control);
	DDX_Control(pDX, IDC_EDIT3, z_edit_control);
	DDX_Text(pDX, IDC_EDIT1, x_edit_control_value);
	DDX_Text(pDX, IDC_EDIT2, y_edit_control_value);
	DDX_Text(pDX, IDC_EDIT3, z_edit_control_value);
	DDX_Check(pDX, IDC_CHECK1, x_check_box_value);
	DDX_Check(pDX, IDC_CHECK2, y_check_box_value);
	DDX_Check(pDX, IDC_CHECK3, z_check_box_value);
}


BEGIN_MESSAGE_MAP(MainDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &MainDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK1, &MainDialog::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &MainDialog::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &MainDialog::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_BUTTON1, &MainDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// MainDialog message handlers


void MainDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

BOOL MainDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//***************************************************************************
	CRect rect;

	// Get size and position of the picture control
	GetDlgItem(IDC_OPENGL)->GetWindowRect(rect);

	// Convert screen coordinates to client coordinates
	ScreenToClient(rect);

	// Create OpenGL Control window
	m_oglWindow.oglCreate(rect, this);

	// Setup the OpenGL Window's timer to render
	m_oglWindow.m_unpTimer = m_oglWindow.SetTimer(1, 1, 0);
	//***************************************************************************

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void MainDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	switch (nType)
	{
	case SIZE_RESTORED:
	{
		if (m_oglWindow.m_bIsMaximized)
		{
			m_oglWindow.OnSize(nType, cx, cy);
			m_oglWindow.m_bIsMaximized = false;
		}

		break;
	}

	case SIZE_MAXIMIZED:
	{
		m_oglWindow.OnSize(nType, cx, cy);
		m_oglWindow.m_bIsMaximized = true;

		break;
	}
	}
}


void MainDialog::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	x_edit_control.EnableWindow(x_check_box_value);
}


void MainDialog::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	y_edit_control.EnableWindow(y_check_box_value);
}


void MainDialog::OnBnClickedCheck3()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	z_edit_control.EnableWindow(z_check_box_value);
}


void MainDialog::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if ((x_check_box_value && y_check_box_value) || (y_check_box_value && z_check_box_value) || (x_check_box_value && z_check_box_value)) {
		if (x_check_box_value && y_check_box_value) {
			highlighted_points = x_tree->searchPoint(x_tree->root, x_edit_control_value, y_edit_control_value, (double)9999);
		}
		else if (y_check_box_value && z_check_box_value) {
			highlighted_points = z_tree->searchPoint(z_tree->root, (double)9999, y_edit_control_value, z_edit_control_value);
		}
		else {
			highlighted_points = x_tree->searchPoint(x_tree->root, x_edit_control_value, (double)9999, z_edit_control_value);
		}
	}
	else if(x_check_box_value || y_check_box_value || z_check_box_value) {
		if (x_check_box_value) {
			highlighted_points = x_tree->searchPoint(x_tree->root, x_edit_control_value, (double)9999, (double)9999);
		}
		else if (y_check_box_value) {
			highlighted_points = y_tree->searchPoint(y_tree->root, (double)9999, y_edit_control_value, (double)9999);
		}
		else {
			highlighted_points = z_tree->searchPoint(z_tree->root, (double)9999, (double)9999, z_edit_control_value);
		}
	}
	else {
		highlighted_points = x_tree->searchPoint(x_tree->root, x_edit_control_value, y_edit_control_value, z_edit_control_value);
	}
	UpdateData(false);
}
