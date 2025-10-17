#include "setCreateParaWindow.h"
#include "../iGraphicsWidget.h"
#include "imageProcess/engineDef.h"
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QRadioButton>

void CSetCreateParaWindow::setCreateParas(CCreateTemplateParasOld *pCreateParas)
{
    assert(pCreateParas != nullptr);
    m_pCreateParas = pCreateParas;
}

QLayout *CSetCreateParaWindow::initPataLayout()
{
    m_pNumFeaturesSB = IGraphicsWidget::getSpinBox(5, 500, 20, 5);

    m_pStartAngle = IGraphicsWidget::getDoubleSpinBox(-180, 0, 0, 0.5);
    m_pStartAngle->setDecimals(1);
    m_pStartExtent = IGraphicsWidget::getDoubleSpinBox(0, 360, 360, 0.5);
    m_pStartExtent->setDecimals(1);
    m_pStartStep = IGraphicsWidget::getDoubleSpinBox(0, 359.9, 0, 0.5);
    m_pStartStep->setDecimals(1);

    const QString cBoolCloseIco = ":/res/para/close.png";
    const QString cBoolOpenIco = ":/res/para/open.png";
    m_pUseSacle = new QRadioButton;
    m_pUseSacle->setCheckable(true);
    m_pUseSacle->setStyleSheet(
        QString(
            "QRadioButton::indicator{\
                        width:%1px;\
                        height:%2px;}\
                    QRadioButton{\
                        background-color: transparent;}\
                    QRadioButton::indicator:unchecked{\
                        image: url(%3);}\
                    QRadioButton::indicator:checked {\
                        image: url(%4);}")
            .arg(70)
            .arg(50)
            .arg(cBoolCloseIco)
            .arg(cBoolOpenIco));

    m_pScaleMin = IGraphicsWidget::getDoubleSpinBox(0.5, 1.0, 0.98, 0.01);
    m_pScaleMin->setDecimals(2);
    m_pScaleMax = IGraphicsWidget::getDoubleSpinBox(1.0, 1.5, 1.02, 0.01);
    m_pScaleMax->setDecimals(2);
    m_pScaleStep = IGraphicsWidget::getDoubleSpinBox(0.01, 1.0, 0.01, 0.01);
    m_pScaleStep->setDecimals(2);
    connect(m_pUseSacle, &QRadioButton::toggled, this, [this](bool checked)
            {
                if(m_pCreateParas!=nullptr)
                {
                    m_pScaleMin->setValue(m_pCreateParas->scaleMin);
                    m_pScaleMax->setValue(m_pCreateParas->scaleMax);
                    m_pScaleStep->setValue(m_pCreateParas->scaleStep);
                }
                m_pScaleMin->setEnabled(checked);
                m_pScaleMax->setEnabled(checked);
                m_pScaleStep->setEnabled(checked); });

    m_pOptimizationTypeCB = new QComboBox;
    m_pOptimizationTypeCB->addItems(cOptimizationCnName);
    m_pOptimizationTypeCB->setCurrentIndex(0);
    m_pOptimizationTypeCB->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_pPolarityTypeCB = new QComboBox;
    m_pPolarityTypeCB->addItems(cPolarityCnName);
    m_pPolarityTypeCB->setCurrentIndex(0);
    m_pPolarityTypeCB->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    QGridLayout *pLayout = new QGridLayout;
    const double cSize =30;
    m_pNumFeaturesSB->setFixedHeight(cSize);
    int row = 0;
    int col = 0;
    pLayout->addWidget(TIGER_UIBasic::getLabel(QObject::tr("最小模板特征数"), 30), row, col++, 1, 1);
    pLayout->addWidget(m_pNumFeaturesSB, row, col++, 1, 1);

    m_pOptimizationTypeCB->setFixedHeight(cSize);
    pLayout->addWidget(TIGER_UIBasic::getLabel(QObject::tr("优化器"), 30), row, col++, 1, 1);
    pLayout->addWidget(m_pOptimizationTypeCB, row, col++, 1, 1);

    m_pPolarityTypeCB->setFixedHeight(cSize);
    pLayout->addWidget(TIGER_UIBasic::getLabel(QObject::tr("极性"), 30), row, col++, 1, 1);
    pLayout->addWidget(m_pPolarityTypeCB, row, col++, 1, 1);
    row++;
    col = 0;

    m_pStartAngle->setFixedHeight(cSize);
    pLayout->addWidget(TIGER_UIBasic::getLabel(QObject::tr("起始角度"), 30), row, col++, 1, 1);
    pLayout->addWidget(m_pStartAngle, row, col++, 1, 1);

    m_pStartExtent->setFixedHeight(cSize);
    pLayout->addWidget(TIGER_UIBasic::getLabel(QObject::tr("角度范围"), 30), row, col++, 1, 1);
    pLayout->addWidget(m_pStartExtent, row, col++, 1, 1);

    m_pStartStep->setFixedHeight(cSize);
    pLayout->addWidget(TIGER_UIBasic::getLabel(QObject::tr("角度步长"), 30), row, col++, 1, 1);
    pLayout->addWidget(m_pStartStep, row, col++, 1, 1);
    row++;
    col = 0;

    pLayout->addWidget(TIGER_UIBasic::getLabel(QObject::tr("启用缩放"), 30), row, col++, 1, 1);
    pLayout->addWidget(m_pUseSacle, row, col++, 1, 1);
    row++;
    col = 0;

    m_pScaleMin->setFixedHeight(cSize);
    pLayout->addWidget(TIGER_UIBasic::getLabel(QObject::tr("最小缩放"), 30), row, col++, 1, 1);
    pLayout->addWidget(m_pScaleMin, row, col++, 1, 1);

    m_pScaleMax->setFixedHeight(cSize);
    pLayout->addWidget(TIGER_UIBasic::getLabel(QObject::tr("最大缩放"), 30), row, col++, 1, 1);
    pLayout->addWidget(m_pScaleMax, row, col++, 1, 1);

    m_pScaleStep->setFixedHeight(cSize);
    pLayout->addWidget(TIGER_UIBasic::getLabel(QObject::tr("缩放步长"), 30), row, col++, 1, 1);
    pLayout->addWidget(m_pScaleStep, row, col++, 1, 1);

    pLayout->setMargin(0);
    pLayout->setSpacing(2);
    return pLayout;
}

