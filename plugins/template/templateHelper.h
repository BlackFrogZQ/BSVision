#pragma once
#include "markParaDef.h"
#include "iTemplate.h"
#include <QMap>
#include <QObject>

namespace BS_2DVision_Template
{
    class CTemplateCreater;
    class CTemplateHelper : public QObject
    {
        Q_OBJECT
        friend class CTemplateCreater;

    public:
        enum
        {
            GlobalMarkId = INT32_MIN
        };
        QImage matchResultImage() const;
        QRectF norRoi() const;
        QVector<CMatchResult> matchResult() const;
        QString errorMsg() const;

        bool isChanged() const;
        void cancel();

    public slots:
        bool saveAs(QString p_file);
        bool load(QString p_file);

        bool init();
        void clearCache();

        bool match(const QImage &p_image, int p_markId = GlobalMarkId, int p_count = 1);
        bool grabImageMatch(int p_markId = GlobalMarkId, int p_count = 1);
        bool singleFrameMatch(int p_markId = GlobalMarkId, int p_count = 1);

    public:
        QMap<int, ITemplate *> m_templateMap;
        QVector<CMarkPoint> m_marks;
        CMarkPoint m_globalMark;
        bool m_usedGlobalMark = false;
        QString m_currFile;

    signals:
        void sigLoadFromFile();

    protected:
        QMap<int, QByteArray> templateDatas() const;

    protected:
        QImage m_image;
        QRectF m_norRoi;
        QString m_errorMsg;
        QVector<CMatchResult> m_matchResult;

        QVector<CMarkPoint> m_oldMarks;
        CMarkPoint m_oldGlobalMark;
        bool m_oldUsedGlobalMark = false;
        QMap<int, QByteArray> m_oldTemplateData;

    protected:
        using QObject::QObject;
        ~CTemplateHelper() = default;
    };
}

BS_2DVision_Template::CTemplateHelper *templateHelper();
