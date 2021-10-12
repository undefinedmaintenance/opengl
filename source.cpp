#include <iostream>
#include <windows.h>
#include <math.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <random>
#define N 100
#define COLORS 59
int woo = 1;

static HGLRC hRC;
// Постоянный контекст рендеринга
static HDC hDC;
// Приватный контекст устройства GDI
BOOL keys[256];
// Массив для процедуры обработки клавиатуры

typedef struct POINTF {
	float x;
	float y;
};

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
	static bool flag = false, flag1 = false, flag2 = false, flag3 = false;
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
		if (keys[VK_LEFT]) flag1 = true;
		if (!keys[VK_LEFT] && flag1) {
			flag1 = false;
			scene--;
		}
		if (keys[VK_UP])/* flag2 = true;
		if (!keys[VK_UP] && flag2) {
			flag2 = false;*/
			woo += 50;
		//}
		if (keys[VK_DOWN]) /*flag3 = true;
		if (!keys[VK_DOWN] && flag3) {
			flag3 = false;*/
			woo -= 50;
		//}
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

double color(int pr)
{
	if (pr < 0)
		return static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
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

int index(int x, int y) {
	return x + y * 7;
}

typedef struct rgb
{
	float r;
	float g;
	float b;
};

GLvoid quantize(GLfloat b[][3], GLfloat start[][3])
{
	rgb error = { 0, 0, 0 };
	for (int y = 1; y < 8; y++) {
		for (int x = 0; x < 7; x++) {
			if ((int)(start[index(x, y)][0] * 255. / woo) / 255. * woo)
				b[index(x, y)][0] = (int)(start[index(x, y)][0] * 255. / woo) / 255. * woo;
			if ((int)(start[index(x, y)][1] * 255. / woo) / 255. * woo)
				b[index(x, y)][1] = (int)(start[index(x, y)][1] * 255. / woo) / 255. * woo;
			if ((int)(start[index(x, y)][2] * 255. / woo) / 255. * woo)
				b[index(x, y)][2] = (int)(start[index(x, y)][2] * 255. / woo) / 255. * woo;

			error.r = start[index(x, y)][0] - b[index(x, y)][0];
			error.g = start[index(x, y)][1] - b[index(x, y)][1];
			error.b = start[index(x, y)][2] - b[index(x, y)][2];
			if (x < 6) {
				b[index(x + 1, y)][0] += error.r * 0.4375;
				b[index(x + 1, y)][1] += error.g * 0.4375;
				b[index(x + 1, y)][2] += error.b * 0.4375;
			}
			if (x >= 1 && y < 7) {
				b[index(x - 1, y + 1)][0] += error.r * 0.1875;
				b[index(x - 1, y + 1)][1] += error.g * 0.1875;
				b[index(x - 1, y + 1)][2] += error.b * 0.1875;
			}
			if (y < 7) {
				b[index(x, y + 1)][0] += error.r * 0.3125;
				b[index(x, y + 1)][1] += error.g * 0.3125;
				b[index(x, y + 1)][2] += error.b * 0.3125;
			}
			if (x < 6 && y < 7)
				b[index(x + 1, y + 1)][0] += error.r * 0.0625;
			b[index(x + 1, y + 1)][1] += error.g * 0.0625;
			b[index(x + 1, y + 1)][2] += error.b * 0.0625;
		}
	}
}

GLvoid ez_quantize(GLfloat b[][3], GLfloat start[][3])
{
	rgb error = { 0, 0, 0 };
	float max = 0;
	int Mindex = 0;
	for (int y = 1; y < 8; y++) {
		for (int x = 0; x < 7; x++) {
			max = start[index(x, y)][0];
			Mindex = 0;
			if (start[index(x, y)][1] > max) {
				max = start[index(x, y)][1];
				Mindex = 1;
			}
			if (start[index(x, y)][2] > max) {
				max = start[index(x, y)][2];
				Mindex = 2;
			}
			switch (Mindex) {
			case 0:
				b[index(x, y)][0] = 255;
				b[index(x, y)][1] = 0;
				b[index(x, y)][2] = 0;
				break;
			case 1:
				b[index(x, y)][0] = 0;
				b[index(x, y)][1] = 255;
				b[index(x, y)][2] = 0;
				break;
			case 2:
				b[index(x, y)][0] = 0;
				b[index(x, y)][1] = 0;
				b[index(x, y)][2] = 255;
				break;

			}
			error.r = start[index(x, y)][0] - b[index(x, y)][0];
			error.g = start[index(x, y)][1] - b[index(x, y)][1];
			error.b = start[index(x, y)][2] - b[index(x, y)][2];
			if (x < 6) {
				b[index(x + 1, y)][0] += error.r * 0.4375;
				b[index(x + 1, y)][1] += error.g * 0.4375;
				b[index(x + 1, y)][2] += error.b * 0.4375;
			}
			if (x >= 1 && y < 7) {
				b[index(x - 1, y + 1)][0] += error.r * 0.1875;
				b[index(x - 1, y + 1)][1] += error.g * 0.1875;
				b[index(x - 1, y + 1)][2] += error.b * 0.1875;
			}
			if (y < 7) {
				b[index(x, y + 1)][0] += error.r * 0.3125;
				b[index(x, y + 1)][1] += error.g * 0.3125;
				b[index(x, y + 1)][2] += error.b * 0.3125;
			}
			if (x < 6 && y < 7)
				b[index(x + 1, y + 1)][0] += error.r * 0.0625;
			b[index(x + 1, y + 1)][1] += error.g * 0.0625;
			b[index(x + 1, y + 1)][2] += error.b * 0.0625;
		}
	}
}

void toRel(POINTF& p)
{
	if (p.x == 0) {}
	if (p.y == 0) {}
	int w = 320, h = 240;
	if (p.x > w)
		p.x = (p.x - w) / w;
	else if (!p.x) {}
	else p.x = -(w - p.x) / w;
	if (p.y > h)
		p.y = -(p.y - h) / h;
	else if (!p.y) {}
	else p.y = (h - p.y) / h;
}

void refill(GLfloat a[][3], POINTF p)
{
	static std::random_device rd; // obtain a random number from hardware
	static std::mt19937 gen(rd()); // seed the generator
	static std::uniform_real_distribution<double> distr(0.1, 0.5);
	static std::uniform_int_distribution<> distr1(0, 359); // define the range
	toRel(p);
	for (int i = 56; i < N; i++)
	{
		a[i][0] = p.x + sin((distr1(gen)) * 3.14 / 180) * distr(gen);
		a[i][1] = p.y + cos((distr1(gen)) * 3.14 / 180) * distr(gen);
	}
}


void arrayFill(GLfloat a[][3], GLfloat b[][3])
{
	int i = 0;
	float theta, scale;
	for (int i = 0; i < 7; i++) {
		theta = (i * 60 + rand() % 50) * 3.14 / 180;
		scale = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		a[i][0] = sin(theta) * scale;
		a[i][1] = cos(theta) * scale;
	}
	for (int i = 1; i < 8; i++)
		for (int k = 7 * i; k < 14 * i; k++)
		{
			a[k][0] = -0.766 + 0.043 * (k - 7. * i);
			a[k][1] = 0.8 - 0.05 * i;
		}
	for (int i = 0; i < 7; i++)
	{
		b[i][0] = color(-1);
		b[i][1] = color(-1);
		b[i][2] = color(-1);
	}
	for (int i = 7; i < COLORS; i++)
	{
		b[i][0] = color(-1);
		b[i][1] = color(-1);
		b[i][2] = color(-1);
	}
}

GLvoid move(GLfloat a[][3])
{
	static int it = 0;
	if (!it) {
		for (int i = 0; i < N; i++)
			a[i][0] += .5;
		it++;
	}
	else if (it == 1) {
		for (int i = 0; i < N; i++)
			a[i][0] += .5;
		it++;
	}
	else {
		for (int i = 0; i < N; i++)
			a[i][0] -= 1;
		it = 0;
	}

}

GLvoid DrawGLScene(int& scene)
{
	static int flag = 0;
	srand(time(NULL));
	glClear(GL_COLOR_BUFFER_BIT);
	static std::random_device rd; // obtain a random number from hardware
	static std::mt19937 gen(rd()); // seed the generator
	static std::uniform_int_distribution<> distr(50, 1000); // define the range
	static std::uniform_int_distribution<> distr1(10, 35); // define the range
	static std::uniform_real_distribution<double> unif(0., 1.);
	static std::uniform_real_distribution<double> dif(0., 0.9);
	static GLfloat aVertex[N][3], aColor[COLORS][3], ditherCA[COLORS][3];
	static POINT prev = { 0, 0 }, cur = { 0, 0 };
	static POINTF p = { 0 , 0 };
	float scale = 0.5;
	if (!flag)
	{
		arrayFill(aVertex, aColor);
		refill(aVertex, { 0, 0.001 });
		flag = 1;
	}
	glVertexPointer(3, GL_FLOAT, 0, aVertex);
	glColorPointer(3, GL_FLOAT, 0, aColor);

	switch (scene)
	{
	case 0:
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glLoadIdentity();
		glPointSize(10);
		glEnable(GL_POINT_SMOOTH);
		glDrawArrays(GL_POINTS, 0, 7);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		break;
	case 2:
		GetCursorPos(&cur);
		if ((cur.x != prev.x) || (cur.y != prev.y)) {
			prev = cur;
			flag = 2;
		}
		if (flag == 2)
		{
			flag = 1;
		}
		else {
			glClear(GL_COLOR_BUFFER_BIT);
			break;
		}
	case 1:
		for (int i = 0; i < 7; i++) {
			glColor3d(unif(gen), unif(gen), unif(gen));
			glPointSize(distr1(gen));
			scale = dif(gen);             //окрестность точки
			glBegin(GL_POINTS);
			glVertex3d(sin((i * 60 + distr(gen) % 50) * 3.14 / 180) * scale, cos((i * 60 + distr(gen) % 50) * 3.14 / 180) * scale, 0);
			glEnd();
		}
		Sleep(100);
		break;
	case 4:
		glDisable(GL_POINT_SMOOTH);
		glPointSize(14);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		ez_quantize(ditherCA, aColor);
		glColorPointer(3, GL_FLOAT, 0, ditherCA);
		move(aVertex);
		glDrawArrays(GL_POINTS, 7, 49);
		move(aVertex);
		quantize(ditherCA, aColor);
		glDrawArrays(GL_POINTS, 7, 49);
		move(aVertex);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	case 3:
		glDisable(GL_POINT_SMOOTH);
		glPointSize(14);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, aColor);
		glDrawArrays(GL_POINTS, 7, 49);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		break;
	case 6:
		GetCursorPos(&cur);
		p.x = cur.x;
		p.y = cur.y;
		refill(aVertex, p);
		glLineWidth(distr1(gen) % 20);
	case 5:
		glEnable(GL_LINE_SMOOTH);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, aColor);
		glDrawArrays(GL_LINES, 56, 44);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		Sleep(100);
		break;
	default:
		scene = 0;
		flag = 1;
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