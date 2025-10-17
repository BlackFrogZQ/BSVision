#include "setMatchParaWindow.h"
#include "../iGraphicsWidget.h"
#include "imageProcess/engineDef.h"
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QRadioButton>

void CSetMatchParaWindow::setMatchParas(CMatchTemplateParas *pMatchParas)
{
    assert(pMatchParas != nullptr);
    m_pMatchParas = pMatchParas;
}

QLayout *CSetMatchParaWindow::initPataLayout()
{
    m_pNumLevelsSB = IGraphicsWidget::getSpinBox(0, 10, 0);
    m_pGreedinessDSB = IGraphicsWidget::getDoubleSpinBox(0, 1, 0.9, 0.05);
    m_pMaxOverlapDSB = IGraphicsWidget::getDoubleSpinBox(0, 1, 0.5, 0.05);

    m_pSubPixelTypeCB = new QComboBox;
    m_pSubPixelTypeCB->addItems(cSubPixelCnName);
    m_pSubPixelTypeCB->setCurrentIndex(0);
    m_pSubPixelTypeCB->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    QGridLayout *pLayout = new QGridLayout;
    const double cSize = 30;
    m_pMaxOverlapDSB->setFixedHeight(cSize);
    int row = 0;
    int col = 0;
    pLayout->addWidget(TIGER_UIBasic::getLabel(QObject::tr("重叠系数"), 30), row, col++, 1, 1);
    pLayout->addWidget(m_pMaxOverlapDSB, row, col++, 1, 1);

    m_pGreedinessDSB->setFixedHeight(cSize);
    pLayout->addWidget(TIGER_UIBasic::getLabel(QObject::tr("加速系数"), 30), row, col++, 1, 1);
    pLayout->addWidget(m_pGreedinessDSB, row, col++, 1, 1);
    row++;
    col = 0;

    m_pNumLevelsSB->setFixedHeight(cSize);
    pLayout->addWidget(TIGER_UIBasic::getLabel(QObject::tr("压缩等级"), 30), row, col++, 1, 1);
    pLayout->addWidget(m_pNumLevelsSB, row, col++, 1, 1);

    m_pSubPixelTypeCB->setFixedHeight(cSize);
    pLayout->addWidget(TIGER_UIBasic::getLabel(QObject::tr("亚像素"), 30), row, col++, 1, 1);
    pLayout->addWidget(m_pSubPixelTypeCB, row, col++, 1, 1);
    row++;
    col = 0;

    pLayout->setMargin(0);
    pLayout->setSpacing(2);
    return pLayout;
}

void CSetMatchParaWindow::initPatas()
{
    assert(m_pMatchParas != nullptr);
    m_pMaxOverlapDSB->setValue(m_pMatchParas->maxOverlap);
    m_pGreedinessDSB->setValue(m_pMatchParas->greediness);
    m_pNumLevelsSB->setValue(m_pMatchParas->numLevels);
    m_pSubPixelTypeCB->setCurrentIndex(m_pMatchParas->subPixel);
}

void CSetMatchParaWindow::updatePatas()
{
    assert(m_pMatchParas != nullptr);
    m_pMatchParas->greediness = m_pGreedinessDSB->value();
    m_pMatchParas->maxOverlap = m_pMaxOverlapDSB->value();
    m_pMatchParas->numLevels = m_pNumLevelsSB->value();
    m_pMatchParas->subPixel = CSubPixelType(m_pSubPixelTypeCB->currentIndex());
}