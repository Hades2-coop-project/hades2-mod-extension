//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "pch.h"

#include "HooksSystem.h"

#include "ModApi.h"
#include "hooks/LoadBufferHook.h"
#include "hooks/LocalizationHook.h"

static HooksSystem *gHooksInstance;

std::function<void()> HooksSystem::LuaLoadCb{};

HooksSystem::HooksSystem() {
    gModApi.gameVariant = eGameVariant::STEAM;

    m_GameMainHandle = LoadLibraryA("Hades2.exe");

    m_symLoader.Initialize();
    m_symLoader.LoadModuleSymbols(m_GameMainHandle, "Hades2.exe");

    InitHookTable();

    Hooks::LoadBufferHook::Install(m_symLoader, m_HookTable.luaL_loadbufferx, [](const wchar_t *fileName) {
        if (LuaLoadCb && std::wstring_view(fileName).ends_with(L"Content\\Scripts\\Main.lua")) {
            LuaLoadCb();
        }
    });

    Hooks::LocalizationHook::Install(m_symLoader);
}

void HooksSystem::InitHookTable() {
    m_HookTable.luaState = m_symLoader.GetSymbolAddress("sgg::ScriptManager::LUA_INTERFACE");
    m_HookTable.lua_pcallk = m_symLoader.GetSymbolAddress("lua_pcallk");
    m_HookTable.luaL_loadbufferx = m_symLoader.GetSymbolAddress("luaL_loadbufferx");
    m_HookTable.HandleAssert = m_symLoader.GetSymbolAddress("sgg__HandleAssert");
}

HooksSystem *HooksSystem::Instance() {
    if (!gHooksInstance)
        gHooksInstance = new HooksSystem();

    return gHooksInstance;
}
