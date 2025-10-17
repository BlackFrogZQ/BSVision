#include "templateHelper.h"
#include "pluginCoreDef.h"

#include "tool/saveFile.h"
#include "para/paraManager.h"
#include "service/serviceManager.h"
#include "service/typeRegistrar.h"

#include <QRegularExpression>
#include <QPainter>
namespace BS_2DVision_Template
{
    QImage CTemplateHelper::matchResultImage() const
    {
        return m_image;
    }

    QRectF CTemplateHelper::norRoi() const
    {
        return m_norRoi;
    }

    QVector<CMatchResult> CTemplateHelper::matchResult() const
    {
        return m_matchResult;
    }

    QString CTemplateHelper::errorMsg() const
    {
        return m_errorMsg;
    }

    bool CTemplateHelper::isChanged() const
    {
        return m_oldUsedGlobalMark != m_usedGlobalMark ||
               m_oldGlobalMark != m_globalMark ||
               m_oldMarks != m_marks ||
               templateDatas() != m_oldTemplateData;
    }

    void CTemplateHelper::cancel()
    {
        if (isChanged())
        {
            m_usedGlobalMark = m_oldUsedGlobalMark;
            m_globalMark = m_oldGlobalMark;
            m_marks = m_oldMarks;
            assert(m_oldTemplateData.keys() == m_templateMap.keys() && "模板数据和支持的数据是一致的");
            for (auto key : m_oldTemplateData.keys())
            {
                m_templateMap[key]->reset();
                m_templateMap[key]->loadBinary(m_oldTemplateData[key]);
            }
            emit sigLoadFromFile();
        }
    }

    bool CTemplateHelper::saveAs(QString p_file)
    {
        int ver = 0;
        QByteArray bytes;
        auto nowDatas = templateDatas();
        if (CBSToolFun::saveToBytes(ver, bytes, QIODevice::WriteOnly) &&
            CBSToolFun::saveToBytes(m_globalMark, bytes, QIODevice::Append) &&
            CBSToolFun::saveToBytes(m_usedGlobalMark, bytes, QIODevice::Append) &&
            CBSToolFun::saveToBytes(m_marks, bytes, QIODevice::Append) &&
            CBSToolFun::saveToBytes(nowDatas, bytes, QIODevice::Append) &&
            CBSToolFun::saveBytes(bytes, p_file, "BS_2DVision_Template::CTemplateHelper"))
        {
            m_oldMarks = m_marks;
            m_oldGlobalMark = m_globalMark;
            m_oldUsedGlobalMark = m_usedGlobalMark;
            m_oldTemplateData = nowDatas;
            m_currFile = p_file;
            return true;
        };
        return false;
    }

    bool CTemplateHelper::load(QString p_file)
    {
        QByteArray bytes;
        if (CBSToolFun::loadBytes(bytes, p_file, "BS_2DVision_Template::CTemplateHelper"))
        {
            int pos = 0;
            int ver = 0;
            if (CBSToolFun::loadFormBytes(ver, bytes, pos))
            {
                QVector<CMarkPoint> marks;
                CMarkPoint globalMark;
                bool usedGlobalMark = false;
                bool successful = false;
                switch (ver)
                {
                case 0:
                    successful = CBSToolFun::loadFormBytes(globalMark, bytes, pos) &&
                                 CBSToolFun::loadFormBytes(usedGlobalMark, bytes, pos) &&
                                 CBSToolFun::loadFormBytes(marks, bytes, pos);
                    break;

                default:
                    break;
                }
                if (successful)
                {
                    auto fileTypes = globalMark.templateParas.keys();
                    // 缺少补齐
                    for (auto key : m_templateMap.keys())
                    {
                        if (!globalMark.templateParas.contains(key))
                        {
                            globalMark.templateParas.insert(key, QVariant());
                            for (auto &mark : marks)
                            {
                                mark.templateParas.insert(key, QVariant());
                            }
                        }
                    }

                    if (globalMark.templateParas.keys() != m_templateMap.keys())
                    {
                        auto intToStr = [](const int &type)
                        {
                            return iToStr(type);
                        };
                        m_errorMsg =
                            tr("There are differences in the types of files(%1) and supported templates(%2)")
                                .arg(listToString(fileTypes, intToStr).join(", "))
                                .arg(listToString(m_templateMap.keys(), intToStr).join(", "));
                        return false;
                    }

                    QMap<int, QByteArray> templateData;
                    if (!CBSToolFun::loadFormBytes(templateData, bytes, pos))
                    {
                        m_errorMsg = tr("Loading template data error") + ".";
                        return false;
                    }
                    assert(fileTypes == templateData.keys() && "模板数据类型字段和mark类型字段一致");

                    // 缺少补齐
                    for (auto key : m_templateMap.keys())
                    {
                        if (!templateData.contains(key))
                        {
                            templateData.insert(key, QByteArray());
                        }
                    }

                    assert(templateData.keys() == m_oldTemplateData.keys() && templateData.keys() == m_templateMap.keys() && "加载模板数据和支持的数据是一致的");

                    for (auto key : templateData.keys())
                    {
                        if (!m_templateMap[key]->loadBinary(templateData[key]))
                        {
                            m_errorMsg = tr("Loading template data error") + " " + QString("Type(%1).").arg(key);
                            cancel();
                            return false;
                        }
                    }

                    m_marks = marks;
                    m_globalMark = globalMark;
                    m_usedGlobalMark = usedGlobalMark;
                    m_oldMarks = marks;
                    m_oldGlobalMark = globalMark;
                    m_oldUsedGlobalMark = usedGlobalMark;
                    m_oldTemplateData = templateData;
                    m_currFile = p_file;
                    emit sigLoadFromFile();
                }
                return successful;
            }
        }
        return false;
    }

