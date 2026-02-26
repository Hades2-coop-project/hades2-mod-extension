//
// Copyright (c) Uladzislau Nikalayevich <thenormalnij@gmail.com>. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#pragma once

#include <hades2/MenuScreen.h>

class HookedMenuScreen : public sgg::MenuScreen {
  public:
    struct HookedMenuScreenVtbl {
        void(__fastcall *OnExit)(struct HookedMenuScreen *self){};
        void(__fastcall *LoadContent)(struct HookedMenuScreen *self){};
        void(__fastcall *UnloadContent)(struct HookedMenuScreen *self){};
        void(__fastcall *Update)(struct HookedMenuScreen *self, float,
                                 const eastl::vector<sgg::InputHandler *, eastl::allocator_forge> *){};
        bool(__fastcall *HandleInput)(struct HookedMenuScreen *self, sgg::InputHandler *, float){};
        void(__fastcall *HandleNoInput)(struct HookedMenuScreen *self){};
        void(__fastcall *Draw)(struct HookedMenuScreen *self, float){};
        void(__fastcall *ExitScreen)(struct HookedMenuScreen *self){};
        void(__fastcall *Clear)(struct HookedMenuScreen *self){};
        void(__fastcall *Init)(struct HookedMenuScreen *self, sgg::ScreenManager *){};
        sgg::ScreenType(__fastcall *GetType)(struct HookedMenuScreen *self) {};
        void(__fastcall *Desctuctor)(struct HookedMenuScreen *self){};
        bool(__fastcall *FreeFormSelect)(HookedMenuScreen *self, sgg::InputHandler *, float){};
        void(__fastcall *OnCancel)(HookedMenuScreen *self){};
        bool(__fastcall *WasClickPressed)(HookedMenuScreen *self, sgg::InputHandler *){};
        void(__fastcall *SelectComponent)(HookedMenuScreen *self, sgg::GUIComponent *, sgg::InputHandler *){};
        bool(__fastcall *NotifyComponentsOfClick)(HookedMenuScreen *self, Vectormath::Vector2){};
        bool(__fastcall *ValidateCursorLocation)(HookedMenuScreen *self, sgg::InputHandler *){};
        void(__fastcall *PostLoad)(HookedMenuScreen *self){};
        void(__fastcall *OnLanguageChanged)(HookedMenuScreen *self){};
        void(__fastcall *OnResolutionChanged)(HookedMenuScreen *self, float, float){};
        void(__fastcall *RestoreDefaults)(HookedMenuScreen *self){};
        void(__fastcall *Load)(HookedMenuScreen *self, const char *){};
        sgg::GUIComponent *(__fastcall *GetFirstUseableComponent)(HookedMenuScreen *self){};
        bool(__fastcall *TryExitScreen)(HookedMenuScreen *self){};
        void(__fastcall *Scroll)(HookedMenuScreen *self, float, bool){};


        void Initialize();
        void Initialize(const HookedMenuScreenVtbl* parent);
    };

    friend HookedMenuScreenVtbl;

    public:
    void IheritedConstrictor(sgg::ScreenManager *screenManager) {
        GET_HOOK(MenuScreen_constructor_ScreenManager,
                 sgg::MenuScreen * (__fastcall *)(sgg::MenuScreen *, sgg::ScreenManager *))(this, screenManager);
        SetVTBL(&VTBL);
    };

    protected:
    void SetVTBL(HookedMenuScreenVtbl *vtbl) { *reinterpret_cast<HookedMenuScreenVtbl **>(this) = vtbl; }
  public:
    static HookedMenuScreenVtbl VTBL;
};
