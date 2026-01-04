# PROJEKT BULÁNCI

## Tento projekt je napodobenina hry Bulánci s použítím SDL2, projekt byl vypracován do předmětu Úvod do Programování

# Návod jak hru přeložit

## Požadavky(pro linux):

- Linux operační systém
- nainstalovaný cmake (sudo apt install cmake)
- nainstalovaná SDL2 knihovna (sudo apt install libsdl2-2.0-0, pro víc informací wiki.libsdl.org/SDL2/installation)

## Návod(pro linux):

- ./PROJEKT_BULANCI
- mkdir build
- cd ./build
- cmake -S ../src/ -B .
- make
- ./Bulanci

## Požadavky(pro windows):

- Windows operační systém
- nainstalovaný cmake
- nainstalovaný vcpkg
    - git clone https://github.com/microsoft/vcpkg.git
    - cd vcpkg
    - bootstrap-vcpkg.bat
- nainstalované SDL knihovny
    - vcpkg install sdl2 libjpeg-turbo sdl2-image[libjpeg-turbo] sdl2-ttf
    - vcpkg integrate install

## Návod(pro windows):

- ./PROJEKT_BULANCI
- mkdir build
- cd .\build
- cmake -S ..\src -B . -DCMAKE_TOOLCHAIN_FILE=C:\Users\lukas_t5auslz\vcpkg\scripts\buildsystems\vcpkg.cmake       !!! VAŠE CESTA SE MŮŽE LIŠIT
- cmake --build . --config Release 
- .\Release\Bulanci.exe
