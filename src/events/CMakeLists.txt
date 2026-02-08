target_sources(DungeonCrawler PRIVATE
    Event.h Event.cpp
    EventBus.h EventBus.cpp
)
target_include_directories(DungeonCrawler PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
