// Omar Khaled Al Haj: 20190351




#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <cmath>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

//////////////////////////
void Swap(int &x1, int &y1, int &x2, int &y2){
    int temp = x1;
    x1 = x2;
    x2 = temp;
    temp = y1;
    y1 = y2;
    y2 = temp;
}

struct Vector{
	double v[2];
	Vector(double x = 0, double y = 0)
	{
        v[0] = x; v[1] = y;
    }
	double& operator[](int i)
    {
        return v[i];
	}
};

void DrawLine1(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	if (abs(dy) <= abs(dx))
	{
		if (x1 > x2){
		    Swap(x1, y1, x2, y2);
		}
		SetPixel(hdc, x1, y1, c);
		int x = x1;
		while (x < x2)
		{
			x++;
			double y = y1 + (double)(x - x1)*dy / dx;
			SetPixel(hdc, x, ceil(y), c);
			SetPixel(hdc, x, floor(y), c);
		}
	}
	else {
		if (y1 > y2){
            Swap(x1, y1, x2, y2);
		}
		SetPixel(hdc, x1, y1, c);
		int y = y1;
		while (y < y2)
		{
			y++;
			double x = x1 + (double)(y - y1)*dx / dy;
			SetPixel(hdc, ceil(x), y, c);
			SetPixel(hdc, floor(x), y, c);
		}
	}

}

void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c)
{
	SetPixel(hdc, xc + x, yc + y, c);
	SetPixel(hdc,xc + x, yc - y, c);
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
		y = sqrt((double)R*R - x*x);
		Draw8Points(hdc, xc, yc, x, round(y), c);
	}
}

void DrawCircle1WithLine(HDC hdc, int xc, int yc, int R, COLORREF c)
{
	int x = 0;
	double y = R;
	Draw8Points(hdc, xc, yc, 0, R, c);
	while (x < y)
	{
		x++;
		y = sqrt((double)R*R - x*x);
		if(x < y) DrawLine1(hdc, xc + x, yc + round(y), xc - x, yc - round(y), c);
		Draw8Points(hdc, xc, yc, x, round(y), c);
	}
}
//////////////////////////

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        HDC hdc;
        static int cntLeft = 0;
        static int cntRight = 0;
        static Vector leftPoint[2];
        static Vector rightPoint[1];
        static int x1, x2, x3, y1, y2, y3;
        static int leftRadius, rightRadius;
    case WM_LBUTTONDOWN:
        if(cntLeft != 2){
            leftPoint[cntLeft] = Vector(LOWORD(lParam), HIWORD(lParam));
            cntLeft++;
        }
		break;
    case WM_RBUTTONDOWN:
        rightPoint[cntRight] = Vector(LOWORD(lParam), HIWORD(lParam));
        x1 = leftPoint[0][0]; y1 = leftPoint[0][1];
        x2 = leftPoint[1][0]; y2 = leftPoint[1][1];
        x3 = rightPoint[0][0]; y3 = rightPoint[0][1];
        leftRadius = sqrt(pow(double(x3 - x1), 2) + pow(double(y3 - y1), 2));
        rightRadius = sqrt(pow(double(x3 - x2), 2) + pow(double(y3 - y2), 2));
        hdc = GetDC(hwnd);
        DrawCircle1WithLine(hdc, x1, y1, leftRadius, RGB(255, 0, 0));
        DrawCircle1(hdc, x2, y2, rightRadius, RGB(255, 0, 0));
        ReleaseDC(hwnd, hdc);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
