#pragma once
#include "Ui/wizard/wizard.h"
namespace BS_2DVision_Template
{
    class CCreateShapeWizard;
    class CCreatePosWizard;
    class CShape;

    class CWizard : public BS_QT_Ui::CCustomWizard
    {
        Q_OBJECT
    public:
        CWizard(QWidget *parent = nullptr);
        void setShape(CShape *pShape);

    protected slots:
        virtual void accept() override;

    protected:
        CCreateShapeWizard *m_pCreateShapeWizard = nullptr;
        CCreatePosWizard *m_pCreatePosWizard = nullptr;
    };
}