#pragma once
#include "../iTemplate.h"
#include <memory>
namespace TIGER_BSVISION
{
    class ITemplMatch;
}
namespace BS_2DVision_Template
{
    struct CCreateTemplateParas : public CCameraPara
    {
        int numFeatures = 128; // 最小模板特征点数。
        bool useAngle = true;
        double startAngle = 0, angleExtent = 360, stepAngle = 1;
        bool useScale = true;
        double scaleMin = 0.98, scaleMax = 1.02, scaleStep = 0.01f;
        int contrast = 60;         // 模板阈值，生成模板时用来评价当前点是否为特征点，该值越高识别的特征点越少。
        int limitMinContrast = 30; // 匹配阈值，在匹配时用在图片处理生成方向量化上，越高方向量化点越少。
    };

    struct CAnchorPt
    {
        float x, y;
        QVector<QPointF> pixPts;
    };

    class CShape
    {
    public:
        bool isEmpty() const;
        void clear();
        void clearTemplate();
        CShape clone() const;
        QVector<QPointF> shapePts() const;
        QVector<QVector<QPointF>> posPts() const;
        QVector<QPointF> previewTemplate(const QPainterPath &p_path) const;
        bool createTemplate(const QPainterPath &p_path);

        // 定位形状
        QVector<CAnchorPt> anchorPt;
        CCreateTemplateParas createParas;
        QRectF createRoi;
        QImage createImage;
        using CTemplMatch = TIGER_BSVISION::ITemplMatch;
        std::shared_ptr<CTemplMatch> templMatchPtr = nullptr;

    protected:
        std::shared_ptr<CTemplMatch> createTemplate(const QPainterPath &p_path,QRectF& p_roi,const CCreateTemplateParas& p_paras) const;
        QVector<QPointF> shapePts(std::shared_ptr<CTemplMatch> pTempl,QPointF p_templOffset) const;
    };

    class CShapes : public QVector<CShape>
    {
    public:
        CShapes clone() const;
        QByteArray saveBinary() const;
        bool loadBinary(const QByteArray &p_datas);
    };
}