#include "SpriteManager.h"
#include <QtCore/qbuffer.h>
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

QString SpriteManager::GetFrameCacheKey(Character::CharacterType characterType,
                                        CharacterItem::CharacterPart whichPart,
                                        int whichGraphicsOption,
                                        int whichFrameId) {
    int frameCacheKey = (int(characterType) * 1000) + (int(whichPart) * 100)
                        + (whichGraphicsOption * 10) + (whichFrameId);
    return QString::number(frameCacheKey);
}

// The hierarchy of images in this static class is :
// Character Type (Human, Goblin, ...) -> CharacterPart(which part do you need a frame for? head, outfit..?) ->
// -> GraphicsOption (each part has multiple graphics options, for the human its choosable by the user), -> frameId
// (Animations happen through frames, all parts and graphics have the same frame id for each movement).

QImage SpriteManager::GetFrameImageFromSprite(Character::CharacterType characterType,
                                              CharacterItem::CharacterPart whichPart,
                                              int whichGraphicsOption,
                                              int whichFrameId) {
    // no need for caching on this as these frames are only requested once in the mainwindow at start.

    if (ID_OF_LAST_FRAME_IN_ROW == 0 || ID_OF_LAST_FRAME_IN_COL == 0) {
        return QImage();
    }

    const int row = (whichFrameId)
                    % ID_OF_LAST_FRAME_IN_ROW; // id = 10, 9 images per row, row = 1 by narrowing.
    const int col = float(whichFrameId) / ID_OF_LAST_FRAME_IN_COL;
    QString spritePath = SPRITE_PATH_BASE.at(characterType).at(whichPart);
    QString path(spritePath + QString::number(whichGraphicsOption) + ".png");
    QImage sprite(path);

    QImage image = sprite.copy({row * FRAME_WIDTH, col * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT});
    return image;
}

QPixmap SpriteManager::GetFrameFromSprite(Character::CharacterType characterType,
                                          CharacterItem::CharacterPart whichPart,
                                          int whichGraphicsOption,
                                          int whichFrameId) {
    QPixmap toReturn;

    if (ID_OF_LAST_FRAME_IN_ROW == 0 || ID_OF_LAST_FRAME_IN_COL == 0) {
        return toReturn; // sprite sheet is empty, or a mistake with parameters, return empty pixmap
    }
    QString frameCacheKey = GetFrameCacheKey(characterType,
                                             whichPart,
                                             whichGraphicsOption,
                                             whichFrameId);

    if (QPixmapCache::find(frameCacheKey, &toReturn)) {
        return toReturn; // if QPixmapCache finds the key, it modifies toReturn with the cached image, and returns true.
    }
    // if the condition above returned false, the pixmap doesnt exist in the cache, its the first time its being requested

    // A sprite that has 10 rows and 8 columns (10,8) would have frames numbered from 0 to 79. ID_OF_LAST_FRAME_IN_ROW would
    // be equal to 7, and ID_OF_LAST_FRAME_IN_COL = 9. To get the last frame in the first row (frame id = 7), we need to
    // get its x and y coords. x = position on row (first=0) * imagewidth. We add 1 to ID_OF_LAST_FRAME_IN_ROW, otherwise
    // 7 % ID_OF_LAST_FRAME_IN_ROW (=7) would equal 0, and we would get the first frame instead. Same reason for y.

    const int x = (whichFrameId % (ID_OF_LAST_FRAME_IN_ROW + 1)) * FRAME_WIDTH;
    const int y = int(float(whichFrameId)) / (ID_OF_LAST_FRAME_IN_COL + 1) * FRAME_HEIGHT;

    QString spritePath;
    try {
        spritePath = SPRITE_PATH_BASE.at(characterType).at(whichPart);
    } catch (std::out_of_range) { // character does not have a sprite for the requested part
        return toReturn;
    }
    QString path(spritePath + QString::number(whichGraphicsOption) + ".png");

    QImage sprite(path);

    QImage image = sprite.copy({x, y, FRAME_WIDTH, FRAME_HEIGHT});
    if (image.isNull()) {
        return toReturn;
    }

    toReturn = QPixmap::fromImage(image);

    QPixmapCache::insert(frameCacheKey, toReturn);
    return toReturn;
}

std::vector<QPixmap> SpriteManager::GetIdleFrameVariants(Character::CharacterType characterType,
                                                         CharacterItem::CharacterPart which,
                                                         ImageProcessingMode imageProcessingMode) {
    int counter = 0;
    std::vector<QPixmap> images;
    while (true) {
        QImage image = GetFrameImageFromSprite(characterType, which, counter++, 0);
        if (image.isNull()) return images;
        ProcessImage(image, imageProcessingMode);
        images.push_back(QPixmap::fromImage(image));
    }
}
