
add_subdirectory(attack-controller)
add_subdirectory(human-controller)
add_subdirectory(guard-controller)
add_subdirectory(abstract-controller)
add_subdirectory(stationary-controller)

target_include_directories(DungeonCrawler PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
