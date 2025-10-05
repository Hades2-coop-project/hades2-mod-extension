//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <functional>
#include "SymbolLoader.h"

class HooksSystem {
  public:
    void SetLuaLoadCallback(std::function<void()> callback) { LuaLoadCb = callback; };
    void *GetGameDllHandle() const noexcept { return m_GameMainHandle; };
    SymbolLoader &GetSymbolLoader() { return m_symLoader; };
    static HooksSystem *Instance();

  private:
    HooksSystem();

  private:
    static std::function<void()> LuaLoadCb;
    HANDLE m_GameMainHandle{};
    SymbolLoader m_symLoader{};
};
