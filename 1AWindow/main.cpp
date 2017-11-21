#include<windows.h>
#include<iostream>
#define WINDOW_CLASS "appclass"
#define WINDOW_NAME "¿§à¬¸ø¸ø"

using namespace std;


LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
   switch(msg)
      {
         case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
            break;
      }
   return DefWindowProc(hWnd, msg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE prevhInst, LPSTR cmdLine, int show)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                     hInst, NULL, NULL, NULL, NULL,
                     "AppClass", NULL };

    RegisterClassEx(&wc);
   // Create the application's window
             HWND hWnd = CreateWindow(WINDOW_CLASS, WINDOW_NAME,
                                WS_OVERLAPPEDWINDOW,
                                100, 100, 640, 480, NULL, NULL,
                                    hInst ,NULL);
         ShowWindow(hWnd, SW_SHOWDEFAULT);
         UpdateWindow(hWnd);
         // Enter the message loop
         MSG msg;
         ZeroMemory(&msg, sizeof(msg));
         while(msg.message != WM_QUIT)
            {
               if(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
                  {
                     TranslateMessage(&msg);
                     DispatchMessage(&msg);
                  }
               else
                  {
                    //Do something to the screen
                }
            }
   // Unregister our window.
   UnregisterClass("AppClass", wc.hInstance);
}
