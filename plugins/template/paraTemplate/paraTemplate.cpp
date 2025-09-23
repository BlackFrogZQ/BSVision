#include "paraTemplate.h"
#include <bscv/hough.h>
#include "../visionEngine/bscvTool.h"

namespace BS_2DVision_Template
{
    bool findCircles(const cv::Mat &p_image,
                     std::vector<cv::Vec3f> &p_circles,
                     int p_minRadius,
                     int p_maxRadius,
                     int p_maxCount,
                     bool p_second,
                     bool p_outContour,
                     float p_minDist, int p_timeout = 5000)
    {
        assert(p_maxRadius >= 0 && p_minRadius >= 0);
        auto pHoughCircle = TIGER_BSVISION::getHoughCircle();
        pHoughCircle->openMask(true);
        pHoughCircle->init(p_image,
                           p_timeout,
                           p_second,
                           p_outContour,
                           p_minRadius,
                           p_maxRadius,
                           p_minDist,
                           p_maxCount);
        if (pHoughCircle->bInit())
        {
            p_circles = pHoughCircle->circles();
            return true;
        }
        return false;
    }
}

namespace BS_2DVision_Template
{
    bool CParaTemplate::init()
    {
        return true;
    }

    int CParaTemplate::type() const
    {
        return CTemplateDef::ParaTemplate;
    }

    QString CParaTemplate::typeString() const
    {
        return QObject::tr("ParaTemplate");
    }

    ITemplateEditWidget *CParaTemplate::newEditWidget(QWidget *p)
    {
        return nullptr;
    }

    IMarkTemplateParaWidget *CParaTemplate::newMarkParaWidget(QWidget *p)
    {
        return new CParaTemplateMarkParaWidget(p);
    }

    void CParaTemplate::setRoiPara(bool p_used, QRectF p_roi)
    {
        m_roi = p_used ? p_roi.toRect() : QRect();
    }

    void CParaTemplate::setMarkParas(QVariant p_paras, int p_maxCount)
    {
        assert(p_paras.canConvert<BS_2DVision_Template::CParaTemplateParas>() && "paraMarkParas类型检查");
        setMarkParas(p_paras.value<BS_2DVision_Template::CParaTemplateParas>(), p_maxCount);
    }

    void CParaTemplate::setMarkParas(const CParaTemplateParas &p_paras, int p_maxCount)
    {
        assert(p_maxCount > 0 && "匹配个数应>0");
        m_paras = p_paras;
        m_maxCount = p_maxCount;
    }

    bool CParaTemplate::match(const QImage &p_image)
    {
        clearCache();
        if (m_roi.isValid())
        {
            bool successful = false;
            auto norRoi = p_image.rect().intersected(m_roi);
            if (norRoi.isValid())
            {
                CParaTemplate paraTemplate;
                paraTemplate.setRoiPara(false);
                paraTemplate.setMarkParas(m_paras, m_maxCount);
                successful = paraTemplate.match(p_image.copy(norRoi));

                // 结果复制
                this->m_results = paraTemplate.m_results;
                this->m_errorMsg = paraTemplate.m_errorMsg;

                auto offset = norRoi.topLeft();
                for (auto &result : m_results)
                {
                    for (auto &pt : result.pos)
                    {
                        pt += offset;
                    }
                    for (auto &pt : result.pixPts)
                    {
                        pt += offset;
                    }
                }
            }
            else
            {
                m_errorMsg = QObject::tr("No valid data within ROI") + ".";
            }
            return successful;
        }
        try
        {
            cv::Mat cvImage;
            if (!TIGER_BSVISION::qImage2cvImage(p_image, cvImage, 1))
            {
                m_errorMsg = QObject::tr("Conversion of OpenCV image failed, unable to recognize and process") + ".";
                return false;
            }
            assert(cvImage.channels() == 1 && "cv图像通道不为1");
            switch (m_paras.type)
            {
            case CParaTemplateDef::Circle:
            {
                auto circleParas = m_paras.circleParas;
                auto minR = 0.5 * std::max(circleParas.d - circleParas.width, 20.0);
                auto maxR = 0.5 * std::min(circleParas.d + circleParas.width, 20.0);
                std::vector<cv::Vec3f> circles;
                if (findCircles(cvImage, circles,
                                minR,
                                maxR,
                                m_maxCount,
                                true,
                                circleParas.priorityOuterContour,
                                10,
                                2000))
                {
                    for (const auto &circle : circles)
                    {
                        m_results << CMatchResult{
                            QVector<QPointF>() << QPointF(circle[0], circle[1]),
                            1.0f, 0.0f,
                            TIGER_BSVISION::getCirclePts(qRound(circle[0]), qRound(circle[1]), qRound(circle[2]))};
                    }
                }
            }
            break;
            default:
                m_errorMsg = QObject::tr("Unsupported segment types") + QString("(%1).").arg(m_paras.type);
                break;
            }
            return m_results.size() > 0;
        }
        catch (cv::Exception &e)
        {
            m_errorMsg = QObject::tr("match Exception") + ":" + QString(e.what());
        }
        return false;
    }

    QVector<CMatchResult> CParaTemplate::matchResult() const
    {
        return m_results;
    }

    QString CParaTemplate::errorMsg() const
    {
        return m_errorMsg;
    }

    void CParaTemplate::reset()
    {
    }

    void CParaTemplate::clearCache()
    {
        m_results.clear();
        m_errorMsg.clear();
    }
}

namespace BS_2DVision_Template
{
    QByteArray CParaTemplate::saveBinary() const
    {
        return QString("ParaTemplate").toUtf8();
    }

    bool CParaTemplate::loadBinary(const QByteArray &p_datas)
    {
        reset();
        return p_datas == saveBinary();
    }

}