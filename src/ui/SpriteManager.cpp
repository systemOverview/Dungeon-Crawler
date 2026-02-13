#include "SpriteManager.h"
#include <QtCore/qdebug.h>
#include <QtGui/qbitmap.h>

void SpriteManager::ProcessImage(QImage& image, ImageProcessingMode imageProcessingMode) {
    switch (imageProcessingMode) {
    case ImageProcessingMode::None:
        return;
    case ImageProcessingMode::TrimTransparent:
        TrimTransparent(image);
        break;
    }
}

void SpriteManager::TrimTransparent(QImage& image) {
    QRect opaqueRect = QRegion(QBitmap::fromImage(image.createAlphaMask())).boundingRect();
    image = image.copy(opaqueRect);
}

QImage SpriteManager::GetImageFromSprite(WhichSprite which, int id) {
    QString path(WEARABLES_SPRITE_PATH_BASE.at(which) + QString::number(id) + ".png");
    qDebug() << path;
    QImage sprite(path);
    QImage image = sprite.copy({0, 0, 32, 64});
    return image;
}

std::vector<QImage> SpriteManager::GetAllImagesAtIDFromSprite(
    WhichSprite which, int id, ImageProcessingMode imageProcessingMode) {
    int counter = 0;
    std::vector<QImage> images;
    while (true) {
        QImage image = GetImageFromSprite(which, counter++);
        if (image.isNull()) return images;
        ProcessImage(image, imageProcessingMode);
        images.push_back(image);
    }
}

std::vector<QImage> SpriteManager::GetSpriteVariantsAtIdle(WhichSprite which,
                                                           ImageProcessingMode imageProcessingMode) {
    int counter = 0;
    std::vector<QImage> images;
    while (true) {
        QImage image = GetImageFromSprite(which, counter++);
        if (image.isNull()) return images;
        ProcessImage(image, imageProcessingMode);
        images.push_back(image);
    }
}
