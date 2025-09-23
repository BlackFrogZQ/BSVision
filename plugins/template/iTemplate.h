#pragma once
#include "templateDef.h"
#include <QVariant>
#include <QImage>
#include <QRectF>
namespace BS_2DVision_Template
{
    struct CMatchResult
    {
        QVector<QPointF> pos;
        float score = 0.0f, angle = 0.0f;
        QVector<QPointF> pixPts;
        QImage drawResult(const QImage &p_image, QColor p_color) const;
    };

    struct CCameraPara
    {
        double exposureTime = 2000;
        double gainRaw = 0;
        double gamma = 1;
    };

// TODO 模板需要不同相机光照
#define ITemplateEngineService_Interface_iid "BS_2DVision_Template.Plugin.EngineInterface"
    class IMarkTemplateParaWidget;
    class ITemplateEditWidget;
    class ITemplate
    {
    public:
        virtual bool init() = 0;
        virtual int type() const = 0;
        virtual QString typeString() const = 0;
        virtual ITemplateEditWidget *newEditWidget(QWidget *p = nullptr) = 0;
        virtual IMarkTemplateParaWidget *newMarkParaWidget(QWidget *p = nullptr) = 0;
        virtual void setRoiPara(bool p_used, QRectF p_roi = QRectF()) = 0;
        virtual void setMarkParas(QVariant p_paras, int p_maxCount = 1) = 0;
        virtual bool match(const QImage &p_image) = 0;

        // virtual QVector<CCameraPara> matchResult() const = 0;

        virtual QVector<CMatchResult> matchResult() const = 0;
        virtual QString errorMsg() const = 0;
        virtual QByteArray saveBinary() const = 0;
        virtual bool loadBinary(const QByteArray &p_datas) = 0;
        // 重置为初始状态未加载任何数据
        virtual void reset() = 0;
        virtual void clearCache() = 0;
    };
}
Q_DECLARE_INTERFACE(BS_2DVision_Template::ITemplate, ITemplateEngineService_Interface_iid)
