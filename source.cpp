#include <iostream>
#include <windows.h>
#include <math.h>
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
GLvoid DrawGLScene(int& scene);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
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
	int scene = 0;
	static bool flag = false;

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
		if (keys[VK_SPACE]) flag = true;
		if (!keys[VK_SPACE] && flag) {
			flag = false;
			scene++;
		}
		DrawGLScene(scene);		// Нарисовать сцену
		SwapBuffers(hDC);	// Переключить буфер экрана
		if (keys[VK_ESCAPE]) SendMessage(hWnd, WM_CLOSE, 0, 0);
		if (keys[VK_CONTROL] && keys[0x46]) ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}

	return 0;
}

GLvoid InitGL(GLsizei Width, GLsizei Height)	//Вызвать после создания окна GL
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Очистка экрана в черный цвет
}

GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height)
{
	if (Height == 0) // Предотвращение деления на ноль, если окно слишком мало
		Height = 1;
	glViewport(0, 0, Width, Height); // Сброс текущей области вывода и перспективных преобразований
}

float randomCol(void)
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
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

GLvoid DrawGLScene(int& scene)
{
	static int flag = 0;
	srand(time(NULL));
	glClear(GL_COLOR_BUFFER_BIT);
	if (scene > 17)
		scene = 0;
	switch (scene) {
	case 0:
		glEnable(GL_POINT_SMOOTH);
		glPointSize(12);  //размер точки
		glBegin(GL_POINTS);
		glColor3d(1, 0, 0);
		for (int i = 0; i < 7; i++) {
			glVertex3d(sin(6.28 / 7 * i) / 2, cos(6.28 / 7 * i) / 2, 0);
		}
		glEnd();
		break;
	case 1:
		glEnable(GL_LINE_STIPPLE);
		glLineWidth(22);
		glLineStipple(1, 0x0FFF);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < 7; i++) {
			glVertex3d(sin(6.28 / 7 * i) / 2, cos(6.28 / 7 * i) / 2, 0); 	// первая точка
		}
		glLineStipple(1, 0x0AAA);
		glDisable(GL_LINE_STIPPLE);
		glEnd();
		glLineStipple(1, 0xFFFF);
		break;
	case 2:
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(12);  //размер точки
		glBegin(GL_LINE_LOOP);
		glColor3d(1, 0, 0);
		for (int i = 0; i < 7; i++) {
			glVertex3d(sin(6.28 / 7 * i) / 2, cos(6.28 / 7 * i) / 2, 0); 	// первая точка
		}
		glEnd();
		break;
	case 3:
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(1);  //размер точки
		glLineStipple(1, 0xFFFF);
		glBegin(GL_LINE_STRIP);
		glVertex3d(-0.814286, 0.523810, 0);
		glVertex3d(-0.557143, -0.690476, 0);
		glVertex3d(-0.328571, -0.071429, 0);
		glVertex3d(0.171429, -0.071429, 0);
		glVertex3d(0.057143, 0.738095, 0);
		glVertex3d(0.914286, 0.738095, 0);
		glVertex3d(0.171429, -0.904762, 0);
		glEnd();
		break;
	case 4:
		glBegin(GL_LINE_LOOP);
		glVertex3d(-0.548571, 0.514286, 0);
		glVertex3d(0.800000, 0.514286, 0);
		glVertex3d(0.274286, 0.000000, 0);
		glVertex3d(0.274286, -0.685714, 0);
		glVertex3d(0.685714, -0.685714, 0);
		glVertex3d(0.685714, -0.914286, 0);
		glVertex3d(-0.354286, -0.914286, 0);
		glVertex3d(0.057143, -0.342857, 0);
		glEnd();
		break;
	case 5:
		glBegin(GL_TRIANGLES);
		glVertex3d(-0.042857, 0.404762, 0);
		glVertex3d(-0.242857, 0.928571, 0);
		glVertex3d(-0.042857, 0.404762, 0);
		glColor3d(color(21), color(21), color(21));
		glVertex3d(-0.242857, 0.928571, 0);
		glVertex3d(0.685714, 0.761905, 0);
		glVertex3d(-0.042857, 0.404762, 0);
		glColor3d(color(21), color(21), color(21));
		glVertex3d(0.685714, 0.761905, 0);
		glVertex3d(0.285714, -1.000000, 0);
		glVertex3d(-0.042857, 0.404762, 0);
		glColor3d(color(21), color(21), color(21));
		glVertex3d(0.685714, 0.761905, 0);
		glVertex3d(0.800000, -0.285714, 0);
		glVertex3d(0.285714, -1.000000, 0);
		glColor3d(color(21), color(21), color(21));
		glVertex3d(-0.785714, -0.285714, 0);
		glVertex3d(-0.042857, 0.404762, 0);
		glVertex3d(-0.457143, -0.452381, 0);
		glColor3d(color(21), color(21), color(21));
		glVertex3d(-0.457143, -0.452381, 0);
		glVertex3d(0.071429, -0.666667, 0);
		glVertex3d(-0.242857, -1.000000, 0);
		glColor3d(color(21), color(21), color(21));
		glVertex3d(-0.785714, -0.285714, 0);
		glVertex3d(-0.242857, -1.000000, 0);
		glVertex3d(-0.457143, -0.452381, 0);
		glColor3d(color(21), color(21), color(21));
		glVertex3d(-0.785714, -0.285714, 0);
		glVertex3d(-0.242857, 0.928571, 0);
		glVertex3d(-0.042857, 0.404762, 0);
		glEnd();
		break;
	case 6:
		glBegin(GL_TRIANGLE_STRIP);
		glColor3d(color(12), color(12), color(12));
		glVertex3d(0.285714, -1.000000, 0);
		glVertex3d(-0.042857, 0.404762, 0);
		glVertex3d(0.800000, -0.285714, 0);
		glColor3d(color(12), color(12), color(12));
		glVertex3d(0.685714, 0.761905, 0);
		glVertex3d(-0.042857, 0.404762, 0);
		glVertex3d(-0.242857, 0.928571, 0);
		glColor3d(color(12), color(12), color(12));
		glVertex3d(-0.785714, -0.285714, 0);
		glVertex3d(-0.042857, 0.404762, 0);
		glVertex3d(-0.457143, -0.452381, 0);
		glColor3d(color(12), color(12), color(12));
		glVertex3d(-0.785714, -0.285714, 0);
		glVertex3d(-0.242857, -1.000000, 0);
		glVertex3d(0.071429, -0.666667, 0);
		glEnd();
		break;
	case 7:
		glBegin(GL_TRIANGLE_FAN);
		glColor3d(color(18), color(18), color(18));
		glVertex3d(-0.042857, 0.404762, 0);
		glVertex3d(0.285714, -1.000000, 0);
		glVertex3d(0.800000, -0.285714, 0);
		glColor3d(color(18), color(18), color(18));
		glVertex3d(0.685714, 0.761905, 0);
		glColor3d(color(18), color(18), color(18));
		glVertex3d(-0.242857, 0.928571, 0);
		glColor3d(color(18), color(18), color(18));
		glVertex3d(-0.785714, -0.285714, 0);
		glColor3d(color(18), color(18), color(18));
		glVertex3d(-0.457143, -0.452381, 0);
		glEnd();
		glBegin(GL_TRIANGLE_FAN);
		glVertex3d(-0.457143, -0.452381, 0);
		glVertex3d(0.071429, -0.666667, 0);
		glVertex3d(-0.242857, -1.000000, 0);
		glColor3d(color(18), color(18), color(18));
		glVertex3d(-0.785714, -0.285714, 0);
		glEnd();
		break;
	case 8:
		glBegin(GL_POLYGON);
		for (int i = 0; i < 7; i++) {
			glColor3d(color(21), color(21), color(21));
			glVertex3d(sin(6.28 / 7 * i) / 2, cos(6.28 / 7 * i) / 2, 0); 	// первая точка
		}
		glEnd();
		break;
	case 14:
		if (!flag)
			flag = 5;
	case 13:
		if (!flag)
			flag = 4;
	case 12:
		if (!flag)
			flag = 3;
	case 11:
		if (!flag)
			flag = 2;
	case 10:
		if (!flag)
			flag = 1;
	case 9:
		switch (flag)
		{
		case 1:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case 2:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
		case 3:
			glPolygonMode(GL_FRONT, GL_POINT);
			break;
		case 4:
			glPolygonMode(GL_FRONT, GL_LINE);
			break;
		case 5:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		}
		flag = 0;
		glBegin(GL_POLYGON);
		glColor3d(color(12), color(12), color(12));
		glVertex3d(0.285714, -1.000000, 0);
		glVertex3d(0.800000, -0.285714, 0);
		glVertex3d(0.685714, 0.761905, 0);
		glVertex3d(-0.042857, 0.404762, 0);
		glEnd();
		glBegin(GL_POLYGON);
		glColor3d(color(12), color(12), color(12));
		glVertex3d(-0.042857, 0.404762, 0);
		glVertex3d(0.685714, 0.761905, 0);
		glVertex3d(-0.242857, 0.928571, 0);
		glVertex3d(-0.785714, -0.285714, 0);
		glEnd();
		glBegin(GL_POLYGON);
		glColor3d(color(12), color(12), color(12));
		glVertex3d(-0.785714, -0.285714, 0);
		glVertex3d(-0.042857, 0.404762, 0);
		glVertex3d(-0.457143, -0.452381, 0);
		glVertex3d(-0.242857, -1.000000, 0);
		glEnd();
		glBegin(GL_POLYGON);
		glColor3d(color(12), color(12), color(12));
		glVertex3d(-0.457143, -0.452381, 0);
		glVertex3d(0.071429, -0.666667, 0);
		glVertex3d(-0.242857, -1.000000, 0);
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	default:
		glBegin(GL_POLYGON);
		glColor3d(color(12), color(12), color(12));
		glVertex3d(0.285714, -1.000000, 0);
		glVertex3d(0.800000, -0.285714, 0);
		glVertex3d(0.685714, 0.761905, 0);
		glVertex3d(-0.042857, 0.404762, 0);
		glEnd();
		break;
	}
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