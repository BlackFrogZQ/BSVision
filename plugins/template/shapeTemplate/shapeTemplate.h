#pragma once
#include "shapeDef.h"
#include "shapeTemplateMarkPara.h"

namespace BS_2DVision_Template
{
    class CShapeTemplateEditWidget;
    class CShapeTemplate : public QObject, public ITemplate
    {
        friend class CShapeTemplateEditWidget;
        Q_INTERFACES(BS_2DVision_Template::ITemplate)
        Q_PLUGIN_METADATA(IID ITemplateEngineService_Interface_iid)
        Q_OBJECT
    public:
        using ITemplate::ITemplate;
        ~CShapeTemplate() = default;
        bool init() override;
        int type() const override;
        QString typeString() const override;

        ITemplateEditWidget *newEditWidget(QWidget *p = nullptr) override;
        IMarkTemplateParaWidget *newMarkParaWidget(QWidget *p = nullptr) override;

        void setRoiPara(bool p_used, QRectF p_roi = QRectF()) override;
        void setMarkParas(QVariant p_paras, int p_maxCount = 1) override;
        void setMarkParas(const CShapeTemplateMarkParas &p_paras, int p_maxCount = 1);

        bool match(const QImage &p_image) override;
        QVector<CMatchResult> matchResult() const override;
        QString errorMsg() const override;
        QByteArray saveBinary() const override;
        bool loadBinary(const QByteArray &p_datas) override;
        void reset() override;
        void clearCache() override;

    signals:
        void sigShapeCountChanged(int p_currCount,int p_oldCount);

    protected:
        CShapeTemplateMarkParas m_paras;
        QRect m_roi;
        int m_maxCount = 1;
        QVector<CMatchResult> m_results;
        QVector<CShapes> m_shapes;
        QString m_errorMsg;
    };
}