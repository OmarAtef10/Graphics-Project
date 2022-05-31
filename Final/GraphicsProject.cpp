#include <Windows.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <CommCtrl.h>
#include <tchar.h>
#include <math.h>
#include "Elipse.h"
#include "Line.h"
#include "Circle.h"
#include "Curve.h"
#include "Fillings.h"
#include "Clippings.h"
#include <string>

using namespace std;

class Drawer {
public:
    string name;
    vector<Point> points;
    COLORREF c;
    int radius = 0;
};

/*=========================================================================================================*/

vector<Drawer> drawings;
bool saveSelected = false, loadSelected = false, clearSelected = false, exitSelected = false;
bool circleDirectSelected = false, circlePolarSelected = false, circleIterativePolarSelected = false, circleMidPointSelected = false, circleModifiedMidPointSelected = false;
bool lineDDASelected = false, lineParametricSelected = false, lineMidPointSelected = false;
bool ellipseDirectSelected = false, ellipsePolarSelected = false, ellipseMidPointSelected = false;
bool hermiteCurveSelected = false, bezierCurveSelected = false, cardinalSplineSelected = false;
bool fillCircleLinesSelected = false, fillShapeHermiteSelected = false, fillShapeBezierSelected = false, fillConvexPolygonSelected = false, fillPolygonGeneralSelected = false, recFloodFillSelected = false, iterFloodFillSelected = false;
bool recPointClipSelected = false, recLineClipSelected = false, recPolygonClipSelected = false, squarePointClipSelected = false, squareLineClipSelected = false, circlePointClipSelected = false, circleLineClipSelected = false, rectangleSelected = false;
bool fillCircleSelected = false, squareSelected = false;
bool colorSelected = false;
COLORREF color, bColor;
static int pointCount = 0;
Point p1, p2, p3, p4, p5, p6, p7, t1, t2;
vector<Point> points;
vector<Point> rec;


void resetOptions() {
    saveSelected = false, loadSelected = false, clearSelected = false, exitSelected = false;
    lineDDASelected = false, lineParametricSelected = false, lineMidPointSelected = false;
    circleDirectSelected = false, circlePolarSelected = false, circleIterativePolarSelected = false, circleMidPointSelected = false, circleModifiedMidPointSelected = false;
    ellipseDirectSelected = false, ellipsePolarSelected = false, ellipseMidPointSelected = false;
    hermiteCurveSelected = false, bezierCurveSelected = false, cardinalSplineSelected = false;
    fillCircleLinesSelected = false, fillShapeHermiteSelected = false, fillShapeBezierSelected = false, fillConvexPolygonSelected = false, fillPolygonGeneralSelected = false, recFloodFillSelected = false, iterFloodFillSelected = false;
    recPointClipSelected = false, recLineClipSelected = false, recPolygonClipSelected = false, squarePointClipSelected = false, squareLineClipSelected = false, circlePointClipSelected = false, circleLineClipSelected = false, rectangleSelected = false;
    squareSelected = false, fillCircleSelected = false;
    points.clear();
}

/*=========================================================================================================*/

void addDrawer(string name, vector<Point> points, COLORREF c, int radius) {
    Drawer a;
    a.c = c;
    a.name = name;
    a.points = points;
    a.radius = radius;
    drawings.push_back(a);
}

void addDrawer(string name, vector<Point> points, COLORREF c) {
    addDrawer(name, points, c, 0);
}


