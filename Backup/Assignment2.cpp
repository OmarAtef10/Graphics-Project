#include <Windows.h>
#include <math.h>
static int CircleCenter[2];

static int CircleRadius;



int Round(double x)
{
	return (int)(x + 0.5);
}

int DistBet2Points(int x1, int x2, int y1, int y2) {
	int res = 0;
	res = Round(sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
	return res;
}

void swap(int& x1, int& y1, int& x2, int& y2)
{
	int tmp = x1;
	x1 = x2;
	x2 = tmp;
	tmp = y1;
	y1 = y2;
	y2 = tmp;
}

void DrawLineClip(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
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
			if (DistBet2Points(x, CircleCenter[0], y, CircleCenter[1]) <= CircleRadius) {
				SetPixel(hdc, x, ceil(y), c);
				SetPixel(hdc, x, floor(y), c);
			}
			else {
				continue;
			}
			
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
			if (DistBet2Points(x, CircleCenter[0], y, CircleCenter[1]) <= CircleRadius) {
				SetPixel(hdc, ceil(x), y, c);
				SetPixel(hdc, floor(x), y, c);
			}
			else {
				continue;
			}


		}
	}
}







////////// Circle Stuff ///////////////////

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

void DrawCircle1(HDC hdc, int xc, int yc, int R, COLORREF c)
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

LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
	static HDC hdc;
	static int index = 0;
	static int circleClicks = 0;
	static int  circleCords [4];
	static int circleCx;
	static int circleCy;
	static bool circleDraw = false;
	static int  lineXCords[2];
	static int  lineYCords[2];


	switch (mcode)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		break;
	case WM_LBUTTONDOWN:

		if (circleDraw==false) 
		{		
				if (circleClicks==0)
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
				else if (circleClicks==1)
				{
					POINT p;
					GetCursorPos(&p);
					circleCords[2] = p.x;
					circleCords[3] = p.y;

					circleClicks++;

					int radius = DistBet2Points(circleCx, circleCords[2], circleCy, circleCords[3]);
					DrawCircle1(hdc, circleCx, circleCy, radius, RGB(0, 0, 0));
					circleDraw = true;
					CircleCenter[0] = circleCx;
					CircleCenter[1] = circleCy;
					CircleRadius = radius;
					break;
				}
		}
		if (circleDraw == true) {
			POINT p;
			GetCursorPos(&p);
			lineXCords[index] = p.x;
			lineYCords[index] = p.y;
			if (index == 1) {
				DrawLineClip(hdc, lineXCords[0], lineYCords[0], lineXCords[1], lineYCords[1], RGB(0, 0, 69));
				index = 0;
			}
			else index++;
			break;
		}
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
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
	HWND hwnd = CreateWindow(L"MyClass", L"My First Window", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hinst, 0);
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