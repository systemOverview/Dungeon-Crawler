target_sources(DungeonCrawler PRIVATE
    Utilities.h Utilities.cpp
    RandomColor.h RandomColor.cpp
)

target_include_directories(DungeonCrawler PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
