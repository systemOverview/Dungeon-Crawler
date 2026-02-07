find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(nlohmann_json 3.12.0 REQUIRED)
add_library(custom-widgets STATIC
        QCharacter.h QCharacter.cpp
        QTile.h QTile.cpp
        QGameField.h QGameField.cpp
        QOverLay.h QOverLay.cpp
        QGraphMatrix.h QGraphMatrix.cpp
        QDjikstraPreviousRegister.h QDjikstraPreviousRegister.cpp
        QTypeWriter.h QTypeWriter.cpp
        QGameOver.h QGameOver.cpp
        QTerminal.h QTerminal.cpp
        QGameWon.h QGameWon.cpp


)

target_link_libraries(custom-widgets
        PUBLIC
        Qt::Core
        Qt::Widgets
)
target_link_libraries(custom-widgets PRIVATE Qt6::Widgets)
target_link_libraries(custom-widgets PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(custom-widgets PRIVATE Qt6::Widgets)
target_link_libraries(custom-widgets PRIVATE Qt6::Widgets)
target_link_libraries(custom-widgets PRIVATE Qt6::Widgets)

target_include_directories(custom-widgets
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)
