#include "stdafx.h"
#include "OpenGLControl.h"
//****************Newly Added Header Files********************
#include "CustomAVLTree.h"
#include <fstream>
//************************************************************

//*****************Global Variables of other files************
extern CString file_path;
extern struct LinkedListNode *highlighted_points ;
CustomAVLTree *x_tree = new CustomAVLTree();
CustomAVLTree *z_tree = new CustomAVLTree();
CustomAVLTree *y_tree = new CustomAVLTree();
struct LinkedListNode *highlight_points = NULL;
float x_min = 9999, x_max = -9999;
float y_min = 9999, y_max = -9999;
float z_min = 9999, z_max = -9999;

//Zoom and refresh
extern bool refresh_button_clicked;
extern bool zoom_in_clicked;
extern bool zoom_out_clicked;

//determines the cube size
GLfloat cube_size_offset = 0.1f;

float x_offset, y_offset, z_offset;

void renderPoints(struct CustomAVLTreeNode*);
//************************************************************

OpenGLControl::OpenGLControl()
{
	m_fPosX = 0.0f;    // X position of model in camera view
	m_fPosY = 0.0f;    // Y position of model in camera view
	m_fZoom = 50.0f;   // Zoom on model in camera view
	m_fRotX = 0.0f;    // Rotation on model in camera view
	m_fRotY = 0.0f;    // Rotation on model in camera view
}


OpenGLControl::~OpenGLControl()
{
}

BEGIN_MESSAGE_MAP(OpenGLControl, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void OpenGLControl::oglCreate(CRect rect, CWnd *parent)
{
	CString className = AfxRegisterWndClass(CS_HREDRAW |
		CS_VREDRAW | CS_OWNDC, NULL,
		(HBRUSH)GetStockObject(BLACK_BRUSH), NULL);

	CreateEx(0, className, _T("OpenGL"), WS_CHILD | WS_VISIBLE |
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, parent, 0);

	// Set initial variables' values
	m_oldWindow = rect;
	m_originalRect = rect;

	hWnd = parent;
}

void OpenGLControl::OnPaint()
{
	//CPaintDC dc(this);    // device context for painting
	ValidateRect(NULL);
}

int OpenGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	oglInitialize();

	return 0;
}

void OpenGLControl::oglInitialize(void)
{
	// Initial Setup:
	//
	static PIXELFORMATDESCRIPTOR pfd =
	{
	   sizeof(PIXELFORMATDESCRIPTOR),
	   1,
	   PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
	   PFD_TYPE_RGBA,
	   32,    // bit depth
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   16,    // z-buffer depth
	   0, 0, 0, 0, 0, 0, 0,
	};

	// Get device context only once.
	hdc = GetDC()->m_hDC;

	// Pixel format.
	m_nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, m_nPixelFormat, &pfd);

	// Create the OpenGL Rendering Context.
	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	//**********************Time Estimation**************************************************
	/*SYSTEMTIME start_time;
	SYSTEMTIME end_time;
	GetSystemTime(&start_time);*/
	//***************************************************************************************

	//**********************Reading Point Cloud into Customized AVL Tree*********************
	float x, y, z;
	int intensity;
	std::fstream fin;
	fin.open(file_path);
	while (fin >> z >> x >> y >> intensity) {
		//*********************getting max and min values of coordinates*********************
		x_min = x > x_min ? x_min : x;
		x_max = x > x_max ? x : x_max;
		y_min = y > y_min ? y_min : y;
		y_max = y > y_max ? y : y_max;
		z_min = z > z_min ? z_min : z;
		z_max = z > z_max ? z : z_max;
		//***********************************************************************************
		x_tree->root = x_tree->insert(x_tree->root, x, x, y, z);
		z_tree->root = z_tree->insert(z_tree->root, z, x, y, z);
		y_tree->root = y_tree->insert(y_tree->root, y, x, y, z);
	}
	fin.close();

	//***********Time Estimation************
	/*GetSystemTime(&end_time);
	std::fstream fout;
	fout.open("log.txt", 'w');
	fout << "start time" << start_time.wSecond << "s" << start_time.wMilliseconds << "ms";
	fout << "end time" << end_time.wSecond << "s" << end_time.wMilliseconds << "ms";
	fout.close();*/
	//**************************************

		//**************************Offset Calculation***************************************
		//x_offset
	if (x_min > 0 && x_max > 0) {
		x_offset = -x_min - (float)((x_max - x_min) / 2);
	}
	else if (x_min < 0 && x_max < 0) {
		x_offset = x_min + (float)(-(x_max - x_min) / 2);
	}
	else {
		x_offset = -1 * (x_min + (float)((x_max - x_min) / 2));
	}
		//y_offset
	if (y_min > 0 && y_max > 0) {
		y_offset = -y_min - (float)((y_max - y_min) / 2);
	}
	else if (y_min < 0 && y_max < 0) {
		y_offset = y_min + (float)(-(y_max - y_min) / 2);
	}
	else {
		y_offset = -1 * (y_min + (float)((y_max - y_min) / 2));
	}
		//z_offset
	if (z_min > 0 && z_max > 0) {
		z_offset = -z_min - (float)((z_max - z_min) / 2);
	}
	else if (z_min < 0 && z_max < 0) {
		z_offset = z_min + (float)(-(z_max - z_min) / 2);
	}
	else {
		z_offset = -1 * (z_min + (float)((z_max - z_min) / 2));
	}
		//***********************************************************************************

		//**************************Zoom Factor Calculation**********************************
		//This calculation of zoom factor is relative to a study on an exisiting point cloud and not a general one.
	m_fZoom = (float)(((x_max - x_min) * 50) / 20.325);
		//***********************************************************************************
	//***************************************************************************************

	// Basic Setup:
	//
	// Set color to use when clearing the background.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);

	// Turn on backface culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Turn on depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Send draw request
	OnDraw(NULL);
}

