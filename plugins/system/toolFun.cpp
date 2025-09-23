#include "toolFun.h"
#include "pluginCoreDef.h"

#include "tool/saveFile.h"
#include "para/paraManager.h"
#include "fileHelper/appHelper.h"

#include <QPainter>
using namespace BS_QT_Framework;
using namespace BS_QT_FlieHelper;
QString saveImageToSysDir(const QImage &p_image,
                          QString p_dirName,
                          bool p_outHint,
                          QList<TIGER_Graphics::CGraphicsRegion> p_drawRegions,
                          QRectF p_roi)
{
    const auto dateTime = QDateTime::currentDateTime();
    const auto cPath = CParaManager::GetValue<QString>(cSaveImagePath, QString("%1/saveImage").arg(CAppFileHelper::appDirPath()));
    const auto cImageFile = QString("%1/%2/%3.%4")
                                .arg(cPath + (p_dirName.isEmpty() ? "" : ("/" + p_dirName)))
                                .arg(dateTime.toString("yyyy.MM.dd"))
                                .arg(dateTime.toString("hh.mm.ss.zzz"))
                                .arg(cImageType);

    const auto cMaxCount = CParaManager::GetValue<int>(cSaveImageMaxCount, 500);
    const auto cMaxTime = dateTime.addDays(-CParaManager::GetValue<int>(cSaveImageMaxTime, 30));

    auto removeImageFiles = [](const QString &p_dir, int p_maxFile, QDateTime p_beforeTimes)
    {
        CFileHelper::removeObsoleteFiles(p_dir, QString(cImageType), p_maxFile, p_beforeTimes, true);
        CFileHelper::removeEmptyDirs(p_dir, false);
    };

    auto drawRegionsFun = [drawRegions = p_drawRegions, roi = p_roi, roiColor = CParaManager::getVarColor(cRoiItemLineColor)](const QImage &p_image)
    {
        auto image = p_image.format() == QImage::Format_RGB888 ? p_image : p_image.convertToFormat(QImage::Format_RGB888);
        QPainter painter(&image);
        for (const auto &region : drawRegions)
        {
            if (!region.points.isEmpty())
            {
                auto pts = region.points.toVector();
                painter.setPen(region.color);
                painter.drawPoints(pts.constData(), pts.size());
            }
        }
        if (roi.isValid())
        {
            painter.setPen(roiColor);
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(roi.toRect());
        }
        return image;
    };

    CBSToolFun::threadSaveImage(p_image,
                                cImageFile,
                                p_outHint,
                                drawRegionsFun,
                                std::bind(removeImageFiles, cPath, cMaxCount, cMaxTime));
    return cImageFile;
}
