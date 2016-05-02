#include "StdAfx.h"
#include "InstallService.h"

CInstallService::CInstallService(void)
{
}

CInstallService::~CInstallService(void)
{
}

BOOL CInstallService::InstallService( LPCTSTR lpServiceName, LPCTSTR lpDisplayName, LPCTSTR lpServicePath )
{
    if ((!lpServiceName || !*lpServiceName)
        || (!lpDisplayName || !*lpDisplayName)
        || (!lpServicePath || !*lpServicePath || !::PathFileExists(lpServicePath)))
    {
        return FALSE;
    }

    BOOL bRet = FALSE;
    CString strServicePath;
    SC_HANDLE scCreate = NULL;
    SERVICE_DESCRIPTION sd = {0};
    SC_HANDLE scManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if (!scManager)
    {
        goto Exit0;
    }

    strServicePath.Format(L"\"%s\"", lpServicePath);
    scCreate = ::CreateService(scManager, lpServiceName, lpDisplayName, SERVICE_ALL_ACCESS,
                              SERVICE_WIN32_SHARE_PROCESS, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
                              strServicePath, L"COM Infrastructure", NULL, NULL, NULL, NULL);
    if (!scCreate)
    {
        if (ERROR_SERVICE_EXISTS == ::GetLastError())
        {
            scCreate = ::OpenService(scManager, lpServiceName, SERVICE_CHANGE_CONFIG);
            if (!scCreate)
            {
                goto Exit0;
            }

            ::ChangeServiceConfig(scCreate, SERVICE_WIN32_SHARE_PROCESS, SERVICE_AUTO_START,
                                  SERVICE_ERROR_NORMAL, strServicePath, L"COM Infrastructure",
                                  NULL, NULL, NULL, NULL, L"HUANG HAIHUA");
        }
        else
        {
            goto Exit0;
        }
    }

    sd.lpDescription = L"HAIHUA ºËÐÄ·þÎñ";
    ::ChangeServiceConfig2(scCreate, SERVICE_CONFIG_DESCRIPTION, &sd);

    bRet = TRUE;
Exit0:
    if (scCreate)
    {
        ::CloseServiceHandle(scCreate);
    }

    if (scManager)
    {
        ::CloseServiceHandle(scManager);
    }

    return bRet;
}

BOOL CInstallService::DeleteService( LPCTSTR lpServiceName )
{
    if (!lpServiceName || !*lpServiceName)
    {
        return FALSE;
    }

    BOOL bRet = FALSE;
    SC_HANDLE scOpen = NULL;
    SERVICE_STATUS ssStatus = {0};
    SC_HANDLE scManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!scManager)
    {
        goto Exit0;
    }

    scOpen = ::OpenService(scManager, lpServiceName, SC_MANAGER_ALL_ACCESS);
    if (!scOpen)
    {
        goto Exit0;
    }

    if (!::QueryServiceStatus(scOpen, &ssStatus))
    {
        goto Exit0;
    }

    if (SERVICE_STOPPED != ssStatus.dwCurrentState)
    {
        goto Exit0;
    }

    if (!::DeleteService(scOpen))
    {
        goto Exit0;
    }

    bRet = TRUE;
Exit0:
    if (scOpen)
    {
        ::CloseServiceHandle(scOpen);
    }

    if (scManager)
    {
        ::CloseServiceHandle(scManager);
    }

    return bRet;
}

BOOL CInstallService::StartService( LPCTSTR lpServiceName )
{
    if (!lpServiceName || !*lpServiceName)
    {
        return FALSE;
    }

    BOOL bRet = FALSE;
    SC_HANDLE scOpen = NULL;
    SC_HANDLE scManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!scManager)
    {
        goto Exit0;
    }

    scOpen = ::OpenService(scManager, lpServiceName, SC_MANAGER_ALL_ACCESS);
    if (!scOpen)
    {
        goto Exit0;
    }

    if (!::StartService(scOpen, 0, NULL))
    {
        goto Exit0;
    }

    bRet = TRUE;
Exit0:
    if (scOpen)
    {
        ::CloseServiceHandle(scOpen);
    }

    if (scManager)
    {
        ::CloseServiceHandle(scManager);
    }

    return bRet;
}

BOOL CInstallService::ControlService( LPCTSTR lpServiceName, DWORD dwControl )
{
    if (!lpServiceName || !*lpServiceName)
    {
        return FALSE;
    }

    BOOL bRet = FALSE;
    SC_HANDLE scOpen = NULL;
    SERVICE_STATUS ssStatus = {0};
    SC_HANDLE scManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!scManager)
    {
        goto Exit0;
    }

    scOpen = ::OpenService(scManager, lpServiceName, SC_MANAGER_ALL_ACCESS);
    if (!scOpen)
    {
        goto Exit0;
    }

    if (!::ControlService(scOpen, dwControl, &ssStatus))
    {
        goto Exit0;
    }

    bRet = TRUE;
Exit0:
    if (scOpen)
    {
        ::CloseServiceHandle(scOpen);
    }

    if (scManager)
    {
        ::CloseServiceHandle(scManager);
    }

    return bRet;
}