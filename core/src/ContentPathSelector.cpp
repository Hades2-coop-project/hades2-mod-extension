//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "pch.h"

#include "ContentPathSelector.h"
#include <hades2/Resources.h>

void ContentPathSelector::SetPath(sgg::fs::ResourceDirectory dirType, const char *path) {
    const size_t pos = static_cast<size_t>(dirType);
    sgg::fs::ResourceDirectoryInfo* originalData = sgg::fs::GetResourceDir(dirType);
    if (m_OriginalDirs[pos].empty()) {
        m_OriginalDirs[pos] = std::string{originalData->path};
    }

    // Invalidate old data
    originalData->path[0] = 0;

    sgg::fs::fsSetPathForResourceDir(originalData->pIO, sgg::fs::ResourceMount::RM_CONTENT, dirType, path);
}

// doesn't work
void ContentPathSelector::ResetPath(sgg::fs::ResourceDirectory dirType) {
    const size_t pos = static_cast<size_t>(dirType);
    if (m_OriginalDirs[pos].empty()) {
        return;
    }

    SetPath(dirType, m_OriginalDirs[pos].c_str());
}
