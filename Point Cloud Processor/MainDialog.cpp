// MainDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Point Cloud Processor.h"
#include "MainDialog.h"
#include "afxdialogex.h"

//******************Newly Added Header Files***********************
#include "CustomAVLTree.h"
#include <fstream>
#include <string>
//*****************************************************************

//******************Global variables*******************************
extern CustomAVLTree *x_tree;
extern CustomAVLTree *z_tree;
extern CustomAVLTree *y_tree;
extern CString file_path;
extern GLfloat cube_size_offset;
struct LinkedListNode *highlighted_points = NULL;
struct LinkedListNode* highlight_single_point = (struct LinkedListNode*)malloc(sizeof(struct LinkedListNode));
struct LinkedListNode* ptr_single_point = NULL;
struct LinkedListNodeForHighlighting* original_linked_list_of_points = NULL;
struct LinkedListNodeForHighlighting* highlighted_linked_list_head_node = NULL;
bool refresh_button_clicked = false;
bool zoom_in_clicked = false;
bool zoom_out_clicked = false;
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
	, m_rich_edit_control_value(_T(""))
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
	DDX_Control(pDX, IDC_BUTTON1, m_get_points_button_control);
	DDX_Control(pDX, IDC_RICHEDIT21, m_rich_edit_control);
	DDX_Text(pDX, IDC_RICHEDIT21, m_rich_edit_control_value);
	DDX_Control(pDX, IDC_MFCBUTTON2, increase_point_size_button_control);
	DDX_Control(pDX, IDC_MFCBUTTON3, decrease_point_size_button_control);
	DDX_Control(pDX, IDC_MFCBUTTON7, move_left_button_control);
	DDX_Control(pDX, IDC_MFCBUTTON8, move_right_button_control);
	DDX_Control(pDX, IDC_MFCBUTTON9, move_fast_right_button_control);
	DDX_Control(pDX, IDC_MFCBUTTON10, move_fast_left_button_control);
	DDX_Control(pDX, IDC_MFCBUTTON12, move_forward_button_control);
	DDX_Control(pDX, IDC_MFCBUTTON11, move_backward_button_control);
	DDX_Control(pDX, IDC_MFCBUTTON13, move_forward_fast_button_control);
	DDX_Control(pDX, IDC_MFCBUTTON14, move_backward_fast_button_control);
}


BEGIN_MESSAGE_MAP(MainDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &MainDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK1, &MainDialog::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &MainDialog::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &MainDialog::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_BUTTON1, &MainDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_MFCBUTTON2, &MainDialog::OnBnClickedMfcbutton2)
	ON_BN_CLICKED(IDC_MFCBUTTON3, &MainDialog::OnBnClickedMfcbutton3)
	ON_BN_CLICKED(IDC_MFCBUTTON4, &MainDialog::OnBnClickedMfcbutton4)
	ON_BN_CLICKED(IDC_MFCBUTTON6, &MainDialog::OnBnClickedMfcbutton6)
	ON_BN_CLICKED(IDC_MFCBUTTON5, &MainDialog::OnBnClickedMfcbutton5)
	ON_BN_CLICKED(IDC_MFCBUTTON7, &MainDialog::OnBnClickedMfcbutton7)
	ON_BN_CLICKED(IDC_MFCBUTTON8, &MainDialog::OnBnClickedMfcbutton8)
	ON_BN_CLICKED(IDC_MFCBUTTON9, &MainDialog::OnBnClickedMfcbutton9)
	ON_BN_CLICKED(IDC_MFCBUTTON10, &MainDialog::OnBnClickedMfcbutton10)
	ON_BN_CLICKED(IDC_MFCBUTTON12, &MainDialog::OnBnClickedMfcbutton12)
	ON_BN_CLICKED(IDC_MFCBUTTON11, &MainDialog::OnBnClickedMfcbutton11)
	ON_BN_CLICKED(IDC_MFCBUTTON13, &MainDialog::OnBnClickedMfcbutton13)
	ON_BN_CLICKED(IDC_MFCBUTTON14, &MainDialog::OnBnClickedMfcbutton14)
	ON_BN_CLICKED(IDC_BUTTON2, &MainDialog::OnBnClickedButton2)
	ON_WM_KEYDOWN()
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

	//Rendering root nodes points at startup
	original_linked_list_of_points = x_tree->inOrder(x_tree->root);
	highlighted_linked_list_head_node = original_linked_list_of_points;
	highlighted_points = highlighted_linked_list_head_node->head_node;
	ptr_single_point = highlighted_linked_list_head_node->head_node;
	x_edit_control_value = highlighted_linked_list_head_node->coordinate_value;
	increase_point_size_button_control.EnableWindow(true);
	move_left_button_control.EnableWindow(false);
	move_fast_left_button_control.EnableWindow(false);
	move_backward_button_control.EnableWindow(false);
	move_backward_fast_button_control.EnableWindow(false);

	//Rich Edit Control content
	CString file_content;
	std::ifstream fin(file_path);
	std::string line;
	if (fin.is_open()) {
		while (std::getline(fin, line)) {
			file_content += CString(line.c_str()) + CString("\n");
		}
		fin.close();
	}
	m_rich_edit_control_value = file_content;
	UpdateData(false);

	//CWnd::SetFocus();

	//***************************************************************************
	ShowWindow(SW_SHOW);
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
	m_get_points_button_control.EnableWindow(x_check_box_value || y_check_box_value || z_check_box_value);
	//CWnd::SetFocus();
	//UpdateData(false);
}


