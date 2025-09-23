#pragma once
#include "template/shapeTemplate/shapeResDef.h"
#include <QObject>
#include <QMap>

namespace TIGER_Graphics
{
    enum CBooleanType : int32_t
    {
        UnitedBoolean = 0,
        IntersectedBoolean,
        SubtractedBoolean
    };
    const QMap<int, QString> cBooleanStrMap =
        {
            {UnitedBoolean, QObject::tr("United")},
            {IntersectedBoolean, QObject::tr("Intersected")},
            {SubtractedBoolean, QObject::tr("Subtracted")}};
    const QMap<int, QString> cBooleanIconMap =
        {
            {UnitedBoolean, cShapeBooleanUnionIcon},
            {IntersectedBoolean, cShapeBooleanInterIcon},
            {SubtractedBoolean, cShapeBooleanSubIcon}};
};