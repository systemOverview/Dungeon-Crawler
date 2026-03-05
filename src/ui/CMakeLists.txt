target_sources(DungeonCrawler PRIVATE
    MainWindow.h MainWindow.cpp
    MainWindow.ui
    SpriteManager.h SpriteManager.cpp
    SquareGridLayout.h SquareGridLayout.cpp
    MainWindowController.h MainWindowController.cpp

    assets/textures.qrc
)
add_subdirectory(custom-items)
add_subdirectory(animations)
add_subdirectory(Game)




target_include_directories(DungeonCrawler PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
