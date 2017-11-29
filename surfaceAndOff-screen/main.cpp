/*使用的新的主要内容
LPDIRECT3DSURFACE9 surface=NULL;
HRESULT ColorFill(
                  IDirect3DSurface9 *pSurface,
                  CONST RECT *pRect,
                  D3DCOLOR color
                  );
d3ddev->ColorFill(surface,NULL,D3DCOLOR_XRGB(255,0,0);

HRESULT StretchRect(
                    IDirect3DSurface9 *pSourceSurface,
                    CONST RECT *pSourceRect,
                    IDIRECT3DSurface9 *pDestSurface,
                    CONST RECT *pDestRect,
                    D3DTEXTUREFILTERTYPE Filter);
d3ddev_>StretchRect(surface,NULL,backbuffer,NULL,D3DTEXF_NONE);

HRESULT GetBackBuff(
                    UINT iswapChain,
                    UINT BackBuffer,
                    D3DBACKBUFFER_TYPE type,
                    IDirect3DSurface9 **ppBackBuffer);
LPDERICT3DSURFACE9 *backbuffer=NULL;
d3ddev->GetBackBuff(0,0,D3DBACKBUFFER_TYPE_MONO,&backbuffer);
*/
#include<windows.h>
#include<d3d9.h>
#include<time.h>
#include<iostream>
using namespace std;
#pragma comment(lib,"d3d9.lib");
#pragma comment(lin,"d3d9x.lib");
//application title
const string APPTITLE="Create Surface Program";
//macro to read the keyboard
#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code)&0x8000?1:0)
//screen resolution
#define SCREENH 1024
#define SCREENW 768
//Direct3D objects
LPDIRECT3D9 d3d=NULL;
LPDIRECT3DDEVICE9 d3ddev=NULL;
LPDIRECT3DSURFACE9 backbuffer=NULL;
LPDIRECT3DSURFACE9 surface=NULL;
bool gameover=false;
//Game initialization function
bool Game_init(HWND hwnd)
{
    //initialize Direct3D
    d3d=Direct3DCreate9(D3D_SDK_VERSION);
    if(d3d==NULL)
    {
        MessageBox(hwnd,"Error initializing Direct3D","Error",MB_OK);
        return false;
    }
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp,sizeof(d3dpp));
    d3dpp.Windowed=true;
    d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow=hwnd;
    d3dpp.BackBufferCount=1;
    d3dpp.BackBufferFormat=D3DFMT_X8R8G8B8;
    d3dpp.BackBufferHeight=SCREENH;
    d3dpp.BackBufferWidth=SCREENW;
    d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,&d3ddev);
    if(!d3ddev)
    {
        MessageBox(hwnd,"Error initializing Direct3D Device","Error",MB_OK);
        return false;
    }
    //set Rabdom number seed
    srand((unsigned int)time(NULL));
    //clear the backbuffer to balck
    d3ddev->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
    //create pointer to the back buffer
    d3ddev->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&backbuffer);
    //create surface
    HRESULT result=d3ddev->CreateOffscreenPlainSurface(
                                        100,                //width of the surface
                                        100,                //height of the surface
                                        D3DFMT_X8R8G8B8,    //surface format
                                        D3DPOOL_DEFAULT,    //memory pool to use
                                        &surface,           //pointer to the surface
                                        NULL);              //reserved (always null)
    if(result!=D3D_OK)
        return false;
    return true;
}

//game update function
void Game_Run(HWND hwnd)
{
    //make sure the direct3d device is valid
    if(!d3ddev)
        return ;
    //start rendering
    if(d3ddev->BeginScene())
    {
        //fill the surface with random color
        int r=rand()%225;
        int g=rand()%225;
        int b=rand()%225;
        d3ddev->ColorFill(surface,NULL,D3DCOLOR_XRGB(r,g,b));
        //copy the surface to the backbuffer
        RECT rect;
        rect.left=rand()%SCREENW/2;
        rect.right=rect.left+rand()%SCREENW/2;
        rect.top=rand()%SCREENH;
        rect.bottom=rand()%SCREENH/2+rect.top;
        //d3ddev->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
        d3ddev->StretchRect(surface,NULL,backbuffer,&rect,D3DTEXF_NONE);
        //stop rendering
        d3ddev->EndScene();
        //copy the back buffer to the present
        d3ddev->Present(NULL,NULL,NULL,NULL);
    }
    //check for escape key (to exit)
    if(KEY_DOWN(VK_ESCAPE))
    {
        PostMessage(hwnd,WM_DESTROY,0,0);
    }
}
//game shut down
void Game_End(HWND hwnd)
{
    if(surface)
        surface->Release();
    if(d3ddev)
        d3ddev->Release();
    if(d3d)
        d3d->Release();
}
//window event call back function
LRESULT WINAPI WinProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
    switch(msg)
    {
    case WM_DESTROY:
        gameover=true;
        PostQuitMessage(NULL);
        return 0;
    }
    return DefWindowProc(hwnd,msg,wparam,lparam);
}
//windows entry point function
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPreIstance,LPSTR lpCmdLine,int nCmdShow)
{
    //create the window class structure
    WNDCLASSEX wc;
    wc.cbSize=sizeof(wc);
    wc.style=CS_HREDRAW|CS_VREDRAW;
    wc.lpfnWndProc=(WNDPROC)WinProc;
    wc.cbClsExtra=0;
    wc.cbWndExtra=0;
    wc.hInstance=hInstance;
    wc.hIcon=NULL;
    wc.hCursor=LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName=NULL;
    wc.lpszClassName="MainWindowClass";
    wc.hIconSm=NULL;
    RegisterClassEx(&wc);
    //create a new window
    HWND window=CreateWindow("MainWindowClass",APPTITLE.c_str(),
                 WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,
                 SCREENW,SCREENH,
                 NULL,NULL,
                 hInstance,NULL);
    //was there an error creating the window
    if(window==0)
        return 0;
    //display the window
    ShowWindow(window,nCmdShow);
    UpdateWindow(window);
    //initialize the game
    if(!Game_init(window))
        return 0;
    MSG message;
    while(!gameover)
    {
        if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        Game_Run(window);
    }
    Game_End(window);
    return message.wParam;
}

