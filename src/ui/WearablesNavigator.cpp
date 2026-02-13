#include "WearablesNavigator.h"
#include <QBitmap>
#include <QtCore/qdebug.h>

QString WearablesNavigator::getFullTexturePath(int texutreIndex) {
    return m_texturePathBase + QString::fromStdString(std::to_string(texutreIndex) + ".png");
}

WearablesNavigator::WearablesNavigator(SpriteManager::WhichSprite whichSprite)
    : m_spriteType{whichSprite} {}

QImage WearablesNavigator::getNextTexture() {
    QImage image = SpriteManager::GetImageFromSprite(m_spriteType, m_currentTextureIndex++);
    if (image.isNull()) {
        return (outOfRangeTexture());
    }
    else {
        QRect opaqueRect = QRegion(QBitmap::fromImage(image.createAlphaMask())).boundingRect();
        image = image.copy(opaqueRect);
        return image;
    }
}

QImage WearablesNavigator::getPrevTexture() {}

QImage WearablesNavigator::outOfRangeTexture() {
    return QImage(":/res/textures/res/outofrange.png");
}
