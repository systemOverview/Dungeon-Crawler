target_sources(DungeonCrawler PRIVATE
    GameBoardView.h GameBoardView.cpp
    CharacterTile_UI_PlacementMediator.h CharacterTile_UI_PlacementMediator.cpp

    )


target_include_directories(DungeonCrawler PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
