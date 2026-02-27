target_sources(DungeonCrawler PRIVATE
    Constants.h
    GraphicalUI.h GraphicalUI.cpp
    MainWindow.h MainWindow.cpp
    startscreen.h startscreen.cpp
    assets/textures.qrc


)
target_include_directories(DungeonCrawler PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(custom-widgets)
