#include "GraphicsNavigator.h"
#include <QtCore/qdebug.h>

QString GraphicsNavigator::getFullTexturePath(int texutreIndex) {
    return m_texturePathBase + QString::fromStdString(std::to_string(texutreIndex) + ".png");
}

GraphicsNavigator::GraphicsNavigator(CharacterWearables::WearableType textureType)
    : m_texturePathBase{CharacterWearables::WEARABLES_CUT_PATH_BASE.at(textureType)} {}

QImage GraphicsNavigator::getNextTexture() {
    QString path = getFullTexturePath(m_currentTextureIndex++);
    QImage texture;
    bool check = texture.load(path);

    if (check)
        return texture;
    else
        return (outOfRangeTexture());
}

QImage GraphicsNavigator::getPrevTexture() {}

QImage GraphicsNavigator::outOfRangeTexture() {
    return QImage(":/res/textures/res/outofrange.png");
}
