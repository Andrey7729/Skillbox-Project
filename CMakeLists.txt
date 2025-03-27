cmake_minimum_required(VERSION 3.25)
project(search_engine1)

set(CMAKE_CXX_STANDARD 20)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(nlohmann_json)

# Указываем include-пути для библиотеки JSON
include_directories(${nlohmann_json_SOURCE_DIR}/include)

# Создаем библиотеку (только .cpp файлы)
add_library(search_engine_lib
        src/ConverterJson.cpp
        src/IvertedIndex.cpp
        src/SearchServer.cpp
)
# Добавляем include-пути для библиотеки
target_include_directories(search_engine_lib
        PRIVATE ${nlohmann_json_SOURCE_DIR}/include
)
# Создаем исполняемый файл
add_executable(search_engine_executable src/main.cpp)

# Связываем библиотеки
target_link_libraries(search_engine_executable PRIVATE
        search_engine_lib
        nlohmann_json::nlohmann_json
)
add_subdirectory(tests)