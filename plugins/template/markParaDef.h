#pragma once
#include <QFrame>
#include <QDataStream>
#include "templateDef.h"
#include "bsBasic.h"

class QCheckBox;
class QRadioButton;
class QComboBox;
class QDoubleSpinBox;
class QStackedWidget;
namespace BS_2DVision_Template
{
    class IMarkTemplateParaWidget : public QFrame
    {
        Q_OBJECT
    public:
        IMarkTemplateParaWidget(QWidget *p = nullptr);
        virtual ~IMarkTemplateParaWidget();
        virtual void attachMarkPara(QVariant *pMarkTemplatePara);

    protected:
        QVariant *m_pMarkTemplatePara = nullptr;
    };

    struct CMarkPoint
    {
        bool usedRoi = false;
        bool adjustRoi = false;
        QRectF roi;

        QPointF oriPos;
        QPointF newPos;

        int templateType = CTemplateDef::ShapeTemplate;
        QMap<int,QVariant> templateParas;

        Macro_EqualsOperatorFun(CMarkPoint);
    };
}

Q_DECLARE_METATYPE(BS_2DVision_Template::CMarkPoint)
QDataStream &operator<<(QDataStream &arch, const BS_2DVision_Template::CMarkPoint &mark);
QDataStream &operator>>(QDataStream &arch, BS_2DVision_Template::CMarkPoint &mark);
