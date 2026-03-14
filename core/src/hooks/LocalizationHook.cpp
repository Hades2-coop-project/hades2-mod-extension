//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "pch.h"

#include "FunctionHook.h"
#include "LibraryComponents.h"
#include "LocalizationHook.h"
#include <EASTL-forge1.51/string.h>
#include <hades2/HashGuid.h>

static std::vector<std::string> localizations{};

static FunctionHook<"sgg::GameDataManager::ReadTextData", void> hook{};

using ReadTextData_params_t = void(__fastcall *)(const char *filePath, void *textStorage, eastl::string *localization);
static ReadTextData_params_t ReadTextData{};

static void *sTextData{};
static sgg::HashGuid *Lang{};

void Hooks::LocalizationHook::Install(SymbolLoader &symLoader) {
    hook.Install(reinterpret_cast<void *>(symLoader.GetSymbolAddress("?ReadTextData@GameDataManager@sgg@@SAXXZ")), 16);

    ReadTextData = reinterpret_cast<ReadTextData_params_t>(symLoader.GetSymbolAddress(
        "?ReadTextData@GameDataManager@sgg@@SAXPEBDAEAV?$string_hash_map@PEAVTextData@sgg@@U?$hash@PEBD@eastl@@U?$str_"
        "equal_to@PEBD@4@Vallocator_forge@4@@eastl@@AEBV?$basic_string@DVallocator_forge@eastl@@@4@@Z"));

    sTextData = reinterpret_cast<void *>(symLoader.GetSymbolAddress("sgg::GameDataManager::sTextData"));
    Lang = reinterpret_cast<sgg::HashGuid *>(symLoader.GetSymbolAddress("sgg::Localization::Lang"));

    hook.onPostFunction = []() {
        LibraryComponents::Instance()->GetContentSelecter().SetPath(sgg::fs::ResourceDirectory::CALCULATES_TEXT,
                                                                    "../Content/Mods");
        for (const auto &path : localizations) {
            eastl::string langStr{Lang->c_str()};
            ReadTextData(path.c_str(), sTextData, &langStr);
        }

        LibraryComponents::Instance()->GetContentSelecter().SetPath(sgg::fs::ResourceDirectory::CALCULATES_TEXT,
                                                                    "../Content/Game/Text");
    };
}

void Hooks::LocalizationHook::AddLocalizationPath(std::string path) { localizations.push_back(path); }
