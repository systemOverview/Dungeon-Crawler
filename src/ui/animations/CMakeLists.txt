find_package(Qt6 REQUIRED COMPONENTS Widgets)
target_sources(DungeonCrawler PRIVATE
    CharacterAnimation.h CharacterAnimation.cpp
)

target_include_directories(DungeonCrawler PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
