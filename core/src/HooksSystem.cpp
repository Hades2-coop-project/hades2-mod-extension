//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "pch.h"

#include "HooksSystem.h"

#include "ModApi.h"
#include "hooks/LoadBufferHook.h"
#include "hooks/LocalizationHook.h"

#include <HookTable.h>

static HooksSystem* gHooksInstance;

std::function<void()> HooksSystem::LuaLoadCb{};

HooksSystem::HooksSystem() {
    gModApi.gameVariant = eGameVariant::STEAM;

    if (!m_symLoader.Initialize()) {
        throw std::exception("Cannot initialize symbol loader");
    }
    
    HANDLE moduleHandle = GetModuleHandleA("..\\Hades2.exe");

    if (!m_symLoader.LoadModuleSymbols(moduleHandle, "..\\Hades2.exe")) {
        throw std::exception("Cannot load Hades2.exe symbols");
    }

    auto find = m_symLoader.GetSymbolAddress("lua_pcallk");

    Hooks::LoadBufferHook::Install(m_symLoader, m_symLoader.GetSymbolAddress("luaL_loadbufferx"), [](const char *fileName) {
        if (LuaLoadCb && std::string_view(fileName) == "Main") {
            LuaLoadCb();
        }
    });

    //Hooks::LocalizationHook::Install(m_symLoader);
}

HooksSystem *HooksSystem::Instance() {
    if (!gHooksInstance) {
        gHooksInstance = new HooksSystem();
        HookTable::Instance().Init([](const char *path) { return gHooksInstance->m_symLoader.GetSymbolAddress(path); });
    }

    return gHooksInstance;
}
