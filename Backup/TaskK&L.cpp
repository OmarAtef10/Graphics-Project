#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLu.h>
#include <math.h>
#pragma comment(lib,"opengl32")
#pragma comment(lib,"glu32")
int leftCounter = 0;
int rightCounter = 0;
int xCord[] = { 0,0,0 };
int yCord[] = { 0,0,0 };
void swap(int& x1, int& y1, int& x2, int& y2)
{
	int tmp = x1;
	x1 = x2;
	x2 = tmp;
	tmp = y1;
	y1 = y2;
	y2 = tmp;
}
int Round(double x)
{
	return (int)(x + 0.5);
}
void DrawLine1(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	if (abs(dy) <= abs(dx))
	{
		if (x1 > x2)swap(x1, y1, x2, y2);
		SetPixel(hdc, x1, y1, c);
		int x = x1;
		while (x < x2)
		{
			x++;
			double y = y1 + (double)(x - x1) * dy / dx;
			SetPixel(hdc, x, ceil(y), c);
			SetPixel(hdc, x, floor(y), c);

		}
	}
	else {
		if (y1 > y2)swap(x1, y1, x2, y2);
		SetPixel(hdc, x1, y1, c);
		int y = y1;
		while (y < y2)
		{
			y++;
			double x = x1 + (double)(y - y1) * dx / dy;
			SetPixel(hdc, ceil(x), y, c);
			SetPixel(hdc, floor(x), y, c);

		}
	}
}
void Draw8PointsSHADE4(HDC hdc, int xc, int yc, int x, int y, COLORREF c)   //quad number 4
{
	SetPixel(hdc, xc + x, yc + y, c);
	SetPixel(hdc, xc + x, yc - y, c);
	DrawLine1(hdc, xc, yc, x + xc, y + yc, c);
	SetPixel(hdc, xc - x, yc - y, c);
	DrawLine1(hdc, xc, yc, xc + y, yc + x, c);
	SetPixel(hdc, xc - x, yc + y, c);
	SetPixel(hdc, xc + y, yc + x, c);
	SetPixel(hdc, xc + y, yc - x, c);
	SetPixel(hdc, xc - y, yc - x, c);
	SetPixel(hdc, xc - y, yc + x, c);
}
void Draw8PointsSHADE2(HDC hdc, int xc, int yc, int x, int y, COLORREF c)  //quad number 2
{
	SetPixel(hdc, xc + x, yc + y, c);
	SetPixel(hdc, xc + x, yc - y, c);
	SetPixel(hdc, xc - x, yc - y, c);
	SetPixel(hdc, xc - x, yc + y, c);
	SetPixel(hdc, xc + y, yc + x, c);
	SetPixel(hdc, xc + y, yc - x, c);
	SetPixel(hdc, xc - y, yc - x, c);
	SetPixel(hdc, xc - y, yc + x, c);
	DrawLine1(hdc, xc, yc, xc - y, yc - x, c);
	DrawLine1(hdc, xc, yc, xc - x, yc - y, c);
}
void Draw8PointsSHADE3(HDC hdc, int xc, int yc, int x, int y, COLORREF c) //quad number 3
{
	SetPixel(hdc, xc + x, yc + y, c);
	SetPixel(hdc, xc + x, yc - y, c);
	DrawLine1(hdc, xc, yc, xc - x, yc + y, c);
	DrawLine1(hdc, xc, yc, xc - y, yc + x, c);

	SetPixel(hdc, xc - x, yc - y, c);
	SetPixel(hdc, xc - x, yc + y, c);
	SetPixel(hdc, xc + y, yc + x, c);
	SetPixel(hdc, xc + y, yc - x, c);
	SetPixel(hdc, xc - y, yc - x, c);
	SetPixel(hdc, xc - y, yc + x, c);
}
void Draw8PointsSHADE1(HDC hdc, int xc, int yc, int x, int y, COLORREF c) // quad number 1
{
	SetPixel(hdc, xc + x, yc + y, c);
	SetPixel(hdc, xc + x, yc - y, c);
	DrawLine1(hdc, xc, yc, xc + x, yc - y, c);
	DrawLine1(hdc, xc, yc, xc + y, yc - x, c);
	SetPixel(hdc, xc - x, yc - y, c);
	SetPixel(hdc, xc - x, yc + y, c);
	SetPixel(hdc, xc + y, yc + x, c);
	SetPixel(hdc, xc + y, yc - x, c);
	SetPixel(hdc, xc - y, yc - x, c);
	SetPixel(hdc, xc - y, yc + x, c);
}