void OpenGLControl::OnDraw(CDC *pDC)
{
	// TODO: Camera controls.
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -m_fZoom);
	glTranslatef(m_fPosX, m_fPosY, 0.0f);
	glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);
	glRotatef(m_fRotY, 0.0f, 1.0f, 0.0f);
}

void OpenGLControl::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 1:
	{
		// Clear color and depth buffer bits
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw OpenGL scene
		oglDrawScene();

		// Swap buffers
		SwapBuffers(hdc);

		break;
	}

	default:
		break;
	}

	CWnd::OnTimer(nIDEvent);
}

void OpenGLControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED) return;

	// Map the OpenGL coordinates.
	glViewport(0, 0, cx, cy);

	// Projection view
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	// Set our current view perspective
	gluPerspective(35.0f, (float)cx / (float)cy, 0.01f, 2000.0f);

	// Model view
	glMatrixMode(GL_MODELVIEW);

	switch (nType)
	{
		// If window resize token is "maximize"
	case SIZE_MAXIMIZED:
	{
		// Get the current window rect
		GetWindowRect(m_rect);

		// Move the window accordingly
		MoveWindow(6, 6, cx - 14, cy - 14);

		// Get the new window rect
		GetWindowRect(m_rect);

		// Store our old window as the new rect
		m_oldWindow = m_rect;

		break;
	}

	// If window resize token is "restore"
	case SIZE_RESTORED:
	{
		// If the window is currently maximized
		if (m_bIsMaximized)
		{
			// Get the current window rect
			GetWindowRect(m_rect);

			// Move the window accordingly (to our stored old window)
			MoveWindow(m_oldWindow.left,
				m_oldWindow.top - 18,
				m_originalRect.Width() - 4,
				m_originalRect.Height() - 4);

			// Get the new window rect
			GetWindowRect(m_rect);

			// Store our old window as the new rect
			m_oldWindow = m_rect;
		}

		break;
	}
	}
}

void OpenGLControl::OnMouseMove(UINT nFlags, CPoint point)
{
	int diffX = (int)(point.x - m_fLastX);
	int diffY = (int)(point.y - m_fLastY);
	m_fLastX = (float)point.x;
	m_fLastY = (float)point.y;

	// Left mouse button
	if (nFlags & MK_LBUTTON)
	{
		m_fRotX += (float)0.5f * diffY;

		if ((m_fRotX > 360.0f) || (m_fRotX < -360.0f))
		{
			m_fRotX = 0.0f;
		}

		m_fRotY += (float)0.5f * diffX;

		if ((m_fRotY > 360.0f) || (m_fRotY < -360.0f))
		{
			m_fRotY = 0.0f;
		}
	}

	// Right mouse button
	else if (nFlags & MK_RBUTTON)
	{
		m_fZoom -= (float)0.1f * diffY;
	}

	// Middle mouse button
	else if (nFlags & MK_MBUTTON)
	{
		m_fPosX += (float)0.05f * diffX;
		m_fPosY -= (float)0.05f * diffY;
	}

	OnDraw(NULL);

	CWnd::OnMouseMove(nFlags, point);
}

