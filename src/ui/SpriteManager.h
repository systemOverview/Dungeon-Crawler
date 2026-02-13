#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#include <QtGui/qimage.h>
#include "Constants.h"
#include <map>
class SpriteManager
{
    inline static constexpr int IMAGE_WIDTH = 32;
    inline static constexpr int IMAGE_HEIGHT = 64;

public:
    enum class ImageProcessingMode { None, TrimTransparent };
    inline static std::map<CharacterGraphics::CharacterPart, QString> WEARABLES_SPRITE_PATH_BASE
        = {{CharacterGraphics::CharacterPart::Base, ":/characters/human/base/"},
           {CharacterGraphics::CharacterPart::Head, ":/characters/human/head/"},
           {CharacterGraphics::CharacterPart::Outfit, ":/characters/human/outfit/"},
           {CharacterGraphics::CharacterPart::Weapon, ""}};
    static void ProcessImage(QImage& image, ImageProcessingMode imageProcessingMode);
    static void TrimTransparent(QImage& image);

public:
    // armors/robes/weapons/base
    // getImageFromSprite(type, id)
    //
    static QImage GetImageFromSprite(CharacterGraphics::CharacterPart which, int id = 0);
    static std::vector<QImage> GetAllImagesAtIDFromSprite(
        CharacterGraphics::CharacterPart which,
        int id = 0,
        ImageProcessingMode imageProcessingMode = ImageProcessingMode::None);

    static std::vector<QImage> GetSpriteVariantsAtIdle(
        // Base/head/weapons etc have different sprites for different styles, but they are all the same.
        // you can only have one at time. For character customization, the main window asks for the default
        // variants of each to present them to the user.
        CharacterGraphics::CharacterPart which,
        ImageProcessingMode imageProcessingMode = ImageProcessingMode::None);

    SpriteManager();
};

#endif // SPRITEMANAGER_H
