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

static std::unordered_map<std::string, std::vector<std::string>> localizations;

static FunctionHook<"sgg::GameDataManager::ReadTextData", void> hook{};

using ReadTextData_params_t = void(__fastcall *)(void *textStorage, const char *filePath, eastl::string *localization);
static ReadTextData_params_t ReadTextData{};

static void *sTextData{};
static sgg::HashGuid *Lang{};

void Hooks::LocalizationHook::Install(SymbolLoader &symLoader) {
    hook.Install(reinterpret_cast<void *>(symLoader.GetSymbolAddress("?ReadTextData@GameDataManager@sgg@@SAXXZ")), 16);

    ReadTextData = reinterpret_cast<ReadTextData_params_t>(symLoader.GetSymbolAddress(
        "?Read@TextData@sgg@@SA_NAEAV?$string_hash_map@PEAVTextData@sgg@@U?$hash@PEBD@eastl@@U"
        "?$str_equal_to@PEBD@4@Vallocator_forge@4@@eastl@@PEBDAEBV?$basic_string@DVallocator_forge@eastl@@@4@@Z"));

    sTextData = reinterpret_cast<void *>(symLoader.GetSymbolAddress("sgg::GameDataManager::sTextData"));
    Lang = reinterpret_cast<sgg::HashGuid *>(symLoader.GetSymbolAddress("sgg::Localization::Lang"));

    hook.onPostFunction = []() {
        const char *cStrLang = Lang->c_str();
        eastl::string langStr{cStrLang};


        for (const auto &data : localizations) {
            LibraryComponents::Instance()->GetContentSelecter().SetPathAbsolute(sgg::fs::ResourceDirectory::CALCULATES_TEXT, data.first.c_str());
            for (const auto &path : data.second) {
                const std::string dataPath = path + "." + cStrLang + ".sjson";
                ReadTextData(sTextData, dataPath.c_str(), &langStr);
            }
        }

        LibraryComponents::Instance()->GetContentSelecter().SetPath(sgg::fs::ResourceDirectory::CALCULATES_TEXT,
                                                                    "../Content/Game/Text");
    };
}

void Hooks::LocalizationHook::AddLocalizationPath(std::string_view rootPath, std::string_view path) {
    auto currrentStorage = localizations.find(std::string(rootPath));
    if (currrentStorage == localizations.end()) {
        localizations.emplace(std::pair{std::string(rootPath), std::vector<std::string>(1, std::string(path))});
    } else {
        currrentStorage->second.emplace_back(path);
    }
}
