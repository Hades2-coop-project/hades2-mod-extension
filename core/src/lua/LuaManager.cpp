//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "pch.h"
#include "LuaManager.h"
#include "lua/defs/ModLuaDefs.h"
#include "lua/defs/TempValuesLuaDefs.h"
#include "lua/defs/LuaGamemodeFunctionDefs.h"
#include "lua/defs/LuaGUIComponentButtonFunctionDefs.h"
#include "lua/defs/LuaGUIComponentFunctionDefs.h"
#include "lua/defs/LuaGUIComponentTextBoxFunctionDefs.h"
#include "lua/defs/LuaMainMenuScreenFunctionDefs.h"
#include "lua/defs/LuaMenuScreenDefs.h"
#include <fstream>
#include <filesystem>

#include <HookTable.h>
#include <lua.hpp>
#include <hookedLua.h>

namespace fs = std::filesystem;

static fs::path luaModsPath = "";

void LuaManager::Reinit() {
    LuaManager::InitScriptPath();
    lua_State *luaState = HookedLua::GetLuaState();
    ModLuaDefs::Load(luaState);
    TempValuesLuaDefs::Load(luaState);
    LuaGamemodeFunctionDefs::Load(luaState);
    LuaGUIComponentFunctionDefs::Load(luaState);
    LuaGUIComponentButtonFunctionDefs::Load(luaState);
    LuaGUIComponentTextBoxFunctionDefs::Load(luaState);
    LuaMainMenuScreenFunctionDefs::Load(luaState);
    LuaMenuScreenFunctionDefs::Load(luaState);
}

void LuaManager::InitScriptPath() {
    if (luaModsPath.empty()) {
        if (fs::exists("Hades2.exe")) {
            luaModsPath = fs::current_path().parent_path();
        } else {
            luaModsPath = fs::current_path();
        }
    }
}

bool LuaManager::DoScriptFile(const std::filesystem::path &path) {
    if (!LoadScriptFile(path))
        return false;

    return HookedLua::lua_pcallk(HookedLua::GetLuaState(), 0, 0, 0, 0, 0) == LUA_OK;
}

bool LuaManager::LoadScriptFile(const std::filesystem::path &path) {
    std::ifstream file{path, std::ifstream::ate};
    if (!file.is_open())
        return false;

    std::vector<char> buffer{};
    buffer.resize(file.tellg());
    file.seekg(0);
    file.read(buffer.data(), buffer.size());
    file.close();

    auto luaName = "@" + path.lexically_relative(luaModsPath).lexically_normal().string();

    int fileLoadStatus = HookedLua::luaL_loadbufferx(HookedLua::GetLuaState(), buffer.data(), buffer.size(), luaName.c_str(), nullptr);

    return fileLoadStatus == LUA_OK;
}

std::filesystem::path LuaManager::GetAbsoluteScriptPath(const std::filesystem::path &path) {
    return luaModsPath / path;
}
