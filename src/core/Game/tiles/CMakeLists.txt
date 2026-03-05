target_sources(DungeonCrawler PRIVATE
    tile.h tile.cpp
    PortalsConnector.h PortalsConnector.cpp
)

target_include_directories(DungeonCrawler PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
