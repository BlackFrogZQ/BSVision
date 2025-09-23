#pragma once
#pragma once
#include "template/shapeTemplate/shapeResDef.h"
#include <QObject>
#include <QMap>

namespace TIGER_Graphics
{
    enum CPosToolType : int32_t
    {
        TemplateCenterPosTool = 0,
        RectCenterPosTool,
        EllipseCenterPosTool,
        TwoCircleCenterPosTool,
        LineCenterPosTool,
        RightAnglePosTool
    };
    const QMap<int, QString> cPosToolStrMap =
        {
            {TemplateCenterPosTool, QObject::tr("Template Center")},
            {RectCenterPosTool, QObject::tr("Rect Center")},
            {EllipseCenterPosTool, QObject::tr("Ellipse Center")},
            {TwoCircleCenterPosTool, QObject::tr("Two Circle Center")},
            {LineCenterPosTool, QObject::tr("Line Center")},
            {RightAnglePosTool, QObject::tr("Right Angle")}
        };
    const QMap<int, QString> cPosToolIconMap =
        {
            {TemplateCenterPosTool, cPosToolTemplateCenterIcon},
            {RectCenterPosTool, cPosToolRectCenterIcon},
            {EllipseCenterPosTool, cPosToolEllipseCenterIcon},
            {TwoCircleCenterPosTool, cPosToolTwoCircleCenterIcon},
            {LineCenterPosTool, cPosToolLineCenterIcon},
            {RightAnglePosTool, cPosToolRightAngleCenterIcon}};
};