#include <iostream>
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>

static HGLRC hRC;
// Постоянный контекст рендеринга
static HDC hDC;
// Приватный контекст устройства GDI
BOOL keys[256];
// Массив для процедуры обработки клавиатуры

GLvoid InitGL(GLsizei Width, GLsizei Height);
GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height);
GLvoid DrawGLScene(GLvoid);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG			msg;	// Структура сообщения Windows
	WNDCLASS	wc; 	// Структура класса Windows для установки типа окна
	HWND		hWnd;	// Сохранение дескриптора окна

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
		MessageBox(0, L"Ошибка регистрации класса окна.",
			L"Ошибка", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	hWnd = CreateWindow(
		L"OpenGL WinClass",
		L"Это минимальная программа OpenGL",
		// Заголовок вверху окна
		WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		0, 0,		// Позиция окна на экране
		720, 640,		// Ширина и высота окна
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
	{
		MessageBox(0, L"Ошибка создания окна.",
			L"Ошибка", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	DEVMODE dmScreenSettings;	// Режим работы
	memset(&dmScreenSettings, 0, sizeof(DEVMODE));
	// Очистка для хранения установок
	dmScreenSettings.dmSize = sizeof(DEVMODE);
	// Размер структуры Devmode
	dmScreenSettings.dmPelsWidth = 640;
	// Ширина экрана
	dmScreenSettings.dmPelsHeight = 480;
	// Высота экрана
	dmScreenSettings.dmFields = DM_PELSWIDTH |
		DM_PELSHEIGHT;
	// Режим Пикселя
	//ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	// Переключение в полный экран

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	SetFocus(hWnd);

	while (1)
	{
		// Обработка всех сообщений
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
		DrawGLScene();		// Нарисовать сцену
		SwapBuffers(hDC);	// Переключить буфер экрана
		if (keys[VK_ESCAPE]) SendMessage(hWnd, WM_CLOSE, 0, 0);
		if (keys[VK_CONTROL] && keys[0x46]) ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}

	return 0;
}

GLvoid InitGL(GLsizei Width, GLsizei Height)	//Вызвать после создания окна GL
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Очистка экрана в черный цвет
}

GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height)
{
	if (Height == 0)
		// Предотвращение деления на ноль, 
		//если окно слишком мало
		Height = 1;
	glViewport(0, 0, Width, Height);
	// Сброс текущей области вывода и
	//перспективных преобразований
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
	// используется позднее для размеров окна
	GLuint	PixelFormat;
	static	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// Размер этой структуры
		1,								// Номер версии 
		PFD_DRAW_TO_WINDOW |			// Формат для Окна
		PFD_SUPPORT_OPENGL |			// Формат для OpenGL
		PFD_DOUBLEBUFFER,				// Формат для двойного буфера
		PFD_TYPE_RGBA,					// Требуется RGBA формат
		16,								// Выбор 16 бит глубины цвета
		0, 0, 0, 0, 0, 0,				// Игнорирование цветовых битов 
		0,								// нет буфера прозрачности
		0,								// Сдвиговый бит игнорируется 
		0,								// Нет буфера аккумуляции
		0, 0, 0, 0,						// Биты аккумуляции игнорируются 
		16,								// 16 битный Z-буфер (буфер глубины)  
		0,								// Нет буфера трафарета
		0,								// Нет вспомогательных буферов 
		PFD_MAIN_PLANE,					// Главный слой рисования
		0,								// Резерв 
		0, 0, 0							// Маски слоя игнорируются 
	};

	switch (message)	// Тип сообщения
	{
	case WM_CREATE:
		hDC = GetDC(hWnd);
		// Получить контекст устройства для окна
		PixelFormat = ChoosePixelFormat(hDC, &pfd);
		// Найти ближайшее совпадение для формата пикселей
		if (!PixelFormat)
		{
			MessageBox(0, L"Не найден подходящий формат пикселя.",
				L"Ошибка", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			// Это сообщение говорит, что
			// программа должна завершиться
			break;		// Предотвращение повтора кода
		}
		if (!SetPixelFormat(hDC, PixelFormat, &pfd))
		{
			MessageBox(0, L"Формат пикселя не установлен.",
				L"Ошибка", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			break;
		}

		hRC = wglCreateContext(hDC);
		if (!hRC)
		{
			MessageBox(0, L"Контекст воспроизведения не создан.",
				L"Ошибка", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			break;
		}

		if (!wglMakeCurrent(hDC, hRC))
		{
			MessageBox(0, L"Невозможно активизировать GLRC.",
				L"Ошибка", MB_OK | MB_ICONERROR);
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