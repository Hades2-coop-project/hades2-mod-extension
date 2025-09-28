//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "pch.h"

#include "Assert.h"
#include "HooksSystem.h"

void Assert::Handle(const char* condition, const char* message, const char* file, int line) {
    reinterpret_cast<bool (__fastcall*)(const char *, const char *, const char *, int)>(HooksSystem::Instance()->GetHoohTable().HandleAssert)(
        condition, message, file, line);
}