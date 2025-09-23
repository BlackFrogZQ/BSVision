#pragma once
#include "bsBasic.h"
#include "message/msgManager.h"

#define qToUtf8_data(str) qToUtf8(str).data()

#define pluginTrace(qDebugMsg) BSTrace("BS_Debug_Plugin").noquote() << "pluginTrace:" << qDebugMsg

void initCamera();
void updateCameraParas();
void releaseCamera();
