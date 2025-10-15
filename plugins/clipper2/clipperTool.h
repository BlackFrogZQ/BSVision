#pragma once
#include "clipper.h"
#include <QPainterPath>

namespace TIGER_ClipperTool
{
    static inline QPainterPath toQPath(const Clipper2Lib::PathsD &p_paths)
    {
        QPainterPath paths;
        for (const auto &path : p_paths)
        {
            QPolygonF polygon;
            for (auto p : path)
            {
                polygon.append(QPointF(p.x, p.y));
            }
            if (path.size() >= 3)
            {
                polygon.append(polygon.first());
            }
            paths.addPolygon(polygon);
        }
        return paths;
    }

    static inline Clipper2Lib::PathsD toPath(const QPainterPath &p_paths)
    {
        Clipper2Lib::PathsD paths;
        for (const auto &path : p_paths.toFillPolygons())
        {
            Clipper2Lib::PathD polygon;
            assert(path.size() > 0);
            for (auto p : path)
            {
                polygon.emplace_back(Clipper2Lib::PointD(p.x(), p.y()));
            }
            if (path.isClosed())
            {
                polygon.resize(polygon.size() - 1);
            }
            paths.emplace_back(polygon);
        }
        return paths;
    }

    static inline Clipper2Lib::PathsD Intersect(const Clipper2Lib::PathsD &p_src, const QPainterPath &p_paths, int decimal_prec = 2)
    {
        return Clipper2Lib::Intersect(p_src, toPath(p_paths), Clipper2Lib::FillRule::EvenOdd, decimal_prec);
    }
    static inline Clipper2Lib::PathsD Union(const Clipper2Lib::PathsD &p_src, const QPainterPath &p_paths, int decimal_prec = 2)
    {
        return Clipper2Lib::Union(p_src, toPath(p_paths), Clipper2Lib::FillRule::EvenOdd, decimal_prec);
    }
    static inline Clipper2Lib::PathsD Difference(const Clipper2Lib::PathsD &p_src, const QPainterPath &p_paths, int decimal_prec = 2)
    {
        return Clipper2Lib::Difference(p_src, toPath(p_paths), Clipper2Lib::FillRule::EvenOdd, decimal_prec);
    }
    static inline Clipper2Lib::PathsD SimplifyPaths(const Clipper2Lib::PathsD &p_src, double epsilon = 1.42)
    {
        Clipper2Lib::PathsD paths = Clipper2Lib::SimplifyPaths(p_src, epsilon);
        Clipper2Lib::PathsD result;
        for (const auto &path : paths)
        {
            if (path.size() >= 3)
            {
                result.emplace_back(path);
            }
        }
        return result;
    }
    static inline bool contains(const Clipper2Lib::PathsD &p_src, const QPointF& p_point)
    {
        int hits = 0;
        Clipper2Lib::PointD p(p_point.x(), p_point.y());
        for (const auto &path : p_src)
        {
            if (path.size() >= 3 && Clipper2Lib::PointInPolygon(p, path) == Clipper2Lib::PointInPolygonResult::IsInside)
            {
                hits += Clipper2Lib::IsPositive(path) ? 1 : -1;
            }
        }
        return hits > 0;
    }
}