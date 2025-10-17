#pragma once
#include "system/myMath.h"
#include "template/visionEngine/bscv/include/bscv/templMatchTypes.h"

#include "bsBasic.h"

#include <QDataStream>
#include <QPainterPath>
#include <QImage>

enum CEngineType : int32_t
{
    ctOpenCV = 0,
    ctUnknown
};

template <class T>
class IRegion
{
public:
    virtual ~IRegion(){};
    CEngineType type() const { return pRegion()->type(); }
    bool isEmpty() { return pRegion()->isEmpty(); }
    void clear() { pRegion()->clear(); }
    T clone() const { return *((T *)(this)); }
    QList<QPointF> toPoints(int p_dilation = 0) const { return pRegion()->toPoints(p_dilation); }
    T fromPath(QSize p_size, const QPainterPath &p_path) const { return pRegion()->fromPath(p_size, p_path); }
    T fromImage(const QImage &p_image) const { return pRegion()->fromImage(p_imageh); }
    void unite2(const T &p_region) { pRegion()->unite2(p_region); }
    // 除区域外，全是透明
    QImage toImage(QSize p_imageSize, QColor p_color = QColor(0xff, 0x00, 0x00, 0xff), int p_dilation = 0)
    {
        return pRegion()->toImage(p_imageSize, p_color, p_dilation);
    }
    // 返回以图像中心为原点,方向与图像坐标系一致的坐标系
    QPolygonF getMinRect(const QSize &p_imageSize = QSize(0, 0)) const { return pRegion()->getMinRect(p_imageSize); }
    bool save(QString p_file) const { return pRegion()->save(p_file); }
    bool load(QString p_file) { return pRegion()->load(p_file); }

    void addPos(double pRow, double pCol);
    T united(const T &p_region) const;
    QImage paintToImage(QImage p_image, QColor p_color = QColor(0xff, 0x00, 0x00, 0xff), int p_dilation = 0);

protected:
    T getPosRegion(double pRow, double pCol) { return pRegion()->getPosRegion(pRow, pCol); }

private:
    T *pRegion() { return static_cast<T *>(this); }
};

template <typename Shape, typename T>
class ITemplateShape
{
public:
    virtual ~ITemplateShape(){};
    bool isEmpty() const { return pShape()->isEmpty(); }
    void clear() { return pShape()->clear(); }
    CEngineType type() const { return pShape()->type(); }
    Shape clone() const { return pShape()->clone(); }
    T toTemplateRegion() const { return pShape()->toTemplateRegion(); }
    T toMatchRegion(int p_templID) const { return pShape()->toMatchRegion(p_templID); }
    T smallesAABBRegion(bool p_hasCentralPoint = true) { return pShape()->smallesAABBRegion(p_hasCentralPoint); }
    CPoint2d getAABBCentralPoint() const { return pShape()->getAABBCentralPoint(); }
    bool save(QString p_file) const { return pShape()->save(p_file); }
    bool load(QString p_file) { return pShape()->load(p_file); }

private:
    Shape *pShape() const { return static_cast<Shape *>(this); }
};

enum CSubPixelType : int32_t
{
    cspNone = 0,
    cspInterpolation,
    cspLeastSquares,
    cspLeastSquaresHigh
};

#define cSubPixelCnName (QStringList() << QObject::tr("不使用") << QObject::tr("差值") << QObject::tr("最小二乘1") << QObject::tr("最小二乘2"))

const double cUndefineExposureTime = -1;
struct CMatchTemplateParas
{
    // 0-1
    double minScore = 0.95;
    // 0-1
    double greediness = 0.9;
    // 0-1
    double maxOverlap = 0.0;
    // 0-20
    int maxNumMatches = 1;
    // 0-10
    int numLevels = 4;
    // cspNone,
    // cspInterpolation,
    // cspLeastSquares,
    // cspLeastSquaresHigh,
    CSubPixelType subPixel = cspLeastSquaresHigh;
    friend QDataStream &operator<<(QDataStream &out, const CMatchTemplateParas &per)
    {
        out
            << per.minScore
            << per.greediness
            << per.maxOverlap
            << per.maxNumMatches
            << per.numLevels
            << (int32_t)per.subPixel;
        return out;
    };
    friend QDataStream &operator>>(QDataStream &in, CMatchTemplateParas &per)
    {
        in >>
            per.minScore >>
            per.greediness >>
            per.maxOverlap >>
            per.maxNumMatches >>
            per.numLevels >>
            (int32_t &)(per.subPixel);
        return in;
    };
};

enum COptimizationType : int32_t
{
    cotAuto = 0,
    cotPointReductionLow,
    cotPointReductionMedium,
    cotPointReductionHigh,
    cotNone,
    cotMax
};