void MainDialog::OnBnClickedCheck2()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	y_edit_control.EnableWindow(y_check_box_value);
	m_get_points_button_control.EnableWindow(x_check_box_value || y_check_box_value || z_check_box_value);
	//CWnd::SetFocus();
	//UpdateData(false);
}


void MainDialog::OnBnClickedCheck3()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	z_edit_control.EnableWindow(z_check_box_value);
	m_get_points_button_control.EnableWindow(x_check_box_value || y_check_box_value || z_check_box_value);
	//CWnd::SetFocus();
	//UpdateData(false);
}


void MainDialog::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (x_check_box_value && y_check_box_value && z_check_box_value) {
		highlighted_points = x_tree->searchPoint(x_tree->root, x_edit_control_value, y_edit_control_value, z_edit_control_value);
	}
	else {
		if ((x_check_box_value && y_check_box_value) || (y_check_box_value && z_check_box_value) || (x_check_box_value && z_check_box_value)) {
			if (x_check_box_value && y_check_box_value) {
				highlighted_points = x_tree->searchPoint(x_tree->root, x_edit_control_value, y_edit_control_value, (float)9999);
				if (highlighted_points != NULL) {
					z_edit_control_value = highlighted_points->z;
				}
				else {
					AfxMessageBox(L"Could not find X or Y coordinate in the point cloud");
				}
			}
			else if (y_check_box_value && z_check_box_value) {
				highlighted_points = z_tree->searchPoint(z_tree->root, (float)9999, y_edit_control_value, z_edit_control_value);
				if (highlighted_points != NULL) {
					x_edit_control_value = highlighted_points->x;
				}
				else {
					AfxMessageBox(L"Could not find Y or Z coordinate in the point cloud");
				}
			}
			else {
				highlighted_points = x_tree->searchPoint(x_tree->root, x_edit_control_value, (float)9999, z_edit_control_value);
				if (highlighted_points != NULL) {
					y_edit_control_value = highlighted_points->y;
				}
				else {
					AfxMessageBox(L"Could not find X or Z coordinate in the point cloud");
				}
			}
		}
		else if (x_check_box_value || y_check_box_value || z_check_box_value) {
			if (x_check_box_value) {
				highlighted_points = x_tree->searchPoint(x_tree->root, x_edit_control_value, (float)9999, (float)9999);
				if (highlighted_points == NULL) {
					AfxMessageBox(L"Could not find the X coordinate in the point cloud");
				}
			}
			else if (y_check_box_value) {
				highlighted_points = y_tree->searchPoint(y_tree->root, (float)9999, y_edit_control_value, (float)9999);
				if (highlighted_points == NULL) {
					AfxMessageBox(L"Could not find the Y coordinate in the point cloud");
				}
			}
			else {
				highlighted_points = z_tree->searchPoint(z_tree->root, (float)9999, (float)9999, z_edit_control_value);
				if (highlighted_points == NULL) {
					AfxMessageBox(L"Could not find the Z coordinate in the point cloud");
				}
			}
		}
		else {
			highlighted_points = x_tree->searchPoint(x_tree->root, x_edit_control_value, y_edit_control_value, z_edit_control_value);

		}
	}
	increase_point_size_button_control.EnableWindow(highlighted_points != NULL);
	decrease_point_size_button_control.EnableWindow(false);
	//CWnd::SetFocus();
	UpdateData(false);
}




