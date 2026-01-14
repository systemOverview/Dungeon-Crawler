find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_library(custom-widgets STATIC
        QCharacter.h QCharacter.cpp
        QTile.h QTile.cpp
        QHealthBar.h QHealthBar.cpp
)

target_link_libraries(custom-widgets
        PUBLIC
        Qt::Core
        Qt::Widgets
)
target_link_libraries(custom-widgets PRIVATE Qt6::Widgets)

target_include_directories(custom-widgets
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)
