#include <windows.h>
#include <stdio.h>
HANDLE ben;
DWORD WINAPI Proc1(
        LPVOID lpParameter   // thread data
);
LRESULT CALLBACK WinSunProc(
        HWND hwnd,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam
);
BOOL WINAPI DllMain(
        HINSTANCE hinstDLL,  // handle to DLL module
        DWORD fdwReason,     // reason for calling function
        LPVOID lpReserved )
{
    switch( fdwReason )
    {
        case DLL_PROCESS_ATTACH:
        {
            ben =  GetModuleHandle(NULL);
            HANDLE thread1;
            thread1=CreateThread(NULL,0,Proc1,NULL,0,NULL);
        }
            break;

        case DLL_THREAD_ATTACH:
            // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
            // Do thread-specific cleanup.
            break;

        case DLL_PROCESS_DETACH:
            // Perform any necessary cleanup.
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

DWORD WINAPI Proc1(LPVOID lpParameter)
{
    WNDCLASS wndcls;
    wndcls.cbClsExtra = 0;

    wndcls.cbWndExtra = 0;

    wndcls.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    wndcls.hCursor = LoadCursor(NULL, IDC_CROSS);

    wndcls.hIcon = NULL;

    wndcls.hInstance = (HINSTANCE)ben;

    wndcls.lpfnWndProc = WinSunProc;

    wndcls.lpszClassName = "firstwind";

    wndcls.lpszMenuName = NULL;

    wndcls.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wndcls);

    HWND hwnd;
    hwnd = CreateWindow("firstwind", "blueboy", WS_OVERLAPPEDWINDOW, 500, 200, 600, 400, NULL,
                        NULL, (HINSTANCE)ben, NULL);



    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WinSunProc(
        HWND hwnd,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam
)
{
    switch(uMsg)
    {
        case WM_CHAR:
            char szChar[50];
            sprintf(szChar,"char code is %d",wParam);
            MessageBox(hwnd,szChar,"char",0);
            break;
        case WM_LBUTTONDOWN:
            MessageBox(hwnd,"mouse clicked","message",0);
            HDC hdc;
            hdc=GetDC(hwnd);//不能在响应WM_PAINT消息时调用
            TextOut(hdc,0,50,"happyblue",strlen("happyblue"));
            //ReleaseDC(hwnd,hdc);
            break;
        case WM_PAINT:
            HDC hDC;
            PAINTSTRUCT ps;
            hDC=BeginPaint(hwnd,&ps);
            TextOut(hDC,0,0,"blueboy",strlen("blueboy"));
            EndPaint(hwnd,&ps);
            break;
        case WM_CLOSE:
            if(IDYES==MessageBox(hwnd,"是否真的结束?","message",MB_YESNO))
            {
                DestroyWindow(hwnd);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd,uMsg,wParam,lParam);
    }
}