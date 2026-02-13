#ifndef WEARABLESNAVIGATOR_H
#define WEARABLESNAVIGATOR_H

#include <QtGui/qimage.h>
#include "Constants.h"
#include "SpriteManager.h"

class WearablesNavigator
{
public:
private:
    QString m_texturePathBase;
    int m_currentTextureIndex = 0;
    QString getFullTexturePath(int texutreIndex);
    SpriteManager::WhichSprite m_spriteType;

public:
    WearablesNavigator(SpriteManager::WhichSprite whichSprite);
    QImage begin();
    QImage getNextTexture();
    QImage getPrevTexture();
    QImage outOfRangeTexture();
};

#endif // WEARABLESNAVIGATOR_H
#ifndef TEXTURESNAVIGATOR_H
#define TEXTURESNAVIGATOR_H

#endif // TEXTURESNAVIGATOR_H
