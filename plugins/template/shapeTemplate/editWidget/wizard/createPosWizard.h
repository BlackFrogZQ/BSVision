#pragma once
#include <QWizardPage>
class QLabel;
class QComboBox;
class QToolButton;
class QTableWidget;

namespace TIGER_Graphics
{
    class CRegionView;
    class IFitToolItem;
};
namespace BS_QT_Ui
{
    class CImageScene;
};
namespace BS_2DVision_Template
{
    class CShape;
    class CCreatePosWizard : public QWizardPage
    {
        Q_OBJECT
    public:
        CCreatePosWizard(QWidget *parent = nullptr);
        ~CCreatePosWizard() = default;
        bool isComplete() const override;
        void initializePage() override;
        void setShape(CShape *pShape);

    protected slots:
        void idChanged(const QModelIndex &current, const QModelIndex &previous);
        void setUpdatePtTable(int p_id, double px, double py);
        void slotFitTool(int p_id);
        void addPt();

    protected:
        TIGER_Graphics::CRegionView *m_pView = nullptr;
        BS_QT_Ui::CImageScene *m_pImageScene = nullptr;
        QVector<TIGER_Graphics::IFitToolItem *> m_toolItems;

        QTableWidget *m_pPosPtTableWidget = nullptr;
        QLabel *m_pTitleLabel = nullptr;
        QToolButton *m_pClearBtn = nullptr;
        QToolButton *m_pAddBtn = nullptr;
        QComboBox *m_pAddtypeTypeCB = nullptr;
        CShape *m_pShape = nullptr;
    };
}