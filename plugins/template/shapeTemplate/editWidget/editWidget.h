#pragma once
#include "template/templateDef.h"
#include "../shapeDef.h"

class QButtonGroup;
class QToolButton;
class QLabel;
namespace BS_2DVision_Template
{
    class CShapesWidget;
    class CIdToolButton;
    class CShapeTemplateEditWidget : public ITemplateEditWidget
    {
        Q_OBJECT
    public:
        CShapeTemplateEditWidget(QVector<CShapes>& p_shapes, QWidget *p_pParent = nullptr);
        ~CShapeTemplateEditWidget();

    protected slots:
        void normalLayout();
        void onlyEditWidgetLayout(QWidget *p_widget);
        void shapesBtnContextMenu(CIdToolButton *pShapesBtn);

    protected slots:
        void newShapes();
        void updateShapes();
        void currIdChanged(int p_curr);

    protected:
        void init();

    protected:
        bool eventFilter(QObject *obj, QEvent *e) override;

    protected:
        QWidget *m_pToolBar = nullptr;
        QToolButton *m_pNewShapesBtn = nullptr;
        QVector<CShapes>& m_shapesList;
        QList<CIdToolButton *> m_pShapesBtns;
        CShapesWidget *m_pShapeWidget = nullptr;
        QButtonGroup* m_pButtonGroup = nullptr;
        int m_selectId = -1;
    };
}