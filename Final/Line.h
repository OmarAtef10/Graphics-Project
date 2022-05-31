#pragma once
#include "UJtils.h"
using namespace std;

static void DrawLineSimpleDDA(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color)
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
			SetPixel(hdc, x, Round(y), color);
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
			SetPixel(hdc, Round(x), y, color);
		}
	}
}

static void DrawLineMidpoint(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
	if (x1 > x2)
	{
		DrawLineMidpoint(hdc,x2, y2, x1, y1,color);
		return;
	}

	int slope;
	int dx, dy, d, x, y;

	dx = x2 - x1;
	dy = y2 - y1;
	d = dx - 2 * dy;
	y = y1;

	if (dy < 0) {
		slope = -1;
		dy = -dy;
	}
	else {
		slope = 1;
	}

	for (x = x1; x < x2; x++) {
		SetPixel(hdc, x, y, color);
		if (d <= 0) {
			d += 2 * dx - 2 * dy;
			y += slope;
		}
		else {
			d += -2 * dy;
		}
	}

}

static void DrawLineParametric(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color)
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
