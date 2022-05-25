#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLu.h>
#include <math.h>
#include <stack>
#include <list>
using namespace std;
#pragma comment(lib,"opengl32")
#pragma comment(lib,"glu32")

struct Point {
	int x, y;
	Point() {}
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

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


/*Convex Polyogn Filling*/
#define MAX_ENTRIES 600
struct Entry {
	int xmax, xmin;
};

void InitializeEntries(Entry table[]) {
	for (int i = 0; i < MAX_ENTRIES; i++) {
		table[i].xmin = MAXINT;
		table[i].xmax = MININT;
	}
}

void ScanEdge(Point v1, Point v2, Entry table[])
{
	if (v1.y == v2.y)return;
	if (v1.y > v2.y)swap(v1, v2);
	double minv = (double)(v2.x - v1.x) / (v2.y - v1.y);
	double x = v1.x;
	int y = v1.y;
	while (y < v2.y)
	{
		if (x < table[y].xmin) {
			table[y].xmin = (int)ceil(x);
		}
		if (x > table[y].xmax) {
			table[y].xmax = (int)floor(x);
		}
		y++;
		x += minv;
	}
}

void DrawScanLines(HDC hdc, Entry table[], COLORREF color)
{
	for (int y = 0; y < MAX_ENTRIES; y++) {
		if (table[y].xmin < table[y].xmax) {
			for (int x = table[y].xmin; x <= table[y].xmax; x++) {
				SetPixel(hdc, x, y, color);
			}
		}
	}
}

void ConvexFill(HDC hdc, Point p[], int n, COLORREF color)
{
	Entry* table = new Entry[MAX_ENTRIES];
	InitializeEntries(table);
	Point v1 = p[n - 1];
	for (int i = 0; i < n; i++)
	{
		Point v2 = p[i];
		ScanEdge(v1, v2, table);
		v1 = p[i];
	}
	DrawScanLines(hdc, table, color);
	delete[]table;
}

/*END CONVEX PPOLYGON FILLING*/

/* GENERAL POLYGON FILLING*/
struct EdgeRec
{
	double x;
	double minv;
	int ymax;
	bool operator<(EdgeRec r)
	{
		return x < r.x;
	}
};
typedef list<EdgeRec> EdgeList;

EdgeRec InitEdgeRec(Point& v1, Point& v2)
{
	if (v1.y > v2.y)swap(v1, v2);
	EdgeRec rec;
	rec.x = v1.x;
	rec.ymax = v2.y;
	rec.minv = (double)(v2.x - v1.x) / (v2.y - v1.y);
	return rec;
}

void InitEdgeTable(Point* polygon, int n, EdgeList table[])
{
	Point v1 = polygon[n - 1];
	for (int i = 0; i < n; i++)
	{
		Point v2 = polygon[i];
		if (v1.y == v2.y) { v1 = v2; continue; }
		EdgeRec rec = InitEdgeRec(v1, v2);
		table[v1.y].push_back(rec);
		v1 = polygon[i];
	}
}

void GeneralPolygonFill(HDC hdc, Point* polygon, int n, COLORREF c)
{
	EdgeList* table = new EdgeList[MAX_ENTRIES];
	InitEdgeTable(polygon, n, table);
	int y = 0;
	while (y < MAX_ENTRIES && table[y].size() == 0)y++;

	if (y == MAX_ENTRIES)return;
	EdgeList ActiveList = table[y];
	while (ActiveList.size() > 0)
	{
		ActiveList.sort();
		for (EdgeList::iterator it = ActiveList.begin(); it != ActiveList.end(); it++)
		{
			int x1 = (int)ceil(it->x);
			it++;
			int x2 = (int)floor(it->x);
			for (int x = x1; x <= x2; x++)SetPixel(hdc, x, y, c);
		}
		y++;
		EdgeList::iterator it = ActiveList.begin();
		while (it != ActiveList.end())
			if (y == it->ymax) it = ActiveList.erase(it); else it++;
		for (EdgeList::iterator it = ActiveList.begin(); it != ActiveList.end(); it++)
			it->x += it->minv;
		ActiveList.insert(ActiveList.end(), table[y].begin(), table[y].end());
	}
	delete[] table;
}

/*END GENERAL POLYGON FILLING*/

void DrawLineSimpleDDA(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color)
{
	int dx = xe - xs;
	int dy = ye - ys;
	SetPixel(hdc, xs, ys, color);
	if (abs(dx) >= abs(dy))
	{
		int x = xs, xinc = dx > 0 ? 1 : -1;
		double y = ys, yinc = (double)dy / dx * xinc;
		while (x != xe)
		{
			x += xinc;
			y += yinc;
			SetPixel(hdc, x, round(y), color);
		}
	}
	else
	{
		int y = ys, yinc = dy > 0 ? 1 : -1;
		double x = xs, xinc = (double)dx / dy * yinc;
		while (y != ye)
		{
			x += xinc;
			y += yinc;
			SetPixel(hdc, round(x), y, color);
		}
	}
}

void DrawLineMidpoint(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color)
{

	int dx = xe - xs;
	int dy = ye - ys;

	double m = dy / (double)dx;

	if (m < 1) {
		if (xe < xs) {
			swap(xs, ys, xe, ye);
		}
		dx = xe - xs;
		dy = ye - ys;
		int x = xs;
		int y = ys;
		int d = dx - (2 * dy);
		int change1 = 2 * (dx - dy);
		int change2 = -(2 * dy);

		SetPixel(hdc, x, y, color);
		while (x < xe) {
			if (d < 0) {
				d += change1;
				y++;
			}
			else {
				d += change2;
			}
			x++;
			SetPixel(hdc, x, y, color);
		}
	}
	else {
		if (ye < ys) {
			swap(xs, ys, xe, ye);
		}
		dx = xe - xs;
		dy = ye - ys;
		int x = xs;
		int y = ys;
		int d = (2 * dx) - dy;
		int change1 = 2 * (dx - dy);
		int change2 = 2 * dx;

		SetPixel(hdc, x, y, color);
		while (y < ye) {
			if (d > 0) {
				d += change1;
				x++;
			}
			else {
				d += change2;
			}
			y++;
			SetPixel(hdc, x, y, color);
		}
	}

}

void DrawLineParametric(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color)
{
	if (xs > xe) {
		swap(xs, ys, xe, ye);
	}
	double t = 0;
	for (; t < 1; t += 0.001) {
		double x = xs + t * (xe - xs);
		double y = ys + t * (ye - ys);
		SetPixel(hdc, Round(x), Round(y), color);
	}
}

/* CIRCLES */

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

void DrawCircleNaive(HDC hdc, int xc, int yc, int R, COLORREF c)
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

void DrawCirclePolar(HDC hdc, int xc, int yc, int R, COLORREF color)
{
	int x = R, y = 0;
	double theta = 0, dtheta = 1.0 / R;
	Draw8Points(hdc, xc, yc, x, y, color);
	while (x > y)
	{
		theta += dtheta;
		x = round(R * cos(theta));
		y = round(R * sin(theta));
		Draw8Points(hdc, xc, yc, x, y, color);
	}
}

void DrawCircleIterativePolar(HDC hdc, int xc, int yc, int R, COLORREF color)
{
	double x = R, y = 0;
	double dtheta = 1.0 / R;
	double cdtheta = cos(dtheta), sdtheta = sin(dtheta);
	Draw8Points(hdc, xc, yc, R, 0, color);
	while (x > y)
	{
		double x1 = x * cdtheta - y * sdtheta;
		y = x * sdtheta + y * cdtheta;
		x = x1;
		Draw8Points(hdc, xc, yc, round(x), round(y), color);
	}
}

void DrawCircleMidpoint(HDC hdc, int xc, int yc, int R, COLORREF color)
{
	int x = 0, y = R;
	int d = 1 - R;
	Draw8Points(hdc, xc, yc, x, y, color);
	while (x < y)
	{
		if (d < 0)
			d += 2 * x + 2;
		else
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
		Draw8Points(hdc, xc, yc, x, y, color);
	}
}


void DrawCircleModifiedMidpoint(HDC hdc, int xc, int yc, int R, COLORREF color)
{
	int x = 0, y = R;
	int d = 1 - R;
	int c1 = 3, c2 = 5 - 2 * R;
	Draw8Points(hdc, xc, yc, x, y, color);
	while (x < y)
	{
		if (d < 0)
		{
			d += c1;
			c2 += 2;
		}
		else
		{
			d += c2;
			c2 += 4;
			y--;
		}
		c1 += 2;
		x++;
		Draw8Points(hdc, xc, yc, x, y, color);
	}
}

void MyFloodFillRec(HDC hdc, int x, int y, COLORREF borderColor, COLORREF fillColor) {
	COLORREF c = GetPixel(hdc, x, y);
	if (c == borderColor || c == fillColor) return;
	SetPixel(hdc, x, y, fillColor);
	MyFloodFillRec(hdc, x + 1, y, borderColor, fillColor);
	MyFloodFillRec(hdc, x, y + 1, borderColor, fillColor);
	MyFloodFillRec(hdc, x - 1, y, borderColor, fillColor);
	MyFloodFillRec(hdc, x, y - 1, borderColor, fillColor);
}


void MyFloodFillNonRec(HDC hdc, int x, int y, COLORREF borderColor, COLORREF fillColor) {
	stack<Point> s;
	s.push(Point(x, y));
	while (!s.empty()) {
		Point current = s.top();
		s.pop();
		COLORREF c = GetPixel(hdc, current.x, current.y);
		if (c == borderColor || c == fillColor) continue;
		SetPixel(hdc, current.x, current.y, fillColor);
		s.push(Point(current.x + 1, current.y));
		s.push(Point(current.x, current.y + 1));
		s.push(Point(current.x - 1, current.y));
		s.push(Point(current.x, current.y - 1));
	}
}

void Draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color)
{
	SetPixel(hdc, xc + x, yc + y, color);
	SetPixel(hdc, xc + x, yc - y, color);
	SetPixel(hdc, xc - x, yc - y, color);
	SetPixel(hdc, xc - x, yc + y, color);
}

