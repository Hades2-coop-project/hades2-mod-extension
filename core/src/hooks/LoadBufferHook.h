//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <functional>
#include "SymbolLoader.h"

namespace Hooks::LoadBufferHook {
void Install(SymbolLoader &symLoader, uintptr_t luaL_loadbufferxPos, std::function<void(const char *)> handler);
}
