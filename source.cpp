#include <iostream>
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

static HGLRC hRC;
// ���������� �������� ����������
static HDC hDC;
// ��������� �������� ���������� GDI
BOOL keys[256];
// ������ ��� ��������� ��������� ����������

GLvoid InitGL(GLsizei Width, GLsizei Height);
GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height);
GLvoid DrawGLScene(GLvoid);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG			msg;	// ��������� ��������� Windows
	WNDCLASS	wc; 	// ��������� ������ Windows ��� ��������� ���� ����
	HWND		hWnd;	// ���������� ����������� ����

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"OpenGL WinClass";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"������ ����������� ������ ����.",
			L"������", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	hWnd = CreateWindow(
		L"OpenGL WinClass",
		L"��� ����������� ��������� OpenGL",
		// ��������� ������ ����
		WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		0, 0,		// ������� ���� �� ������
		720, 640,		// ������ � ������ ����
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
	{
		MessageBox(0, L"������ �������� ����.",
			L"������", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	DEVMODE dmScreenSettings;	// ����� ������
	memset(&dmScreenSettings, 0, sizeof(DEVMODE));
	// ������� ��� �������� ���������
	dmScreenSettings.dmSize = sizeof(DEVMODE);
	// ������ ��������� Devmode
	dmScreenSettings.dmPelsWidth = 640;
	// ������ ������
	dmScreenSettings.dmPelsHeight = 480;
	// ������ ������
	dmScreenSettings.dmFields = DM_PELSWIDTH |
		DM_PELSHEIGHT;
	// ����� �������
	//ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	// ������������ � ������ �����

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	SetFocus(hWnd);

	while (1)
	{
		// ��������� ���� ���������
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				return TRUE;
			}
		}
		DrawGLScene();		// ���������� �����
		SwapBuffers(hDC);	// ����������� ����� ������
		if (keys[VK_ESCAPE]) SendMessage(hWnd, WM_CLOSE, 0, 0);
		if (keys[VK_CONTROL] && keys[0x46]) ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}

	return 0;
}

GLvoid InitGL(GLsizei Width, GLsizei Height)	//������� ����� �������� ���� GL
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// ������� ������ � ������ ����
}

GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height)
{
	if (Height == 0)
		// �������������� ������� �� ����, 
		//���� ���� ������� ����
		Height = 1;
	glViewport(0, 0, Width, Height);
	// ����� ������� ������� ������ �
	//������������� ��������������
}

GLvoid DrawGLScene(GLvoid)
{
	static float i = 0, j = 0, k = 0, ang = 0, size = 1;
	float ambient[4] = { 0.1, 0.4, 0.1, 1 };
	GLfloat diffuse[3] = { 0, 0, 1 }, shininess[1] = { 79 }, lightpos[4] = { 0, 7, 8, 1}, lightdir[3] = { 0, -1, 1 };
	GLUquadricObj* quadricObj = gluNewQuadric();

	glMatrixMode(GL_PROJECTION);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	gluQuadricDrawStyle(quadricObj, GLU_FILL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightdir);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, new GLfloat[4]{ 0, 1, .2, 1 });
	glLightfv(GL_LIGHT1, GL_POSITION, new GLfloat[4]{ -6, -6, 6, 1 });
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, new GLfloat[3]{ 1, 1, -1 });
	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, new GLfloat[4]{ 0.3, 1, 0.7, 1 });
	glLightfv(GL_LIGHT2, GL_POSITION, new GLfloat[4]{ 9, 0, 4, 1 });
	glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, new GLfloat[1]{ 20 });
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, new GLfloat[3]{ -1, 0, 0 });
	glPushMatrix();

	if (keys[VK_UP]) j -= .05;
	if (keys[VK_DOWN]) j += .05;
	if (keys[VK_RIGHT]) i += .05;
	if (keys[VK_LEFT]) i -= .05;
	if (keys[VK_F18]) k += .05;
	if (keys[VK_F19]) k -= .05;
	if (keys[VK_F16]) ang += 1;
	if (keys[VK_F17]) ang -= 1;
	if (keys[VK_F14]) size += .1;
	if (keys[VK_F15]) size -= .1;
	
	gluPerspective(80, 1, 2, 100);
	//glTranslatef(2, 7, -9);
	gluLookAt(i, j, k, 0, 0, 0, 0, 0, -1);
	//glTranslatef(i, j, k);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	gluSphere(quadricObj, 5, 50, 50);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	gluSphere(quadricObj, 5, 50, 50);
	//glTranslatef(-2, -7, 9);
	//glTranslatef(-i, -j, -k);

	glPopMatrix();
	gluDeleteQuadric(quadricObj);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT	Screen;
	// ������������ ������� ��� �������� ����
	GLuint	PixelFormat;
	static	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// ������ ���� ���������
		1,								// ����� ������ 
		PFD_DRAW_TO_WINDOW |			// ������ ��� ����
		PFD_SUPPORT_OPENGL |			// ������ ��� OpenGL
		PFD_DOUBLEBUFFER,				// ������ ��� �������� ������
		PFD_TYPE_RGBA,					// ��������� RGBA ������
		16,								// ����� 16 ��� ������� �����
		0, 0, 0, 0, 0, 0,				// ������������� �������� ����� 
		0,								// ��� ������ ������������
		0,								// ��������� ��� ������������ 
		0,								// ��� ������ �����������
		0, 0, 0, 0,						// ���� ����������� ������������ 
		16,								// 16 ������ Z-����� (����� �������)  
		0,								// ��� ������ ���������
		0,								// ��� ��������������� ������� 
		PFD_MAIN_PLANE,					// ������� ���� ���������
		0,								// ������ 
		0, 0, 0							// ����� ���� ������������ 
	};

	switch (message)	// ��� ���������
	{
	case WM_CREATE:
		hDC = GetDC(hWnd);
		// �������� �������� ���������� ��� ����
		PixelFormat = ChoosePixelFormat(hDC, &pfd);
		// ����� ��������� ���������� ��� ������� ��������
		if (!PixelFormat)
		{
			MessageBox(0, L"�� ������ ���������� ������ �������.",
				L"������", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			// ��� ��������� �������, ���
			// ��������� ������ �����������
			break;		// �������������� ������� ����
		}
		if (!SetPixelFormat(hDC, PixelFormat, &pfd))
		{
			MessageBox(0, L"������ ������� �� ����������.",
				L"������", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			break;
		}

		hRC = wglCreateContext(hDC);
		if (!hRC)
		{
			MessageBox(0, L"�������� ��������������� �� ������.",
				L"������", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			break;
		}

		if (!wglMakeCurrent(hDC, hRC))
		{
			MessageBox(0, L"���������� �������������� GLRC.",
				L"������", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			break;
		}

		GetClientRect(hWnd, &Screen);
		InitGL(Screen.right, Screen.bottom);
		break;

	case WM_DESTROY:
	case WM_CLOSE:
		ChangeDisplaySettings(NULL, 0);
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);
		ReleaseDC(hWnd, hDC);
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		keys[wParam] = TRUE;
		break;

	case WM_KEYUP:
		keys[wParam] = FALSE;
		break;

	case WM_SIZE:
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));
		break;

	default:
		return (DefWindowProc(hWnd, message, wParam, lParam));
	}

	return (0);
}