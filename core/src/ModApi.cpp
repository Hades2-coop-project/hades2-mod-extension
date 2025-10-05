//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "include/HadesModApi.h"
#include "HooksSystem.h"
#include "HookTable.h"

static void *GetGameHandle() { return HooksSystem::Instance()->GetGameDllHandle(); }

static uint64_t GetSymbolAddress(const char *symbolName) {
    return HooksSystem::Instance()->GetSymbolLoader().GetSymbolAddress(symbolName);
}

static bool LoadDllSymbols(void *handle, const char *dllName) {
    return HooksSystem::Instance()->GetSymbolLoader().LoadModuleSymbols(handle, dllName);
}

static HookTable* GetHookTable() {
    return &HookTable::Instance(); }

IModApi gModApi{.version{MOD_API_VERSION},
                .gameVariant{eGameVariant::UNKNOWN},
                .GetGameHandle = GetGameHandle,
                .GetSymbolAddress = GetSymbolAddress,
                .LoadDllSymbols = LoadDllSymbols,
                .GetHookTable = GetHookTable};
