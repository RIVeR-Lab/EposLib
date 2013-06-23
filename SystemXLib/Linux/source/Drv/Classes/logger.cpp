#include "logger.h"
#include <sys/time.h>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <errno.h>

struct timeval g_logger_ref_time;
CStdString g_logFileName;

using namespace std;

int log_init(CStdString p_fileName)
{
    g_logFileName = p_fileName;

    struct timeval tv;
    int result = gettimeofday(&tv, NULL);
    if( result == 0 )
        g_logger_ref_time = tv;

    return result;
}

int log_calc_timestamp()
{
    int timestamp = -1;
    struct timeval tv;
    if( gettimeofday(&tv, NULL) == 0 )
        timestamp = (int)(((tv.tv_sec-g_logger_ref_time.tv_sec)*1000) + (tv.tv_usec-g_logger_ref_time.tv_usec)/1000);

    return timestamp;
}

int log_write_file(CStdString p_Msg)
{
    if( g_logFileName.GetLength() > 0 )
    {
        ofstream log_file(g_logFileName.c_str(), ios::out | ios::app);
        if( log_file.good() )
        {
            log_file << p_Msg.c_str() << endl;
            log_file.close();
        }
    }
    return 0;
}

int log_write_file(CStdString p_Msg, unsigned char* p_pData, unsigned long p_dataSize)
{
	if( p_pData == NULL || p_dataSize == 0 )
		return 0;

	if( g_logFileName.GetLength() > 0 )
    {
        ofstream log_file(g_logFileName.c_str(), ios::out | ios::app | ios::binary);
        if( log_file.good() )
        {
            log_file << p_Msg.c_str();
            log_file.write((const char*)p_pData, p_dataSize);
			log_file << endl;

            log_file.close();
        }
    }
    return 0;
}

int log_error(CStdString p_errorMsg)
{
    int timestamp = -1;
    if( (timestamp = log_calc_timestamp() ) != -1 )
    {
        CStdString errorMsg;

        int n=errno;
        errorMsg.Format("%6d ERROR: code #%d: %s, details:%s", timestamp, n, strerror(n),p_errorMsg);
        log_write_file(errorMsg);
        errorMsg += "\r\n";
        printf(errorMsg.c_str());
    }

    return timestamp != -1 ? 0 : -1;
}

int log_info(CStdString p_Msg)
{
    int timestamp = -1;
    if( (timestamp = log_calc_timestamp() ) != -1 )
    {
        CStdString msg;
        msg.Format("%6d info: %s", timestamp, p_Msg.c_str() );
        log_write_file(msg);
        msg += "\r\n";
        printf(msg.c_str());
    }

    return timestamp != -1 ? 0 : -1;
}

int log_warning(CStdString p_Msg)
{
    int timestamp = -1;
    if( (timestamp = log_calc_timestamp() ) != -1 )
    {
        CStdString msg;
        msg.Format("%6d warning: %s\r\n", timestamp, p_Msg.c_str() );
        log_write_file(msg);
        msg += "\r\n";
        printf(msg.c_str());
    }

    return timestamp != -1 ? 0 : -1;
}

int log_debug(CStdString p_Msg)
{
    int timestamp = -1;
    if( (timestamp = log_calc_timestamp() ) != -1 )
    {
        CStdString msg;
        msg.Format("%6d debug: %s", timestamp, p_Msg.c_str() );
        log_write_file(msg);
        msg += "\r\n";
        printf(msg.c_str());
    }

    return timestamp != -1 ? 0 : -1;
}

int log_debug(CStdString p_Msg, unsigned char* p_pData, unsigned long p_dataSize)
{
    int timestamp = -1;
    if( (timestamp = log_calc_timestamp() ) != -1 )
    {
        CStdString msg;
        msg.Format("%6d debug: %s", timestamp, p_Msg.c_str() );
        log_write_file(msg, p_pData, p_dataSize);
        msg += "\r\n";
        printf(msg.c_str());
    }

    return timestamp != -1 ? 0 : -1;
}
