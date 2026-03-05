find_package(Qt6 REQUIRED COMPONENTS Widgets)
target_sources(DungeonCrawler PRIVATE
    CharacterItem.cpp CharacterItem.h
    TileItem.h TileItem.cpp
    CustomLevelCreator.h CustomLevelCreator.cpp
    GameItem.h GameItem.cpp
    ButtonItem.h ButtonItem.cpp
)

target_include_directories(DungeonCrawler PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
