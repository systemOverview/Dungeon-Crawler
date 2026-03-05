target_sources(DungeonCrawler PRIVATE
    GameController.h GameController.cpp

    )

add_subdirectory(GameBoard)
target_include_directories(DungeonCrawler PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