void DrawElipseDirect(HDC hdc, int xc, int yc, int A, int B, COLORREF color) {
	int x = 0;
	double y = B;
	Draw4Points(hdc, xc, yc, 0, B, color);
	while (x * B * B < y * A * A) {
		x++;
		y = B * sqrt(1 - ((1.0 * x * x) / (A * A)));
		Draw4Points(hdc, xc, yc, x, Round(y), color);
	}
	int y1 = 0;
	double x1 = A;
	Draw4Points(hdc, xc, yc, A, 0, color);
	while (x1 * B * B > y1 * A * A) {
		y1++;
		x1 = A * sqrt(1 - ((1.0 * y1 * y1) / (B * B)));
		Draw4Points(hdc, xc, yc, Round(x1), y1, color);
	}
}

void DrawElipsePolar(HDC hdc, int xc, int yc, int A, int B, COLORREF color) {
	for (double theta = 0; theta < 90; theta += 0.01) {
		int x = A * cos(theta);
		int y = B * sin(theta);
		Draw4Points(hdc, xc, yc, x, y, color);
	}
}


void DrawElipseMidpoint(HDC hdc, int xc, int yc, double A, double B, COLORREF c)
{
	double dx, dy, d1, d2, x, y;
	x = 0;
	y = B;

	// Initial decision parameter of region 1
	d1 = (B * B) - (A * A * B) + (0.25 * A * A);
	dx = 2 * B * B * x;
	dy = 2 * A * A * y;

	// For region 1
	while (dx < dy)
	{
		Draw4Points(hdc, xc, yc, x, y, c);
		// Print points based on 4-way symmetry
		// Checking and updating value of
		// decision parameter based on algorithm
		//either  at d<0 change is d(x+1,y) else change is d(x+1,y-1)
		if (d1 < 0)
		{
			x++;
			dx += (2 * B * B);
			d1 += dx + (B * B);
		}
		else
		{
			x++;
			y--;
			dx = dx + (2 * B * B);
			dy = dy - (2 * A * A);
			d1 = d1 + dx - dy + (B * B);
		}
	}

	// Decision parameter of region 2
	d2 = ((B * B) * ((x + 0.5) * (x + 0.5))) +
		((A * A) * ((y - 1) * (y - 1))) -
		(A * A * B * B);

	// Plotting points of region 2
	while (y >= 0)
	{
		// Print points based on 4-way symmetry
		Draw4Points(hdc, xc, yc, x, y, c);

		// Checking and updating parameter
		// value based on algorithm
		if (d2 > 0)
		{
			y--;
			dy = dy - (2 * A * A);
			d2 = d2 + (A * A) - dy;
		}
		else
		{
			y--;
			x++;
			dx = dx + (2 * B * B);
			dy = dy - (2 * A * A);
			d2 = d2 + dx - dy + (A * A);
		}
	}
}





