#include "shapeTemplate.h"
#include "editWidget/editWidget.h"
#include "para/paraManager.h"
#include "pluginCoreDef.h"
#include <QResource>
namespace BS_2DVision_Template
{
    bool CShapeTemplate::init()
    {
        return QResource::registerResource(
            BS_QT_Framework::CParaManager::GetValue<QString>(cPluginDir)+
            QString("/%1.rcc")
            .arg(__bsShapeTemplateRcc));
    }

    int CShapeTemplate::type() const
    {
        return CTemplateDef::ShapeTemplate;
    }

    QString CShapeTemplate::typeString() const
    {
        return QObject::tr("ShapeTemplate");
    }

    ITemplateEditWidget *CShapeTemplate::newEditWidget(QWidget *p)
    {
        return new CShapeTemplateEditWidget(m_shapes, p);
    }

    IMarkTemplateParaWidget *CShapeTemplate::newMarkParaWidget(QWidget *p)
    {
        auto pMarkParaWidget = new CShapeTemplateMarkParaWidget(p);
        connect(this,
                &CShapeTemplate::sigShapeCountChanged,
                pMarkParaWidget,
                &CShapeTemplateMarkParaWidget::slotModelCountChanged, Qt::QueuedConnection);
        return pMarkParaWidget;
    }

    void CShapeTemplate::setRoiPara(bool p_used, QRectF p_roi)
    {
        m_roi = p_used ? p_roi.toRect() : QRect();
    }

    void CShapeTemplate::setMarkParas(QVariant p_paras, int p_maxCount)
    {
        assert(p_paras.canConvert<BS_2DVision_Template::CShapeTemplateMarkParas>() && "shapeMarkParas类型检查");
        setMarkParas(p_paras.value<BS_2DVision_Template::CShapeTemplateMarkParas>(), p_maxCount);
    }

    void CShapeTemplate::setMarkParas(const CShapeTemplateMarkParas &p_paras, int p_maxCount)
    {
        assert(p_maxCount > 0 && "匹配个数应>0");
        m_paras = p_paras;
        m_maxCount = p_maxCount;
    }

    bool CShapeTemplate::match(const QImage &p_image)
    {
        clearCache();
        return false;
    }

    QVector<CMatchResult> CShapeTemplate::matchResult() const
    {
        return m_results;
    }

    QString CShapeTemplate::errorMsg() const
    {
        return m_errorMsg;
    }

    void CShapeTemplate::reset()
    {
        if (!m_shapes.isEmpty())
        {
            emit sigShapeCountChanged(0, m_shapes.size());
            m_shapes.clear();
        }
    }

    void CShapeTemplate::clearCache()
    {
        m_results.clear();
        m_errorMsg.clear();
    }
}

namespace BS_2DVision_Template
{
    QByteArray CShapeTemplate::saveBinary() const
    {
        return QByteArray();
    }

    bool CShapeTemplate::loadBinary(const QByteArray &p_datas)
    {
        return true;
    }
}
