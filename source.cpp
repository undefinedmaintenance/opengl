#include <iostream>
#include <windows.h>
#include <math.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#define N 20
#define COLORS 30

static HGLRC hRC;
// ���������� �������� ����������
static HDC hDC;
// ��������� �������� ���������� GDI
BOOL keys[256];
// ������ ��� ��������� ��������� ����������

GLvoid InitGL(GLsizei Width, GLsizei Height);
GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height);
GLvoid DrawGLScene(int& scene);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
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
	int scene = 0;
	static bool flag = false;

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
		if (keys[VK_SPACE]) flag = true;
		if (!keys[VK_SPACE] && flag) {
			flag = false;
			scene++;
		}
		DrawGLScene(scene);		// ���������� �����
		SwapBuffers(hDC);	// ����������� ����� ������
		if (keys[VK_ESCAPE]) SendMessage(hWnd, WM_CLOSE, 0, 0);
		if (keys[VK_CONTROL] && keys[0x46]) ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}

	return 0;
}

GLvoid InitGL(GLsizei Width, GLsizei Height)	//������� ����� �������� ���� GL
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // ������� ������ � ������ ����
}

GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height)
{
	if (Height == 0) // �������������� ������� �� ����, ���� ���� ������� ����
		Height = 1;
	glViewport(0, 0, Width, Height); // ����� ������� ������� ������ � ������������� ��������������
}

double color(int pr)
{
	static double re[30];
	static bool once = false;
	static int iteration = 0;
	if (iteration >= pr)
		iteration = 0;
	if (!once) {
		for (int a = 0; a < 30; a++)
			re[a] = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		once = true;
	}
	return re[iteration++];
}

void arrayFill(GLfloat a[][3], GLfloat b[][3])
{
	int i = 0;
	for (i = 0; i < 7; i++) {
		a[i][0] = sin(6.28 / 7 * i);
		a[i][1] = cos(6.28 / 7 * i);
	}
	a[7][0] = -0.913636;
	a[7][1] = -0.657143;
	a[8][0] = -0.745455;
	a[8][1] = 0.074286;
	a[9][0] = -0.577273;
	a[9][1] = -0.657143;
	a[10][0] = -0.454545;
	a[10][1] = 0.074286;
	a[11][0] = 0.340909;
	a[11][1] = 0.074286;
	a[12][0] = 0.340909;
	a[12][1] = -0.657143;
	a[13][0] = -0.454545;
	a[13][1] = -0.657143;
	a[14][0] = 0.522727;
	a[14][1] = -0.291429;
	a[15][0] = 0.943182;
	a[15][1] = -0.291429;
	a[16][0] = 0;
	a[16][1] = 0;
	a[17][0] = -1;
	a[17][1] = 0;
	a[18][0] = -0.5;
	a[18][1] = -0.87;



	for (int i = 0; i < COLORS; i++)
	{
		b[i][0] = color(30);
		b[i][1] = color(30);
		b[i][2] = color(30);
	}
}

GLvoid DrawGLScene(int& scene)
{
	static int flag = 0;
	srand(time(NULL));
	glClear(GL_COLOR_BUFFER_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(5);
	glMatrixMode(GL_MODELVIEW);
	static GLfloat aVertex[N][3], aColor[COLORS][3];

	if (!flag)
	{
		arrayFill(aVertex, aColor);
		flag = 1;
	}
 	glVertexPointer(3, GL_FLOAT, 0, aVertex);
	glColorPointer(3, GL_FLOAT, 0, aColor);
	switch (scene)
	{
	case 0:
		glLoadIdentity();
		glDrawArrays(GL_POLYGON, 0, 7);
		break;
	case 1:
		glBegin(GL_POLYGON);
		glArrayElement(1);
		glArrayElement(2);
		glArrayElement(5);
		glArrayElement(6);
		glEnd();
		glBegin(GL_POLYGON);
		glArrayElement(0);
		glArrayElement(6);
		glArrayElement(3);
		glArrayElement(2);
		glEnd();
		glBegin(GL_POLYGON);
		glArrayElement(0);
		glArrayElement(1);
		glArrayElement(4);
		glArrayElement(5);
		glEnd();
		glBegin(GL_POLYGON);
		glArrayElement(6);
		glArrayElement(1);
		glArrayElement(3);
		glArrayElement(4);
		glEnd();
		break;
	case 2:
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 7);
		glBegin(GL_TRIANGLES);
		glArrayElement(5);
		glArrayElement(6);
		glArrayElement(0);
		glEnd();
		break;
	case 3:
		glDrawArrays(GL_LINE_LOOP, 0, 7);
		break;
	case 8:
		flag = 5;
	case 7:
		if (flag < 4)
			flag = 4;
	case 6:
		if (flag < 3)
			flag = 3;
	case 5:
		if (flag < 2)
			flag = 2;
	case 4:
		glPushMatrix();
		if(flag >= 2)
			glScalef(0.09, 0.09, 0);
		if (flag >= 3) {
			glTranslatef(-5, 10, 0);
			glScalef(3, 0.5, 0);
		}
		glDrawArrays(GL_TRIANGLES, 7, 3);
		glPopMatrix();
		glPushMatrix();
		if (flag >= 4) {
			glScalef(0.15, 0.15, 0);
			glTranslatef(-5, -5, 0);
			glRotatef(45, 0, 0, 1);
			glTranslatef(5, 5, 0);
		}
		glDrawArrays(GL_POLYGON, 10, 4);
		glPopMatrix();
		glPushMatrix();
		if (flag < 3 && flag >= 2)
			glScalef(0.09, 0.09, 0);
		if (flag >= 5) {
			glRotatef(-70, 0, 0, 1);
		}
		glDrawArrays(GL_LINES, 14, 2);
		glPopMatrix();
		break;
	case 9:
		glDrawArrays(GL_LINE_LOOP, 16, 3);
		glRotatef(120, 0, 0, 1);
		glDrawArrays(GL_LINE_LOOP, 16, 3);
		glRotatef(120, 0, 0, 1);
		glDrawArrays(GL_LINE_LOOP, 16, 3);
		glLoadIdentity();
		break;
	default:
		scene = 0;
		flag = 1;
		break;
	}
	glDisableClientState(GL_VERTEX_ARRAY);
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