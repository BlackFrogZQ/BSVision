
#pragma once
#include "../markParaDef.h"
#include <QMetaType>
#include <QDataStream>
class QSpinBox;
class QDoubleSpinBox;
namespace BS_2DVision_Template
{
    struct CShapeTemplateMarkParas
    {
        bool usedGlobalCamPara = false;
        int modelId = -1;
        float minScore = 0.9;
        CShapeTemplateMarkParas(bool p_usedGlobal = false, int p_modelId = -1);
        CShapeTemplateMarkParas(const QVariant &p_var);
        operator QVariant() const;
        CShapeTemplateMarkParas &operator=(const QVariant &p_var);
    };

    class CShapeTemplateMarkParaWidget : public IMarkTemplateParaWidget
    {
        Q_OBJECT
    public:
        CShapeTemplateMarkParaWidget(QWidget *p = nullptr);
        void attachMarkPara(QVariant *pMarkPara) override;

    public slots:
        void slotModelCountChanged(int p_currCount,int p_oldCount);

    protected:
        void initLayout();

    protected:
        CShapeTemplateMarkParas *m_paras = nullptr;
        QCheckBox *m_pUsedGlobalCheckBox = nullptr;
        QDoubleSpinBox *m_pMinScoreSpinBox = nullptr;
        QSpinBox *m_pUsedModelIDSpinBox = nullptr;
    };
}
Q_DECLARE_METATYPE(BS_2DVision_Template::CShapeTemplateMarkParas)

QDataStream &operator<<(QDataStream &arch, const BS_2DVision_Template::CShapeTemplateMarkParas &paras);
QDataStream &operator>>(QDataStream &arch, BS_2DVision_Template::CShapeTemplateMarkParas &paras);