void MainDialog::OnBnClickedMfcbutton2()
{
	// TODO: Add your control notification handler code here
	cube_size_offset += 0.1;
	decrease_point_size_button_control.EnableWindow(true);
	CWnd::SetFocus();
}


void MainDialog::OnBnClickedMfcbutton3()
{
	// TODO: Add your control notification handler code here
	if (cube_size_offset >= (GLfloat)0.2) {
		cube_size_offset -= 0.1;
	}
	else {
		decrease_point_size_button_control.EnableWindow(false);
	}
	CWnd::SetFocus();
}


void MainDialog::OnBnClickedMfcbutton4()
{
	// TODO: Add your control notification handler code here
	refresh_button_clicked = true;
	highlighted_linked_list_head_node = original_linked_list_of_points;
	highlighted_points = highlighted_linked_list_head_node->head_node;
	ptr_single_point = highlighted_linked_list_head_node->head_node;
	move_left_button_control.EnableWindow(false);
	move_fast_left_button_control.EnableWindow(false);
	move_backward_button_control.EnableWindow(false);
	move_backward_fast_button_control.EnableWindow(false);
	CWnd::SetFocus();
}


void MainDialog::OnBnClickedMfcbutton6()
{
	// TODO: Add your control notification handler code here
	zoom_in_clicked = true;
	CWnd::SetFocus();
}


void MainDialog::OnBnClickedMfcbutton5()
{
	// TODO: Add your control notification handler code here
	
	zoom_out_clicked = true;
	CWnd::SetFocus();
}


void MainDialog::OnBnClickedMfcbutton7()
{
	// TODO: Add your control notification handler code here
	if (highlighted_linked_list_head_node->left_next != NULL) {
		highlighted_linked_list_head_node = highlighted_linked_list_head_node->left_next;
		highlighted_points = highlighted_linked_list_head_node->head_node;
		ptr_single_point = highlighted_linked_list_head_node->head_node;
		move_right_button_control.EnableWindow(true);
		move_fast_right_button_control.EnableWindow(true);
	}
	if (highlighted_linked_list_head_node->left_next == NULL) {
		move_left_button_control.EnableWindow(false);
		move_fast_left_button_control.EnableWindow(false);
	}
	CWnd::SetFocus();
}


void MainDialog::OnBnClickedMfcbutton8()
{
	// TODO: Add your control notification handler code here
	if (highlighted_linked_list_head_node->right_next != NULL) {
		highlighted_linked_list_head_node = highlighted_linked_list_head_node->right_next;
		highlighted_points = highlighted_linked_list_head_node->head_node;
		ptr_single_point = highlighted_linked_list_head_node->head_node;
		move_left_button_control.EnableWindow(true);
		move_fast_left_button_control.EnableWindow(true);
	}
	if (highlighted_linked_list_head_node->right_next == NULL) {
		move_right_button_control.EnableWindow(false);
		move_fast_right_button_control.EnableWindow(false);
	}
	CWnd::SetFocus();
}


void MainDialog::OnBnClickedMfcbutton9()
{
	// TODO: Add your control notification handler code here
	if (highlighted_linked_list_head_node->right_next != NULL) {
		for (int i = 0;i < 20 && highlighted_linked_list_head_node->right_next != NULL;i++) {
			highlighted_linked_list_head_node = highlighted_linked_list_head_node->right_next;
		}
		highlighted_points = highlighted_linked_list_head_node->head_node;
		ptr_single_point = highlighted_linked_list_head_node->head_node;
		move_left_button_control.EnableWindow(true);
		move_fast_left_button_control.EnableWindow(true);
	}
	if (highlighted_linked_list_head_node->right_next == NULL) {
		move_right_button_control.EnableWindow(false);
		move_fast_right_button_control.EnableWindow(false);
	}
	CWnd::SetFocus();
}


