#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#include <QMap>
#include <QtGui/qimage.h>
#include "Constants.h"
#include <map>
class SpriteManager
{
public:
    enum class ImageProcessingMode { None, TrimTransparent };

private:
    // Each part has a frame register that caches images that were already requested
    //(moving/walking/idle frames are requested thousands of times over the game).
    // so QMap maps a part to another map, which key is the graphics option id (user-selected)
    // and the id of the frame in the sprite.
    using PartFramesCacheRegister = QMap<std::pair<int, int>, QPixmap>;
    inline static QMap<CharacterGraphics::CharacterPart, PartFramesCacheRegister> FRAMES_CACHE = {};

    inline static constexpr int IMAGE_WIDTH = 32;
    inline static constexpr int IMAGE_HEIGHT = 64;
    inline static constexpr int ID_OF_LAST_FRAME_IN_ROW
        = 9; //count from 0 better than having to do math in frame getter functions
    inline static constexpr int ID_OF_LAST_FRAME_IN_COL = 9;

    inline static std::map<CharacterGraphics::CharacterPart, QString> WEARABLES_SPRITE_PATH_BASE
        = {{CharacterGraphics::CharacterPart::Base, ":/characters/human/base/"},
           {CharacterGraphics::CharacterPart::Head, ":/characters/human/head/"},
           {CharacterGraphics::CharacterPart::Outfit, ":/characters/human/outfit/"},
           {CharacterGraphics::CharacterPart::Weapon, ""}};

    // Public functions return QPixmaps only as Qt recommends using QPixmap for displaying.
    // The private helper functions need a QImage for image processing.
    static QImage GetFrameImageFromSprite(CharacterGraphics::CharacterPart whichPart,
                                          int whichGraphicsOption,
                                          int whichFrameId);
    static void ProcessImage(QImage& image, ImageProcessingMode imageProcessingMode);
    static void TrimTransparent(QImage& image);

public:
    static QPixmap GetFrameFromSprite(CharacterGraphics::CharacterPart whichPart,
                                      int whichGraphicsOption,
                                      int whichFrameId);

    static std::vector<QPixmap> GetIdleFrameVariants(
        // Base/head/weapons etc have different sprites for different styles, but they are all the same.
        // you can only have one at time. For character customization, the main window asks for the default
        // variants of each to present them to the user.
        CharacterGraphics::CharacterPart which,
        ImageProcessingMode imageProcessingMode = ImageProcessingMode::None);

    SpriteManager();
};

#endif // SPRITEMANAGER_H
