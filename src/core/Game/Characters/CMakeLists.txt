target_sources(DungeonCrawler PRIVATE
    Character.cpp Character.h
    )
add_subdirectory(Controllers)
target_include_directories(DungeonCrawler PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
