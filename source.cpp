#include <fstream>
#include <vector>
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

#pragma warning (disable : 4996)

GLuint texture;

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
GLuint loadBMP_custom(const char* imagepath);


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
	loadBMP_custom("C:\\a\\e.bmp");


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
	glClear(GL_COLOR_BUFFER_BIT);
	static float t = 5, i = 0.5, x = 0;
	static bool flag = true;
	
	if (keys[VK_UP]) t += 1;
	if (keys[VK_DOWN]) t -= 1;
	if (keys[VK_RIGHT]) i += 0.1;
	if (keys[VK_LEFT]) i -= 0.1;
	if (keys[VK_F18]) x += 0.1;
	if (keys[VK_F19]) x -= 0.1;
	if (keys[VK_F17]) flag = flag ? false : true;

	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	//glEnable(GL_TEXTURE_GEN_S);
	//glEnable(GL_TEXTURE_GEN_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MAG_FILTER,	GL_NEAREST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	if (flag) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		//i = 1 / 7.;
	}

	gluPerspective(80, 1, 2, 100);
	glTranslatef(0, 0, -t);
	glRotatef(x, 0, 0, 1);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-2, -2, 0); //����� ������ ����
	glTexCoord2f(5.0 * i, 0.0 *i);
	glVertex3f(3, -2, 0); //������ ������ ����
	glTexCoord2f(7.0 * i, 4.0 * i);
	glVertex3f(5, 2, 0); //������ ������� ����
	glTexCoord2f(2.0 * i, 4.0 * i);
	glVertex3f(0, 2, 0); //����� ������� ����
	glEnd();

	//glDisable(GL_TEXTURE_GEN_S);
	//glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
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

std::vector<unsigned char> readFile(const char* path)
{
	std::ifstream file(path, std::ios::binary);
	std::streampos size;
	file.seekg(0, std::ios::end);
	size = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<unsigned char> data(size);
	file.read((char*)&data[0], size);
	return data;
}

GLuint loadBMP_custom(const char* imagepath)
{
	int width = 512;
	int height = 512;
	std::vector<unsigned char> data = readFile(imagepath);
	for (int i = 0; i < width * height; ++i)
	{
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];

		data[index] = R;
		data[index + 2] = B;
	}
	unsigned char* set = &data[0];
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height,0, GL_RGB, GL_UNSIGNED_BYTE, set);
	data.clear();
	return texture;
}