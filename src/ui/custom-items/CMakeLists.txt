find_package(Qt6 REQUIRED COMPONENTS Widgets)
target_sources(DungeonCrawler PRIVATE
    CharacterItem.cpp CharacterItem.h
    ButtonItem.cpp ButtonItem.h
)

target_include_directories(DungeonCrawler PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