struct Vector {
	double v[2];
	Vector(double x = 0, double y = 0)
	{
		v[0] = x; v[1] = y;
	}
	double& operator[](int i) {
		return v[i];
	}
};

void DrawHermiteCurve(HDC hdc, Vector& p1, Vector& T1, Vector& p2, Vector& T2, COLORREF c)
{
	double a0 = p1[0], a1 = T1[0],
		a2 = -3 * p1[0] - 2 * T1[0] + 3 * p2[0] - T2[0],
		a3 = 2 * p1[0] + T1[0] - 2 * p2[0] + T2[0];
	double b0 = p1[1], b1 = T1[1],
		b2 = -3 * p1[1] - 2 * T1[1] + 3 * p2[1] - T2[1],
		b3 = 2 * p1[1] + T1[1] - 2 * p2[1] + T2[1];
	for (double t = 0; t <= 1; t += 0.001)
	{
		double t2 = t * t, t3 = t2 * t;
		double x = a0 + a1 * t + a2 * t2 + a3 * t3;
		double y = b0 + b1 * t + b2 * t2 + b3 * t3;
		SetPixel(hdc, Round(x), Round(y), RGB(255, 0, 0));

		SetPixel(hdc, Round(x) + 1, Round(y) + 1, RGB(255, 0, 0));
		SetPixel(hdc, Round(x) + 2, Round(y) + 2, RGB(255, 0, 0));
	}
}

