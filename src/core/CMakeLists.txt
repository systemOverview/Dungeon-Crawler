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
    List.tpp
    StationaryController.h StationaryController.cpp
    GuardController.h GuardController.cpp
    LevelGraph.h LevelGraph.cpp
    AttackController.h AttackController.cpp
    JsonObjects.cpp
    JsonObjects.h
    AbstractController.cpp AbstractController.h
    )
target_include_directories(DungeonCrawler PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
