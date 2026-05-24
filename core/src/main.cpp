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
#include <iostream>

void UseHadesExeCWD() {
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(NULL, path, MAX_PATH);
    std::filesystem::path h2ExePath{path};
    auto h2ExeDirPath = h2ExePath.parent_path();
    std::cout << h2ExeDirPath << std::endl;
    SetCurrentDirectoryW(h2ExeDirPath.generic_wstring().c_str());
}

static BOOL APIENTRY DllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH: {
        try {
            UseHadesExeCWD();
            HooksSystem::Instance();
            HookedMenuScreen::VTBL.Initialize();
            LibraryComponents::Initialize();
            ModManager::Instance()->Init();
        } catch (std::exception &ex) {
             std::cout << "Cannot load Hades2ModExtension: " << ex.what();
        }
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