void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c)
{
	SetPixel(hdc, xc + x, yc + y, c);
	SetPixel(hdc, xc + x, yc - y, c);
	SetPixel(hdc, xc - x, yc - y, c);
	SetPixel(hdc, xc - x, yc + y, c);
	SetPixel(hdc, xc + y, yc + x, c);
	SetPixel(hdc, xc + y, yc - x, c);
	SetPixel(hdc, xc - y, yc - x, c);
	SetPixel(hdc, xc - y, yc + x, c);
}


void DrawQuad3(HDC hdc, int xc, int yc, int x, int y, COLORREF c) //Quad number 3
{
	SetPixel(hdc, xc - y, yc + x, c);
	SetPixel(hdc, xc - x, yc + y, c);
}

void DrawQuad1(HDC hdc, int xc, int yc, int x, int y, COLORREF c) //Quad number 1
{
	SetPixel(hdc, xc + x, yc - y, c);
	SetPixel(hdc, xc + y, yc - x, c);
}

void DrawQuad2(HDC hdc, int xc, int yc, int x, int y, COLORREF c) //Quad number 2
{
	SetPixel(hdc, xc - y, yc - x, c);
	SetPixel(hdc, xc - x, yc - y, c);
}

void DrawQuad4(HDC hdc, int xc, int yc, int x, int y, COLORREF c) //Quad number 4
{
	SetPixel(hdc, xc + y, yc + x, c);
	SetPixel(hdc, xc + x, yc + y, c);

}



void DrawCircle(HDC hdc, int xc, int yc, int R, COLORREF c)
{
	int x = 0;
	double y = R;
	Draw8Points(hdc, xc, yc, 0, R, c);
	while (x < y)
	{
		x++;
		y = sqrt((double)R * R - x * x);
		Draw8Points(hdc, xc, yc, x, Round(y), c);
	}
}


void DrawCircle1(HDC hdc, int xc, int yc, int R, COLORREF c)
{
	int x = 0;
	double y = R;
	Draw8PointsSHADE1(hdc, xc, yc, 0, R, c);
	while (x < y)
	{
		x++;
		y = sqrt((double)R * R - x * x);
		Draw8PointsSHADE1(hdc, xc, yc, x, Round(y), c);
	}
}

void DrawCircle2(HDC hdc, int xc, int yc, int R, COLORREF c)
{
	int x = 0;
	double y = R;
	Draw8PointsSHADE2(hdc, xc, yc, 0, R, c);
	while (x < y)
	{
		x++;
		y = sqrt((double)R * R - x * x);
		Draw8PointsSHADE2(hdc, xc, yc, x, Round(y), c);
	}
}

void DrawCircle3(HDC hdc, int xc, int yc, int R, COLORREF c)
{
	int x = 0;
	double y = R;
	Draw8PointsSHADE3(hdc, xc, yc, 0, R, c);
	while (x < y)
	{
		x++;
		y = sqrt((double)R * R - x * x);
		Draw8PointsSHADE3(hdc, xc, yc, x, Round(y), c);
	}
}

void DrawCircle4(HDC hdc, int xc, int yc, int R, COLORREF c)
{
	int x = 0;
	double y = R;
	Draw8PointsSHADE4(hdc, xc, yc, 0, R, c);
	while (x < y)
	{
		x++;
		y = sqrt((double)R * R - x * x);
		Draw8PointsSHADE4(hdc, xc, yc, x, Round(y), c);
	}
}

void DrawCircleQuad1(HDC hdc, int xc, int yc, int R, COLORREF c)
{
	int x = 0;
	double y = R;
	DrawQuad1(hdc, xc, yc, 0, R, c);
	while (x < y)
	{
		x++;
		y = sqrt((double)R * R - x * x);
		DrawQuad1(hdc, xc, yc, x, Round(y), c);
	}
}