void OpenGLControl::oglDrawScene(void)
{
	//Zoom and Refresh checking.
	if (zoom_in_clicked) {
		if (m_fZoom > 15.0f) {
			m_fZoom = m_fZoom - 10.0f;
			OnDraw(NULL);
		}
		zoom_in_clicked = false;
	}
	if (zoom_out_clicked) {
		m_fZoom += 10.0f;
		OnDraw(NULL);
		zoom_out_clicked = false;
	}
	if (refresh_button_clicked) {
		m_fPosX = 0.0f;
		m_fPosY = 0.0f;
		m_fRotX = 0.0f;
		m_fRotY = 0.0f;
		OnDraw(NULL);
		refresh_button_clicked = false;
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_POINTS);
	glPointSize(2.0);

	renderPoints(x_tree->root);
	glEnd();

	//Rendering the Highlighted section.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBegin(GL_QUADS);
	highlight_points = highlighted_points;
	GLfloat x_coordinate;
	GLfloat y_coordinate;
	GLfloat z_coordinate;
	
	if (highlighted_points != NULL) {
		//Different color for highlighting Turquoise Green
		//glColor3f((GLfloat)(0.06667), (GLfloat)(0.57647), (GLfloat)(0.443137));

		//Different color for highlighting Magenta Crimson Red
		glColor3f((GLfloat)0.746, (GLfloat)0.0664, (GLfloat)0.1680);

		//glPointSize(0.0);
		while (highlight_points != NULL) {
			x_coordinate = highlight_points->x + x_offset;
			y_coordinate = highlight_points->y + y_offset;
			z_coordinate = highlight_points->z + z_offset;

			//Cube Rendering
			//Top Side
			glVertex3f(x_coordinate + cube_size_offset, y_coordinate + cube_size_offset, z_coordinate + cube_size_offset);
			glVertex3f(x_coordinate + cube_size_offset, y_coordinate + cube_size_offset, z_coordinate - cube_size_offset);
			glVertex3f(x_coordinate - cube_size_offset, y_coordinate + cube_size_offset, z_coordinate - cube_size_offset);
			glVertex3f(x_coordinate - cube_size_offset, y_coordinate + cube_size_offset, z_coordinate + cube_size_offset);

			//Bottom Side
			glVertex3f(x_coordinate - cube_size_offset, y_coordinate - cube_size_offset, z_coordinate - cube_size_offset);
			glVertex3f(x_coordinate + cube_size_offset, y_coordinate - cube_size_offset, z_coordinate - cube_size_offset);
			glVertex3f(x_coordinate + cube_size_offset, y_coordinate - cube_size_offset, z_coordinate + cube_size_offset);
			glVertex3f(x_coordinate - cube_size_offset, y_coordinate - cube_size_offset, z_coordinate + cube_size_offset);

			//Front Side
			glVertex3f(x_coordinate + cube_size_offset, y_coordinate + cube_size_offset, z_coordinate + cube_size_offset);
			glVertex3f(x_coordinate - cube_size_offset, y_coordinate + cube_size_offset, z_coordinate + cube_size_offset);
			glVertex3f(x_coordinate - cube_size_offset, y_coordinate - cube_size_offset, z_coordinate + cube_size_offset);
			glVertex3f(x_coordinate + cube_size_offset, y_coordinate - cube_size_offset, z_coordinate + cube_size_offset);

			//Rear Side
			glVertex3f(x_coordinate - cube_size_offset, y_coordinate - cube_size_offset, z_coordinate - cube_size_offset);
			glVertex3f(x_coordinate - cube_size_offset, y_coordinate + cube_size_offset, z_coordinate - cube_size_offset);
			glVertex3f(x_coordinate + cube_size_offset, y_coordinate + cube_size_offset, z_coordinate - cube_size_offset);
			glVertex3f(x_coordinate + cube_size_offset, y_coordinate - cube_size_offset, z_coordinate - cube_size_offset);

			//Left Side
			glVertex3f(x_coordinate - cube_size_offset, y_coordinate - cube_size_offset, z_coordinate - cube_size_offset);
			glVertex3f(x_coordinate - cube_size_offset, y_coordinate - cube_size_offset, z_coordinate + cube_size_offset);
			glVertex3f(x_coordinate - cube_size_offset, y_coordinate + cube_size_offset, z_coordinate + cube_size_offset);
			glVertex3f(x_coordinate - cube_size_offset, y_coordinate + cube_size_offset, z_coordinate - cube_size_offset);

			//Right Side
			glVertex3f(x_coordinate + cube_size_offset, y_coordinate + cube_size_offset, z_coordinate + cube_size_offset);
			glVertex3f(x_coordinate + cube_size_offset, y_coordinate - cube_size_offset, z_coordinate + cube_size_offset);
			glVertex3f(x_coordinate + cube_size_offset, y_coordinate - cube_size_offset, z_coordinate - cube_size_offset);
			glVertex3f(x_coordinate + cube_size_offset, y_coordinate + cube_size_offset, z_coordinate - cube_size_offset);

			highlight_points = highlight_points->right_next;
		}
	}
	glEnd();
}

void renderPoints(struct CustomAVLTreeNode* root) {
	//***************Inorder Traversal of Custom AVL Tree**********************
	if (root != NULL) {
		renderPoints(root->left);
		struct LinkedListNode* temporary_node = root->head_node;
		float depth_color_value;
		while (temporary_node != NULL) {
			//color range normalization to 0.05 to 0.8
			depth_color_value = (GLfloat)(0.8 - (((0.8 - 0.05)*(temporary_node->y - y_min)) / (y_max - y_min)));
			glColor3f(depth_color_value, depth_color_value, depth_color_value);
			//point alignment and rendering
			glVertex3f(temporary_node->x + x_offset, temporary_node->y + y_offset, temporary_node->z + z_offset);
			temporary_node = temporary_node->right_next;
		}
		renderPoints(root->right);
	}
	//************************************************************************
}