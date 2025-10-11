//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "pch.h"

#include "LoadBufferHook.h"

#include "LuaManager.h"
#include "Mem.h"

static std::function<void(const char *)> hookKandler = nullptr;

static uint64_t __fastcall loadBufferHook(void *rdx, const char *buffer, size_t size, const char *name,
                                          const char *mode) {
    std::string newName{"@Content/Scripts/"};
    newName = newName + name + ".lua";

    // TODO move all to this
    hookKandler(name);

    return LuaManager::luaL_loadbufferx(LuaManager::GetLuaState(), buffer, size, newName.c_str(), mode);
}

static void Path(void* pos) {
    uint8_t path[] = {0x48, 0xB9, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xCC, 0xBB, 0xAA, // mov rcx, address
                      0xFF, 0xD1};                                                // call rcx

    auto *address = reinterpret_cast<uintptr_t *>(&path[2]);
    *address = reinterpret_cast<uintptr_t>(&loadBufferHook);

    Mem::MemCpyUnsafe(pos, (void *)path, sizeof(path));
}

// Searchs "call luaL_loadbuffer" in sgg::ScriptManager::Load
// And returns the first argument position
static void *SearchPathPos(SymbolLoader &symLoader, uintptr_t luaL_loadbufferxPos) {
    // This is lenght of the sgg::ScriptManager::Load function
    constexpr uint64_t searchSize = 0x85E;
    constexpr uint8_t CALL_BYTE = 0xE8;
    constexpr uint8_t FIRST_ARGUMENT_OFFSET = 7;
    // We have Lua::DoBytes in the debug version, and "sgg::ScriptManager::Load in the release
    uintptr_t searchPos = symLoader.GetSymbolAddress("?DoBytes@Lua@@QEAAHPEBE_KPEBD@Z");

    if (searchPos)
        return reinterpret_cast<void*>(searchPos + 0x64 - FIRST_ARGUMENT_OFFSET);
    
    searchPos = symLoader.GetSymbolAddress("sgg::ScriptManager::Load");

    if (!searchPos)
        return nullptr;

    const uintptr_t endPos = searchPos + searchSize;
    while (searchPos <= endPos) {
        uint32_t loadBufferOffset = luaL_loadbufferxPos - searchPos - 0x5;

        if (*reinterpret_cast<uint8_t *>(searchPos) == CALL_BYTE &&
            *reinterpret_cast<uint32_t *>(searchPos + 1) == loadBufferOffset) {
            return reinterpret_cast<void *>(searchPos - FIRST_ARGUMENT_OFFSET);
        }

        searchPos++;
    }
    
    return nullptr;
}

void Hooks::LoadBufferHook::Install(SymbolLoader &symLoader, uintptr_t luaL_loadbufferxPos,
                                    std::function<void(const char *)> handler) {
    hookKandler = handler;
    auto pos = SearchPathPos(symLoader, luaL_loadbufferxPos);
    if (pos) {
        Path(pos);
    } else {
        throw std::exception("LoadBufferHook is not installed");
    }
}
