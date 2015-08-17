
#include <windows.h>
#include "main.h"
#include "ribbon.h"

global bool32 globalRunning;
global uint64 globalPerfCountFrequency;
global HWND globalWindow;

internal void
openFile(HWND windowHandle)
{
    OPENFILENAME ofn = {};
    char filename[MAX_PATH] = "";

    ofn.lStructSize = sizeof(ofn); 
    ofn.hwndOwner = windowHandle;
    //ofn.lpstrFilter = "your mom (*.mom)\0*.mom\0ALL THE FILES (*.*)\0*.*\0";
    ofn.lpstrFilter = fileExtention("yourmom", "*.mom")
        fileExtention("all files","*.*");
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "txt";
    
    if(GetOpenFileName(&ofn))
    {
        //do eet
    }
}

LRESULT
mainWindowCallback(HWND window,
                   UINT message,
                   WPARAM wParam,
                   LPARAM lParam)
{
    LRESULT result = 0;

    switch(message)
    {
        case WM_ACTIVATEAPP:
        {
        } break;
        case WM_CLOSE:
        {
            globalRunning = false;
        } break;

        case WM_DESTROY:
        {
            globalRunning = false;
        } break;
        
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            assert(!"Keyboard input no longer handled here.");
        } break;

        default:
        {
            result = DefWindowProcA(window, message, wParam, lParam);
        } break;
    }

    return result;
}

internal int32
error(char *msg)
{
    int result = GetLastError();
    MessageBoxA(0, msg, "error\n\r", 0);
    return result;
}

internal void
processPendingMessages(HWND window)
{
    MSG message;
    while(PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
    {
        switch(message.message)
        {
            case WM_QUIT:
            {
                globalRunning = false;
            } break;

            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP:
            {
                uint32 VKCode = (uint32)message.wParam;
                bool32 wasDown = (message.lParam & (1 << 30)) != 0;
                bool32 isDown = (message.lParam & (1 << 31)) == 0;
                if(wasDown != isDown)
                {
                    if(VKCode == VK_ESCAPE)
                    {
                    }
                    else if(VKCode == VK_SPACE)
                    {
                    }
                }

                //alt-f4
                if ((VKCode == VK_F4) && (message.lParam & (1 << 29)))
                {
                    globalRunning = false;
                }
            } break;

            case WM_COMMAND:
            {
                uint32 selection = (uint32)message.wParam;
                if (!strcmp(menu[selection],"exit"))
                {
                    globalRunning = false;
                }
                if (!strcmp(menu[selection],"open"))
                {
                    openFile(window);
                }
            } break;

            default:
            {
                TranslateMessage(&message);
                DispatchMessageA(&message);
            } break;
        };
    }
}

int32 execute(
    UINT nCmdID,
    UI_EXECUTIONVERB verb,
    __in_opt const PROPERTYKEY* key,
    __in_opt const PROPVARIANT* ppropvarValue,
    __in_opt IUISimplePropertySet* pCommandExecutionProperties)
{
    switch (nCmdID)
    {
        case IDC_CMD_EXIT:
        {
            globalRunning = false;
        } break;
        case cmdButton1:
        {
            error("Did it work???");
        } break;
        case cmdButton2:
        {
            openFile(globalWindow);
        } break;
    }

    return S_OK;
}


int CALLBACK
WinMain(HINSTANCE instance,
        HINSTANCE,
        LPSTR,
        int)
{
    LARGE_INTEGER perfFrequencyResult;
    QueryPerformanceFrequency(&perfFrequencyResult);
    globalPerfCountFrequency = perfFrequencyResult.QuadPart;

    WNDCLASS windowClass = {};
  
    windowClass.style = CS_HREDRAW|CS_VREDRAW;
    windowClass.lpfnWndProc = mainWindowCallback;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = "mainWindowClass";
    windowClass.hCursor = LoadCursor(0, IDC_ARROW);
    windowClass.hIcon = LoadIcon(0, IDI_APPLICATION);

    executeFunction = &execute;

    if(RegisterClassA(&windowClass))
    {
        globalWindow =
            CreateWindowExA(
                0,
                windowClass.lpszClassName,
                "main",
                WS_CAPTION|WS_SYSMENU|WS_VISIBLE|WS_MINIMIZEBOX|WS_MAXIMIZEBOX,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                0,0,instance,0);
        if(globalWindow)
        {
            if(initializeRibbon(globalWindow))
            {
                globalRunning = true;

                while(globalRunning)
                {
                    processPendingMessages(globalWindow);
                }
            } else {
                error("ribbon failed");
            }
        } else {
            error("window creation failed");
        }
    } else {
        error("window class registration failed");
    }

    return 0;
}
 
