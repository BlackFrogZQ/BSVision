#pragma once
#include <ostream>
#include <limits>
#include <cmath>
#include <QDebug>
#include <QRectF>
template <class Number>
class CPoint2
{
public:
    Number x, y;
    CPoint2<Number>(const Number _x = std::numeric_limits<Number>::lowest(), const Number _y = std::numeric_limits<Number>::lowest()) : x(_x), y(_y) {}

    CPoint2<Number>(const CPoint2<Number> &p) : x(p.x), y(p.y) {}

    CPoint2<Number> operator+(const CPoint2<Number> &p) const
    {
        return CPoint2<Number>(x + p.x, y + p.y);
    }

    CPoint2<Number> operator-(const CPoint2<Number> &p) const
    {
        return CPoint2<Number>(x - p.x, y - p.y);
    }

    CPoint2<Number> operator-() const
    {
        return CPoint2<Number>(-x, -y);
    }

    CPoint2<Number> operator*(const CPoint2<Number> &p) const
    {
        return CPoint2<Number>(x * p.x, y * p.y);
    }

    CPoint2<Number> operator/(const CPoint2<Number> &p) const
    {
        return CPoint2<Number>(x / p.x, y / p.y);
    }

    CPoint2<Number> &operator+=(const CPoint2<Number> &p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }

    CPoint2<Number> &operator-=(const CPoint2<Number> &p)
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    CPoint2<Number> &operator*=(const CPoint2<Number> &p)
    {
        x *= p.x;
        y *= p.y;
        return *this;
    }

    CPoint2<Number> &operator/=(const CPoint2<Number> &p)
    {
        x /= p.x;
        y /= p.y;
        return *this;
    }
    //模平方
    Number vSize2() const
    {
        return x * x + y * y;
    }

    //模
    Number vSize() const
    {
        return std::sqrt(vSize2());
    }
    template <typename num_t, typename = typename std::enable_if<std::is_arithmetic<num_t>::value, num_t>::type>
    CPoint2<Number> operator*(const num_t i) const
    {
        return CPoint2<Number>(x * i, y * i);
    }

    template <typename num_t, typename = typename std::enable_if<std::is_arithmetic<num_t>::value, num_t>::type>
    CPoint2<Number> operator/(const num_t i) const
    {
        return CPoint2<Number>(x / i, y / i);
    }

    template <typename num_t, typename = typename std::enable_if<std::is_arithmetic<num_t>::value, num_t>::type>
    CPoint2<Number> &operator*=(const num_t i)
    {
        x *= i;
        y *= i;
        return *this;
    }

    template <typename num_t, typename = typename std::enable_if<std::is_arithmetic<num_t>::value, num_t>::type>
    CPoint2<Number> &operator/=(const num_t i)
    {
        x /= i;
        y /= i;
        return *this;
    }

    bool operator==(const CPoint2<Number> &p) const
    {
        return x == p.x && y == p.y;
    }

    bool operator!=(const CPoint2<Number> &p) const
    {
        return x != p.x || y != p.y;
    }
    friend QDebug operator<<(QDebug out, const CPoint2<Number> &info)
    {
        out << "CPoint2(" << info.x << "," << info.y << ")";
        return out;
    }
};
template <class Number>
inline uint qHash(const CPoint2<Number> &point, uint seed)
{
    return qHash(double(point.x), seed) + qHash(double(point.y), seed);
}

template <class Number>
class CPoint3
{
public:
    Number x, y, z;
    CPoint3<Number>(const Number _x = std::numeric_limits<Number>::lowest(), const Number _y = std::numeric_limits<Number>::lowest(), const Number _z = std::numeric_limits<Number>::lowest()) : x(_x), y(_y), z(_z) {}

    CPoint3<Number>(const CPoint3<Number> &p) : x(p.x), y(p.y), z(p.z) {}

    CPoint3<Number> operator+(const CPoint3<Number> &p) const
    {
        return CPoint3<Number>(x + p.x, y + p.y, z + p.z);
    }

    CPoint3<Number> operator-(const CPoint3<Number> &p) const
    {
        return CPoint3<Number>(x - p.x, y - p.y, z - p.z);
    }

    CPoint3<Number> operator-() const
    {
        return CPoint3<Number>(-x, -y, -z);
    }

    CPoint3<Number> operator*(const CPoint3<Number> &p) const
    {
        return CPoint3<Number>(x * p.x, y * p.y, z * p.z);
    }

