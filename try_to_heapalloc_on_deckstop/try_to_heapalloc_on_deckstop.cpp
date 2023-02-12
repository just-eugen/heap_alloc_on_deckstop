// try_to_heapalloc_on_deckstop.cpp : Defines the entry point for the application.
//?? - вопросы боровского

#include "framework.h"
#include "try_to_heapalloc_on_deckstop.h"

#define MAX_LOADSTRING 100
#define MAX_MATRIX_SIZE 10000
#define RANGE_X 50
#define RANGE_Y 15

struct Matrix {

    int** a;

    Matrix(int matrix_size)
    {
        a = (int**)HeapAlloc(GetProcessHeap(), NULL, matrix_size * sizeof(int*) + matrix_size * matrix_size * sizeof(int));

        for (int i = 0; i < matrix_size; i++)
            a[i] = (int*)(((int)a + matrix_size * sizeof(int*)) + i * matrix_size * sizeof(int));
    }

    ~Matrix()
    {
        HeapFree(GetProcessHeap(), NULL, a);
    }
};

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    WCHAR szTitle[MAX_LOADSTRING];
    WCHAR szWindowClass[MAX_LOADSTRING];
    
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TRYTOHEAPALLOCONDECKSTOP, szWindowClass, MAX_LOADSTRING);

    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRYTOHEAPALLOCONDECKSTOP));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);
    
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
        (int)(GetSystemMetrics(SM_CXSCREEN) / 4), (int)(GetSystemMetrics(SM_CYSCREEN) / 4),
        (int)(GetSystemMetrics(SM_CXSCREEN) / 2), (int)(GetSystemMetrics(SM_CYSCREEN) / 2), nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRYTOHEAPALLOCONDECKSTOP));
    MSG msg;
    
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }   
       
    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static Matrix matrix(MAX_MATRIX_SIZE);
    
    SCROLLINFO scroll_info;
    RECT client_rect;

    int x, y;

    switch (message)
    {  
    case WM_CREATE:
    {
        int i = 1, j, k, p = MAX_MATRIX_SIZE / 2;

        for (k = 1; k <= p; k++)
        {
            for (j = k - 1; j < MAX_MATRIX_SIZE - k + 1; j++) matrix.a[k - 1][j] = i++;
            for (j = k; j < MAX_MATRIX_SIZE - k + 1; j++) matrix.a[j][MAX_MATRIX_SIZE - k] = i++;
            for (j = MAX_MATRIX_SIZE - k - 1; j >= k - 1; --j) matrix.a[MAX_MATRIX_SIZE - k][j] = i++;
            for (j = MAX_MATRIX_SIZE - k - 1; j >= k; j--) matrix.a[j][k - 1] = i++;
        }

        if (MAX_MATRIX_SIZE % 2 == 1)
            matrix.a[p][p] = MAX_MATRIX_SIZE * MAX_MATRIX_SIZE;
    }
    return 0;

    case WM_SIZE: 
    {
        GetClientRect(hWnd, &client_rect);

        scroll_info.cbSize = sizeof(scroll_info);
        scroll_info.fMask = SIF_RANGE | SIF_PAGE;
        scroll_info.nMin = 0;
        scroll_info.nMax = RANGE_Y + MAX_LOADSTRING * RANGE_Y - client_rect.bottom;
        scroll_info.nPage = RANGE_Y * 2;
        SetScrollInfo(hWnd, SB_VERT, &scroll_info, TRUE);

        scroll_info.cbSize = sizeof(scroll_info);
        scroll_info.fMask = SIF_RANGE | SIF_PAGE;
        scroll_info.nMin = 0;
        scroll_info.nMax = RANGE_X + MAX_LOADSTRING * RANGE_X - client_rect.right;
        scroll_info.nPage = RANGE_X * 2;
        SetScrollInfo(hWnd, SB_HORZ, &scroll_info, TRUE);            
    }
    return 0;

    case WM_VSCROLL:
    {            
        scroll_info.cbSize = sizeof(scroll_info);
        scroll_info.fMask = SIF_ALL;
        GetScrollInfo(hWnd, SB_VERT, &scroll_info);

        y = scroll_info.nPos;
        switch (LOWORD(wParam))
        {

        case SB_TOP:
            scroll_info.nPos = scroll_info.nMin;
            break;

        case SB_BOTTOM:
            scroll_info.nPos = scroll_info.nMax;
            break;

        case SB_LINEUP:
            scroll_info.nPos -= 5;
            break;

        case SB_LINEDOWN:
            scroll_info.nPos += 5;
            break;

        case SB_PAGEUP:
            scroll_info.nPos -= scroll_info.nPage;
            break;

        case SB_PAGEDOWN:
            scroll_info.nPos += scroll_info.nPage;
            break;

        case SB_THUMBTRACK:
            scroll_info.nPos = scroll_info.nTrackPos;
            break;

        default:
            break;
        }

        scroll_info.fMask = SIF_POS;
        SetScrollInfo(hWnd, SB_VERT, &scroll_info, TRUE);
        GetScrollInfo(hWnd, SB_VERT, &scroll_info);

        if (scroll_info.nPos != y)
        {
            ScrollWindow(hWnd, 0, y - scroll_info.nPos, NULL, NULL);
            UpdateWindow(hWnd);
        }
    }
    return 0;

    case WM_HSCROLL:
    {
        scroll_info.cbSize = sizeof(scroll_info);
        scroll_info.fMask = SIF_ALL;

        GetScrollInfo(hWnd, SB_HORZ, &scroll_info);
        x = scroll_info.nPos;
        switch (LOWORD(wParam))
        {
        case SB_LINELEFT:
            scroll_info.nPos -= 5;
            break;

        case SB_LINERIGHT:
            scroll_info.nPos += 5;
            break;

        case SB_PAGELEFT:
            scroll_info.nPos -= scroll_info.nPage;
            break;

        case SB_PAGERIGHT:
            scroll_info.nPos += scroll_info.nPage;
            break;

        case SB_THUMBTRACK:
            scroll_info.nPos = scroll_info.nTrackPos;
            break;

        default:
            break;
        }

        scroll_info.fMask = SIF_POS;
        SetScrollInfo(hWnd, SB_HORZ, &scroll_info, TRUE);
        GetScrollInfo(hWnd, SB_HORZ, &scroll_info);

        if (scroll_info.nPos != x)
        {
            ScrollWindow(hWnd, x - scroll_info.nPos, 0, NULL, NULL);
            UpdateWindow(hWnd);
        }
    }
    return 0;

    case WM_PAINT:
    {   
        PAINTSTRUCT ps;
        HDC hdc;
        RECT rc;
        HFONT cs_font;
        TCHAR buff[10];

        hdc = BeginPaint(hWnd, &ps);
        GetClientRect(hWnd, &client_rect);
        cs_font = CreateFont(12, 0, 0, 0, FW_DONTCARE, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
        SelectObject(hdc, cs_font);

        scroll_info.fMask = SIF_POS;
        GetScrollInfo(hWnd, SB_VERT, &scroll_info);
        y = scroll_info.nPos;

        scroll_info.fMask = SIF_POS;
        GetScrollInfo(hWnd, SB_HORZ, &scroll_info);
        x = scroll_info.nPos;

        int a_size = MAX_LOADSTRING;       

        for (int i = (y / RANGE_Y) - 1; i < (y + client_rect.bottom) / RANGE_Y + 1; i++)
        {
            for (int j = (x / RANGE_X) - 1; j < (x + client_rect.right) / RANGE_X + 1 ; j++)
            {
                if (i >= 0 && j >= 0 && i < MAX_LOADSTRING && j < MAX_LOADSTRING)
                {
                    _itot_s(matrix.a[i][j], buff, 10);
                    SetRect(&rc, RANGE_X + RANGE_X * j - x, RANGE_Y + RANGE_Y * i - y,
                        RANGE_X + RANGE_X * j - x + RANGE_X, RANGE_Y + RANGE_Y * i - y + RANGE_Y);
                    DrawText(hdc, buff, -1, &rc, DT_LEFT);
                }
            }
        }
        DeleteObject(cs_font);
        EndPaint(hWnd, &ps);
    }
    return 0;      

    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
    return 0;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}