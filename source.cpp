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
	static float i = 0, j = 7, k = 0, ang = 0, size = 1;
	GLUquadricObj* quadricObj = gluNewQuadric();
	glMatrixMode(GL_PROJECTION);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	gluQuadricDrawStyle(quadricObj, GLU_LINE);
	glEnable(GL_DEPTH_TEST);
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
	glRotatef(ang, 0., 1, 0.);
	gluPerspective(40, 1, 5, 10);
	glTranslatef(size * k, 0., 0.);
	
	gluLookAt(j*sin(i), j*cos(i), 0, 0, 0, 0, 0, 0, -1);
	gluSphere(quadricObj, size * .239, 10, 10);		//sun
	glTranslatef(size * .28, .0, .0);
	gluSphere(quadricObj, size * .004, 10, 10);		//mercury
	glTranslatef(size * .1, .0, .0);
	gluSphere(quadricObj, size * .012, 10, 10);		//venus
	glTranslatef(size * .1, .0, .0);
	gluSphere(quadricObj, size * .0127, 10, 10);	//earth
	glTranslatef(size * .1, .0, .0);
	gluSphere(quadricObj, size * .006, 10, 10);		//mars
	glTranslatef(size * .2, .0, .0);
	gluSphere(quadricObj, size * .14, 10, 10);		//jupiter
	glRotated(45, 1, 1, -1);
	gluDisk(quadricObj, size * .17, size * .21, 10, 10);
	glRotated(-45, 1, 1, -1);
	glTranslatef(size * .32, .0, .0);
	gluSphere(quadricObj, size * .116, 10, 10);		//saturn
	glTranslatef(size * .2, .0, .0);
	gluSphere(quadricObj, size * .0507, 10, 10);	//uranus
	glTranslatef(size * .13, .0, .0);
	gluSphere(quadricObj, size * .0492, 10, 10);	//neptune

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