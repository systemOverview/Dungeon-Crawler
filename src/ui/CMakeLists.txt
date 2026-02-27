target_sources(DungeonCrawler PRIVATE
    GraphicalUI.h GraphicalUI.cpp
    MainWindow.h MainWindow.cpp
    AbstractUI.h AbstractUI.cpp
    Constants.h
    MainWindow.ui
    assets/textures.qrc
)
add_subdirectory(custom-widgets)
add_subdirectory(custom-items)
add_subdirectory(animations)



target_include_directories(DungeonCrawler PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