/*=========================================================================================================*/
// not working on convex filling
// Floodfill strange lmao ???
void draw(Drawer a, HWND hwnd) {
    if (a.points.size() == 0)
        return;
    HDC hdc = GetDC(hwnd);
    cout << "Action : " << a.name << endl;

    if (a.name == "DrawLineDDA") {
        DrawLineSimpleDDA(hdc, a.points[0].x, a.points[0].y, a.points[1].x, a.points[1].y, a.c);
    } else if (a.name == "DrawLineMidPoint") {
        DrawLineMidpoint(hdc, a.points[0].x, a.points[0].y, a.points[1].x, a.points[1].y, a.c);
    } else if (a.name == "DrawLineParametric") {
        DrawLineParametric(hdc, a.points[0].x, a.points[0].y, a.points[1].x, a.points[1].y, color);
    }
        /*----------------------------------*/
    else if (a.name == "DrawCircleDirect") {
        int R = distanceBetweenPoints(a.points[0], a.points[1]);
        DrawCircleNaive(hdc, a.points[0].x, a.points[1].y, R, a.c);

    } else if (a.name == "DrawCirclePolar") {
        int r = sqrt((a.points[0].x - a.points[1].x) * (a.points[0].x - a.points[1].x) +
                     (a.points[0].y - a.points[1].y) * (a.points[0].y - a.points[1].y));
        DrawCirclePolar(hdc, a.points[0].x, a.points[0].y, r, a.c);
    } else if (a.name == "DrawCircleIterativePolar") {
        int r = sqrt((a.points[0].x - a.points[1].x) * (a.points[0].x - a.points[1].x) +
                     (a.points[0].y - a.points[1].y) * (a.points[0].y - a.points[1].y));
        DrawCircleIterativePolar(hdc, a.points[0].x, a.points[0].y, r, a.c);
    } else if (a.name == "DrawCircleMidPoint") {
        int r = sqrt((a.points[0].x - a.points[1].x) * (a.points[0].x - a.points[1].x) +
                     (a.points[0].y - a.points[1].y) * (a.points[0].y - a.points[1].y));
        DrawCircleMidpoint(hdc, a.points[0].x, a.points[0].y, r, a.c);
    } else if (a.name == "DrawCircleModifiedMidPoint") {
        int r = sqrt((a.points[0].x - a.points[1].x) * (a.points[0].x - a.points[1].x) +
                     (a.points[0].y - a.points[1].y) * (a.points[0].y - a.points[1].y));
        DrawCircleModifiedMidpoint(hdc, a.points[0].x, a.points[0].y, r, a.c);
    }
        /*----------------------------------*/
    else if (a.name == "DrawEllipseDirect") {
        int A = distanceBetweenPoints(a.points[0], a.points[1]);
        int B = distanceBetweenPoints(a.points[0], a.points[2]);
        DrawElipseDirect(hdc, a.points[0].x, a.points[0].y, A, B, a.c);
    } else if (a.name == "DrawEllipsePolar") {

        int A = distanceBetweenPoints(a.points[0], a.points[1]);
        int B = distanceBetweenPoints(a.points[0], a.points[2]);
        DrawElipsePolar(hdc, a.points[0].x, a.points[0].y, A, B, a.c);
    } else if (a.name == "DrawEllipseMidPoint") {

        int A = distanceBetweenPoints(a.points[0], a.points[1]);
        int B = distanceBetweenPoints(a.points[0], a.points[2]);
        DrawElipseMidpoint(hdc, a.points[0].x, a.points[0].y, A, B, a.c);
    }
        /*----------------------------------*/
    else if (a.name == "DrawHermiteCurve") {
        Vector v1(a.points[0].x, a.points[0].y);
        Vector v2(a.points[1].x, a.points[1].y);
        Vector v3(a.points[2].x, a.points[2].y);
        Vector v4(a.points[3].x, a.points[3].y);

        Vector T0(3 * (v2.v[0] - v1.v[0]), 3 * (v2.v[1] - v1.v[1]));
        Vector T1(3 * (v4.v[0] - v3.v[0]), 3 * (v4.v[1] - v3.v[1]));
        DrawHermiteCurve(hdc, v1, T0, v4, T1, a.c);
    } else if (a.name == "DrawBezierCurve") {
        Vector v1(a.points[0].x, a.points[0].y);
        Vector v2(a.points[1].x, a.points[1].y);
        Vector v3(a.points[2].x, a.points[2].y);
        Vector v4(a.points[3].x, a.points[3].y);
        DrawBezierCurve(hdc, v1, v2, v3, v4, a.c);
    }
    else if (a.name == "DrawCardinalSpline") {
        Vector p[7];
        for (int i = 0; i < 7; i++) {
            p[i][0] = a.points[i].x;
            p[i][1] = a.points[i].y;
        }
        DrawCardinalSpline(hdc, p, 7, 0.1, a.c);
    }
        /*----------------------------------*/
    else if (a.name == "FillCircleLines") {
        circleCx = a.points[1].x;
        circleCy = a.points[1].y;
        fillCircleLine(hdc, a.points[0].x, a.points[0].y, a.radius, a.c);
    } else if (a.name == "FillCircleCircles") {
        circleCx = a.points[1].x;
        circleCy = a.points[1].y;
        fillCircleQuad(hdc, a.points[0].x, a.points[0].y, a.radius, a.c);
    } else if (a.name == "FillSquareHermite") {
        int r = sqrt((a.points[0].x - a.points[1].x) * (a.points[0].x - a.points[1].x) +
                     (a.points[0].y - a.points[1].y) * (a.points[0].y - a.points[1].y));
        drawSquareFillHermite(hdc, a.points[0], a.points[1], a.c);
    } else if (a.name == "FillRectBezier") {
        drawRectangleFillBezier(hdc, a.points[0], a.points[1], a.c);
    } else if (a.name == "FillPolygon") {
        Point p[5];
        for (int i = 0; i < 5; i++) {
            p[i].x = a.points[i].x;
            p[i].y = a.points[i].y;
        }
        ConvexFill(hdc, p, 5, a.c);
    } else if (a.name == "Non-ConvexPloygon") {
        Point p[5];
        for (int i = 0; i < 5; i++) {
            p[i].x = a.points[i].x;
            p[i].y = a.points[i].y;
        }
        GeneralPolygonFill(hdc, p, 5, a.c);
    }
    else if (a.name == "Recursive-FloodFill") {
        MyFloodFillRec(hdc, a.points[0].x, a.points[0].y, bColor, a.c);
    }
    else if (a.name == "NonRecursive-FloodFill") {
        MyFloodFillNonRec(hdc, a.points[0].x, a.points[0].y, a.c, a.c);
    }
        /*----------------------------------*/

    else if (a.name == "Square") {
        DrawSqClip(hdc, a.points[0], a.points[1], a.c);
    } else if (a.name == "PointClipping-Rectangle") {
        DrawRec(hdc, a.points[1], a.points[2], a.c);
        recPointClip(hdc, a.points[0].x, a.points[0].y, a.points[1].x, a.points[1].y, a.points[2].x, a.points[2].y,
                     a.c);
    } else if (a.name == "LineClipping-Rectangle") {
        DrawRec(hdc, a.points[2], a.points[3], a.c);
        CohenSuth(hdc, a.points[0].x, a.points[0].y, a.points[1].x, a.points[1].y, a.points[2].x, a.points[2].y,
                  a.points[3].x, a.points[3].y, a.c);
    } else if (a.name == "PolygonClipping-Rectangle") {
        DrawRec(hdc, a.points[5], a.points[6], a.c);
        vector<Point> tempVec = {a.points[0], a.points[1], a.points[2], a.points[3], a.points[4]};
        polygonClip(hdc, tempVec, 5, a.points[5].x, a.points[5].y,
                    a.points[6].x, a.points[6].y, a.c);
    } else if (a.name == "PointClipping-Square") {
        DrawSquare(hdc, a.points[1], a.points[2], a.c);
        sqPointClip(hdc, a.points[0].x, a.points[0].y, a.c);
    } else if (a.name == "LineClipping-Square") {
        DrawSquare(hdc, a.points[2], a.points[3], a.c);
        CohenSuth(hdc, a.points[0].x, a.points[0].y, a.points[1].x, a.points[1].y, a.points[2].x, a.points[2].y,
                  a.points[3].x, a.points[3].y, a.c);
    } else if (a.name == "PointClipping-Circle") {
        circlePointClip(hdc, a.points[0], a.radius, a.c);
    } else if (a.name == "LineClipping-Circle") {
        DrawLineClip(hdc, a.points[0].x, a.points[0].y, a.points[1].x, a.points[1].y, a.radius, a.c);
    }

    ReleaseDC(hwnd, hdc);
}

