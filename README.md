# Hades II script API extension

# Building

## Using CMake for Windows x64

```powershell
cmake -A x64 . -B build_msvc
cmake --build build_msvc --target install --config Release
```

Binary files are located in the `bin` folder.

# Installation

## With Hell2Modding

1. Download and install [Hell2Modding](https://thunderstore.io/c/hades-ii/p/Hell2Modding/Hell2Modding/)
2. Download the latest release of this mod from the [Releases](https://github.com/Hades2-coop-project/hades2-mod-extension/releases) page.
3. Copy HadesModNativeExtension.asi in the `Hades II/Ship/plugins` folder. Create the folder if it doesn't exist.

## With ASI Loader

This method wotks with debug builds and doesn't require Hell2Modding.

1. In the `Hades II/Ship`, `Hades II/Debug`, or `Hades II/Release` folder, rename `bink2w64.dll` to `bink2w64Hooked.dll`.
2. Download bink2w64.dll with [ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases) and place it in the same folder.
3. Copy HadesModNativeExtension.asi in the `Hades II/<build type>/plugins` folder. Create the folder if it doesn't exist.