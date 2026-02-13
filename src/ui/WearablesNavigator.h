#ifndef GRAPHICSNAVIGATOR_H
#define GRAPHICSNAVIGATOR_H

#include <QtGui/qimage.h>
#include "Constants.h"

class GraphicsNavigator
{
public:
private:
    QString m_texturePathBase;
    int m_currentTextureIndex = 0;
    QString getFullTexturePath(int texutreIndex);

public:
    GraphicsNavigator(CharacterWearables::WearableType);
    QImage begin();
    QImage getNextTexture();
    QImage getPrevTexture();
    QImage outOfRangeTexture();
};

#endif // GRAPHICSNAVIGATOR_H
#ifndef TEXTURESNAVIGATOR_H
#define TEXTURESNAVIGATOR_H

#endif // TEXTURESNAVIGATOR_H
