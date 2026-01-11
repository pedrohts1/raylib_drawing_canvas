cmake_minimum_required(VERSION 3.27)
project(drawing_canvas)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

# --- 1. Adicionando Raylib ---
message(STATUS "Baixando Raylib...")
FetchContent_Declare(raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG        master
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(raylib)

# --- 2. Adicionando Raygui ---
message(STATUS "Baixando Raygui...")
FetchContent_Declare(raygui
        GIT_REPOSITORY https://github.com/raysan5/raygui.git
        GIT_TAG        master
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
)
# Nota: Raygui geralmente é header-only, mas usamos MakeAvailable para preencher as variáveis de caminho
FetchContent_MakeAvailable(raygui)

add_executable(drawing_canvas main.cpp
        utils.hpp
        shape.hpp
        board.hpp
        shape.cpp
        board.cpp
        game_ui.hpp
        game_ui.cpp)

# --- 3. Configurando os Includes ---
# Agora as variáveis ${raylib_SOURCE_DIR} e ${raygui_SOURCE_DIR} funcionam
# porque não estão presas dentro de uma função.

target_include_directories(drawing_canvas PRIVATE
        ${raylib_INCLUDE_DIRS}      # Include padrão da Raylib
        ${raygui_SOURCE_DIR}/src    # Include da Raygui (pasta src onde está o .h)
)

target_link_libraries(drawing_canvas PRIVATE raylib)