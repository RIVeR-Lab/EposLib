#ifndef ___LOGGER___H
#define ___LOGGER___H

#include <StdString.h>

int log_init(CStdString p_fileName);
int log_error(CStdString p_msg);
int log_info(CStdString p_msg);
int log_warning(CStdString p_msg);
int log_debug(CStdString p_msg);
int log_debug(CStdString p_Msg, unsigned char* p_pData, unsigned long p_dataSize);

#endif
