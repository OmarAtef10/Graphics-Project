#pragma once
#include <math.h>
#include "UJtils.h"
using namespace std;


static void Draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color)
{
	SetPixel(hdc, xc + x, yc + y, color);
	SetPixel(hdc, xc + x, yc - y, color);
	SetPixel(hdc, xc - x, yc - y, color);
	SetPixel(hdc, xc - x, yc + y, color);
}

static void DrawElipseDirect(HDC hdc, int xc, int yc, int A, int B, COLORREF color) {
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

static void DrawElipsePolar(HDC hdc, int xc, int yc, int A, int B, COLORREF color) {
	for (double theta = 0; theta < 90; theta += 0.01) {
		int x = A * cos(theta);
		int y = B * sin(theta);
		Draw4Points(hdc, xc, yc, x, y, color);
	}
}


static void DrawElipseMidpoint(HDC hdc, int xc, int yc, double A, double B, COLORREF c)
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

