#include "library.h"
#include <Windows.h>
#include "detours.h"
#include <stdio.h>
#include <time.h>
#pragma comment(lib,"detours.lib")

static int(WINAPI* OLD_MessageBoxW)(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType) = MessageBoxW;
static BOOL(WINAPI* OLD_TextOutA)(HDC hdc, int x, int y,  LPCSTR lpString, _In_ int c)=TextOutA;

int WINAPI NEW_MessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
//    修改输入参数，调用原函数
    int ret = OLD_MessageBoxW(hWnd, L"test3", L"test3", uType);
    return ret;
}


FILE *f;
HWND hwnd;

BOOL WINAPI NEW_TextOutA(HDC hdc,  int x,  int y,  LPCSTR lpString,  int c)
{

    PAINTSTRUCT     ps ;
    OLD_TextOutA(hdc,x,y,lpString,c);

    if (hwnd !=0){
        if (x==3){
            hwnd = BeginPaint ( hwnd, &ps ) ;
            OLD_TextOutA(hwnd,x,y,lpString,c);
            hwnd= EndPaint(hwnd,&ps);
        }
    }
//    char str_x[10];
//
//    sprintf(str_x,"x: %d\n",x);
//    fputs(str_x,f);
//
//    char str_y[10];
//    sprintf(str_y,"y: %d\n",y);
//    fputs(str_y,f);
//
//    fputs(lpString,f);
//
//    fputc('\n',f);
//
//    fputs("---------------------\n",f);
}


//void Hook()
//{
//    DetourRestoreAfterWith();//恢复原来状态
//    DetourTransactionBegin();//拦截开始
//    DetourUpdateThread(GetCurrentThread());//刷新当前线程
//    DetourAttach((void**)&OLD_MessageBoxW,NEW_MessageBoxW);
//    DetourTransactionCommit();//拦截生效
//}

void Hook()
{
    DetourRestoreAfterWith();

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&((PVOID)(OLD_TextOutA)), NEW_TextOutA);
    DetourTransactionCommit();
}
//void UnHook()
//{
//
//    DetourTransactionBegin();
//    DetourUpdateThread(GetCurrentThread());
//    DetourDetach((void**)&OLD_MessageBoxW, NEW_MessageBoxW);//撤销拦截函数
//    DetourTransactionCommit();//
//}

HANDLE ben;
LRESULT CALLBACK WinSunProc(
        HWND hwnd,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam
);

DWORD WINAPI Proc1(
        LPVOID lpParameter   // thread data
);

BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved){

//
//    char aa[10];
//    itoa(f,aa,10);
//    MessageBoxW(NULL,aa , L"test", 0);
//

    if (f==0){
        f = fopen("C:\\Users\\soyum\\Desktop\\hook.txt","w+");
        if (f == 0){
            MessageBoxW(NULL,"OPEN FILE FAIL" , L"test", 0);
        }
    }

    if (ul_reason_for_call==DLL_PROCESS_ATTACH) {
        Hook();
        ben =  GetModuleHandle(NULL);
        HANDLE thread1;
        thread1=CreateThread(NULL,0,Proc1,NULL,0,NULL);
    } else if (ul_reason_for_call==DLL_THREAD_ATTACH){
        Hook();
    }

    return TRUE;
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
//            InvalidateRect( hWnd, nullptr, TRUE );
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