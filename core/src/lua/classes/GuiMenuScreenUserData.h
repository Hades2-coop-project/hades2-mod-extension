//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "lua/helpers/LuaClass.h"
#include "hud/HookedMenuScreen.h"

class GuiMenuScreenUserData : public LuaClassWrapper<HookedMenuScreen> {
  public:
    using Parent = void;
    static constexpr const char *LuaClassMeta = "MainMenuApi.MenuScreen";
    static constexpr const char *LuaClassName = "MenuScreen";
};
