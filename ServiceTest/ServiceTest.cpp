// ServiceTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define SERVICE_NAME_ONE L"ServiceTestOne"
#define SERVICE_NAME_TWO L"ServiceTestTwo"

HANDLE g_hStopEventOne = NULL;
HANDLE g_hStopEventTwo = NULL;

SERVICE_STATUS_HANDLE g_ssHandleOne = NULL;
SERVICE_STATUS_HANDLE g_ssHandleTwo = NULL;

VOID WINAPI HandlerOne(DWORD fdwControl)
{
    SERVICE_STATUS ssStatus = {0};
    ssStatus.dwServiceType = SERVICE_WIN32_SHARE_PROCESS;
    ssStatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP;
    ssStatus.dwWin32ExitCode = NO_ERROR;
    ssStatus.dwServiceSpecificExitCode = NO_ERROR;
    ssStatus.dwCheckPoint = 0;
    ssStatus.dwWaitHint = 0;

    switch (fdwControl)
    {
    case SERVICE_ACCEPT_STOP:
        {
            ::SetEvent(g_hStopEventOne);

            ssStatus.dwCurrentState = SERVICE_STOPPED;
            ::SetServiceStatus(g_ssHandleOne, &ssStatus);
            break;
        }
    case SERVICE_ACCEPT_SHUTDOWN:
        {
            ::SetEvent(g_hStopEventOne);
            break;
        }
    default:
        {
            break;
        }
    }
}

VOID WINAPI ServiceMainOne(DWORD dwArgc, LPTSTR* lpszArgv)
{
    g_ssHandleOne = ::RegisterServiceCtrlHandler(SERVICE_NAME_ONE, HandlerOne);
    if (!g_ssHandleOne)
    {
        return;
    }

    SERVICE_STATUS ssStatus = {0};
    ssStatus.dwServiceType = SERVICE_WIN32_SHARE_PROCESS;
    ssStatus.dwCurrentState = SERVICE_RUNNING;
    ssStatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP;
    ssStatus.dwWin32ExitCode = NO_ERROR;
    ssStatus.dwServiceSpecificExitCode = NO_ERROR;
    ssStatus.dwCheckPoint = 0;
    ssStatus.dwWaitHint = 0;
    if (!::SetServiceStatus(g_ssHandleOne, &ssStatus))
    {
        return;
    }

    do 
    {
        if (WAIT_TIMEOUT == ::WaitForSingleObject(g_hStopEventOne, 1000))
        {
            ::Sleep(1000);
        }
        else
        {
            break;
        }

    } while (TRUE);
}

VOID WINAPI HandlerTwo(DWORD fdwControl)
{
    SERVICE_STATUS ssStatus = {0};
    ssStatus.dwServiceType = SERVICE_WIN32_SHARE_PROCESS;
    ssStatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP;
    ssStatus.dwWin32ExitCode = NO_ERROR;
    ssStatus.dwServiceSpecificExitCode = NO_ERROR;
    ssStatus.dwCheckPoint = 0;
    ssStatus.dwWaitHint = 0;

    switch (fdwControl)
    {
    case SERVICE_ACCEPT_STOP:
        {
            ::SetEvent(g_hStopEventTwo);

            ssStatus.dwCurrentState = SERVICE_STOPPED;
            ::SetServiceStatus(g_ssHandleTwo, &ssStatus);
            break;
        }
    case SERVICE_ACCEPT_SHUTDOWN:
        {
            ::SetEvent(g_hStopEventTwo);
            break;
        }
    default:
        {
            break;
        }
    }
}

VOID WINAPI ServiceMainTwo(DWORD dwArgc, LPTSTR* lpszArgv)
{
    SERVICE_STATUS_HANDLE g_ssHandleTwo = ::RegisterServiceCtrlHandler(SERVICE_NAME_TWO, HandlerTwo);
    if (!g_ssHandleTwo)
    {
        return;
    }

    SERVICE_STATUS ssStatus = {0};
    ssStatus.dwServiceType = SERVICE_WIN32_SHARE_PROCESS;
    ssStatus.dwCurrentState = SERVICE_RUNNING;
    ssStatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP;
    ssStatus.dwWin32ExitCode = NO_ERROR;
    ssStatus.dwServiceSpecificExitCode = NO_ERROR;
    ssStatus.dwCheckPoint = 0;
    ssStatus.dwWaitHint = 0;
    if (!::SetServiceStatus(g_ssHandleTwo, &ssStatus))
    {
        return;
    }

    do 
    {
        if (WAIT_TIMEOUT == ::WaitForSingleObject(g_hStopEventTwo, 1000))
        {
            ::Sleep(1000);
        }
        else
        {
            break;
        }

    } while (TRUE);
}

int _tmain(int argc, _TCHAR* argv[])
{
    g_hStopEventOne = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    g_hStopEventTwo = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    if (!g_hStopEventOne || !g_hStopEventTwo)
    {
        return 0;
    }

    SERVICE_TABLE_ENTRY stEntry[] = {{SERVICE_NAME_ONE, ServiceMainOne}, {SERVICE_NAME_TWO, ServiceMainTwo}, {NULL, NULL}};
    if (!::StartServiceCtrlDispatcher(stEntry))
    {
        DWORD dw = ::GetLastError();
    }

    return 0;
}