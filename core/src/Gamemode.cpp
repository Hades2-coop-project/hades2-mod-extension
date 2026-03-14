//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "pch.h"

#include "Gamemode.h"

#include <HookedLua.h>
#include <lua.hpp>

void Gamemode::Activate() const {
    lua_State *L = HookedLua::GetLuaState();
    callback.PushFunction(L);
    lua_pushstring(L, name.c_str());
    HookedLua::lua_pcallk(L, 1, 0, 0, 0, 0);
}
