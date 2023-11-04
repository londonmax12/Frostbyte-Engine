#include "Platform/WindowsImpl/WindowsWindow.h"

#ifdef FROSTBYTE_PLATFORM_WINDOWS

#include "Logging/Logging.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

bool Frostbyte::WindowsWindow::OnInit(const WindowConfig& config)
{
    FROSTBYTE_INFO("Creating Windows Window:");
    FROSTBYTE_INFO("\t| Window Name: {}", config.Name);
    FROSTBYTE_INFO("\t| Size: [{}, {}]", config.Width, config.Height);

    m_WindowData = malloc(sizeof(WindowsWindowData));
    WindowsWindowData* windowData = (WindowsWindowData*)m_WindowData;

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "FrostbyteWindowClass";
    RegisterClass(&wc);

    windowData->Handle = CreateWindowEx(0,
        "FrostbyteWindowClass",
        config.Name,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        config.Width, config.Height,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL);

    if (!windowData->Handle) {
        FROSTBYTE_INFO("Failed to create Win32 Window: {}", config.Name);
        return false;
    }

    ShowWindow(windowData->Handle, SW_SHOWDEFAULT);
    UpdateWindow(windowData->Handle);
    
	return true;
}
void Frostbyte::WindowsWindow::OnShutdown()
{
	delete m_WindowData;
	m_WindowData = nullptr;
}
void Frostbyte::WindowsWindow::OnUpdate()
{
    MSG msg = {};
    if (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
#endif FROSTBYTE_PLATFORM_WINDOWS