void DrawCircleQuad2(HDC hdc, int xc, int yc, int R, COLORREF c)
{
	int x = 0;
	double y = R;
	DrawQuad2(hdc, xc, yc, 0, R, c);
	while (x < y)
	{
		x++;
		y = sqrt((double)R * R - x * x);
		DrawQuad2(hdc, xc, yc, x, Round(y), c);
	}
}void DrawCircleQuad3(HDC hdc, int xc, int yc, int R, COLORREF c)
{
	int x = 0;
	double y = R;
	DrawQuad3(hdc, xc, yc, 0, R, c);
	while (x < y)
	{
		x++;
		y = sqrt((double)R * R - x * x);
		DrawQuad3(hdc, xc, yc, x, Round(y), c);
	}
}void DrawCircleQuad4(HDC hdc, int xc, int yc, int R, COLORREF c)
{
	int x = 0;
	double y = R;
	DrawQuad4(hdc, xc, yc, 0, R, c);
	while (x < y)
	{
		x++;
		y = sqrt((double)R * R - x * x);
		DrawQuad4(hdc, xc, yc, x, Round(y), c);
	}
}


double DistBet2Points(int x1, int x2, int y1, int y2) {
	double res = 0.0;
	res = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	return res;
}
void AdjustWindowFor2D(HDC hdc, int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SwapBuffers(hdc);
}
void EndOpenGl(HGLRC glrc)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(glrc);
}
LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
	static HDC hdc;
	static int index = 0;
	static int circleClicks = 0;
	static int  circleCords[4];
	static int circleCx;
	static int circleCy;
	static bool circleDraw = false;
	static int radius;

	switch (mcode)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		break;
	case WM_SIZE:
		AdjustWindowFor2D(hdc, LOWORD(lp), HIWORD(lp));
		break;
	case WM_LBUTTONDOWN:


		if (circleDraw == false)
		{
			if (circleClicks == 0)
			{
				POINT p;
				GetCursorPos(&p);

				circleCords[0] = p.x;
				circleCx = circleCords[0];

				circleCords[1] = p.y;
				circleCy = circleCords[1];
				circleClicks++;
				break;
			}
			else if (circleClicks == 1)
			{
				POINT p;
				GetCursorPos(&p);
				circleCords[2] = p.x;
				circleCords[3] = p.y;

				circleClicks++;

				radius = DistBet2Points(circleCx, circleCords[2], circleCy, circleCords[3]);
				DrawCircle(hdc, circleCx, circleCy, radius, RGB(0, 0, 0));
			
				circleDraw = true;

				break;
			}
		}
		if (circleDraw == true) {
			int temp = radius;
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(hwnd, &p);
			if (p.x > circleCx && p.y < circleCy) {
				while (temp >= 0)
				{
					DrawCircleQuad1(hdc, circleCx, circleCy, temp, RGB(0, 0, 0));
					temp -= 1;

				}		
				break;
			}
			else if (p.x < circleCx && p.y < circleCy) {
				while (temp >= 0)
				{
					DrawCircleQuad2(hdc, circleCx, circleCy, temp, RGB(0, 0, 0));
					temp -= 1;

				}
				break;

			}
			else if (p.x < circleCx && p.y > circleCy) {
				while (temp >= 0)
				{
					DrawCircleQuad3(hdc, circleCx, circleCy, temp, RGB(0, 0, 0));
					temp -= 1;

				}
				break;
			}

			else if (p.x > circleCx && p.y > circleCy) {
				while (temp >= 0)
				{
					DrawCircleQuad4(hdc, circleCx, circleCy, temp, RGB(0, 0, 0));
					temp -= 1;

				}
				break;
			}
			break;
		}
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		ReleaseDC(hwnd, hdc);
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hwnd, mcode, wp, lp);
	}
	return 0;
}
int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmd, int nsh)
{
	WNDCLASS wc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hinst;
	wc.lpfnWndProc = MyWndProc;
	wc.lpszClassName = L"MyClass";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	HWND hwnd = CreateWindow(L"MyClass", L"My First Window", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 800, 600, NULL, NULL, hinst, 0);
	ShowWindow(hwnd, nsh);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
