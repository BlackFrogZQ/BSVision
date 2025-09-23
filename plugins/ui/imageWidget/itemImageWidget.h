#pragma once
#include "showImageWidget.h"
class QMenu;
class QAction;
class CItemImageWidget : public CShowAutoImageWidget
{
    Q_OBJECT
    Q_PROPERTY(bool pixelLinkage MEMBER m_pixelLinkage WRITE setPixelLinkage READ isPixelLink NOTIFY sigPixelLinkageChanged)
    Q_PROPERTY(bool usedItem MEMBER m_usedItem WRITE setUsedItem READ usedItem NOTIFY sigUsedItemChanged)
    Q_PROPERTY(bool itemsEnable MEMBER m_itemsEnabled WRITE setItemsEnable READ itemsEnabled NOTIFY sigItemsEnableChanged)
public:
    CItemImageWidget(QWidget *p_pParent = nullptr);
    ~CItemImageWidget();
    void addLineItem(double p_offset, Qt::Orientation p_orientation = Qt::Orientation::Horizontal);
    void addItem(BS_QT_Ui::IGraphicsBasicItem *pItem);
    void clearItem();

    bool usedItem() const;
    bool itemsEnabled() const;
    bool isPixelLink() const;

    bool pixelLinkPos(QPoint globalPos, double &x, double &y) const;

public slots:
    void setPixelLinkage(bool p_pixelLinkage);
    void setUsedItem(bool p_used);
    void setItemsEnable(bool p_enable);

signals:
    void sigPixelLinkage(double p_scenePosX, double p_scenePosY);
    void sigPixelLinkageChanged(bool isLink);
    void sigUsedItemChanged(bool isUsed);
    void sigItemsEnableChanged(bool p_enable);

protected slots:
    virtual void slotUpdateItemState();

protected:
    void initItemsContextMenu();
    void loadItemParas();
    void saveItemParas() const;
    void addItem_pri(BS_QT_Ui::IGraphicsBasicItem *pItem);

protected:
    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

protected:
    bool m_pixelLinkage;
    bool m_usedItem;
    bool m_itemsEnabled;
    QMenu* m_pItemsContextMenu;
    QList<QAction *> m_manuItemActions;
    QAction *m_pItemsEnabledAction;
};