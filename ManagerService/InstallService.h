#pragma once

#define SERVICE_NAME_ONE L"ServiceTestOne"
#define SERVICE_NAME_TWO L"ServiceTestTwo"

#define SERVICE_DISPLAYNAME_ONE L"HAIHUAHUANG Service One"
#define SERVICE_DISPLAYNAME_TWO L"HAIHUAHUANG Service Two"

class CInstallService
{
public:
    CInstallService(void);
    ~CInstallService(void);

    BOOL InstallService(LPCTSTR lpServiceName, LPCTSTR lpDisplayName, LPCTSTR lpServicePath);
    BOOL DeleteService(LPCTSTR lpServiceName);

    BOOL StartService(LPCTSTR lpServiceName);
    BOOL ControlService(LPCTSTR lpServiceName, DWORD dwControl);
};
