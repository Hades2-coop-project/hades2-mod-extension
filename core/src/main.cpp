//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "pch.h"
#include <windows.h>
#include "ModManager.h"
#include "hud/HookedMenuScreen.h"
#include "LibraryComponents.h"
#include "HooksSystem.h"

static BOOL APIENTRY DllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH: {
        HooksSystem::Instance();
        HookedMenuScreen::VTBL.Initialize();
        LibraryComponents::Initialize();
        ModManager::Instance()->Init();
        break;
    }
    case DLL_THREAD_ATTACH: {
        break;
    }
    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        ModManager::Instance()->Deinit();
        break;
    }

    return TRUE;
}
