//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <vector>
#include <memory>
#include "Mod.h"

class ModManager {
  public:
    void AddSearchPath(std::filesystem::path path) { m_ModSearchPaths.emplace_back(std::move(path)); }
    void SetCoreModContentPath(const std::filesystem::path &path) { m_CoreContentPath = path.string(); }
    const std::string& GetThisModResourcesDir() const noexcept { return m_CoreContentPath; }

    void Init();
    void Deinit();
    Mod& GetModByName(const char* name) const noexcept;
    Mod& GetThisCoreMod() const noexcept;

    static ModManager *Instance();

  private:
    void LoadModInFolder(const std::filesystem::path &modDir);
    void OnLuaCreated();
    void ScanMods();
    void SortMods();

  private:
    std::vector<std::filesystem::path> m_ModSearchPaths;
    std::vector<std::unique_ptr<Mod>> m_Mods;
    std::string m_CoreContentPath;
};
