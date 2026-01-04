cmake_minimum_required(VERSION 3.15)
project(Bulanci C)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)

add_executable(Bulanci
    main.c
    engine.c
    ui.c
    player.c
    sceneManager.c
    weapon.c
    collisionMap.c
    weaponPickup.c
)

target_link_libraries(Bulanci PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
)

add_custom_command(TARGET Bulanci POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:Bulanci>/assets
)