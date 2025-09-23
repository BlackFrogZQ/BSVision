#include "wizard.h"

#include "wizardDef.h"
#include "createShapeWizard.h"
#include "createPosWizard.h"

#include "Ui/uiHelper.h"

#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
namespace BS_2DVision_Template
{
    CWizard::CWizard(QWidget *parent)
        : BS_QT_Ui::CCustomWizard(parent),
          m_pCreateShapeWizard(new CCreateShapeWizard(this)),
          m_pCreatePosWizard(new CCreatePosWizard(this))
    {
        setWindowFlag(Qt::Widget);
        addPage(m_pCreateShapeWizard);
        addPage(m_pCreatePosWizard);
        m_pListWidget->hide();
        setSideWidget(nullptr);
        setContentsMargins(0,0,0,0);
    }

    void CWizard::accept()
    {
        CCustomWizard::accept();
    }

    void CWizard::setShape(CShape *pShape)
    {
        m_pCreateShapeWizard->setShape(pShape);
        m_pCreatePosWizard->setShape(pShape);
    }
}