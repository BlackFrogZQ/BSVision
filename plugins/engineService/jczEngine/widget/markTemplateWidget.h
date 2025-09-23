#pragma once
#include "bsBasic.h"
#include "template/templateDef.h"
#include "template/markParaDef.h"
#include <QFrame>
#include <QVariant>
#include <QDataStream>
class QCheckBox;
class QComboBox;

class CMarkTemplateWidget : public QFrame
{
    Q_OBJECT
public:
    CMarkTemplateWidget(QWidget *p = nullptr);
    ~CMarkTemplateWidget();
    QRectF roi() const;
    bool usedRoi() const;
    void attachMark(BS_2DVision_Template::CMarkPoint *pMark);

public slots:
    void setRoi(QRectF p_roi);
    void setUsedRoi(bool p_usedRoi);

protected:
    void initLayout();

protected:
    QComboBox *m_pTemplateComboBox = nullptr;
    QMap<int,int>m_comboBoxIdMap;
    QStackedWidget *m_pParaStackedWidget = nullptr;
    QMap<int,BS_2DVision_Template::IMarkTemplateParaWidget *>m_pMakeParaWidgets;
    BS_2DVision_Template::CMarkPoint *m_pMark = nullptr;
};
