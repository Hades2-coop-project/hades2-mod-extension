//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "pch.h"

#include <filesystem>
#include "ModManager.h"
#include "HooksSystem.h"
#include "lua/LuaManager.h"
#include <HookedLua.h>
#include "hooks/LocalizationHook.h"
#include "GamemodeManager.h"

static ModManager gModCore{}; 

void ModManager::Init() {
    HooksSystem::Instance()->SetLuaLoadCallback([this]() { this->OnLuaCreated(); });
    ScanMods();
}

void ModManager::Deinit() {

}

ModManager *ModManager::Instance() { return &gModCore; }

void ModManager::OnLuaCreated() {
    // Reset gamemode list and wait next MainMenuAPIAddGamemode
    GamemodeManager::Instance().Reset();

    LuaManager::Reinit();
    auto luaState = HookedLua::GetLuaState();

    for (auto &mod : m_Mods) {
        mod->OnLuaCreated(luaState);
    }
}

void ModManager::ScanMods() {
    for (const auto& searchPath : m_ModSearchPaths) {
        LoadModInFolder(searchPath);
    }
    SortMods();

    for (auto &mod : m_Mods) {
        mod->Start();
    }
}

void ModManager::LoadModInFolder(const std::filesystem::path &modsPath) {
    namespace fs = std::filesystem;

    const auto modsPathAsString = modsPath.string();
    for (auto &modDir : fs::directory_iterator(modsPath)) {
        if (!fs::is_directory(modDir))
            continue;

        auto mod = std::make_unique<Mod>(modDir);

        if (mod->Load()) {
            for (const auto &localPath : mod->GetLocalization()) {
                const fs::path fullPath = modDir.path().filename() / localPath;
                Hooks::LocalizationHook::AddLocalizationPath(modsPathAsString, std::move(fullPath.string()));
            }
            m_Mods.emplace_back(std::move(mod));
        }
    }
}

void ModManager::SortMods() {
    std::sort(m_Mods.begin(), m_Mods.end(), [](const std::unique_ptr<Mod> &a, const std::unique_ptr<Mod> &b) {
        return a->GetPriority() > b->GetPriority();
    });
}
