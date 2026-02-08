cmake_minimum_required(VERSION 3.19)
project(prak LANGUAGES CXX)
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(nlohmann_json 3.12.0 REQUIRED)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

# add_subdirectory(custom-widgets)
qt_add_executable(prak WIN32 MACOSX_BUNDLE

        main.cpp
        tile.cpp
        tile.h
        Character.cpp
        Character.h
        header.h
        DungeonCrawler.cpp
        DungeonCrawler.h
        Level.cpp
        Level.h
        TerminalUI.cpp
        TerminalUI.h
        AbstractUI.cpp
        AbstractUI.h
        PassiveAktive.cpp
        PassiveAktive.h
        startscreen.h startscreen.cpp startscreen.ui
        textures.qrc
        GraphicalUI.h GraphicalUI.cpp
        MainWindow.h MainWindow.cpp MainWindow.ui
        dialog.ui
        fifo.cpp
        fifo.h
        list.cpp
        list.h
        StationaryController.h StationaryController.cpp
        GuardController.h GuardController.cpp
        Observer.h Observer.cpp
        Subject.h Subject.cpp
        LevelGraph.h LevelGraph.cpp
        AttackController.h AttackController.cpp
        Utilities.h Utilities.cpp
        RandomColor.h RandomColor.cpp
        Constants.h
        gamedata/level.json
        JsonObjects.cpp
        JsonObjects.h
    )
add_subdirectory(events)
add_subdirectory(custom-widgets)
find_package(Boost REQUIRED)

target_link_libraries(prak
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::Multimedia
)
target_link_libraries(prak PRIVATE Qt6::Widgets)
# target_link_libraries(prak PRIVATE custom-widgets)
target_link_libraries(prak PUBLIC nlohmann_json::nlohmann_json)


include(GNUInstallDirs)

install(TARGETS prak
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET prak
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
target_include_directories(prak PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


