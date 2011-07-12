#include <Windows.h>
#include "globals.h"
#include "watermarker.h"
#include "..\\dwmaxx\\dwmaxx.h"

LRESULT CALLBACK WndProcProlog(int nCode, WPARAM wParam, LPARAM lParam)
{
    CWPSTRUCT       *msg = (CWPSTRUCT *)lParam;

    if (IsTopLevelWindow(msg->hwnd) == TRUE)
    switch (msg->message)
    {
    case WM_SHOWWINDOW:
        if ((BOOL)msg->wParam == FALSE && IsTopLevelWindow(msg->hwnd))
            DwmaxxRemoveWindow((HWND)msg->hwnd);
        else if (IsWindowVisible(msg->hwnd))
            WriteWatermark(msg->hwnd);
        break;
    case WM_SIZE:
        if (msg->wParam == SIZE_MINIMIZED || msg->wParam == SIZE_MAXIMIZED)
            if (IsWindowVisible(msg->hwnd))
                WriteWatermark(msg->hwnd);
        break;
    case WM_EXITSIZEMOVE:
        if (IsWindowVisible(msg->hwnd))
            WriteWatermark(msg->hwnd);
        break;
    }

    return CallNextHookEx(g_wndProcHook, nCode, wParam, lParam);
}


LRESULT CALLBACK WndProcEpilog(int nCode, WPARAM wParam, LPARAM lParam)
{
    CWPRETSTRUCT    *msg = (CWPRETSTRUCT *)lParam;

    switch (msg->message)
    {
    case WM_NCPAINT:
    case WM_PAINT:
        if (IsTopLevelWindow(msg->hwnd) == TRUE && IsWindowVisible(msg->hwnd) == TRUE)
            WriteWatermark(msg->hwnd);
        break;
    }

    return CallNextHookEx(g_wndProcRetHook, nCode, wParam, lParam);
}

LRESULT CALLBACK ShellProcProlog(int nCode, WPARAM wParam, LPARAM lParam)
{
    switch (nCode)
    {
    case HSHELL_WINDOWREPLACED:
    case HSHELL_WINDOWCREATED:
        if (IsTopLevelWindow((HWND)wParam) == TRUE && IsWindowVisible((HWND)wParam) == TRUE)
            WriteWatermark((HWND)wParam);
        break;
    case HSHELL_WINDOWDESTROYED:
        if (IsTopLevelWindow((HWND)wParam) == TRUE)
            DwmaxxRemoveWindow((HWND)wParam);
        break;
    }

    return CallNextHookEx(g_shellHook, nCode, wParam, lParam);
}
