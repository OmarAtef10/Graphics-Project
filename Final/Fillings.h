#pragma once

#include "Curve.h"
#include "Circle.h"


#include <iostream>
#include <stack>
#include <list>
#define MAXINT 69696969
#define MININT -69696969

static int circleCx;
static int circleCy;
static int radius;

using namespace std;

static void MyFloodFillRec(HDC hdc, int x, int y, COLORREF borderColor, COLORREF fillColor) {
    COLORREF c = GetPixel(hdc, x, y);
    if (c == borderColor || c == fillColor) return;
    SetPixel(hdc, x, y, fillColor);
    MyFloodFillRec(hdc, x + 1, y, borderColor, fillColor);
    MyFloodFillRec(hdc, x, y + 1, borderColor, fillColor);
    MyFloodFillRec(hdc, x - 1, y, borderColor, fillColor);
    MyFloodFillRec(hdc, x, y - 1, borderColor, fillColor);
}


static void MyFloodFillNonRec(HDC hdc, int x, int y, COLORREF borderColor, COLORREF fillColor) {
    std::stack<Point> s;
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


/*Convex Polyogn Filling*/
#define MAX_ENTRIES 600
struct Entry {
    int xmax, xmin;
};

static void InitializeEntries(Entry table[]) {
    for (int i = 0; i < MAX_ENTRIES; i++) {
        table[i].xmin = MAXINT;
        table[i].xmax = MININT;
    }
}

static void ScanEdge(Point v1, Point v2, Entry table[]) {
    if (v1.y == v2.y)return;
    if (v1.y > v2.y)swap(v1, v2);
    double minv = (double) (v2.x - v1.x) / (v2.y - v1.y);
    double x = v1.x;
    int y = v1.y;
    while (y < v2.y) {
        if (x < table[y].xmin) {
            table[y].xmin = (int) ceil(x);
        }
        if (x > table[y].xmax) {
            table[y].xmax = (int) floor(x);
        }
        y++;
        x += minv;
    }
}

static void DrawScanLines(HDC hdc, Entry table[], COLORREF color) {
    for (int y = 0; y < MAX_ENTRIES; y++) {
        if (table[y].xmin < table[y].xmax) {
            for (int x = table[y].xmin; x <= table[y].xmax; x++) {
                SetPixel(hdc, x, y, color);
            }
        }
    }
}

static void ConvexFill(HDC hdc, Point p[], int n, COLORREF color) {
    Entry *table = new Entry[MAX_ENTRIES];
    InitializeEntries(table);
    Point v1 = p[n - 1];
    for (int i = 0; i < n; i++) {
        Point v2 = p[i];
        ScanEdge(v1, v2, table);
        v1 = p[i];
    }
    DrawScanLines(hdc, table, color);
    delete[]table;
}

/*END CONVEX PPOLYGON FILLING*/

/* GENERAL POLYGON FILLING*/
struct EdgeRec {
    double x;
    double minv;
    int ymax;

    bool operator<(EdgeRec r) {
        return x < r.x;
    }
};

typedef list<EdgeRec> EdgeList;

static EdgeRec InitEdgeRec(Point &v1, Point &v2) {
    if (v1.y > v2.y)swap(v1, v2);
    EdgeRec rec;
    rec.x = v1.x;
    rec.ymax = v2.y;
    rec.minv = (double) (v2.x - v1.x) / (v2.y - v1.y);
    return rec;
}

static void InitEdgeTable(Point *polygon, int n, EdgeList table[]) {
    Point v1 = polygon[n - 1];
    for (int i = 0; i < n; i++) {
        Point v2 = polygon[i];
        if (v1.y == v2.y) {
            v1 = v2;
            continue;
        }
        EdgeRec rec = InitEdgeRec(v1, v2);
        table[v1.y].push_back(rec);
        v1 = polygon[i];
    }
}

static void GeneralPolygonFill(HDC hdc, Point *polygon, int n, COLORREF c) {
    EdgeList *table = new EdgeList[MAX_ENTRIES];
    InitEdgeTable(polygon, n, table);
    int y = 0;
    while (y < MAX_ENTRIES && table[y].size() == 0)y++;

    if (y == MAX_ENTRIES)return;
    EdgeList ActiveList = table[y];
    while (ActiveList.size() > 0) {
        ActiveList.sort();
        for (EdgeList::iterator it = ActiveList.begin(); it != ActiveList.end(); it++) {
            int x1 = (int) ceil(it->x);
            it++;
            int x2 = (int) floor(it->x);
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

static void drawRectangleFillBezier(HDC hdc, Point p1, Point p2, COLORREF c) {
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

        DrawBezierCurve(hdc, p1b, p1bb, p2bb, p2b, c);
    }

}

static void drawSquareFillHermite(HDC hdc, Point p1, Point p2, COLORREF c) {
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
        Vector p2bb(p1.x + i, p1.y - distance);
        Vector p2b(p1.x + i, p1.y - distance);

        Vector T0(3 * (p1bb.v[0] - p1b.v[0]), 3 * (p1bb.v[1] - p1b.v[1]));
        Vector T1(3 * (p2b.v[0] - p2bb.v[0]), 3 * (p2b.v[1] - p2bb.v[1]));

        DrawHermiteCurve(hdc, p1b, T0, p2bb, T1, c);
    }

}

/*************************** TASK K & L ******************************************/
static void DrawLine1(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (abs(dy) <= abs(dx)) {
        if (x1 > x2)swap(x1, y1, x2, y2);
        SetPixel(hdc, x1, y1, c);
        int x = x1;
        while (x < x2) {
            x++;
            double y = y1 + (double) (x - x1) * dy / dx;
            SetPixel(hdc, x, ceil(y), c);
            SetPixel(hdc, x, floor(y), c);

        }
    } else {
        if (y1 > y2)swap(x1, y1, x2, y2);
        SetPixel(hdc, x1, y1, c);
        int y = y1;
        while (y < y2) {
            y++;
            double x = x1 + (double) (y - y1) * dx / dy;
            SetPixel(hdc, ceil(x), y, c);
            SetPixel(hdc, floor(x), y, c);

        }
    }
}

static void Draw8PointsSHADE4(HDC hdc, int xc, int yc, int x, int y, COLORREF c)   //quad number 4
{

    DrawLine1(hdc, xc, yc, x + xc, y + yc, c);
    DrawLine1(hdc, xc, yc, xc + y, yc + x, c);

}

static void Draw8PointsSHADE2(HDC hdc, int xc, int yc, int x, int y, COLORREF c)  //quad number 2
{

    DrawLine1(hdc, xc, yc, xc - y, yc - x, c);
    DrawLine1(hdc, xc, yc, xc - x, yc - y, c);
}

static void Draw8PointsSHADE3(HDC hdc, int xc, int yc, int x, int y, COLORREF c) //quad number 3
{

    DrawLine1(hdc, xc, yc, xc - x, yc + y, c);
    DrawLine1(hdc, xc, yc, xc - y, yc + x, c);


}

static void Draw8PointsSHADE1(HDC hdc, int xc, int yc, int x, int y, COLORREF c) // quad number 1
{

    DrawLine1(hdc, xc, yc, xc + x, yc - y, c);
    DrawLine1(hdc, xc, yc, xc + y, yc - x, c);
}


static void DrawCircle(HDC hdc, int xc, int yc, int R, COLORREF c) {
    int x = 0;
    double y = R;
    Draw8Points(hdc, xc, yc, 0, R, c);
    while (x < y) {
        x++;
        y = sqrt((double) R * R - x * x);
        Draw8Points(hdc, xc, yc, x, Round(y), c);
    }
}


static void DrawCircle1(HDC hdc, int xc, int yc, int R, COLORREF c) {
    int x = 0;
    double y = R;
    Draw8PointsSHADE1(hdc, xc, yc, 0, R, c);
    while (x < y) {
        x++;
        y = sqrt((double) R * R - x * x);
        Draw8PointsSHADE1(hdc, xc, yc, x, Round(y), c);
    }
}

static void DrawCircle2(HDC hdc, int xc, int yc, int R, COLORREF c) {
    int x = 0;
    double y = R;
    Draw8PointsSHADE2(hdc, xc, yc, 0, R, c);
    while (x < y) {
        x++;
        y = sqrt((double) R * R - x * x);
        Draw8PointsSHADE2(hdc, xc, yc, x, Round(y), c);
    }
}

static void DrawCircle3(HDC hdc, int xc, int yc, int R, COLORREF c) {
    int x = 0;
    double y = R;
    Draw8PointsSHADE3(hdc, xc, yc, 0, R, c);
    while (x < y) {
        x++;
        y = sqrt((double) R * R - x * x);
        Draw8PointsSHADE3(hdc, xc, yc, x, Round(y), c);
    }
}

static void DrawCircle4(HDC hdc, int xc, int yc, int R, COLORREF c) {
    int x = 0;
    double y = R;
    Draw8PointsSHADE4(hdc, xc, yc, 0, R, c);
    while (x < y) {
        x++;
        y = sqrt((double) R * R - x * x);
        Draw8PointsSHADE4(hdc, xc, yc, x, Round(y), c);
    }
}


static void fillCircleLine(HDC hdc, int x, int y, int radius, COLORREF c) {

    if (x > circleCx && y < circleCy) {
        DrawCircle1(hdc, circleCx, circleCy, radius, c);

    } else if (x < circleCx && y < circleCy) {

        DrawCircle2(hdc, circleCx, circleCy, radius, c);


    } else if (x < circleCx && y > circleCy) {

        DrawCircle3(hdc, circleCx, circleCy, radius, c);

    } else if (x > circleCx && y > circleCy) {

        DrawCircle4(hdc, circleCx, circleCy, radius, c);

    }
}/**************************************************************************************/


static void DrawQuad3(HDC hdc, int xc, int yc, int x, int y, COLORREF c) //Quad number 3
{
    SetPixel(hdc, xc - y, yc + x, c);
    SetPixel(hdc, xc - x, yc + y, c);
}

static void DrawQuad1(HDC hdc, int xc, int yc, int x, int y, COLORREF c) //Quad number 1
{
    SetPixel(hdc, xc + x, yc - y, c);
    SetPixel(hdc, xc + y, yc - x, c);
}

static void DrawQuad2(HDC hdc, int xc, int yc, int x, int y, COLORREF c) //Quad number 2
{
    SetPixel(hdc, xc - y, yc - x, c);
    SetPixel(hdc, xc - x, yc - y, c);
}

static void DrawQuad4(HDC hdc, int xc, int yc, int x, int y, COLORREF c) //Quad number 4
{
    SetPixel(hdc, xc + y, yc + x, c);
    SetPixel(hdc, xc + x, yc + y, c);

}


static void DrawCircleQuad1(HDC hdc, int xc, int yc, int R, COLORREF c) {
    int x = 0;
    double y = R;
    DrawQuad1(hdc, xc, yc, 0, R, c);
    while (x < y) {
        x++;
        y = sqrt((double) R * R - x * x);
        DrawQuad1(hdc, xc, yc, x, Round(y), c);
    }
}

static void DrawCircleQuad2(HDC hdc, int xc, int yc, int R, COLORREF c) {
    int x = 0;
    double y = R;
    DrawQuad2(hdc, xc, yc, 0, R, c);
    while (x < y) {
        x++;
        y = sqrt((double) R * R - x * x);
        DrawQuad2(hdc, xc, yc, x, Round(y), c);
    }
}

static void DrawCircleQuad3(HDC hdc, int xc, int yc, int R, COLORREF c) {
    int x = 0;
    double y = R;
    DrawQuad3(hdc, xc, yc, 0, R, c);
    while (x < y) {
        x++;
        y = sqrt((double) R * R - x * x);
        DrawQuad3(hdc, xc, yc, x, Round(y), c);
    }
}

static void DrawCircleQuad4(HDC hdc, int xc, int yc, int R, COLORREF c) {
    int x = 0;
    double y = R;
    DrawQuad4(hdc, xc, yc, 0, R, c);
    while (x < y) {
        x++;
        y = sqrt((double) R * R - x * x);
        DrawQuad4(hdc, xc, yc, x, Round(y), c);
    }
}


static void fillCircleQuad(HDC hdc, int x, int y, int radius1, COLORREF c) {
    int temp = radius1;
    if (x > circleCx && y < circleCy) {
        while (temp >= 0) {
            DrawCircleQuad1(hdc, circleCx, circleCy, temp, c);
            temp -= 1;

        }
    } else if (x < circleCx && y < circleCy) {
        while (temp >= 0) {
            DrawCircleQuad2(hdc, circleCx, circleCy, temp, c);
            temp -= 1;

        }

    } else if (x < circleCx && y > circleCy) {
        while (temp >= 0) {
            DrawCircleQuad3(hdc, circleCx, circleCy, temp, c);
            temp -= 1;

        }
    } else if (x > circleCx && y > circleCy) {
        while (temp >= 0) {
            DrawCircleQuad4(hdc, circleCx, circleCy, temp, c);
            temp -= 1;
        }
    }
}
