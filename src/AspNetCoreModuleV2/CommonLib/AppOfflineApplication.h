// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#pragma once

#include "application.h"
#include "requesthandler.h"
#include "PollingAppOfflineApplication.h"

class AppOfflineApplication: public PollingAppOfflineApplication
{
public:
    AppOfflineApplication(IHttpApplication& pApplication)
        : PollingAppOfflineApplication(pApplication, PollingAppOfflineApplicationMode::StopWhenRemoved)
    {
    }

    HRESULT CreateHandler(IHttpContext* pHttpContext, IREQUEST_HANDLER** pRequestHandler) override;

    HRESULT OnAppOfflineFound() override;

    static bool ShouldBeStarted(IHttpApplication& pApplication);

private:
    std::string m_strAppOfflineContent;
};

