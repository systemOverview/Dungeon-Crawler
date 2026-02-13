cmake_minimum_required(VERSION 3.19)
project(DungeonCrawler LANGUAGES CXX)
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(nlohmann_json 3.12.0 REQUIRED)
find_package(Boost REQUIRED)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

qt_add_executable(DungeonCrawler WIN32 MACOSX_BUNDLE
    src/ui/Constants.cpp
    src/ui/WearablesNavigator.h src/ui/WearablesNavigator.cpp
    src/ui/assets/characters/human/base/0.png src/ui/assets/characters/human/base/1.png src/ui/assets/characters/human/base/2.png src/ui/assets/characters/human/head/0.png src/ui/assets/characters/human/head/1.png src/ui/assets/characters/human/head/2.png src/ui/assets/characters/human/head/3.png src/ui/assets/characters/human/head/4.png src/ui/assets/characters/human/head/5.png src/ui/assets/characters/human/head/6.png src/ui/assets/characters/human/head/7.png src/ui/assets/characters/human/outfit/0.png src/ui/assets/characters/human/outfit/1.png src/ui/assets/characters/human/outfit/2.png src/ui/assets/characters/human/outfit/3.png
    src/ui/SpriteManager.h src/ui/SpriteManager.cpp
   )

add_subdirectory(src)

target_link_libraries(DungeonCrawler
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::Multimedia
)
target_link_libraries(DungeonCrawler PRIVATE Qt6::Widgets)
target_link_libraries(DungeonCrawler PUBLIC nlohmann_json::nlohmann_json)


include(GNUInstallDirs)

install(TARGETS DungeonCrawler
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET DungeonCrawler
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
target_include_directories(DungeonCrawler PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


