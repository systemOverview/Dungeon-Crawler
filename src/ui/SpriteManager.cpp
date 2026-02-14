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

QImage SpriteManager::GetFrameImageFromSprite(CharacterGraphics::CharacterPart whichPart,
                                              int whichGraphicsOption,
                                              int whichFrameId) {
    // no need for caching on this as these frames are only requested once in the mainwindow at start.
    if (ID_OF_LAST_FRAME_IN_ROW == 0 || ID_OF_LAST_FRAME_IN_COL == 0) {
        return QImage();
    }

    const int row = float(whichFrameId)
                    / ID_OF_LAST_FRAME_IN_ROW; // id = 10, 9 images per row, row = 1 by narrowing.
    const int col = float(whichFrameId) / ID_OF_LAST_FRAME_IN_COL;

    QString path(WEARABLES_SPRITE_PATH_BASE.at(whichPart) + QString::number(whichGraphicsOption)
                 + ".png");
    QImage sprite(path);

    QImage image = sprite.copy({row * IMAGE_WIDTH, col * IMAGE_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT});
    return image;
}

QPixmap SpriteManager::GetFrameFromSprite(CharacterGraphics::CharacterPart whichPart,
                                          int whichGraphicsOption,
                                          int whichFrameId) {
    if (FRAMES_CACHE[whichPart].contains({whichGraphicsOption, whichFrameId})) {
        return FRAMES_CACHE[whichPart][{whichGraphicsOption, whichFrameId}];
    }

    if (ID_OF_LAST_FRAME_IN_ROW == 0 || ID_OF_LAST_FRAME_IN_COL == 0) {
        return QPixmap();
    }

    const int row = (whichFrameId) % ID_OF_LAST_FRAME_IN_ROW;
    const int col = float(whichFrameId) / ID_OF_LAST_FRAME_IN_COL;
    QString path(WEARABLES_SPRITE_PATH_BASE.at(whichPart) + QString::number(whichGraphicsOption)
                 + ".png");
    QImage sprite(path);

    QImage image = sprite.copy({row * IMAGE_WIDTH, col * IMAGE_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT});
    QPixmap pixmap = QPixmap::fromImage(image);
    FRAMES_CACHE[whichPart].insert({whichGraphicsOption, whichFrameId}, pixmap);
    return pixmap;
}

std::vector<QPixmap> SpriteManager::GetIdleFrameVariants(CharacterGraphics::CharacterPart which,
                                                         ImageProcessingMode imageProcessingMode) {
    int counter = 0;
    std::vector<QPixmap> images;
    while (true) {
        QImage image = GetFrameImageFromSprite(which, counter++, 0);
        if (image.isNull()) return images;
        ProcessImage(image, imageProcessingMode);
        images.push_back(QPixmap::fromImage(image));
    }
}

