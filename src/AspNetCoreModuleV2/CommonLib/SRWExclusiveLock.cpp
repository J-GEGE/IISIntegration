// Copyright (c) .NET Foundation. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

#include "stdafx.h"
#include "SRWExclusiveLock.h"

SRWExclusiveLock::  SRWExclusiveLock(const SRWLOCK& lock)
    : m_lock(lock)
{
    AcquireSRWLockExclusive(const_cast<SRWLOCK*>(&m_lock));
}

SRWExclusiveLock::~SRWExclusiveLock()
{
    ReleaseSRWLockExclusive(const_cast<SRWLOCK*>(&m_lock));
}
