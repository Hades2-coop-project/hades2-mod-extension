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

using path = std::filesystem::path;

path UseHadesExeCWD() {
    wchar_t modulePath[MAX_PATH];
    GetModuleFileNameW(NULL, modulePath, MAX_PATH);
    path h2ExePath{modulePath};
    auto h2ExeDirPath = h2ExePath.parent_path();
    SetCurrentDirectoryW(h2ExeDirPath.generic_wstring().c_str());
    return h2ExeDirPath;
}

static BOOL APIENTRY DllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH: {
        try {
            // The library is loaded from ASI loader
            // So CWD is this library folder
            auto libraryDir = std::filesystem::current_path();

            // It's safe to change CWD at this moment
            const path gameExePath = UseHadesExeCWD();

            HooksSystem::Instance();
            HookedMenuScreen::VTBL.Initialize();
            LibraryComponents::Initialize();

            auto modManager = ModManager::Instance();

            modManager->AddSearchPath(gameExePath.parent_path() / "Content" / "Mods");
            if (libraryDir.generic_string().ends_with("TheNormalnijMods-Hades2ModExtension")) {
                modManager->AddSearchPath(libraryDir.parent_path());
                modManager->SetCoreModContentPath(libraryDir);
            } else {
                modManager->SetCoreModContentPath(path{"../Mods/TN_Core"});
            }
            modManager->Init();
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
