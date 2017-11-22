/*使用的新的主要内容

LPDIRECT3D9 d3d= NULL;
LPDIRECT3DDEVICE9 d3ddev=NULL;

d3d=Direct3DCreate9(D3D_SDK_VERSION)
d3d->createdevice(D3DADAPTER_DEFAULT,  //use default video card
                    D3DDEVTYPE_HAL,      //use the hardware renderer
                    hwnd,
                    D3DCREATE_SOFTWARE_VERTEXPROCESSING   //do not use T&L(for compatibility)
                    &d3dpp,              //presentation parameters
                    &d3ddev);            //
D3DPRESENT_PARAMETERS d3dpp;
ZeroMemory(&d3dpp,sizeof(d3dpp));
d3dpp.Windowed=true;
d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;
d3dpp.BackBufferFormat=D3DFMT_UNKNOWN;
*/
#include<windows.h>
#include<d3d9.h>
#include<time.h>
#include<iostream>
using namespace std;
#pragma comment(lib,"d3d9.lib")
//program setting
const string APPTITLE="Direct3D_Windowed";
const int SCREENW=1024;
const int SCREENH=768;
//Direct3D objects
LPDIRECT3D9 d3d=NULL;
LPDIRECT3DDEVICE9 d3ddev=NULL;
bool gameover=false;
//macro to detect key presses
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000?1:0))

//game initialization function
bool Game_init(HWND hwnd)
{
    //initialize Direct3D
    d3d=Direct3DCreate9(D3D_SDK_VERSION);
    if(d3d==NULL)
    {
        MessageBox(hwnd,"Error initialize Direct3D","Error",MB_OK);
        return FALSE;
    }
    //set direct3d present parameters
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp,sizeof(d3dpp));
    d3dpp.Windowed=TRUE;
    d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat=D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount=1;
    d3dpp.BackBufferHeight=SCREENH;
    d3dpp.BackBufferWidth=SCREENW;
    d3dpp.hDeviceWindow=hwnd;
    //create direct3d device;
    d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&d3ddev);
    if(d3ddev=NULL)
    {
        MessageBox(hwnd,"Error creating direct3D device ","Error",MB_OK);
        return FALSE;
    }
    return true;
}

//game uodate function
void Game_Run(HWND hwnd)
{
    //make sure direct3D device is valid
    if(!d3ddev)
        return;
    //clear the backbuffer to bright green
    d3ddev->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,255),1.0f,0);
    //start rendering
    if(d3ddev->BeginScene())
    {
        //do something
        d3ddev->EndScene();
        //copy back buffer to the frame buffer
        d3ddev->Present(NULL,NULL,NULL,NULL);
    }
    //check for escape key (to exit program)
    if(KEY_DOWN(VK_ESCAPE))
        PostMessage(hwnd,WM_DESTROY,0,0);
}
//game shut down free memeory
void Game_End(HWND hwnd)
{
    if(d3ddev)
    {
        d3ddev->Release();
        d3ddev=NULL;
    }
    if(d3d)
    {
        d3d->Release();
        d3ddev=NULL;
    }
}

//window event handing function
LRESULT winProc (HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
    switch (msg)
    {
    case WM_DESTROY:
        gameover=TRUE;
        break;
    }
    return DefWindowProc(hwnd,msg,wparam,lparam);
}
