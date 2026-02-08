target_sources(prak PRIVATE
    Event.h Event.cpp
    EventBus.h EventBus.cpp
)
target_include_directories(prak PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
