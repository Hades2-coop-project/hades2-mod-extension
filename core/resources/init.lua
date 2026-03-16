
-- Register default gamemode
MainMenuAPIAddGamemode("Default", function()
    SetTempRuntimeData("Gamemode", nil)
    MainMenuOpenProfiles()
end)
