#pragma once
#include "ui/graphicsView/regionDef.h"
#include <QString>
#include <QImage>

QString saveImageToSysDir(const QImage &p_image,
                          QString p_dirName = QString(),
                          bool p_outHint = true,
                          QList<TIGER_Graphics::CGraphicsRegion> p_drawRegions = QList<TIGER_Graphics::CGraphicsRegion>(),
                          QRectF p_roi = QRectF());
