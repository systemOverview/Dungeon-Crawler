target_sources(prak PRIVATE
    startscreen.h startscreen.cpp
    GraphicalUI.h GraphicalUI.cpp
    MainWindow.h MainWindow.cpp
    startscreen.ui MainWindow.ui
)
target_include_directories(prak PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
