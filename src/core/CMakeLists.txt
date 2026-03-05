target_sources(DungeonCrawler PRIVATE
    DungeonCrawler.cpp
    DungeonCrawler.h
    Level.cpp
    Level.h
    PassiveAktive.cpp
    PassiveAktive.h
    JsonObjects.cpp
    JsonObjects.h
    )
add_subdirectory(Game)
target_include_directories(DungeonCrawler PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
