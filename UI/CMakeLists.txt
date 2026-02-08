target_sources(prak PRIVATE
    startscreen.h startscreen.cpp
    GraphicalUI.h GraphicalUI.cpp
    MainWindow.h MainWindow.cpp
    AbstractUI.h AbstractUI.cpp
    startscreen.ui MainWindow.ui
    Constants.h
)
target_include_directories(prak PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
