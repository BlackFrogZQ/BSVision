#pragma once
#include <QtCore/qglobal.h>

#ifdef _WIN32
#  if defined(BS_SHARED) || !defined(BS_STATIC)
#    ifdef BS_STATIC
#      error "Both BS_SHARED and BS_STATIC defined, please make up your mind"
#    endif
#    ifndef BS_SHARED
#      define BS_SHARED
#    endif
#    if defined(BS_BUILD_PLUGIN)
#      define BS_Dxf_EXPORT Q_DECL_EXPORT
#    else
#      define BS_Dxf_EXPORT Q_DECL_IMPORT
#    endif
#  else
#    define BS_Dxf_EXPORT
#  endif
#endif