// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#include "aspnetcore_shim_config.h"

#include "config_utility.h"
#include "hostfxr_utility.h"
#include "ahutil.h"

HRESULT
ASPNETCORE_SHIM_CONFIG::Populate(
    IHttpServer      *pHttpServer,
    IHttpApplication *pHttpApplication
)
{
    STACK_STRU(strHostingModel, 12);
    STRU                            strApplicationFullPath;
    IAppHostAdminManager           *pAdminManager = NULL;
    CComPtr<IAppHostElement>        pAspNetCoreElement;

    pAdminManager = pHttpServer->GetAdminManager();
    RETURN_IF_FAILED(m_struConfigPath.Copy(pHttpApplication->GetAppConfigPath()));
    RETURN_IF_FAILED(m_struApplicationPhysicalPath.Copy(pHttpApplication->GetApplicationPhysicalPath()));

    const CComBSTR bstrAspNetCoreSection = CS_ASPNETCORE_SECTION;

    RETURN_IF_FAILED(pAdminManager->GetAdminSection(bstrAspNetCoreSection,
        m_struConfigPath.QueryStr(),
        &pAspNetCoreElement));

    RETURN_IF_FAILED(GetElementStringProperty(pAspNetCoreElement,
        CS_ASPNETCORE_PROCESS_EXE_PATH,
        &m_struProcessPath));

    // Swallow this error for backward compatability
    // Use default behavior for empty string
    GetElementStringProperty(pAspNetCoreElement,
        CS_ASPNETCORE_HOSTING_MODEL,
        &strHostingModel);

    if (strHostingModel.IsEmpty() || strHostingModel.Equals(L"outofprocess", TRUE))
    {
        m_hostingModel = HOSTING_OUT_PROCESS;
    }
    else if (strHostingModel.Equals(L"inprocess", TRUE))
    {
        m_hostingModel = HOSTING_IN_PROCESS;
    }
    else
    {
        // block unknown hosting value
        RETURN_IF_FAILED(HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED));
    }

    RETURN_IF_FAILED(GetElementStringProperty(pAspNetCoreElement,
        CS_ASPNETCORE_PROCESS_ARGUMENTS,
        &m_struArguments));

    RETURN_IF_FAILED(ConfigUtility::FindHandlerVersion(pAspNetCoreElement, &m_struHandlerVersion));

    return S_OK;
}
