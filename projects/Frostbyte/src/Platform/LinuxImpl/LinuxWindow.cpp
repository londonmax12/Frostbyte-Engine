#include "Platform/LinuxImpl/LinuxWindow.h"

#ifdef FROSTBYTE_PLATFORM_LINUX

#include "Logging/Logging.h"
#include "Application/EventDispatcher.h"
#include "Event/WindowEvents.h"

#if _POSIX_C_SOURCE >= 199309L
#include <time.h>
#else
#include <unistd.h>
#endif

#include <sys/time.h>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>

bool Frostbyte::LinuxWindow::OnInit(const WindowConfig& config)
{
    FROSTBYTE_INFO("Creating Linux window:");
    FROSTBYTE_INFO("\t| Window Name: {}", config.Name);
    FROSTBYTE_INFO("\t| Size: {}", config.WindowSize);

    m_WindowData = malloc(sizeof(LinuxWindowData));
    LinuxWindowData* windowData = (LinuxWindowData*)m_WindowData;

    windowData->_Display = XOpenDisplay(0);

    XAutoRepeatOff(windowData->_Display);
    windowData->Connection = XGetXCBConnection(windowData->_Display);

    if(xcb_connection_has_error(windowData->Connection)) {
        FROSTBYTE_FATAL("Failed to connect to X server via XCB");
        return false;
    }

    const struct xcb_setup_t* setup = xcb_get_setup(windowData->Connection);
    
    xcb_screen_iterator_t it = xcb_setup_roots_iterator(setup);
    int screenP = 0;
    for (int i = screenP; i > 0; i--) {
        xcb_screen_next(&it);
    }

    windowData->Screen = it.data;
    windowData->Window = xcb_generate_id(windowData->Connection);

    int eventMask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    int eventValues =   XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
                        XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
                        XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION |
                        XCB_EVENT_MASK_STRUCTURE_NOTIFY;

    int valueList[] = {windowData->Screen->black_pixel, eventValues};

    xcb_void_cookie_t cookie = xcb_create_window(windowData->Connection, XCB_COPY_FROM_PARENT, windowData->Window, windowData->Screen->root, 0, 0, config.WindowSize.x, config.WindowSize.y, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, windowData->Screen->root_visual, eventMask, valueList);

    xcb_change_property(windowData->Connection, XCB_PROP_MODE_REPLACE, windowData->Window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, strlen(config.Name), config.Name);
    
    xcb_intern_atom_cookie_t wmDeleteCookie = xcb_intern_atom(windowData->Connection, 0, strlen("WM_DELETE_WINDOW"), "WM_DELETE_WINDOW");
    xcb_intern_atom_cookie_t wmProtocolsCookie = xcb_intern_atom(windowData->Connection, 0, strlen("WM_PROTOCOLS"), "WM_PROTOCOLS");
    xcb_intern_atom_reply_t* wmDeleteReply = xcb_intern_atom_reply(windowData->Connection, wmDeleteCookie, 0);
    xcb_intern_atom_reply_t* wmProtocolsReply = xcb_intern_atom_reply(windowData->Connection, wmProtocolsCookie, 0);
    windowData->WMDeleteWin = wmDeleteReply->atom;
    windowData->WMProtocols = wmProtocolsReply->atom;

    xcb_change_property(windowData->Connection, XCB_PROP_MODE_REPLACE, windowData->Window, wmProtocolsReply->atom, 4, 32, 1, &wmDeleteReply->atom);
	xcb_map_window(windowData->Connection, windowData->Window);

    int streamResult = xcb_flush(windowData->Connection);
    if (streamResult <= 0) {
        FROSTBYTE_FATAL("Failed to flush stream: {}", streamResult);
        return false;
    }

    return true;
}
void Frostbyte::LinuxWindow::OnShutdown()
{
    LinuxWindowData* windowData = (LinuxWindowData*)m_WindowData;

    XAutoRepeatOn(windowData->_Display);
    xcb_destroy_window(windowData->Connection, windowData->Window);

	delete m_WindowData;
	m_WindowData = nullptr;
}
void Frostbyte::LinuxWindow::OnUpdate()
{
    LinuxWindowData* windowData = (LinuxWindowData*)m_WindowData;

    xcb_generic_event_t* event;
    while ((event = xcb_poll_for_event(windowData->Connection))) {
        switch (event->response_type & ~0x80) {
            case XCB_CONFIGURE_NOTIFY: {
                xcb_configure_notify_event_t* configureEvent = (xcb_configure_notify_event_t*)event;
                m_Size = { configureEvent->width, configureEvent->height };
                Frostbyte::EventDispatcher::GetInstance()->QueueEvent((IEvent*)new Frostbyte::WindowResizeEvent(m_Size));
                break;
            }
            case XCB_CLIENT_MESSAGE: {
                xcb_client_message_event_t* clientMessageEvent = (xcb_client_message_event_t*)event;
                if (clientMessageEvent->data.data32[0] == windowData->WMDeleteWin) {
                    Frostbyte::EventDispatcher::GetInstance()->QueueEvent((IEvent*)new Frostbyte::WindowCloseEvent());
                }
                break;
            }
        }

        free(event);
    }
}
void Frostbyte::LinuxWindow::OnResize(VectorInt2)
{
    
}
#endif