void DrawBezierCurve(HDC hdc, Vector& P0, Vector& P1, Vector& P2, Vector& P3, COLORREF c)
{
	Vector T0(3 * (P1.v[0] - P0.v[0]), 3 * (P1.v[1] - P0.v[1]));
	Vector T1(3 * (P3.v[0] - P2.v[0]), 3 * (P3.v[1] - P2.v[1]));
	DrawHermiteCurve(hdc, P0, T0, P3, T1, c);
}


void DrawCardinalSpline(HDC hdc, Vector P[], int n, double c, COLORREF color)
{
	double c1 = 1 - c;
	Vector T0(c1 * (P[2].v[0] - P[0].v[0]), c1 * (P[2].v[1] - P[0].v[1]));
	for (int i = 2; i < n - 1; i++)
	{
		Vector T1(c1 * (P[i + 1].v[0] - P[i - 1].v[0]), c1 * (P[i + 1].v[1] - P[i - 1].v[1]));
		DrawHermiteCurve(hdc, P[i - 1], T0, P[i], T1, color);
		T0 = T1;
	}
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

void drawRectangleFillBezier(HDC hdc, Point p1, Point p2, COLORREF c)
{
	if (p1.x > p2.x) {
		int temp = p1.x;
		p1.x = p2.x;
		p2.x = temp;
	}

	double distance = (p2.x - p1.x);
	double width = 0.5 * distance;

	DrawLineSimpleDDA(hdc, p1.x, p1.y, p1.x + distance, p1.y, c);

	DrawLineSimpleDDA(hdc, p1.x, p1.y - width, p1.x + distance, p1.y - width, c);

	DrawLineSimpleDDA(hdc, p1.x, p1.y, p1.x, p1.y - width, c);

	DrawLineSimpleDDA(hdc, p1.x + distance, p1.y, p1.x + distance, p1.y - width, c);

	for (int i = 0; i < width; i++) {
		Vector p1b(p1.x, p1.y - i);
		Vector p1bb(p1.x, p1.y - i);
		Vector p2bb(p1.x + distance - 1, p1.y - i);
		Vector p2b(p1.x + distance - 1, p1.y - i);

		DrawBezierCurve(hdc, p1b, p1bb, p2bb, p2b, RGB(255, 0, 0));
	}

}

void drawSquareFillHermite(HDC hdc, Point p1, Point p2, COLORREF c)
{
	if (p1.x > p2.x) {
		int temp = p1.x;
		p1.x = p2.x;
		p2.x = temp;
	}

	double distance = (p2.x - p1.x);


	DrawLineSimpleDDA(hdc, p1.x, p1.y, p1.x + distance, p1.y, c);

	DrawLineSimpleDDA(hdc, p1.x, p1.y - distance, p1.x + distance, p1.y - distance, c);

	DrawLineSimpleDDA(hdc, p1.x, p1.y, p1.x, p1.y - distance, c);

	DrawLineSimpleDDA(hdc, p1.x + distance, p1.y, p1.x + distance, p1.y - distance, c);

	for (int i = 0; i < distance; i++) {
		Vector p1b(p1.x + i, p1.y);
		Vector p1bb(p1.x + i, p1.y);
		Vector p2bb(p1.x + i, p1.y-distance);
		Vector p2b(p1.x + i, p1.y-distance);

		Vector T0(3 * (p1bb.v[0]- p1b.v[0]), 3 * (p1bb.v[1] - p1b.v[1]));
		Vector T1(3 * (p2b.v[0] - p2bb.v[0]), 3 * (p2b.v[1] - p2bb.v[1]));

		DrawHermiteCurve(hdc, p1b, T0, p2bb, T1, RGB(255, 0, 0));
	}

}




void EndOpenGl(HGLRC glrc)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(glrc);
}
LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
	static HDC hdc;
	static Vector p[4];
	static Point sp[2];
	static int index = 0;
	static HGLRC glrc;
	static Point pol[4];
	switch (mcode)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		break;
	case WM_SIZE:
		AdjustWindowFor2D(hdc, LOWORD(lp), HIWORD(lp));
		break;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);
		//DrawCircleNaive(hdc,50,50,20,RGB(255,0,0));
		//DrawCirclePolar(hdc, 100, 100, 40, RGB(255, 0, 0));
		//DrawCircleIterativePolar(hdc, 50, 50, 20, RGB(255, 0, 0));
		//DrawCircleMidpoint(hdc, 50, 50, 20, RGB(255, 0, 0));
		//DrawCircleModifiedMidpoint(hdc, 50, 50, 20, RGB(255, 0, 0));
		//MyFloodFillNonRec(hdc, 50, 50, RGB(255, 0, 0), RGB(0, 0, 255));
		//DrawElipsePolar(hdc,250,200,100,50,RGB(255,0,0));
		//DrawElipseMidpoint(hdc, 250, 200, 100, 50, RGB(255, 0, 0));
		sp[index] = Point(LOWORD(lp), HIWORD(lp));
		if (index == 1) {
			drawSquareFillHermite(hdc, sp[0], sp[1], RGB(255, 0, 0));
		}
		else index++;
		/*
		p[index] = Vector(LOWORD(lp), HIWORD(lp));
		if (index == 3) {
			Vector T1(3 * (p[1][0] - p[0][0]), 3 * (p[1][1] - p[0][1]));
			Vector T2(3 * (p[3][0] - p[2][0]), 3 * (p[3][1] - p[2][1]));
			hdc = GetDC(hwnd);
			//DrawBezierCurve(hdc, p[0], p[1], p[2], p[3], RGB(255, 0, 0));

				DrawHermiteCurve(hdc, p[0], T1, p[3], T2, RGB(0, 0, 255));

			ReleaseDC(hwnd, hdc);
			index = 0;
		}
		else index++;
		*/
		//pol[0].x = 100; pol[0].y = 400;
		//pol[1].x = 100; pol[1].y = 500;


		//pol[2].x = 1000; pol[3].y = 400;
		//pol[3].x = 200; pol[2].y = 500;
		//GeneralPolygonFill(hdc, pol, 4, RGB(255, 0, 0));
		//DrawCardinalSpline(hdc,P,5,4,RGB(255,0,0));
		ReleaseDC(hwnd, hdc);
		break;
	case WM_RBUTTONDOWN:

		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		EndOpenGl(glrc);
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
