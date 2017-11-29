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

