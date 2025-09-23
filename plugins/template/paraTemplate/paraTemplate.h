#pragma once
#include "../iTemplate.h"
#include "paraTemplateMarkPara.h"

namespace BS_2DVision_Template
{
    class CParaTemplate :public QObject, public ITemplate
    {
        Q_INTERFACES(BS_2DVision_Template::ITemplate)
        Q_PLUGIN_METADATA(IID ITemplateEngineService_Interface_iid)
        Q_OBJECT

    public:
        using ITemplate::ITemplate;
        ~CParaTemplate() = default;
        bool init() override;
        int type() const override;
        QString typeString() const override;

        ITemplateEditWidget *newEditWidget(QWidget *p = nullptr) override;
        IMarkTemplateParaWidget *newMarkParaWidget(QWidget *p = nullptr) override;

        void setRoiPara(bool p_used, QRectF p_roi = QRectF()) override;
        void setMarkParas(QVariant p_paras, int p_maxCount = 1) override;
        void setMarkParas(const CParaTemplateParas &p_paras, int p_maxCount = 1);

        bool match(const QImage &p_image) override;
        QVector<CMatchResult> matchResult() const override;
        QString errorMsg() const override;
        QByteArray saveBinary() const override;
        bool loadBinary(const QByteArray &p_datas) override;
        void reset() override;
        void clearCache() override;

    protected:
        CParaTemplateParas m_paras;
        QRect m_roi;
        int m_maxCount = 1;
        QVector<CMatchResult> m_results;
        QString m_errorMsg;
    };
}