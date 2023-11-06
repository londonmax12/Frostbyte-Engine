#include "Platform/WindowsImpl/WindowsWindow.h"

#ifdef FROSTBYTE_PLATFORM_WINDOWS

#include "Logging/Logging.h"
#include "Application/EventDispatcher.h"
#include "Event/WindowEvents.h"

using namespace Frostbyte;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        Frostbyte::EventDispatcher::GetInstance()->QueueEvent((IEvent*)new Frostbyte::WindowCloseEvent());
        break;
    case WM_DESTROY:
        Frostbyte::EventDispatcher::GetInstance()->QueueEvent((IEvent*)new Frostbyte::WindowCloseEvent());
        PostQuitMessage(0);
        break;
    case WM_SIZE:
        Frostbyte::EventDispatcher::GetInstance()->QueueEvent((IEvent*)new Frostbyte::WindowResizeEvent({ LOWORD(lParam), HIWORD(lParam) }));
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        EndPaint(hwnd, &ps);
        break;
    }
    default:
        break;
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

bool Frostbyte::WindowsWindow::OnInit(const WindowConfig& config)
{
    FROSTBYTE_INFO("Creating Windows window:");
    FROSTBYTE_INFO("\t| Window Name: {}", config.Name);
    FROSTBYTE_INFO("\t| Size: {}", config.WindowSize);

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
        config.WindowSize.x, config.WindowSize.y,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    if (!windowData->Handle) {
        FROSTBYTE_INFO("Failed to create Win32 Window: {}", config.Name);
        return false;
    }

    ShowWindow(windowData->Handle, SW_SHOWDEFAULT);
    UpdateWindow(windowData->Handle);

    Frostbyte::EventDispatcher::GetInstance()->AddListener(EVENT_WINDOW_RESIZE, [&](IEvent* e){ 
        m_Size = ((WindowResizeEvent*)e)->Size;
    });
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
    if (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
void Frostbyte::WindowsWindow::OnResize(VectorInt2)
{
    
}
#endif FROSTBYTE_PLATFORM_WINDOWS