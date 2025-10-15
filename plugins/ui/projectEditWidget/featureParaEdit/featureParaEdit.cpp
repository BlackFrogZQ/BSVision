#include "featureParaEdit.h"
IFeatureParaEdit::IFeatureParaEdit(QWidget *parent) : QWidget(parent),m_isSave(false)
{
    setWindowFlag(Qt::WindowStaysOnTopHint, CUiBasic::isTop);
}

IFeatureParaEdit::~IFeatureParaEdit()
{
}

bool IFeatureParaEdit::isSave() const
{
    return m_isSave;
}