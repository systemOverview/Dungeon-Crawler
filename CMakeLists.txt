# cmake_minimum_required(VERSION 3.16)

# include(GNUInstallDirs)
# install(TARGETS prak
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

# cmake_minimum_required(VERSION 3.19)
# project(prak LANGUAGES CXX)

# find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

# qt_standard_project_setup()


# project(prak LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# qt_add_executable(prak WIN32 MACOSX_BUNDLE

#         main.cpp
#         tile.cpp
#         tile.h
#         Character.cpp
#         Character.h
#         header.h
#         DungeonCrawler.cpp
#         DungeonCrawler.h
#         Level.cpp
#         Level.h
#         TerminalUI.cpp
#         TerminalUI.h
#         AbstractUI.cpp
#         AbstractUI.h
#         PassiveAktive.cpp
#         PassiveAktive.h
#         startscreen.h startscreen.cpp startscreen.ui)


# # qt_add_executable(prak
# #     WIN32 MACOSX_BUNDLE
# #     main.cpp
# #     startscreen.cpp
# #     startscreen.h
# #     startscreen.ui
# # )

# target_link_libraries(prak
#     PRIVATE
#         Qt::Core
#         Qt::Widgets
# )

# include(GNUInstallDirs)

# install(TARGETS prak
#     BUNDLE  DESTINATION .
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
# )

# qt_generate_deploy_app_script(
#     TARGET prak
#     OUTPUT_SCRIPT deploy_script
#     NO_UNSUPPORTED_PLATFORM_ERROR
# )
# install(SCRIPT ${deploy_script})

cmake_minimum_required(VERSION 3.19)
project(prak LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)


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

    )


target_link_libraries(prak
    PRIVATE
        Qt::Core
        Qt::Widgets
)

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
