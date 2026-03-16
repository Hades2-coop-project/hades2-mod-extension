//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "pch.h"

#include "lua/helpers/LuaCallback.h"
#include <lua.hpp>
#include <hookedLua.h>

constexpr char const *szUniqueCallbackStorage = "_ScriptExAPICB";

LuaCallback::LuaCallback(lua_State *L, int stackIndex) {
    // Ensure the callback storage table exists in the registry
    HookedLua::luaL_getsubtable(L, LUA_REGISTRYINDEX, szUniqueCallbackStorage);

    // Get the next available index in the callback storage table
    int nextIndex = lua_rawlen(L, -1) + 1;

    // Push the value at stackIndex onto the callback storage table
    HookedLua::lua_pushvalue(L, stackIndex);
    HookedLua::lua_rawseti(L, -2, nextIndex);

    // Save the index for later use
    callbackIndex = nextIndex;

    // Remove the callback storage table from the stack
    lua_pop(L, 1);
}

void LuaCallback::PushFunction(lua_State* L) const {
    HookedLua::luaL_getsubtable(L, LUA_REGISTRYINDEX, szUniqueCallbackStorage);

    lua_rawgeti(L, -1, callbackIndex);

    lua_remove(L, -2);
}