/*=========================================================================================================*/

void SaveData()        //save function for all data in screen in file    //Not Working !!
{
    ofstream f;
    f.open("file.txt");
    int size = drawings.size();
    cout << "size = " << size << endl;
    for (int i = 0; i < size; i++) {
        cout << "Action Name : " << drawings[i].name << endl;
        f << drawings[i].name;
        f << " ";
        f << drawings[i].c;
        f << " ";
        f << drawings[i].radius;
        f << " ";
        f << drawings[i].points.size();
        f << " ";
        for (int j = 0; j < drawings[i].points.size(); j++) {
            cout << "Point " << j + 1 << ": (" << drawings[i].points[j].x << "," << drawings[i].points[j].y << ") "
                 << endl;
            std::string line = to_string(drawings[i].points[j].x) + " " + to_string(drawings[i].points[j].y);
            f << line;
            if (j != drawings[i].points.size())
                f << " ";
        }
        f << "\n";
    }
    f.close();
}

/*=========================================================================================================*/

void LoadData(HWND hwnd)        //load function to load data from file			//Not Working !!
{
    int size;
    ifstream f("file.txt");
    vector<Drawer> ac;
    if (!f.is_open()) {
        cout << "File doesn't exist !!" << endl;
    } else {
        while (!f.eof()) {
            Drawer a;
            int len;
            f >> a.name;
            f >> a.c;
            f >> a.radius;
            if (f.eof()) break;
            f >> len;

            for (int i = 0; i < len; i++) {
                Point p;
                f >> p.x;
                f >> p.y;
                a.points.push_back(p);
            }
            drawings.push_back(a);
        }

        f.close();
        for (int i = 0; i < drawings.size(); i++) {
            draw(drawings[i], hwnd);
        }
    }
}

/*=========================================================================================================*/

LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp) {
    HDC hdc;
    static HWND infoList;
    static HWND lineList;
    static HWND circleList;
    static HWND ellipseList;
    static HWND curveList;
    static HWND fillingsList;
    static HWND clippingList;
    static HWND colorList;
    static int x[100];
    static int y[100];
    static Drawer currentShape;
    static COLORREF color;

    switch (mcode) {
        case WM_CREATE:
            infoList = CreateWindow(WC_COMBOBOX, TEXT(""),
                                CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                                0, 0, 150, 550, hwnd, NULL, NULL, NULL);
            SendMessage(infoList, (UINT) CB_SETCUEBANNER, 0, (LPARAM) TEXT("Edit"));
            SendMessage(infoList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("Save"));
            SendMessage(infoList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("Load"));
            SendMessage(infoList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("Clear"));
            SendMessage(infoList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("Exit"));
            
            
            lineList = CreateWindow(WC_COMBOBOX, TEXT(""),
                CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                                    150, 0, 150, 550, hwnd, NULL, NULL, NULL);
            SendMessage(lineList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("LineDDA"));
            SendMessage(lineList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("LineMidPoint"));
            SendMessage(lineList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("LineParametric"));

            circleList = CreateWindow(WC_COMBOBOX, TEXT(""),
                                      CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                                      300, 0, 150, 550, hwnd, NULL, NULL, NULL);
            SendMessage(circleList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("CircleDirect"));
            SendMessage(circleList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("CirclePolar"));
            SendMessage(circleList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("CircleIterativePolar"));
            SendMessage(circleList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("CircleMidPoint"));
            SendMessage(circleList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("CircleModifiedMidPoint"));

            ellipseList = CreateWindow(WC_COMBOBOX, TEXT(""),
                                       CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                                       450, 0, 150, 550, hwnd, NULL, NULL, NULL);
            SendMessage(ellipseList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("EllipseDirect"));
            SendMessage(ellipseList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("EllipsePolar"));
            SendMessage(ellipseList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("EllipseMidPoint"));

            curveList = CreateWindow(WC_COMBOBOX, TEXT(""),
                                     CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                                     600, 0, 150, 550, hwnd, NULL, NULL, NULL);
            SendMessage(curveList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("HermiteCurve"));
            SendMessage(curveList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("BezierCurve"));
            SendMessage(curveList, (UINT) CB_ADDSTRING, 0, (LPARAM)TEXT("CardinalSpline"));
            colorList = CreateWindow(WC_COMBOBOX, TEXT(""),
                                     CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                                     750, 0, 150, 550, hwnd, NULL, NULL, NULL);
            SendMessage(colorList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("Red"));
            SendMessage(colorList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("Blue"));
            SendMessage(colorList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("Black"));
            SendMessage(colorList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("Dark-Green"));
            SendMessage(colorList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("Yellow-Green"));
            SendMessage(colorList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("Orange"));
            SendMessage(colorList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("Purple"));

            fillingsList = CreateWindow(WC_COMBOBOX, TEXT(""),
                                       CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                                       900, 0, 200, 550, hwnd, NULL, NULL, NULL);
            SendMessage(fillingsList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("FillCircleLines"));
            SendMessage(fillingsList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("FillCircleCircles"));
            SendMessage(fillingsList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("FillSquareHermite"));
            SendMessage(fillingsList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("FillRectBezier"));
            SendMessage(fillingsList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("ConvexPolygon"));
            SendMessage(fillingsList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("Non-ConvexPolygon"));
            SendMessage(fillingsList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("Recursive-FloodFill"));
            SendMessage(fillingsList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("NonRecursive-FloodFill"));

            clippingList = CreateWindow(WC_COMBOBOX, TEXT(""),
                                    CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                                    1100, 0, 200, 550, hwnd, NULL, NULL, NULL);
            SendMessage(clippingList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("Rectangle"));
            SendMessage(clippingList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("PointClipping-Rectangle"));
            SendMessage(clippingList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("LineClipping-Rectangle"));
            SendMessage(clippingList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("PolygonClipping-Rectangle"));
            SendMessage(clippingList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("Square"));
            SendMessage(clippingList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("PointClipping-Square"));
            SendMessage(clippingList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("LineClipping-Square"));
            SendMessage(clippingList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("PointClipping-Circle"));
            SendMessage(clippingList, (UINT) CB_ADDSTRING, 0, (LPARAM) TEXT("LineClipping-Circle"));

            break;

        case WM_COMMAND:
            if (HIWORD(wp) == CBN_SELCHANGE) {
                int ItemIdx = SendMessage((HWND) lp, (UINT) CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                TCHAR ListItem[256];
                (TCHAR) SendMessage((HWND) lp, (UINT) CB_GETLBTEXT, (WPARAM) ItemIdx, (LPARAM) ListItem);
                if (!_tcscmp(ListItem, _T("Red"))) {
                    SendMessage(colorList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    color = RGB(255, 0, 0);
                    colorSelected = true;
                } else if (!_tcscmp(ListItem, _T("Dark-Green"))) {
                    SendMessage(colorList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    color = RGB(0, 100, 0);
                    colorSelected = true;
                } else if (!_tcscmp(ListItem, _T("Blue"))) {
                    SendMessage(colorList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    color = RGB(0, 0, 255);
                    colorSelected = true;
                } else if (!_tcscmp(ListItem, _T("Yellow-Green"))) {
                    SendMessage(colorList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    color = RGB(154, 205, 50);
                    colorSelected = true;
                } else if (!_tcscmp(ListItem, _T("Black"))) {
                    SendMessage(colorList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    color = RGB(0, 0, 0);
                    colorSelected = true;
                } else if (!_tcscmp(ListItem, _T("Orange"))) {
                    SendMessage(colorList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    color = RGB(255, 140, 0);
                    colorSelected = true;
                } else if (!_tcscmp(ListItem, _T("Purple"))) {
                    SendMessage(colorList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    color = RGB(128, 0, 128);
                    colorSelected = true;
                }

                /*-----------------------------------------------------*/
                if (!_tcscmp(ListItem, _T("LineDDA"))) {
                    SendMessage(lineList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    currentShape.name = "LineDDA";
                    resetOptions();
                    lineDDASelected = true;
                } else if (!_tcscmp(ListItem, _T("LineMidPoint"))) {
                    SendMessage(lineList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    currentShape.name = "LineMidPoint";
                    resetOptions();
                    lineMidPointSelected = true;
                } else if (!_tcscmp(ListItem, _T("LineParametric"))) {
                    SendMessage(lineList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    currentShape.name = "LineParametric";
                    resetOptions();
                    lineParametricSelected = true;
                }
                    /*-----------------------------------------------------*/
                else if (!_tcscmp(ListItem, _T("Save"))) {
                    SendMessage(lineList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    resetOptions();
                    saveSelected = true;
                } else if (!_tcscmp(ListItem, _T("Load"))) {
                    SendMessage(lineList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    resetOptions();
                    loadSelected = true;
                } else if (!_tcscmp(ListItem, _T("Clear"))) {
                    SendMessage(lineList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    resetOptions();
                    clearSelected = true;
                } else if (!_tcscmp(ListItem, _T("Exit"))) {
                    SendMessage(lineList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    resetOptions();
                    exitSelected = true;
                }
                    /*-----------------------------------------------------*/
                else if (!_tcscmp(ListItem, _T("CircleDirect"))) {
                    SendMessage(circleList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    currentShape.name = "CircleDirect";
                    resetOptions();
                    circleDirectSelected = true;
                } else if (!_tcscmp(ListItem, _T("CirclePolar"))) {
                    SendMessage(circleList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    currentShape.name = "CirclePolar";
                    resetOptions();
                    circlePolarSelected = true;
                } else if (!_tcscmp(ListItem, _T("CircleIterativePolar"))) {
                    SendMessage(circleList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    currentShape.name = "CircleIterativePolar";
                    resetOptions();
                    circleIterativePolarSelected = true;
                } else if (!_tcscmp(ListItem, _T("CircleMidPoint"))) {
                    SendMessage(circleList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    currentShape.name = "CircleMidPoint";
                    resetOptions();
                    circleMidPointSelected = true;
                } else if (!_tcscmp(ListItem, _T("CircleModifiedMidPoint"))) {
                    SendMessage(circleList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    currentShape.name = "CircleModifiedMidPoint";
                    resetOptions();
                    circleModifiedMidPointSelected = true;
                }
                    /*-----------------------------------------------------*/
                else if (!_tcscmp(ListItem, _T("EllipseDirect"))) {
                    SendMessage(ellipseList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    resetOptions();
                    ellipseDirectSelected = true;

                } else if (!_tcscmp(ListItem, _T("EllipsePolar"))) {
                    SendMessage(ellipseList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    resetOptions();
                    ellipsePolarSelected = true;
                } else if (!_tcscmp(ListItem, _T("EllipseMidPoint"))) {
                    SendMessage(ellipseList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    resetOptions();
                    ellipseMidPointSelected = true;
                }
                    /*-----------------------------------------------------*/
                else if (!_tcscmp(ListItem, _T("HermiteCurve"))) {
                    SendMessage(curveList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    resetOptions();
                    hermiteCurveSelected = true;
                } else if (!_tcscmp(ListItem, _T("BezierCurve"))) {
                    SendMessage(curveList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    resetOptions();
                    bezierCurveSelected = true;
                } else if (!_tcscmp(ListItem, _T("CardinalSpline"))) {
                    SendMessage(curveList, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                    resetOptions();
                    cardinalSplineSelected = true;
                }
                    /*-----------------------------------------------------*/
                else if (!_tcscmp(ListItem, _T("FillCircleLines"))) {
                    SendMessage(fillingsList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    currentShape.name = "FillCircleLines";
                    resetOptions();
                    fillCircleLinesSelected = true;    //FillSquareHermite
                } else if (!_tcscmp(ListItem, _T("FillCircleCircles"))) {
                    SendMessage(fillingsList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    currentShape.name = "FillCircleCircles";
                    resetOptions();
                    fillCircleSelected = true;
                } else if (!_tcscmp(ListItem, _T("FillSquareHermite"))) {
                    SendMessage(fillingsList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    currentShape.name = "FillSquareHermite";
                    resetOptions();
                    fillShapeHermiteSelected = true;
                } else if (!_tcscmp(ListItem, _T("FillRectBezier"))) {
                    SendMessage(fillingsList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    currentShape.name = "FillRectBezier";
                    resetOptions();
                    fillShapeBezierSelected = true;
                } else if (!_tcscmp(ListItem, _T("ConvexPolygon"))) {
                    SendMessage(fillingsList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    currentShape.name = "ConvexPolygon";
                    resetOptions();
                    fillConvexPolygonSelected = true;
                } else if (!_tcscmp(ListItem, _T("Non-ConvexPolygon"))) {
                    SendMessage(fillingsList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    currentShape.name = "Non-ConvexPolygon";
                    resetOptions();
                    fillPolygonGeneralSelected = true;
                } else if (!_tcscmp(ListItem, _T("Recursive-FloodFill"))) {
                    SendMessage(fillingsList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    currentShape.name = "Recursive-FloodFill";
                    resetOptions();
                    recFloodFillSelected = true;
                } else if (!_tcscmp(ListItem, _T("NonRecursive-FloodFill"))) {
                    SendMessage(fillingsList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    currentShape.name = "NonRecursive-FloodFill";
                    resetOptions();
                    iterFloodFillSelected = true;
                }
                    /*-----------------------------------------------------*/
                else if (!_tcscmp(ListItem, _T("Rectangle"))) {
                    SendMessage(clippingList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    resetOptions();
                    currentShape.name = "Rectangle";
                    rectangleSelected = true;
                } else if (!_tcscmp(ListItem, _T("PointClipping-Rectangle"))) {
                    SendMessage(clippingList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    resetOptions();
                    currentShape.name = "PointClipping-Rectangle";
                    recPointClipSelected = true;
                } else if (!_tcscmp(ListItem, _T("LineClipping-Rectangle"))) {
                    SendMessage(clippingList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    resetOptions();
                    currentShape.name = "LineClipping-Rectangle";
                    recLineClipSelected = true;
                } else if (!_tcscmp(ListItem, _T("PolygonClipping-Rectangle"))) {
                    SendMessage(clippingList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    resetOptions();
                    currentShape.name = "PolygonClipping-Rectangle";
                    recPolygonClipSelected = true;
                } else if (!_tcscmp(ListItem, _T("Square"))) {
                    SendMessage(clippingList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    resetOptions();
                    currentShape.name = "Square";
                    squareSelected = true;
                } else if (!_tcscmp(ListItem, _T("PointClipping-Square"))) {
                    SendMessage(clippingList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    resetOptions();
                    currentShape.name = "PointClipping-Square";
                    squarePointClipSelected = true;
                } else if (!_tcscmp(ListItem, _T("LineClipping-Square"))) {
                    SendMessage(clippingList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    resetOptions();
                    currentShape.name = "LineClipping-Square";
                    squareLineClipSelected = true;
                } else if (!_tcscmp(ListItem, _T("PointClipping-Circle"))) {
                    SendMessage(clippingList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    resetOptions();
                    currentShape.name = "PointClipping-Circle";
                    circlePointClipSelected = true;

                } else if (!_tcscmp(ListItem, _T("LineClipping-Circle"))) {
                    SendMessage(clippingList, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                    resetOptions();
                    currentShape.name = "LineClipping-Circle";
                    circleLineClipSelected = true;
                }


            }

            break;

        case WM_LBUTTONDOWN:

            if (clearSelected) {
                HDC hdc = GetDC(hwnd);
                InvalidateRect(hwnd, NULL, true);
                points.clear();
                drawings.clear();
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
            } else if (loadSelected) {
                LoadData(hwnd);
                pointCount = 0;
            } else if (saveSelected) {
                SaveData();
                pointCount = 0;
            } else if (exitSelected) {
                pointCount = 0;
                exit(0);
            }
                /*-----------------------------------------------------*/
            else if (pointCount == 0) {
                cout << "First Point" << endl;
                p1.x = LOWORD(lp);
                p1.y = HIWORD(lp);
                points.push_back(p1);
                pointCount++;

            } else if (pointCount == 1) {
                cout << "Second Point" << endl;
                p2.x = LOWORD(lp);
                p2.y = HIWORD(lp);
                points.push_back(p2);
                pointCount++;

            } else if (pointCount == 2) {
                cout << "Third Point" << endl;
                p3.x = LOWORD(lp);
                p3.y = HIWORD(lp);
                points.push_back(p3);
                pointCount++;

            } else if (pointCount == 3) {
                cout << "Fourth Point" << endl;
                p4.x = LOWORD(lp);
                p4.y = HIWORD(lp);
                points.push_back(p4);
                pointCount++;

            } else if (pointCount == 4) {
                cout << "Fifth Point" << endl;
                p5.x = LOWORD(lp);
                p5.y = HIWORD(lp);
                points.push_back(p5);
                pointCount++;

            }else if (pointCount == 5) {
                cout << "Sixth Point" << endl;
                p6.x = LOWORD(lp);
                p6.y = HIWORD(lp);
                points.push_back(p6);
                pointCount++;
            }
            else if (pointCount == 6) {
                cout << "Seventh Point" << endl;
                p7.x = LOWORD(lp);
                p7.y = HIWORD(lp);
                points.push_back(p7);
                pointCount++;

            }
            /*-----------------------------------------------------*/
            if (lineDDASelected && pointCount >= 2 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                DrawLineSimpleDDA(hdc, p1.x, p1.y, p2.x, p2.y, color);
                addDrawer("DrawLineDDA", points, color);
                ReleaseDC(hwnd, hdc);
                pointCount = 0;
                points.clear();
                cout << "Draw Line DDA" << endl;
            } else if (lineParametricSelected && pointCount >= 2 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                pointCount = 0;
                DrawLineParametric(hdc, p1.x, p1.y, p2.x, p2.y, color);
                addDrawer("DrawLineParametric", points, color);
                points.clear();
                ReleaseDC(hwnd, hdc);
                cout << "Draw Line Parametric" << endl;
            } else if (pointCount >= 2 && lineMidPointSelected && colorSelected) {
                HDC hdc = GetDC(hwnd);
                pointCount = 0;
                DrawLineMidpoint(hdc, p1.x, p1.y, p2.x, p2.y, color);
                addDrawer("DrawLineMidPoint", points, color);
                points.clear();
                ReleaseDC(hwnd, hdc);
                cout << "Draw Line MidPoint" << endl;
            }
                /*-----------------------------------------------------*/
            else if (circleDirectSelected && pointCount >= 2 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                int R = distanceBetweenPoints(p1, p2);
                DrawCircleNaive(hdc, p1.x, p1.y, R, color);
                circleCx = p1.x;
                circleCy = p1.y;
                radius = R;
                CircleCenter[0] = p1.x;
                CircleCenter[1] = p1.y;
                addDrawer("DrawCircleDirect", points, color);
                pointCount = 0;
                points.clear();
                ReleaseDC(hwnd, hdc);
                cout << "Draw Direct Circle" << endl;

            } else if (circlePolarSelected && pointCount >= 2 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                int r = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
                DrawCirclePolar(hdc, p1.x, p1.y, r, color);
                circleCx = p1.x;
                circleCy = p1.y;
                radius = r;
                CircleCenter[0] = p1.x;
                CircleCenter[1] = p1.y;
                addDrawer("DrawCirclePolar", points, color);
                points.clear();
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
                cout << "Draw Polar Circle" << endl;

            } else if (circleIterativePolarSelected && pointCount >= 2 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                int r = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
                DrawCircleIterativePolar(hdc, p1.x, p1.y, r, color);
                circleCx = p1.x;
                circleCy = p1.y;
                radius = r;
                CircleCenter[0] = p1.x;
                CircleCenter[1] = p1.y;
                addDrawer("DrawCircleIterativePolar", points, color);
                points.clear();
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
                cout << "Draw Iterative Polar Circle" << endl;
            } else if (circleMidPointSelected && pointCount >= 2 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                int r = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
                DrawCircleMidpoint(hdc, p1.x, p1.y, r, color);
                circleCx = p1.x;
                circleCy = p1.y;
                radius = r;
                CircleCenter[0] = p1.x;
                CircleCenter[1] = p1.y;
                addDrawer("DrawCircleMidPoint", points, color);
                points.clear();
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
                cout << "Draw MidPoint Circle" << endl;

            } else if (circleModifiedMidPointSelected && pointCount >= 2 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                int r = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
                DrawCircleModifiedMidpoint(hdc, p1.x, p1.y, r, color);
                circleCx = p1.x;
                circleCy = p1.y;
                radius = r;
                CircleCenter[0] = p1.x;
                CircleCenter[1] = p1.y;
                addDrawer("DrawCircleModifiedMidPoint", points, color);
                points.clear();
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
                cout << "Draw Modified MidPoint Circle" << endl;
            }
                /*-----------------------------------------------------*/
            else if (fillCircleLinesSelected && pointCount >= 1 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                fillCircleLine(hdc, p1.x, p1.y, radius, color);
                Point p;
                p.x = circleCx;
                p.y = circleCy;
                points.push_back(p);
                addDrawer("FillCircleLines", points, color, radius);
                points.clear();
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
                cout << "Fill Circle With Lines" << endl;

            } else if (fillCircleSelected && pointCount >= 1 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                fillCircleQuad(hdc, p1.x, p1.y, radius, color);
                Point p;
                p.x = circleCx;
                p.y = circleCy;
                points.push_back(p);
                addDrawer("FillCircleCircles", points, color, radius);
                points.clear();
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
                cout << "Fill Circle With Circles" << endl;

            } else if (fillShapeHermiteSelected && pointCount >= 2 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                int r = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
                drawSquareFillHermite(hdc, p1, p2, color);
                addDrawer("FillSquareHermite", points, color);
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
                points.clear();
                cout << "Fill Square Using Hermite Curves" << endl;

            } else if (fillShapeBezierSelected && pointCount >= 2 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                drawRectangleFillBezier(hdc, p1, p2, color);
                addDrawer("FillRectBezier", points, color);
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
                points.clear();
                cout << "Fill Rectangle Using Bezier Curves" << endl;

            } else if (fillConvexPolygonSelected && pointCount >= 5 && colorSelected) {    //??
                HDC hdc = GetDC(hwnd);
                Point v[5] = {p1, p2, p3, p4, p5};
                ConvexFill(hdc, v, 5, color);
                addDrawer("FillPolygon", points, color);
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
                points.clear();
                cout << "Convex Polygon Fill" << endl;

            } else if (fillPolygonGeneralSelected && pointCount >= 5 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                Point v[5] = {p1, p2, p3, p4, p5};
                GeneralPolygonFill(hdc, v, 5, color);
                addDrawer("Non-ConvexPloygon", points, color);
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
                points.clear();
                cout << "General Polygon Fill" << endl;

            } else if (recFloodFillSelected && pointCount >= 1) {
                HDC hdc = GetDC(hwnd);
                MyFloodFillRec(hdc, p1.x, p1.y, color, RGB(218, 112, 214)); //static color why??
                addDrawer("Recursive-FloodFill", points, color);
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
                points.clear();
                cout << "Recursive Flood Fill" << endl;

            } else if (iterFloodFillSelected && pointCount >= 1) {
                HDC hdc = GetDC(hwnd);
                MyFloodFillNonRec(hdc, p1.x, p1.y, color, RGB(218, 112, 214));
                addDrawer("NonRecursive-FloodFill", points, color);
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
                points.clear();
                cout << "Non-Recursive Flood Fill" << endl;
            }

                /*-----------------------------------------------------*/
            else if (recPointClipSelected && pointCount >= 1 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                recPointClip(hdc, p1.x, p1.y, t1.x, t1.y, t2.x, t2.y, color);
                points.push_back(t1);
                points.push_back(t2);
                addDrawer("PointClipping-Rectangle", points, color);
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
                points.clear();
                cout << "Point Clipping - Rectangle" << endl;
            } else if (rectangleSelected && pointCount >= 2) {
                HDC hdc = GetDC(hwnd);
                DrawRec(hdc, p1, p2, color);
                t1.x = p1.x;
                t1.y = p1.y;
                t2.x = p2.x;
                t2.y = p2.y;
                pointCount = 0;
                points.clear();
                ReleaseDC(hwnd, hdc);
                cout << "Draw Rectangle" << endl;
            } else if (recLineClipSelected && pointCount >= 2 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                CohenSuth(hdc, p1.x, p1.y, p2.x, p2.y, t1.x, t1.y, t2.x, t2.y, color);
                points.push_back(t1);
                points.push_back(t2);
                addDrawer("LineClipping-Rectangle", points, color);
                points.clear();
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
                cout << "Line Clipping - Rectangle" << endl;
            } else if (recPolygonClipSelected && pointCount >= 5 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                vector<Point> tempVec = {p1, p2, p3, p4, p5};
                polygonClip(hdc, tempVec, 5, t1.x, t1.y, t2.x, t2.y, color);
                points.push_back(t1);
                points.push_back(t2);
                addDrawer("PolygonClipping-Rectangle", points, color);
                points.clear();
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
                cout << "Polygon Clipping - Rectangle" << endl;
            } else if (squareSelected && pointCount >= 2) {
                HDC hdc = GetDC(hwnd);
                DrawSqClip(hdc, p1, p2, color);
                double distance = (p2.x - p1.x);
                p2.x = p1.x + distance;
                p2.y = p1.y;
                Point p3;
                p3.x = p1.x;
                p3.y = p1.y - distance;
                Point p4;
                p4.x = p2.x;
                p4.y = p2.y - distance;
                t1.x = p1.x;
                t1.y = p3.y;
                t2.x = p2.x;
                t2.y = p2.y;
                pointCount = 0;
                points.clear();
                ReleaseDC(hwnd, hdc);
                cout << "Draw Square" << endl;
            } else if (squarePointClipSelected && pointCount >= 1 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                recPointClip(hdc, p1.x, p1.y, t1.x, t1.y, t2.x, t2.y, color);
                points.push_back(t1);
                points.push_back(t2);

                addDrawer("PointClipping-Square", points, color);
                points.clear();
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
                cout << "Point Clipping - Square" << endl;
            } else if (squareLineClipSelected && pointCount >= 2 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                CohenSuth(hdc, p1.x, p1.y, p2.x, p2.y, t1.x, t1.y, t2.x, t2.y, color);
                points.push_back(t1);
                points.push_back(t2);
                addDrawer("LineClipping-Square", points, color);
                points.clear();
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
                cout << "Line Clipping - Square" << endl;
            } else if (circlePointClipSelected && pointCount >= 1 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                circlePointClip(hdc, p1, radius, color);
                addDrawer("PointClipping-Circle", points, color, radius);
                points.clear();
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
                cout << "Point Clipping - Circle" << endl;
            } else if (circleLineClipSelected && pointCount >= 2 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                DrawLineClip(hdc, p1.x, p1.y, p2.x, p2.y, radius, color);
                addDrawer("LineClipping-Circle", points, color, radius);
                points.clear();
                pointCount = 0;
                ReleaseDC(hwnd, hdc);
                cout << "Line Clipping - Circle" << endl;
            }

                /*-----------------------------------------------------*/
            else if (ellipseDirectSelected && pointCount >= 3 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                pointCount = 0;
                int A = distanceBetweenPoints(p2, p1);
                int B = distanceBetweenPoints(p3, p1);
                DrawElipseDirect(hdc, p1.x, p1.y, A, B, color);
                addDrawer("DrawEllipseDirect", points, color);
                points.clear();
                ReleaseDC(hwnd, hdc);
                cout << "Draw Ellipse Direct" << endl;
            } else if (ellipsePolarSelected && pointCount >= 3 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                pointCount = 0;
                int A = distanceBetweenPoints(p2, p1);
                int B = distanceBetweenPoints(p3, p1);
                DrawElipsePolar(hdc, p1.x, p1.y, A, B, color);
                addDrawer("DrawEllipsePolar", points, color);
                points.clear();
                ReleaseDC(hwnd, hdc);
                cout << "Draw Ellipse Polar" << endl;
            } else if (ellipseMidPointSelected && pointCount >= 3 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                pointCount = 0;
                int A = distanceBetweenPoints(p2, p1);
                int B = distanceBetweenPoints(p3, p1);
                DrawElipseMidpoint(hdc, p1.x, p1.y, A, B, color);
                addDrawer("DrawEllipseMidPoint", points, color);
                ReleaseDC(hwnd, hdc);
                points.clear();
                cout << "Draw Ellipse MidPoint" << endl;
            }
                /*-----------------------------------------------------*/
            else if (hermiteCurveSelected && pointCount >= 4 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                pointCount = 0;
                Vector v1(p1.x, p1.y);
                Vector v2(p2.x, p2.y);
                Vector v3(p3.x, p3.y);
                Vector v4(p4.x, p4.y);

                Vector T0(3 * (v2.v[0] - v1.v[0]), 3 * (v2.v[1] - v1.v[1]));
                Vector T1(3 * (v4.v[0] - v3.v[0]), 3 * (v4.v[1] - v3.v[1]));
                DrawHermiteCurve(hdc, v1, T0, v4, T1, color);
                addDrawer("DrawHermiteCurve", points, color);
                points.clear();
                ReleaseDC(hwnd, hdc);
                cout << "Draw Hermite Curve" << endl;
            } else if (bezierCurveSelected && pointCount >= 4 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                pointCount = 0;
                Vector v1;
                v1[0] = p1.x;
                v1[1] = p1.y;
                Vector v2;
                v2[0] = p2.x;
                v2[1] = p2.y;
                Vector v3;
                v3[0] = p3.x;
                v3[1] = p3.y;
                Vector v4;
                v4[0] = p4.x;
                v4[1] = p4.y;
                DrawBezierCurve(hdc, v1, v2, v3, v4, color);
                addDrawer("DrawBezierCurve", points, color);
                points.clear();
                ReleaseDC(hwnd, hdc);
                cout << "Draw Bezier Curve" << endl;
            } else if(cardinalSplineSelected && pointCount >= 7 && colorSelected) {
                HDC hdc = GetDC(hwnd);
                Vector p[7];
                p[0][0] = p1.x;
                p[0][1] = p1.y;
                p[1][0] = p2.x;
                p[1][1] = p2.y;
                p[2][0] = p3.x;
                p[2][1] = p3.y;
                p[3][0] = p4.x;
                p[3][1] = p4.y;
                p[4][0] = p5.x;
                p[4][1] = p5.y;
                p[5][0] = p6.x;
                p[5][1] = p6.y;
                p[6][0] = p7.x;
                p[6][1] = p7.y;
                DrawCardinalSpline(hdc, p, 7, 0.1, color);
                addDrawer("DrawCardinalSpline", points, color);
                points.clear();
                pointCount = 0;
                cout << "Draw Cardinal Spline" << endl;
            }
            /*-----------------------------------------------------*/


            break;

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, mcode, wp, lp);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmd, int nsh) {
    WNDCLASS wc;
    wc.cbClsExtra = wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_HAND);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = hinst;
    wc.lpfnWndProc = MyWndProc;
    wc.lpszClassName = "MyClass";
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("MyClass", "Project Window", WS_OVERLAPPEDWINDOW, 0, 0, 1500, 800, NULL, NULL, hinst, 0);
    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}