//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include "FunctionHook.h"
#include <hades2/MainMenuScreen.h>

class MainMenuHandler {
  public:
    MainMenuHandler();
    ~MainMenuHandler() = default;

    void AddGamemode(const char *name) { gamemodes.push_back(name); }
    void InitializeCustomButtons();
    sgg::MainMenuScreen *GetMainMenu() const noexcept { return mainMenuScreen; };

private:
    FunctionHook<"sgg::MainMenuScreen::MainMenuScreen", sgg::MainMenuScreen *, sgg::MainMenuScreen *, void *, void *>
        funHook;

    sgg::MainMenuScreen *mainMenuScreen = nullptr;

    std::vector<std::string> gamemodes;
};
