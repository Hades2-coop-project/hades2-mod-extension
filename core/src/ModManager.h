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
    void AddSearchPath(std::filesystem::path path) { m_modSearchPaths.emplace_back(std::move(path)); }

    void Init();
    void Deinit();

    static ModManager *Instance();

  private:
    void LoadModInFolder(const std::filesystem::path &modDir);
    void OnLuaCreated();
    void ScanMods();
    void SortMods();

  private:
    std::vector<std::filesystem::path> m_modSearchPaths;
    std::vector<std::unique_ptr<Mod>> m_Mods;
};
