//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "pch.h"

#include <filesystem>
#include "ModManager.h"
#include "HooksSystem.h"
#include "lua/ModLuaDefs.h"
#include "lua/LuaManager.h"
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
    auto luaState = LuaManager::GetLuaState();

    for (auto &mod : m_Mods) {
        mod->OnLuaCreated(luaState);
    }
}

void ModManager::ScanMods() {
    namespace fs = std::filesystem;
    using path = fs::path;

    auto gamePath = fs::current_path().parent_path().parent_path();
    auto modPath = gamePath / "Content" / "Mods";

    if (!fs::is_directory(modPath)) {
        fs::create_directory(modPath);
        return;
    }

    for (auto &modDir : fs::directory_iterator(modPath)) {
        if (!fs::is_directory(modDir))
            continue;
         
        auto mod = std::make_unique<Mod>(modDir);

        if (mod->Load()) {
            for (const auto &localPath : mod->GetLocalization()) {
                const path fullPath = modDir.path().filename() / localPath;
                Hooks::LocalizationHook::AddLocalizationPath(std::move(fullPath.string()));
            }
            m_Mods.emplace_back(std::move(mod));
        }
    }

    SortMods();

    for (auto &mod : m_Mods) {
        mod->Start();
    }
}

void ModManager::SortMods() {
    std::sort(m_Mods.begin(), m_Mods.end(), [](const std::unique_ptr<Mod> &a, const std::unique_ptr<Mod> &b) {
        return a->GetPriority() > b->GetPriority();
    });
}
