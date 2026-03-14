//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <hades2/Resources.h>

constexpr size_t MAX_PATHS = 0x1D;

class ContentPathSelector {
  public:
    ContentPathSelector() { m_OriginalDirs.resize(MAX_PATHS); }

    void SetPath(sgg::fs::ResourceDirectory dirType, const char *path);
    void ResetPath(sgg::fs::ResourceDirectory dirType);

private:
    std::vector<std::string> m_OriginalDirs{};
};