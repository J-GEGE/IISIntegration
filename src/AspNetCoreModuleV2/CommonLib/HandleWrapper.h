// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the Apache License, Version 2.0. See License.txt in the project root for license information.

#pragma once

#include <Windows.h>

struct InvalidHandleTraits
{
    using HandleType = HANDLE;
    static const HANDLE DefaultHandle;
    static void Close(HANDLE handle) { CloseHandle(handle); }
};

// Workaround for VC++ bug https://developercommunity.visualstudio.com/content/problem/33928/constexpr-failing-on-nullptr-v141-compiler-regress.html
const HANDLE InvalidHandleTraits::DefaultHandle = INVALID_HANDLE_VALUE;

struct NullHandleTraits
{
    using HandleType = HANDLE;
    static constexpr HANDLE DefaultHandle = NULL;
    static void Close(HANDLE handle) { CloseHandle(handle); }
};

template<typename traits>
class HandleWrapper
{
public:
    using HandleType = typename traits::HandleType;

    HandleWrapper(HandleType handle = traits::DefaultHandle) : m_handle(handle) { }
    ~HandleWrapper()
    {
        if (m_handle != traits::DefaultHandle)
        {
            traits::Close(m_handle);
        }
    }

    operator HandleType() { return m_handle; }
    HandleWrapper& operator =(HandleType value)
    {
        DBG_ASSERT(m_handle == traits::DefaultHandle);
        m_handle = value;
        return *this;
    }

    HandleType* operator&() { return &m_handle; }

private:
    HandleType m_handle;
};
