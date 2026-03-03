find_package(Qt6 REQUIRED COMPONENTS Widgets)
target_sources(DungeonCrawler PRIVATE
    CharacterAnimation.h CharacterAnimation.cpp
    FightAnimation.h FightAnimation.cpp
    LoopingAnimation.h LoopingAnimation.cpp
    MoveAnimation.h MoveAnimation.cpp

)

target_include_directories(DungeonCrawler PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
