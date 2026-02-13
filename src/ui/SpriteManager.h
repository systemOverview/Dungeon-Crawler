#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#include <QtGui/qimage.h>
#include <map>
class SpriteManager
{
    inline static constexpr int IMAGE_WIDTH = 32;
    inline static constexpr int IMAGE_HEIGHT = 64;

public:
    enum class WhichSprite { Base, Head, Outfit, Weapon, PAST_END };
    enum class ImageProcessingMode { None, TrimTransparent };
    inline static std::map<WhichSprite, QString> WEARABLES_SPRITE_PATH_BASE
        = {{WhichSprite::Base, ":/characters/human/base/"},
           {WhichSprite::Head, ":/characters/human/head/"},
           {WhichSprite::Outfit, ":/characters/human/outfit/"},
           {WhichSprite::Weapon, ""}};
    static void ProcessImage(QImage& image, ImageProcessingMode imageProcessingMode);
    static void TrimTransparent(QImage& image);

public:
    // armors/robes/weapons/base
    // getImageFromSprite(type, id)
    //
    static QImage GetImageFromSprite(WhichSprite which, int id = 0);
    static std::vector<QImage> GetAllImagesAtIDFromSprite(
        WhichSprite which,
        int id = 0,
        ImageProcessingMode imageProcessingMode = ImageProcessingMode::None);
    static std::vector<QImage> GetSpriteVariantsAtIdle(
        // Base/head/weapons etc have different sprites for different styles, but they are all the same.
        // you can only have one at time. For character customization, the main window asks for the default
        // variants of each to present them to the user.
        WhichSprite which,
        ImageProcessingMode imageProcessingMode = ImageProcessingMode::None);

    SpriteManager();
};

#endif // SPRITEMANAGER_H
