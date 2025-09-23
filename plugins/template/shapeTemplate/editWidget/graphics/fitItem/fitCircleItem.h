#pragma once
#include "fitDef.h"

namespace BS_QT_Ui
{
    class CCtrlPtItem;
}

namespace TIGER_Graphics
{
    class CEllipseCenterFitTool;
    class CTwoCircleFitTool;

    class CFitCircleItem : public IBasicFitItem
    {
        friend class CEllipseCenterFitTool;
        friend class CTwoCircleFitTool;
        Q_OBJECT

    public:
        enum CAdjustPtFlag : int32_t
        {
            NoneAdjustPt = 0,
            MoveAdjustPt = 1 << 0,
            ExtendMajorAxisAdjustPt = 1 << 1,
            ExtendMinorAxisAdjustPt = 1 << 2,
            ExtendAxisAdjustPt = ExtendMajorAxisAdjustPt | ExtendMinorAxisAdjustPt,
            FitRectHeightAdjustPt = 1 << 3,
            FitRectWidthAdjustPt = 1 << 4,
            FitRectAdjustPt = FitRectHeightAdjustPt | FitRectWidthAdjustPt,
            AllAdjustPt = MoveAdjustPt | ExtendAxisAdjustPt | FitRectAdjustPt
        };
        Q_ENUM(CAdjustPtFlag)
        Q_DECLARE_FLAGS(CAdjustPtFlags, CAdjustPtFlag)
    public:
        CFitCircleItem(QGraphicsItem *parent = nullptr);
        ~CFitCircleItem();

        QPainterPath shape() const override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    public slots:
        void setUsedMinorAxis(bool p_used);
        void setFreeMinorAxis(bool p_free);
        void setRectCount(int p_count);
        void setAdjustPtFlag(CAdjustPtFlag p_flag, bool p_on = true);
        void setAdjustPtFlags(CAdjustPtFlags p_flags);

    protected slots:
        void slotMove(QPointF p_scenePos, int p_buttons);
        void slotHalHeightMove(QPointF p_scenePos, int p_buttons);
        void slotHalWidthMove(QPointF p_scenePos, int p_buttons);
        void slotMajorExtendMove(QPointF p_scenePos, int p_buttons);
        void slotMinorExtendMove(QPointF p_scenePos, int p_buttons);
        void updatePtPos();

    protected:
        QPainterPath drawCirclePath() const;

    protected:
        BS_QT_Ui::CCtrlPtItem *m_pMovePtItem = nullptr;
        BS_QT_Ui::CCtrlPtItem *m_pExtendMajorPtItem = nullptr;
        BS_QT_Ui::CCtrlPtItem *m_pExtendMinorPtItem = nullptr;

        BS_QT_Ui::CCtrlPtItem *m_pRectHalfHeightPtItem = nullptr;
        BS_QT_Ui::CCtrlPtItem *m_pRectHalfWidthPtItem = nullptr;

        // 场景坐标
        QPointF m_centerPt = QPointF(0, 0);
        QPointF m_majorAxisPt = QPointF(100, 0);
        QPointF m_latstPressPt;

        QPointF m_lineNor;
        QPointF m_lineDir;
        double m_fitRectInterval;

        bool m_usedMinorAxis = true;
        bool m_freeMajorAxis = true;

        double m_minorAxisLen = 100;
        double m_majorAxisLen = 100;
        double m_fitRectHalHeight = 20;
        double m_fitRectHalWidth = 5;
        int m_fitRectCount = 5;
        CAdjustPtFlags m_ctrlPtFlags = AllAdjustPt;
    };
    Q_DECLARE_OPERATORS_FOR_FLAGS(CFitCircleItem::CAdjustPtFlags)
}