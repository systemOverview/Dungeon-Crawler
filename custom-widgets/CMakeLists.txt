find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(nlohmann_json 3.12.0 REQUIRED)
target_sources(prak PRIVATE
        QCharacter.h QCharacter.cpp
        QTile.h QTile.cpp
        QGameField.h QGameField.cpp
        QOverLay.h QOverLay.cpp
        QGraphMatrix.h QGraphMatrix.cpp
        QTypeWriter.h QTypeWriter.cpp
        QGameOver.h QGameOver.cpp
        QTerminal.h QTerminal.cpp
        QGameWon.h QGameWon.cpp
)

target_include_directories(prak PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR})
