target_sources(${PROJECT_NAME} PRIVATE
    screen_gameplay.cpp
    screen_logo.c
    screen_title.c
    raylib_game.c
    ecs/Entity.cpp
    ecs/World.cpp
    ecs/PhysicsSystem.cpp
    ecs/PlayerSystem.cpp
    ecs/RenderSystem.cpp
    ecs/CurrencySystem.cpp
    ecs/WorldGenSystem.cpp
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/ecs/Entity.hpp>
)

set_source_files_properties(
    screen_logo.c
    screen_title.c
    raylib_game.c
    PROPERTIES SKIP_PRECOMPILE_HEADERS ON
)