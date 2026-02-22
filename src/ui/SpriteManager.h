#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#include <QMap>
#include <QtGui/qimage.h>
#include <QtGui/qpixmapcache.h>
#include "CharacterItem.h"
#include "Constants.h"
#include <map>
class SpriteManager
{
public:
    enum class ImageProcessingMode { None, TrimTransparent };
    struct SpriteFrame
    {
        QPixmap pixmap;
        QPointF offset;
    };

private:
    // Each part has a frame register that caches images that were already requested
    //(moving/walking/idle frames are requested thousands of times over the game).
    // so QMap maps a part to another map, which key is the graphics option id (user-selected)
    // and the id of the frame in the sprite.
    CharacterItem::CharacterType m_type;

    using PartFramesCacheRegister = QMap<std::pair<int, int>, QPixmap>;
    // inline static QMap<CharacterItem::CharacterPart, PartFramesCacheRegister> FRAMES_CACHE = {};
    // inline static QMap<CharacterItem::CharacterType,QMap<CharacterItem::CharacterPart, PartFramesCacheRegister>> ALL
    inline static constexpr int IMAGE_WIDTH = 32;
    inline static constexpr int IMAGE_HEIGHT = 64;
    inline static constexpr int ID_OF_LAST_FRAME_IN_ROW
        = 9; //count from 0 better than having to do math in frame getter functions
    inline static constexpr int ID_OF_LAST_FRAME_IN_COL = 9;

    // *** START OF PATH BASE MAPS ***//
    inline static std::unordered_map<CharacterItem::CharacterPart, QString> HUMAN_SPRITE_PATH_BASE
        = {{CharacterItem::CharacterPart::Base, ":/characters/human/base/"},
           {CharacterItem::CharacterPart::Head, ":/characters/human/head/"},
           {CharacterItem::CharacterPart::Outfit, ":/characters/human/outfit/"},
           {CharacterItem::CharacterPart::Weapon, ""}};
    inline static std::unordered_map<CharacterItem::CharacterPart, QString> GOBLIN_SPRITE_PATH_BASE
        = {
                   {CharacterItem::CharacterPart::Base, "://characters/goblin/base/"},
    };

    inline static std::map<CharacterItem::CharacterType,
                           std::unordered_map<CharacterItem::CharacterPart, QString>>
        SPRITE_PATH_BASE = {
                   // Maps each character type to its path base map,
                   // convenience map to retrieve with at() instead of a switch case
                   {CharacterItem::CharacterType::Human, HUMAN_SPRITE_PATH_BASE},
                   {CharacterItem::CharacterType::Goblin, GOBLIN_SPRITE_PATH_BASE},

    };
    // *** END OF PATH BASE MAPS ***//

    // Public functions return QPixmaps only as Qt recommends using QPixmap for displaying.
    // The private helper functions need a QImage for image processing.
    static QImage GetFrameImageFromSprite(CharacterItem::CharacterType characterType,
                                          CharacterItem::CharacterPart whichPart,
                                          int whichGraphicsOption,
                                          int whichFrameId);
    static void ProcessImage(QImage& image, ImageProcessingMode imageProcessingMode);
    static QString GetFrameCacheKey(CharacterItem::CharacterType characterType,
                                    CharacterItem::CharacterPart whichPart,
                                    int whichGraphicsOption,
                                    int whichFrameId);

public:
    static void TrimTransparent(QImage& image);
    static QPixmap GetFrameFromSprite(CharacterItem::CharacterType characterType,
                                      CharacterItem::CharacterPart whichPart,
                                      int whichGraphicsOption,
                                      int whichFrameId);

    static std::vector<QPixmap> GetIdleFrameVariants(
        CharacterItem::CharacterType characterType,
        // Base/head/weapons etc have different sprites for different styles, but they are all the same.
        // you can only have one at time. For character customization, the main window asks for the default
        // variants of each to present them to the user.
        CharacterItem::CharacterPart which,
        ImageProcessingMode imageProcessingMode = ImageProcessingMode::None);
    SpriteManager();
};

#endif // SPRITEMANAGER_H
