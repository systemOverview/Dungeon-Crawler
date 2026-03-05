add_subdirectory(Characters)
add_subdirectory(tiles)

target_include_directories(DungeonCrawler PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
