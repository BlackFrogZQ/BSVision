#pragma once
#include "idToolButton.h"
#include <QFrame>
class QToolButton;
class QButtonGroup;
class CShowAutoImageWidget;
namespace BS_2DVision_Template
{
    class CShapes;

    class CShapeWidget : public CIdToolButton
    {
        Q_OBJECT
    public:
        CShapeWidget(QButtonGroup *pGroup, QWidget *p = nullptr);
        ~CShapeWidget() = default;

    signals:
        void sigDel(int p_id);
        void sigEdit(int p_id);

    protected:
        QToolButton *pDelBtn = nullptr;
        QToolButton *pEditBtn = nullptr;
    };

    class CShapesWidget : public QFrame
    {
        Q_OBJECT
    public:
        using QFrame::QFrame;
        ~CShapesWidget() = default;

    signals:
        void sigEditWidget(QWidget* p);
        void sigEditFinished();

    public slots:
        void init();
        void attachShapes(CShapes *p_pShapes);
        void newShape();

    protected slots:
        void currIdChanged(int p_curr);
        void editShape(int p_id);
        void updateShapeBar();

    protected:
        QWidget *m_pToolBar = nullptr;
        QToolButton *m_pNewShapeBtn = nullptr;
        CShapes *m_pShapes = nullptr;
        QButtonGroup* m_pButtonGroup = nullptr;
        QList<CShapeWidget*> m_pShapeWidgets;
        int m_selectId = INT32_MIN;
        CShowAutoImageWidget *m_showShapeWidget = nullptr;
    };
}