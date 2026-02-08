target_sources(DungeonCrawler PRIVATE
    tile.cpp
    tile.h
    Character.cpp
    Character.h
    DungeonCrawler.cpp
    DungeonCrawler.h
    Level.cpp
    Level.h
    PassiveAktive.cpp
    PassiveAktive.h
    fifo.cpp
    fifo.h
    list.cpp
    list.h
    StationaryController.h StationaryController.cpp
    GuardController.h GuardController.cpp
    LevelGraph.h LevelGraph.cpp
    AttackController.h AttackController.cpp
    JsonObjects.cpp
    JsonObjects.h
    )
target_include_directories(DungeonCrawler PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