    bool CTemplateHelper::init()
    {
        m_templateMap.clear();

        {
            auto enginePluginDir = BS_QT_Framework::CParaManager::GetValue<QString>(cPluginDir) + "/" + __bsVisionEnginePluginDir;
            auto plugins = BS_QT_FlieHelper::CFileHelper::scanfDir(
                enginePluginDir,
                QStringList() << QString("*.dll"), QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks, QDir::Name);
            QRegularExpression regex("^BS.*TemplateEngine(d|rd)?\\.dll$");

            for (auto plugin : plugins)
            {
                if (!regex.match(QFileInfo(plugin).fileName()).hasMatch())
                {
                    continue;
                }

                auto pPluginObj = BS_QT_Service::CServiceManager::loadPluginService(plugin);
                auto pPlugin = qobject_cast<BS_2DVision_Template::ITemplate *>(pPluginObj);
                if (pPlugin != nullptr)
                {
                    pPlugin->init();
                    m_templateMap[pPlugin->type()] = pPlugin;
                    BS_QT_Service::CServiceManager::RegistPluginService(pPluginObj, QString(ITemplateEngineService_Interface_iid) + "_" + pPlugin->typeString());
                }
            }
            Out_SyncLog << tr("Supported template types") +
                               ":{" +
                               listToString(m_templateMap.keys(), [](int i)
                                            { return iToStr(i); })
                                   .join(",") +
                               "}";
            BS_QT_Service::CTypeRegistrar::regAllType();

            // TODO 删除
            m_globalMark.templateType = CTemplateDef::ParaTemplate;
            m_globalMark.templateParas[CTemplateDef::ParaTemplate] = QVariant();
            m_globalMark.templateParas[CTemplateDef::ShapeTemplate] = QVariant();
            m_marks = QVector<CMarkPoint>(5, m_globalMark);
        }

        m_oldTemplateData = templateDatas();
        return true;
    }

    void CTemplateHelper::clearCache()
    {
        m_image = QImage();
        m_errorMsg.clear();
        m_matchResult.clear();
        m_norRoi = QRectF();
        for (auto pTemplate : m_templateMap)
        {
            pTemplate->clearCache();
        }
    }

    QMap<int, QByteArray> CTemplateHelper::templateDatas() const
    {
        QMap<int, QByteArray> datas;
        for (auto key : m_templateMap.keys())
        {
            datas[key] = m_templateMap[key]->saveBinary();
        }
        return datas;
    }

    bool CTemplateHelper::match(const QImage &p_image, int p_markId, int p_count)
    {
        clearCache();
        m_image = p_image;
        if (m_image.isNull())
        {
            m_errorMsg = tr("Invalid Image");
            return false;
        }

        auto mark = m_globalMark;
        if (p_markId != GlobalMarkId)
        {
            if (p_markId >= 0 &&
                p_markId < m_marks.size())
            {
                mark = m_marks[p_markId];
            }
            else
            {
                m_errorMsg = tr("Invalid Mark Id(%1), id range[0,%2)")
                                 .arg(p_markId)
                                 .arg(m_marks.size());
                return false;
            }
        }

        assert(m_templateMap.contains(mark.templateType) && "不支持的模板类型");
        auto pTemplate = m_templateMap[mark.templateType];
        if (mark.usedRoi)
        {
            m_norRoi = mark.roi;
            m_norRoi.moveCenter(mark.roi.center() + QPointF(p_image.width() * 0.5, p_image.height() * 0.5));
        }
        pTemplate->setRoiPara(mark.usedRoi, m_norRoi);
        pTemplate->setMarkParas(mark.templateParas[mark.templateType], p_count);
        if (pTemplate->match(p_image))
        {
            m_matchResult = pTemplate->matchResult();
        }
        m_errorMsg = pTemplate->errorMsg();
        return m_matchResult.size() > 0;
    }
}

#include "system/camera.h"
namespace BS_2DVision_Template
{
    bool CTemplateHelper::grabImageMatch(int p_markId, int p_count)
    {
        bool isAcquire = visionCamera()->isAcquire();
        auto paras = visionCamera()->paras();
        bool result = singleFrameMatch(p_markId, p_count);
        if (isAcquire && !visionCamera()->isAcquire())
        {
            visionCamera()->setExposureTime(paras.exposureTime.value);
            visionCamera()->setGainRaw(paras.gainRaw.value);
            visionCamera()->setGamma(paras.gamma.value);
            visionCamera()->acquireChange();
        }
        return result;
    }

    bool CTemplateHelper::singleFrameMatch(int p_markId, int p_count)
    {
        if (visionCamera()->isAcquire())
        {
            if (!visionCamera()->acquireChange())
            {
                m_errorMsg = tr("Failed to stop camera acquisition, unable to take photos for recognition.") + QString(" Error:%1").arg(visionCamera()->error());
                return false;
            }
        }
        if (!(visionCamera()->isConnected() && visionCamera()->singleFrame()))
        {
            m_errorMsg = tr("The camera fails to capture and cannot obtain image recognition.") + QString(" Error:%1").arg(visionCamera()->error());
            return false;
        }
        auto image = visionCamera()->lastImage();
        visionCamera()->clearLastImage();
        return match(image, p_markId, p_count);
    }
}

#include "templateCreater.h"
BS_2DVision_Template::CTemplateHelper *templateHelper()
{
    return BS_2DVision_Template::CTemplateCreater::templateInstance();
}
