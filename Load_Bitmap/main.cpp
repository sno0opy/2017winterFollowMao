/*使用的新的主要内容
HRESULT D3DXLoadSurfaceFromFile(
                                LPDIRECT3DSURFACE9 pDestSurface,
                                CONST PALETTEENTRY* pDestPalette,
                                CONST RECT* pDestRect,
                                LPCTSTR pStcFile
                                CONST RECT* pSrcRect,
                                DWORD FIlter,
                                D3DCOLOR Colorkey,
                                D3DXIMAGE_INFO* pSrcInfo);
*/
#include<windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include<time.h>
#include<iostream>
using namespace std;
#pragma comment(lib,"d3d9.lib");
#pragma comment(lib,"d3dx9.lib");
//program values
const string APPTILE="Draw Bitmap Program";
const int SCREENW=1024;
const int SCREENH=768;
//key macro
#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code)&0x8000?1:0)
//direct3D objects
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
LPDIRECT3DSURFACE9 backbuffer=NULL;
LPDIRECT3DSURFACE9 surface=NULL;
bool gameover=false;
//Game initializatioin fucntion
bool Game_init(HWND hwnd)
{
    //initialize Direct3D
    d3d=Direct3DCreate9(D3D_SDK_VERSION);
    if(!d3d)
    {
        MessageBox(hwnd,"Error initializing Direct3D","Error",MB_OK);
        return false;
    }
    //set Direct3d presentation paramenters
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp,sizeof(d3dpp));
    d3dpp.hDeviceWindow=hwnd;
    d3dpp.Windowed=TRUE;
    d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat=D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount=1;
    d3dpp.BackBufferHeight=SCREENH;
    d3dpp.BackBufferWidth=SCREENW;
    //create Direct3D device
    d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&d3ddev);
    if(!d3ddev)
    {
        MessageBox(hwnd,"Error creating direct3d device","Error",MB_OK);
        return false;
    }
    //clear hte back buffer to black
    d3ddev->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
    //create surface
    HRESULT result=d3ddev->CreateOffscreenPlainSurface(SCREENW,SCREENH,
                                                       D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT,
                                                       &surface,NULL);
    if(result!=D3D_OK)
        return false;
    //load surface from file into newly created surface
    result=D3DXLoadSurfaceFromFile(surface,NULL,NULL,"1.jpg",NULL,D3DX_DEFAULT,0,NULL);
    if(result!=D3D_OK)
    {
        MessageBox(hwnd,"Error loading file","Error",MB_OK);
                return false;
    }
    return true;
}
//Game update function
void Game_Run(HWND hwnd)
{
    //make sur direct3d deivice if valid
    if(!d3ddev)
        return ;
    //create pointer to the back buffer
    d3ddev->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&backbuffer);
    //start rendering
    if(d3ddev->BeginScene())
    {
        //Draw surface to the back buffer
        //d3ddev->StretchRect(surface,NULL,backbuffer,NULL,D3DTEXF_NONE);
        //只加载部分图
        RECT srcRect;
        srcRect.left=100;
        srcRect.right=500;
        srcRect.top=200;
        srcRect.bottom=400;
        d3ddev->StretchRect(surface,&srcRect,backbuffer,NULL,D3DTEXF_NONE);
        //stop rendering
        d3ddev->EndScene();
        d3ddev->Present(NULL,NULL,NULL,NULL);
    }
    //check fir escape key(to exit game)
    if(KEY_DOWN(VK_ESCAPE))
        PostMessage(hwnd,WM_DESTROY,0,0);
}
//Game shutdown function
void Game_End(HWND hwnd)
{
    if(surface)
        surface->Release();
    if(d3ddev)
        d3ddev->Release();
    if(d3d)
        d3d->Release();
}
//windows ebent handing function
LRESULT WINAPI winProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
    switch(msg)
    {
    case WM_DESTROY:
        gameover=true;
        break;
    }
    return DefWindowProc(hwnd,msg,wparam,lparam);
}
//Windows entry point function
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR CmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG message;
    ZeroMemory(&message,sizeof(message));
    //initialize window setting
    wc.cbSize=sizeof(WNDCLASSEX);
    wc.style=CS_HREDRAW|CS_VREDRAW;
    wc.lpfnWndProc=(WNDPROC)winProc;
    wc.cbClsExtra=0;
    wc.cbWndExtra=0;
    wc.hIcon=NULL;
    wc.hIconSm=NULL;
    wc.hCursor=LoadCursor(NULL,IDC_ARROW);
    wc.hInstance=hInstance;
    wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName=NULL;
    wc.lpszClassName="MainWindowClass";
    if(!RegisterClassEx(&wc))
        return FALSE;
    //create a new window
    hwnd=CreateWindow("MainWindowClass",APPTILE.c_str(),
                      WS_OVERLAPPEDWINDOW,
                      CW_USEDEFAULT,CW_USEDEFAULT,
                      SCREENW,SCREENH,
                      NULL,NULL,
                      hInstance,NULL);
    //was there an error creating the window
    if(hwnd==0)
        return 0;
    //display window
    ShowWindow(hwnd,nCmdShow);
    UpdateWindow(hwnd);
    //initialize the game
    if(!Game_init(hwnd))
        return 0;
    //Main message loop
    while(!gameover)
    {
        if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        Game_Run(hwnd);
    }
    Game_End(hwnd);
    return message.wParam;
}