const QStringList cOptimizationName = {"auto", "point_reduction_low", "point_reduction_medium", "point_reduction_high", "none"};
#define cOptimizationCnName (QStringList() << QObject::tr("自动优化") << QObject::tr("优化到低点") << QObject::tr("优化到中点") << QObject::tr("优化到高点") << QObject::tr("不优化"))

enum CPolarityType : int32_t
{
    cptIgnore = 0,
    cptSame,
    cptOpposite,
    cptMax
};
const QStringList cPolarityName = {"ignore_polarity", "use_same_polarity", "use_opposite_polarity"};
#define cPolarityCnName (QStringList() << QObject::tr("忽略极性") << QObject::tr("与模型一致") << QObject::tr("与模型相反"))

struct CCreateTemplateParasOld
{
    int numFeatures = 128; // 最小模板特征点数。
    double startAngle = 0, angleExtent = 360, stepAngle = 1;
    bool useScale = true;
    double scaleMin = 0.98, scaleMax = 1.02, scaleStep = 0.01f;
    int contrast = 60;         // 模板阈值，生成模板时用来评价当前点是否为特征点，该值越高识别的特征点越少。
    int limitMinContrast = 30; // 匹配阈值，在匹配时用在图片处理生成方向量化上，越高方向量化点越少。
    COptimizationType optimizationType = COptimizationType::cotNone;
    CPolarityType polarityType = CPolarityType::cptIgnore;

    friend QDataStream &operator<<(QDataStream &out, const CCreateTemplateParasOld &per)
    {
        out
            << per.numFeatures
            << per.startAngle
            << per.stepAngle
            << per.useScale
            << per.scaleMin
            << per.scaleStep
            << per.scaleMin
            << per.contrast
            << per.limitMinContrast
            << (int32_t)per.optimizationType
            << (int32_t)per.polarityType;
        return out;
    };
    friend QDataStream &operator>>(QDataStream &in, CCreateTemplateParasOld &per)
    {
        in >>
            per.numFeatures >>
            per.startAngle >>
            per.stepAngle >>
            per.useScale >>
            per.scaleMin >>
            per.scaleStep >>
            per.scaleMin >>
            per.contrast >>
            per.limitMinContrast >>
            (int32_t &)per.optimizationType >>
            (int32_t &)per.polarityType;
        return in;
    };
};

struct CGrayTemplateParas
{
    QPainterPath grayPath;
    double grayValue = 0;
    double lowerDeviation = 15;
    double upperDeviation = 15;

    friend QDataStream &operator<<(QDataStream &out, const CGrayTemplateParas &_paras)
    {
        out
            << _paras.grayPath
            << _paras.grayValue
            << _paras.lowerDeviation
            << _paras.upperDeviation;
        return out;
    };
    friend QDataStream &operator>>(QDataStream &in, CGrayTemplateParas &_paras)
    {
        in >>
            _paras.grayPath >>
            _paras.grayValue >>
            _paras.lowerDeviation >>
            _paras.upperDeviation;
        return in;
    };
};

void initVisionEngine();

#include <QPainter>
template <class T>
void IRegion<T>::addPos(double pRow, double pCol)
{
    unite2(getPosRegion(pRow, pCol));
}

template <class T>
T IRegion<T>::united(const T &p_region) const
{
    T reg;
    reg.unite2(p_region);
    return reg;
}

template <class T>
QImage IRegion<T>::paintToImage(QImage p_image, QColor p_color, int p_dilation)
{
    if (!p_image.isNull() && !isEmpty())
    {
        if (p_image.format() != QImage::Format_RGB888)
        {
            p_image = p_image.convertToFormat(QImage::Format_RGB888);
        }
        QPainter painter(&p_image);
        painter.drawImage(0, 0, toImage(p_image.size(), p_color, p_dilation));
    }
    return p_image;
}

/////////////////////////////////////////////////////////////////
// 类型转换

inline TIGER_BSVISION::CPolarityType convertToBscvPolarity(CPolarityType _type)
{
    switch (_type)
    {
        case cptIgnore:   return TIGER_BSVISION::cptIgnore;
        case cptSame:     return TIGER_BSVISION::cptSame;
        case cptOpposite: return TIGER_BSVISION::cptOpposite;
        default:
            // 防御性处理：记录日志 / 抛异常 / 返回默认值
            assert(false && "Unknown TIGER_BSVISION::CPolarityType");
            return TIGER_BSVISION::cptIgnore;
    }
}

inline CPolarityType convertToPolarity(TIGER_BSVISION::CPolarityType _type)
{
    switch (_type)
    {
        case TIGER_BSVISION::cptIgnore:   return cptIgnore;
        case TIGER_BSVISION::cptSame:     return cptSame;
        case TIGER_BSVISION::cptOpposite: return cptOpposite;
        default:
            // 防御性处理：记录日志 / 抛异常 / 返回默认值
            assert(false && "Unknown CPolarityType");
            return cptIgnore;
    }
}