void MainDialog::OnBnClickedMfcbutton10()
{
	// TODO: Add your control notification handler code here
	if (highlighted_linked_list_head_node->left_next != NULL) {
		for (int i = 0;i < 20 && highlighted_linked_list_head_node->left_next != NULL;i++) {
			highlighted_linked_list_head_node = highlighted_linked_list_head_node->left_next;
		}
		highlighted_points = highlighted_linked_list_head_node->head_node;
		ptr_single_point = highlighted_linked_list_head_node->head_node;
		move_right_button_control.EnableWindow(true);
		move_fast_right_button_control.EnableWindow(true);
	}
	if (highlighted_linked_list_head_node->left_next == NULL) {
		move_left_button_control.EnableWindow(false);
		move_fast_left_button_control.EnableWindow(false);
	}
	CWnd::SetFocus();
}


void MainDialog::OnBnClickedMfcbutton12()
{
	// TODO: Add your control notification handler code here
	// move forward button
	if (ptr_single_point->right_next != NULL) {
		ptr_single_point = ptr_single_point->right_next;
		highlight_single_point->x = ptr_single_point->x;
		highlight_single_point->y = ptr_single_point->y;
		highlight_single_point->z = ptr_single_point->z;
		highlight_single_point->left_next = NULL;
		highlight_single_point->right_next = NULL;
		highlighted_points = highlight_single_point;
		move_backward_button_control.EnableWindow(true);
		move_backward_fast_button_control.EnableWindow(true);
	}
	if (ptr_single_point->right_next == NULL) {
		move_forward_button_control.EnableWindow(false);
		move_forward_fast_button_control.EnableWindow(false);
	}
	CWnd::SetFocus();
}


void MainDialog::OnBnClickedMfcbutton11()
{
	// TODO: Add your control notification handler code here
	// move backward button
	if (ptr_single_point->left_next != NULL) {
		ptr_single_point = ptr_single_point->left_next;
		highlight_single_point->x = ptr_single_point->x;
		highlight_single_point->y = ptr_single_point->y;
		highlight_single_point->z = ptr_single_point->z;
		highlight_single_point->left_next = NULL;
		highlight_single_point->right_next = NULL;
		highlighted_points = highlight_single_point;
		move_forward_button_control.EnableWindow(true);
		move_forward_fast_button_control.EnableWindow(true);
	}
	if (ptr_single_point->left_next == NULL) {
		move_backward_button_control.EnableWindow(false);
		move_backward_fast_button_control.EnableWindow(false);
	}
	CWnd::SetFocus();
}


void MainDialog::OnBnClickedMfcbutton13()
{
	// TODO: Add your control notification handler code here
	// move forward fast
	if (ptr_single_point->right_next != NULL) {
		for (int i = 0;i < 20 && ptr_single_point->right_next != NULL;i++) {
			ptr_single_point = ptr_single_point->right_next;
		}
		highlight_single_point->x = ptr_single_point->x;
		highlight_single_point->y = ptr_single_point->y;
		highlight_single_point->z = ptr_single_point->z;
		highlight_single_point->left_next = NULL;
		highlight_single_point->right_next = NULL;
		highlighted_points = highlight_single_point;
		move_backward_button_control.EnableWindow(true);
		move_backward_fast_button_control.EnableWindow(true);
	}
	if (ptr_single_point->right_next == NULL) {
		move_forward_button_control.EnableWindow(false);
		move_forward_fast_button_control.EnableWindow(false);
	}
	CWnd::SetFocus();
}


void MainDialog::OnBnClickedMfcbutton14()
{
	// TODO: Add your control notification handler code here
	// move backward fast
	if (ptr_single_point->left_next != NULL) {
		for (int i = 0;i < 20 && ptr_single_point->left_next != NULL;i++) {
			ptr_single_point = ptr_single_point->left_next;
		}
		highlight_single_point->x = ptr_single_point->x;
		highlight_single_point->y = ptr_single_point->y;
		highlight_single_point->z = ptr_single_point->z;
		highlight_single_point->left_next = NULL;
		highlight_single_point->right_next = NULL;
		highlighted_points = highlight_single_point;
		move_forward_button_control.EnableWindow(true);
		move_forward_fast_button_control.EnableWindow(true);
	}
	if (ptr_single_point->left_next == NULL) {
		move_backward_button_control.EnableWindow(false);
		move_backward_fast_button_control.EnableWindow(false);
	}
	CWnd::SetFocus();
}


