
#pragma once
#include "../markParaDef.h"
#include "../templateDef.h"
#include <QMetaType>
#include <QDataStream>

namespace BS_2DVision_Template
{
    class CParaTemplateDef
    {
        Q_GADGET
    public:
        enum CTemplateType : int32_t
        {
            Circle = CTemplateDef::Subdivision_UnknownType + 1,
            CrossLine,
            LLine,
            Unknown = CTemplateDef::Subdivision_UnknownType
        };
        Q_ENUM(CTemplateType)
    };

    struct CCircleParas
    {
        bool priorityOuterContour = true;
        double d = 1;
        double width = 0.1;
        Macro_EqualsOperatorFun(CCircleParas);
    };

    struct CParaTemplateParas
    {
        int type = CParaTemplateDef::Circle;
        CCircleParas circleParas;
        CParaTemplateParas(CParaTemplateDef::CTemplateType type = CParaTemplateDef::Circle);
        CParaTemplateParas(const QVariant &p_var);
        operator QVariant() const;
        CParaTemplateParas &operator=(const QVariant &p_var);
        Macro_EqualsOperatorFun(CParaTemplateParas);
    };

    class CCircleTemplateMarkParaWidget;
    class CParaTemplateMarkParaWidget : public IMarkTemplateParaWidget
    {
        Q_OBJECT

    public:
        CParaTemplateMarkParaWidget(QWidget *p = nullptr);
        void attachMarkPara(QVariant *pMarkPara) override;

    protected:
        void initLayout();

    protected:
        CParaTemplateParas *m_paras = nullptr;
        QMap<int, QCheckBox *> m_subdivisionCheckBox;
        QStackedWidget *m_pStackedWidget = nullptr;
        CCircleTemplateMarkParaWidget *m_pCircleWidget = nullptr;
    };
}
Q_DECLARE_METATYPE(BS_2DVision_Template::CCircleParas)
Q_DECLARE_METATYPE(BS_2DVision_Template::CParaTemplateParas)

QDataStream &operator<<(QDataStream &arch, const BS_2DVision_Template::CCircleParas &paras);
QDataStream &operator>>(QDataStream &arch, BS_2DVision_Template::CCircleParas &paras);
QDataStream &operator<<(QDataStream &arch, const BS_2DVision_Template::CParaTemplateParas &paras);
QDataStream &operator>>(QDataStream &arch, BS_2DVision_Template::CParaTemplateParas &paras);