    CPoint3<Number> operator/(const CPoint3<Number> &p) const
    {
        return CPoint3<Number>(x / p.x, y / p.y, z / p.z);
    }

    CPoint3<Number> &operator+=(const CPoint3<Number> &p)
    {
        x += p.x;
        y += p.y;
        z += p.z;
        return *this;
    }

    CPoint3<Number> &operator-=(const CPoint3<Number> &p)
    {
        x -= p.x;
        y -= p.y;
        z -= p.z;
        return *this;
    }

    CPoint3<Number> &operator*=(const CPoint3<Number> &p)
    {
        x *= p.x;
        y *= p.y;
        z *= p.z;
        return *this;
    }

    CPoint3<Number> &operator/=(const CPoint3<Number> &p)
    {
        x /= p.x;
        y /= p.y;
        z /= p.z;
        return *this;
    }
    //模平方
    Number vSize2() const
    {
        return x * x + y * y  + z * z;
    }

    //模
    Number vSize() const
    {
        return std::sqrt(vSize2());
    }
    template <typename num_t, typename = typename std::enable_if<std::is_arithmetic<num_t>::value, num_t>::type>
    CPoint3<Number> operator*(const num_t i) const
    {
        return CPoint3<Number>(x * i, y * i, z * i);
    }

    template <typename num_t, typename = typename std::enable_if<std::is_arithmetic<num_t>::value, num_t>::type>
    CPoint3<Number> operator/(const num_t i) const
    {
        return CPoint3<Number>(x / i, y / i, z / i);
    }

    template <typename num_t, typename = typename std::enable_if<std::is_arithmetic<num_t>::value, num_t>::type>
    CPoint3<Number> &operator*=(const num_t i)
    {
        x *= i;
        y *= i;
        z *= i;
        return *this;
    }

    template <typename num_t, typename = typename std::enable_if<std::is_arithmetic<num_t>::value, num_t>::type>
    CPoint3<Number> &operator/=(const num_t i)
    {
        x /= i;
        y /= i;
        z /= i;
        return *this;
    }

    bool operator==(const CPoint3<Number> &p) const
    {
        return x == p.x && y == p.y && z == p.z;
    }

    bool operator!=(const CPoint3<Number> &p) const
    {
        return x != p.x || y != p.y || z != p.z;
    }
    friend QDebug operator<<(QDebug out, const CPoint3<Number> &info)
    {
        out << "CPoint3(" << info.x << "," << info.y << "," << info.z << ")";
        return out;
    }
};
template <class Number>
inline uint qHash(const CPoint3<Number> &point, uint seed)
{
    return qHash(double(point.x), seed) + qHash(double(point.y), seed) + qHash(double(point.z), seed);
}

template <class Number>
class CRect
{
public:
    Number x, y, w, h;
    CRect<Number>(const Number _x = Number(0), const Number _y = Number(0), const Number _w = Number(0), const Number _h = Number(0)) : x(_x), y(_y), w(_w), h(_h) {}

    CRect<Number>(const QRectF& p_rect):x(p_rect.x()),y(p_rect.y()),w(p_rect.width()),h(p_rect.height())
    {
    }

    template <typename num_t, typename = typename std::enable_if<std::is_arithmetic<num_t>::value, num_t>::type>
    CRect<Number> offset(const num_t xOffset, const num_t yOffset) const
    {
        CRect<Number> rect = *this;
        rect.x += xOffset;
        rect.y += yOffset;
        return rect;
    }

    CRect<Number> offset(CPoint2<Number> p_offset) const
    {
        return offset(p_offset.x, p_offset.y);
    }

    CPoint2<Number> center() const
    {
        return CPoint2<Number>((2 * x + w) * 0.5, (2 * y + h) * 0.5);
    }
    QRectF toQRectF() const
    {
        return QRectF(x, y, w, h);
    }
    template <typename num_t, typename = typename std::enable_if<std::is_arithmetic<num_t>::value, num_t>::type>
    CRect<Number> moveCenter(const num_t _x, const num_t _y) const
    {
        CPoint2<Number> center = this->center();
        CPoint2<Number> to = CPoint2<Number>(_x, _y);
        return this->offset(to - center);
    }
};
using CPoint2d = CPoint2<double>;
using CPoint3d = CPoint3<double>;
using CPoint2i = CPoint2<int>;
using CRectd = CRect<double>;