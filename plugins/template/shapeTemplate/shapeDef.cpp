#include "shapeDef.h"
#include <QPainterPath>
#include <QPainter>
#include "../visionEngine/bscvTool.h"

namespace BS_2DVision_Template
{
    bool CShape::isEmpty() const
    {
        return templMatchPtr == nullptr || templMatchPtr->isEmpty();
    }

    void CShape::clear()
    {
        anchorPt.clear();
        createParas = CCreateTemplateParas();
        createRoi = QRectF();
        QImage createImage = QImage();
        clearTemplate();
    }

    void CShape::clearTemplate()
    {
        if (templMatchPtr != nullptr)
        {
            templMatchPtr->clear();
            templMatchPtr.reset();
        }
    }

    CShape CShape::clone() const
    {
        CShape shape = *this;
        shape.templMatchPtr = std::shared_ptr<TIGER_BSVISION::ITemplMatch>(TIGER_BSVISION::newTemplMatch(templMatchPtr.get()));
        return shape;
    }

    QVector<QPointF> CShape::shapePts() const
    {
        QVector<QPointF> pts;
        if (!isEmpty())
        {
            pts = shapePts(this->templMatchPtr, this->createRoi.topLeft());
        }
        return pts;
    }

    QVector<QVector<QPointF>> CShape::posPts() const
    {
        QVector<QVector<QPointF>> pos;

        return pos;
    }

    QVector<QPointF> CShape::previewTemplate(const QPainterPath &p_path) const
    {
        auto paras = this->createParas;
        paras.useAngle = false;
        paras.useScale = false;
        QRectF roi;
        auto pTempl = createTemplate(p_path, roi, paras);
        return shapePts(pTempl, roi.topLeft());
    }

    bool CShape::createTemplate(const QPainterPath &p_path)
    {
        this->templMatchPtr = createTemplate(p_path, this->createRoi, this->createParas);
        return !isEmpty();
    }

    std::shared_ptr<CShape::CTemplMatch> CShape::createTemplate(const QPainterPath &p_path, QRectF &p_roi, const CCreateTemplateParas &p_paras) const
    {
        std::shared_ptr<CTemplMatch> pTempl = nullptr;
        auto creatRect = QRectF(this->createImage.rect()).intersected(p_path.boundingRect());
        do
        {
            if (!creatRect.isValid())
            {
                break;
            }

            int x = std::floor(creatRect.left());
            int y = std::floor(creatRect.top());
            int w = std::ceil(creatRect.right()) - x;
            int h = std::ceil(creatRect.bottom() - y);
            QImage qimage = QImage(w, h, QImage::Format_Grayscale8);
            qimage.fill(Qt::black);
            QPainter painter(&qimage);
            painter.setPen(Qt::white);
            painter.setBrush(Qt::white);
            painter.translate(-x, -y);
            painter.drawPath(p_path);
            painter.end();
            auto roiMask = cv::Mat();
            if (!TIGER_BSVISION::qImage2cvImage(qimage, roiMask, 1))
            {
                break;
            }

            auto roiRectImage = cv::Mat();
            if (!TIGER_BSVISION::qImage2cvImage(this->createImage.copy(x, y, w, h), roiRectImage, 1))
            {
                break;
            }
            p_roi = QRectF(x, y, w, h);
            pTempl = std::shared_ptr<CTemplMatch>(TIGER_BSVISION::newTemplMatch());
            if (p_paras.useAngle || p_paras.useScale)
            {
                if (!pTempl->create_shape_model(roiRectImage, roiMask,
                                                p_paras.useAngle ? p_paras.startAngle : 0.0,
                                                p_paras.useAngle ? p_paras.angleExtent : 0,
                                                p_paras.stepAngle,
                                                p_paras.useScale ? p_paras.scaleMin : 1.0,
                                                p_paras.useScale ? p_paras.scaleMax : 1.0,
                                                p_paras.scaleStep,
                                                p_paras.limitMinContrast, p_paras.contrast, p_paras.numFeatures))
                {
                    pTempl.reset();
                    break;
                }
            }
            else
            {
                if (!pTempl->create_one_shape_model(roiRectImage, roiMask,
                                                    p_paras.limitMinContrast,
                                                    p_paras.contrast,
                                                    p_paras.numFeatures))
                {
                    pTempl.reset();
                    break;
                }
            }
        } while (0);
        return pTempl;
    }

    QVector<QPointF> CShape::shapePts(std::shared_ptr<CTemplMatch> pTempl, QPointF p_templOffset) const
    {
        QVector<QPointF> pts;
        if (pTempl != nullptr && !pTempl->isEmpty())
        {
            auto templ = pTempl->getTempl(0);
            auto offset = QPointF(p_templOffset.x() + templ.tl_x, p_templOffset.y() + templ.tl_y);
            for (auto &feature : templ.features)
            {
                pts << QPointF(feature.x, feature.y) + offset;
            }
        }
        return pts;
    }
}

namespace BS_2DVision_Template
{
    CShapes CShapes::clone() const
    {
        CShapes copyShapes;
        copyShapes.reserve(this->size());
        for (const auto &shape : *this)
        {
            copyShapes << shape.clone();
        }
        return copyShapes;
    }
}