void MainDialog::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	//Show values of highlighted points.
	if (highlighted_points->right_next == NULL) {
		x_edit_control_value = ptr_single_point->x;
		y_edit_control_value = ptr_single_point->y;
		z_edit_control_value = ptr_single_point->z;
	}
	else {
		x_edit_control_value = ptr_single_point->x;
		y_edit_control_value = 0;
		z_edit_control_value = 0;
	}
	//CWnd::SetFocus();
	UpdateData(false);
}


void MainDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	//implemented below using pretranslatemessage function.

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL MainDialog::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_LEFT)
		{
			if (highlighted_linked_list_head_node->left_next != NULL) {
				for (int i = 0;i < 5 && highlighted_linked_list_head_node->left_next != NULL;i++) {
					highlighted_linked_list_head_node = highlighted_linked_list_head_node->left_next;
				}
				highlighted_points = highlighted_linked_list_head_node->head_node;
				ptr_single_point = highlighted_linked_list_head_node->head_node;
				move_right_button_control.EnableWindow(true);
				move_fast_right_button_control.EnableWindow(true);
			}
			if (highlighted_linked_list_head_node->left_next == NULL) {
				move_left_button_control.EnableWindow(false);
				move_fast_left_button_control.EnableWindow(false);
			}
		}
		if (pMsg->wParam == VK_RIGHT)
		{
			if (highlighted_linked_list_head_node->right_next != NULL) {
				for (int i = 0;i < 5 && highlighted_linked_list_head_node->right_next != NULL;i++) {
					highlighted_linked_list_head_node = highlighted_linked_list_head_node->right_next;
				}
				highlighted_points = highlighted_linked_list_head_node->head_node;
				ptr_single_point = highlighted_linked_list_head_node->head_node;
				move_left_button_control.EnableWindow(true);
				move_fast_left_button_control.EnableWindow(true);
			}
			if (highlighted_linked_list_head_node->right_next == NULL) {
				move_right_button_control.EnableWindow(false);
				move_fast_right_button_control.EnableWindow(false);
			}
		}
		if (pMsg->wParam == VK_UP)
		{
			if (ptr_single_point->right_next != NULL) {
				for (int i = 0;i < 5 && ptr_single_point->right_next != NULL;i++) {
					ptr_single_point = ptr_single_point->right_next;
				}
				highlight_single_point->x = ptr_single_point->x;
				highlight_single_point->y = ptr_single_point->y;
				highlight_single_point->z = ptr_single_point->z;
				highlight_single_point->left_next = NULL;
				highlight_single_point->right_next = NULL;
				highlighted_points = highlight_single_point;
				move_backward_button_control.EnableWindow(true);
				move_backward_fast_button_control.EnableWindow(true);
			}
			if (ptr_single_point->right_next == NULL) {
				move_forward_button_control.EnableWindow(false);
				move_forward_fast_button_control.EnableWindow(false);
			}
		}
		if (pMsg->wParam == VK_DOWN)
		{
			if (ptr_single_point->left_next != NULL) {
				for (int i = 0;i < 5 && ptr_single_point->left_next != NULL;i++) {
					ptr_single_point = ptr_single_point->left_next;
				}
				highlight_single_point->x = ptr_single_point->x;
				highlight_single_point->y = ptr_single_point->y;
				highlight_single_point->z = ptr_single_point->z;
				highlight_single_point->left_next = NULL;
				highlight_single_point->right_next = NULL;
				highlighted_points = highlight_single_point;
				move_forward_button_control.EnableWindow(true);
				move_forward_fast_button_control.EnableWindow(true);
			}
			if (ptr_single_point->left_next == NULL) {
				move_backward_button_control.EnableWindow(false);
				move_backward_fast_button_control.EnableWindow(false);
			}
		}
	}
	//CWnd::SetFocus();
	return CDialog::PreTranslateMessage(pMsg);
}
