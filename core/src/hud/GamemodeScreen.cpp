//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "pch.h"

#include "GamemodeScreen.h"
#include <EASTL-forge1.51/string.h>
#include "HookTable.h"
#include "GamemodeManager.h"
#include "HookedClassFactory.h"

// TODO: Move this to Lua

void GamemodeScreen::IheritedConstrictor(sgg::ScreenManager* screenManager) {
    HookedMenuScreen::IheritedConstrictor(screenManager);
    mBlockLowerInput = true;
    mOverPauseMenu = true;
    CreateComponents();
}

void GamemodeScreen::CreateComponents() {
    CreateBack(0.8);
    eastl::string textureName{""};
    CreateBackground(&textureName);
    CreateTitleText(this);
    CreateCancelButton(this);

    mCancelButton->GetActivateAction().AddCallBack([this]() { ExitScreen(); });

    auto *buttonTemplate = HookedClassFactory::Create<sgg::GUIComponentButton, sgg::MenuScreen *>(this);
    buttonTemplate->SetParent(this);
    AddComponent(buttonTemplate);
    GetReflectionHelper().ReflectComponent("mSettingsButton", buttonTemplate);

    Load("GUI/SettingsMenuScreen.sjson");
    buttonTemplate->SetLocation({0.0f, 0.0f});
    buttonTemplate->SetScale(0.0f);

    float offsetY = 0.0f;

    for (auto &gamemode : GamemodeManager::Instance().GetGamemodes()) {
        auto *gamemodeBtn = HookedClassFactory::Create<sgg::GUIComponentButton, sgg::MenuScreen *>(this);
        gamemodeBtn->Load(&buttonTemplate->GetComponentData());

        gamemodeBtn->SetLocation({960.0f, 420.0f + offsetY});
        gamemodeBtn->SetParent(this);
        gamemodeBtn->SetText(gamemode.GetName().c_str());
        AddComponent(gamemodeBtn);

        gamemodeBtn->SetFont(sgg::HashGuid::StringIntern("MainMenuP22UndergroundSCHeavy"));
        gamemodeBtn->SetTextColor(0xFFCCCCCC);

        offsetY += 75.0f;

        gamemodeBtn->GetActivateAction().AddCallBack([&gamemode, this]() {
            //ExitScreen();
            gamemode.Activate();
        });
    }

    auto &componentDef = mTitleText->GetComponentData().GetDef();
    componentDef.mHelpTextId = sgg::HashGuid::StringIntern("MainMenuScreen_PlayGame");

    mTitleText->UseDefaultText();
}