void CSetCreateParaWindow::initPatas()
{
    assert(m_pCreateParas != nullptr);
    m_pStartAngle->setValue(m_pCreateParas->startAngle);
    m_pStartExtent->setValue(m_pCreateParas->angleExtent);
    m_pStartStep->setValue(m_pCreateParas->stepAngle);
    m_pScaleMin->setValue(m_pCreateParas->scaleMin);
    m_pScaleMax->setValue(m_pCreateParas->scaleMax);
    m_pScaleStep->setValue(m_pCreateParas->scaleStep);
    m_pNumFeaturesSB->setValue(m_pCreateParas->numFeatures);
    m_pOptimizationTypeCB->setCurrentIndex(m_pCreateParas->optimizationType);
    m_pPolarityTypeCB->setCurrentIndex(m_pCreateParas->polarityType);
    m_pUseSacle->setChecked(m_pCreateParas->useScale);
    m_pUseSacle->toggled(m_pUseSacle->isChecked());
}

void CSetCreateParaWindow::updatePatas()
{
    assert(m_pCreateParas != nullptr);
    m_pCreateParas->startAngle = m_pStartAngle->value();
    m_pCreateParas->angleExtent = m_pStartExtent->value();
    m_pCreateParas->stepAngle = m_pStartStep->value();

    m_pCreateParas->scaleMin = m_pScaleMin->value();
    m_pCreateParas->scaleMax = m_pScaleMax->value();
    m_pCreateParas->scaleStep = m_pScaleStep->value();
    m_pCreateParas->useScale = m_pUseSacle->isChecked();

    m_pCreateParas->numFeatures = m_pNumFeaturesSB->value();
    m_pCreateParas->optimizationType = COptimizationType(m_pOptimizationTypeCB->currentIndex());
    m_pCreateParas->polarityType = CPolarityType(m_pPolarityTypeCB->currentIndex());
}