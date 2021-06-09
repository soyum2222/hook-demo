#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>


DWORD GetProcID(char *name){
    HANDLE all = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    PROCESSENTRY32 info;

    info.dwSize = sizeof(PROCESSENTRY32);
    for (; Process32Next(all, &info);) {

        printf("%s\n",info.szExeFile);
        if (strcmp(name, info.szExeFile) == 0) {
            return info.th32ProcessID;
        }
    }

    return 0;
}
int main() {


    DWORD id =GetProcID("mytrader_spqh.exe");
    printf("%d\n",id);

    HANDLE p =OpenProcess(PROCESS_ALL_ACCESS,FALSE,id);
    printf("%d\n",p);


    char *path  = "D:\\clang\\untitled1\\cmake-build-debug\\untitled1.dll";

    LPVOID base = VirtualAllocEx(p,NULL,strlen(path),MEM_COMMIT,PAGE_READWRITE);
    printf("%d\n",base);

    if (!WriteProcessMemory(p,base,path,strlen(path),NULL)){
        printf("write memory failed %d \n",GetLastError());
    }

    FARPROC addr =GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")),"LoadLibraryA");

    HANDLE h = CreateRemoteThread(p,NULL,0,(LPTHREAD_START_ROUTINE)addr,base,0,NULL);
    if (h==0){
        printf("has error %d\n",GetLastError());
    }
}


//#include <windows.h>
//#include "detours.h"
//#include <stdio.h>
//#pragma comment(lib,"detours.lib")
//
//static LONG dwSlept = 0;
//
//// Target pointer for the uninstrumented Sleep API.
////
//static VOID (WINAPI * TrueSleep)(DWORD dwMilliseconds) = Sleep;
//
//// Detour function that replaces the Sleep API.
////
//VOID WINAPI TimedSleep(DWORD dwMilliseconds)
//{
//
//    printf("111111");
//    // Save the before and after times around calling the Sleep API.
//    DWORD dwBeg = GetTickCount();
//    TrueSleep(dwMilliseconds);
//    DWORD dwEnd = GetTickCount();
//
//    InterlockedExchangeAdd(&dwSlept, dwEnd - dwBeg);
//}
//
//// DllMain function attaches and detaches the TimedSleep detour to the
//// Sleep target function.  The Sleep target function is referred to
//// through the TrueSleep target pointer.
////
//int main()
//{
//
//        DetourRestoreAfterWith();
//
//        DetourTransactionBegin();
//        DetourUpdateThread(GetCurrentThread());
//        DetourAttach(&((PVOID)(TrueSleep)), TimedSleep);
//        DetourTransactionCommit();
//    Sleep(100);
//    return TRUE;
//}


//
//
//#include <Windows.h>
//#include "detours.h"
//#include <stdio.h>
//#include <time.h>
//#pragma comment(lib,"detours.lib")
//
//static int(WINAPI* OLD_MessageBoxW)(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType) = MessageBoxW;
//static BOOL(WINAPI* OLD_TextOutA)(HDC hdc, int x, int y,  LPCSTR lpString, _In_ int c)=TextOutA;
//
//int WINAPI NEW_MessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
//{
////    修改输入参数，调用原函数
//    int ret = OLD_MessageBoxW(hWnd, L"test3", L"test3", uType);
//    return ret;
//}
//FILE *f;
//
//BOOL WINAPI NEW_TextOutA(HDC hdc,  int x,  int y,  LPCSTR lpString,  int c)
//{
//    char *str_x = "";
//    sprintf(str_x,"%d\n",x);
//    fputs(str_x,f);
//
//    char *str_y="";
//    sprintf(str_y,"%d\n",y);
//    fputs(str_y,f);
//
//    fputs(lpString,f);
//
//    fputc('\n',f);
//
//    fputs("---------------------",f);
//}
//
//
////void Hook()
////{
////    DetourRestoreAfterWith();//恢复原来状态
////    DetourTransactionBegin();//拦截开始
////    DetourUpdateThread(GetCurrentThread());//刷新当前线程
////    DetourAttach((void**)&OLD_MessageBoxW,NEW_MessageBoxW);
////    DetourTransactionCommit();//拦截生效
////}
//
//void Hook()
//{
//    DetourRestoreAfterWith();
//
//    DetourTransactionBegin();
//    DetourUpdateThread(GetCurrentThread());
//    DetourAttach(&((PVOID)(OLD_TextOutA)), NEW_TextOutA);
//    DetourTransactionCommit();
//}
////void UnHook()
////{
////
////    DetourTransactionBegin();
////    DetourUpdateThread(GetCurrentThread());
////    DetourDetach((void**)&OLD_MessageBoxW, NEW_MessageBoxW);//撤销拦截函数
////    DetourTransactionCommit();//
////}
//
//
//void main(){
//
//    f = fopen("C:\\Users\\soyum\\Desktop\\hook.txt","w+");
//    if (f == 0){
//        MessageBoxW(NULL,"OPEN FILE FAIL" , L"test", 0);
//    }
//
//    Hook();
//
//    TextOutA(NULL,0,0,"TEXT",0);
//}