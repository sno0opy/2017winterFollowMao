#include <windows.h>
#include <iostream>
#include <time.h>

using namespace std;
const string APPTITLE="Game //free// Loop";
HWND window;
HDC device;
//捕获.jpg c.bmp 捕获.bmp
const string filename ="捕获.bmp";
bool gameover=false;
// 看书的p48
//loads and draws a bitmao from file and the frees the memory
void DrawBitmap(const char *lpfilename ,int x, int y)
{
    //load the bitmap image
    HBITMAP image=(HBITMAP)LoadImage(0,lpfilename,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
    //read the bitmap's properties
    BITMAP bm;
    GetObject(image,sizeof(BITMAP),&bm);
    //create a device context for bitmap
    HDC hdcImage= CreateCompatibleDC(device);
    SelectObject(hdcImage,image);
    //draw the bitmap to the window (bit block transfer)
    BitBlt(device,x,y,bm.bmWidth,bm.bmHeight,hdcImage,0,0,SRCCOPY);
    //delete the device context and bitmap
    DeleteDC(hdcImage);
    DeleteObject(image);

}

//startup and loading code goes there
bool Game_init(){
    //start up the random number generator
    srand(time(NULL));
    return 1;
}
//update function called from inside game loop
void game_Run()
{
    if (gameover==true)
        return;
    //get the drawing surface
    RECT rect;
    GetClientRect(window,&rect);
    //draw bitmap at random location;
    int x=rand()%(rect.right-rect.left);
    int y=rand()%(rect.bottom-rect.top);
    DrawBitmap(filename.c_str(),x,y);
}
//shutdown code
void Game_End()
{
    //free device
    ReleaseDC(window,device);
}

LRESULT CALLBACK winProc(HWND hwnd ,UINT message,WPARAM wparam,LPARAM lparam)
{
    switch(message)
    {
    case WM_DESTROY :
        gameover=0;
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd,message,wparam,lparam);
}

//myRegisterClass function sets program window porperties
ATOM myRegisterClass(HINSTANCE hInstance)
{
    //create window class structure
    WNDCLASSEX wc;
    wc.cbSize=sizeof(WNDCLASSEX);
    //fill the struct with info
    wc.style =CS_HREDRAW|CS_VREDRAW;
    wc.lpfnWndProc=(WNDPROC)winProc;
    wc.cbClsExtra=0;
    wc.cbWndExtra=0;
    wc.hInstance=hInstance;
    wc.hIcon=NULL;
    wc.hCursor=LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName=NULL;
    wc.lpszClassName=APPTITLE.c_str();
    wc.hIconSm=NULL;
    //set up the window with the class info
    return RegisterClassEx(&wc);

}

//Helper function to create the window and refresh it
BOOL InitInstance(HINSTANCE hinstance,int nCmdShow)
{
    //create a new window
    window=CreateWindow(APPTITLE.c_str(),APPTITLE.c_str(),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,640,480,NULL,NULL,hinstance,NULL);
    if(window==0)
        return 0;
    //display the window
    ShowWindow(window,nCmdShow);
    UpdateWindow(window);
    //get device context for drawing
    device =GetDC(window);
    return 1;
}
//Entry point function
int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
    MSG msg;
    //create window
    myRegisterClass(hinstance);
    if(!InitInstance(hinstance,nCmdShow))
        return 0;
    //initialize the game
    if(!Game_init())
        return 0;
    //message loop
    while(!gameover)
    {
        //process Windows event
        if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        //process game loop
        game_Run();
    }
    //free game resource
    Game_End();
    return msg.wParam;
}
