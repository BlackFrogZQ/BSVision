#pragma once
#include "ui/basic.h"
class CNavigationBar;
class QStackedWidget;
class QPushButton;
class IFeatureParaEdit : public QWidget
{
    Q_OBJECT
public:
    IFeatureParaEdit(QWidget *parent = nullptr);
    virtual ~IFeatureParaEdit();

    bool isSave() const;

signals:
    void sigClose();

protected:
    bool m_isSave;
};
