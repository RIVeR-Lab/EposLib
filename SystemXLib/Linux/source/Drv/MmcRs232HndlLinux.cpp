#include <Drv/Rs232/MmcRs232HndlLinux.h>
#include <memory.h>
#include <string.h>
#include <fstream>
#include <dirent.h>
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <time.h>
#include "Classes/logger.h"

using namespace std;

//#define _LOG_RS232_ACTIVITY

CMmcRs232HndlBase::CMmcRs232HndlBase()
{
	m_Handle = 0;
	m_oFirstInit = false;
#ifdef _LOG_RS232_ACTIVITY
	log_init("/var/log/eposcmd.log");
#endif //_LOG_RS232_ACTIVITY
}

CMmcRs232HndlBase::~CMmcRs232HndlBase()
{
}

int msleep(unsigned long milisec)
{
    struct timespec req={0},rem={0};

    time_t sec=(int)(milisec/1000);

    milisec=milisec-(sec*1000);

    req.tv_sec=sec;

    req.tv_nsec=milisec*1000000L;

    nanosleep(&req,&rem);

    return 1;
}


bool CMmcRs232HndlBase::Read(unsigned char* pDataBuffer, unsigned int nBytesToRead, unsigned int *pBytesRead)
{
	int counter = 5;
	int offset = 0;
	do
	{
		*pBytesRead = read( m_Handle, pDataBuffer+offset, nBytesToRead);

		if(*pBytesRead==nBytesToRead)
		{
#ifdef _LOG_RS232_ACTIVITY
			log_debug("r:", pDataBuffer+offset, nBytesToRead);
#endif //_LOG_RS232_ACTIVITY
			return true;
		}

		if( *pBytesRead < nBytesToRead && *pBytesRead > 0 )
		{
			unsigned int ulBytesRead = 0;

#ifdef _LOG_RS232_ACTIVITY
	log_debug("r: repeat");
#endif // _LOG_RS232_ACTIVITY


			bool oResult = Read(pDataBuffer+*pBytesRead, nBytesToRead - *pBytesRead, &ulBytesRead);

			if( oResult )
				*pBytesRead += ulBytesRead;

			{
#ifdef _LOG_RS232_ACTIVITY
				log_debug("r: unreached");
#endif // _LOG_RS232_ACTIVITY
			}

			return oResult;
		}

		msleep(10);
		counter--;
#ifdef _LOG_RS232_ACTIVITY
		log_debug("r: wait");
#endif // _LOG_RS232_ACTIVITY
	}
	while( *pBytesRead == 0 && counter > 0 );

	return false;
}

bool CMmcRs232HndlBase::Write(unsigned char* pDataBuffer, unsigned int nBytesToWrite, unsigned int *pBytesWritten)
{
	*pBytesWritten = write(m_Handle, pDataBuffer, nBytesToWrite);

	if( *pBytesWritten == nBytesToWrite )
	{
#ifdef _LOG_RS232_ACTIVITY
		log_debug("w:");
#endif // _LOG_RS232_ACTIVITY
		return true;
	}

	return false;
}

bool CMmcRs232HndlBase::Purge()
{
	struct termios origtty, tty;

	/* store the current tty settings */
	tcgetattr(m_Handle, &origtty);

	// Flush input and output buffers
	tty=origtty;
	tcsetattr(m_Handle, TCSAFLUSH, &tty);

	/* reset the tty to its original settings */
	tcsetattr(m_Handle, TCSADRAIN, &origtty);

#ifdef _LOG_RS232_ACTIVITY
		log_debug("purge");
#endif // _LOG_RS232_ACTIVITY

	return true;
}

bool CMmcRs232HndlBase::SetTimeout(unsigned int dTimeout)
{
	struct termios options;

	/*
	 * Get the current options for the port...
	 */

	tcgetattr(m_Handle, &options);

	options.c_cc[VMIN]  = 0;
	options.c_cc[VTIME] = 10;

	tcsetattr(m_Handle, TCSANOW, &options);

#ifdef _LOG_RS232_ACTIVITY
		CStdString msg;
		msg.Format("set timeout: %ld",dTimeout);
		log_debug(msg);
#endif // _LOG_RS232_ACTIVITY

	return true;
}

bool CMmcRs232HndlBase::SetPortMode(unsigned short wPortMode)
{
	struct termios options;

	/*
	 * Get the current options for the port...
	 */

	tcgetattr(m_Handle, &options);

	options.c_cflag|=CREAD|CLOCAL;
	options.c_lflag&=(~(ICANON|ECHO|ECHOE|ECHOK|ECHONL|ISIG));
	options.c_iflag&=(~(INPCK|IGNPAR|PARMRK|ISTRIP|ICRNL|IXANY));
	options.c_oflag&=(~OPOST);
	options.c_cc[VMIN]=0;
	options.c_cc[VINTR] = _POSIX_VDISABLE;
	options.c_cc[VQUIT] = _POSIX_VDISABLE;
	options.c_cc[VSTART] = _POSIX_VDISABLE;
	options.c_cc[VSTOP] = _POSIX_VDISABLE;
	options.c_cc[VSUSP] = _POSIX_VDISABLE;

	options.c_cflag &=(~CRTSCTS);
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

#ifdef CNEW_RTSCTS
	options.c_cflag &= ~CNEW_RTSCTS;

	options.c_cc[VINTR]    = 0;     /* Ctrl-c */
	options.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
	options.c_cc[VERASE]   = 0;     /* del */
	options.c_cc[VKILL]    = 0;     /* @ */
	options.c_cc[VEOF]     = 4;     /* Ctrl-d */
	options.c_cc[VTIME]    = 0;     /* inter-character timer unused */
	options.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
	//options.c_cc[VMIN]     = 0;     /* blocking read until 1 character arrives */
	options.c_cc[VSWTC]    = 0;     /* '\0' */
	options.c_cc[VSTART]   = 0;     /* Ctrl-q */
	options.c_cc[VSTOP]    = 0;     /* Ctrl-s */
	options.c_cc[VSUSP]    = 0;     /* Ctrl-z */
	options.c_cc[VEOL]     = 0;     /* '\0' */
	options.c_cc[VREPRINT] = 0;     /* Ctrl-r */
	options.c_cc[VDISCARD] = 0;     /* Ctrl-u */
	options.c_cc[VWERASE]  = 0;     /* Ctrl-w */
	options.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
	options.c_cc[VEOL2]    = 0;     /* '\0' */

	tcflush(m_Handle, TCIFLUSH);
#endif
	tcsetattr(m_Handle, TCSANOW, &options);

	tcflush(m_Handle, TCIFLUSH);

#ifdef _LOG_RS232_ACTIVITY
		CStdString msg;
		msg.Format("set port mode: %x",wPortMode);
		log_debug(msg);
#endif // _LOG_RS232_ACTIVITY

	return true;
}

bool CMmcRs232HndlBase::ClosePort()
{
#ifdef _LOG_RS232_ACTIVITY
		log_debug("close");
#endif // _LOG_RS232_ACTIVITY

	if(m_Handle!=-1)
	{
        close(m_Handle);
        m_Handle = -1;

		return true;
	}
	return false;
}

bool CMmcRs232HndlBase::SetPortSettings(unsigned int dBaudrate, unsigned int dTimeout, bool oChangeOnly)
{
	//Do only if changed
	if(!oChangeOnly || (dTimeout != m_dTimeout) || (dBaudrate != m_dBaudrate))
	{
		if(!oChangeOnly || !m_oFirstInit)
		{
			//Configure Buffers
		}

		//Do only if changed
		if(!oChangeOnly || (dTimeout != m_dTimeout))
		{
			if(!SetTimeout(dTimeout))
				return false;
		}

		if(!oChangeOnly || !m_oFirstInit)
		{
			//Configure PortMode
			if(!SetPortMode(m_wPortMode))
				return false;
		}

		//Do only if changed
		if(!oChangeOnly || (dBaudrate != m_dBaudrate))
		{
			struct termios options;

			/*
			 * Get the current options for the port...
			 */

			tcgetattr(m_Handle, &options);

			switch(dBaudrate)
			{
				case k_9600Baud:
				{
                    cfsetispeed(&options, B9600);
					cfsetospeed(&options, B9600);
				} break;
				case k_14400Baud:
				{
					return false;
				} break;
				case k_19200Baud:
				{
                    cfsetispeed(&options, B19200);
					cfsetospeed(&options, B19200);
				} break;
				case k_38400Baud:
				{
                    cfsetispeed(&options, B38400);
					cfsetospeed(&options, B38400);
				} break;
				case k_57600Baud:
				{
                    cfsetispeed(&options, B57600);
					cfsetospeed(&options, B57600);
				} break;
				case k_115200Baud:
				{
                    cfsetispeed(&options, B115200);
					cfsetospeed(&options, B115200);
				} break;
				default:
				{
					dBaudrate = k_115200Baud;
					cfsetispeed(&options, B115200);
					cfsetospeed(&options, B115200);
				}
			}

			tcsetattr(m_Handle, TCSANOW, &options);

			//Baudrate is set
			m_dBaudrate = dBaudrate;

#ifdef _LOG_RS232_ACTIVITY
		CStdString msg;
		msg.Format("set baudrate: %ld",dBaudrate);
		log_debug(msg);
#endif // _LOG_RS232_ACTIVITY
		}

		if(!oChangeOnly || !m_oFirstInit)
		{
			//Purge the buffer
			if(!Purge())
				return false;

			m_oFirstInit = true;
		}
	}

	return true;
}

bool CMmcRs232HndlBase::OpenPort(CStdString strPortName)
{
	m_Handle = open(strPortName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

    if (m_Handle != -1)
	{
		/* clear input & output buffers, then switch to "blocking mode" */
		tcflush(m_Handle, TCOFLUSH);
		tcflush(m_Handle, TCIFLUSH);

		fcntl(m_Handle, F_SETFL, fcntl(m_Handle, F_GETFL) & ~O_NONBLOCK);

#ifdef _LOG_RS232_ACTIVITY
		CStdString msg;
		msg.Format("open port: %s",strPortName);
		log_debug(msg);
#endif // _LOG_RS232_ACTIVITY

		return true;
	}

	return false;
}

vector<CStdString> CMmcRs232HndlBase::InitPort()
{
	DIR           *d;
	struct dirent *dir;
	vector<CStdString> result;

  d = opendir("/dev");
  if (d)
  {
	while ((dir = readdir(d)) != NULL)
	{
		std::string file = dir->d_name;
		std::string filter;

#if defined(_TTY_IRIX_)
		filter="ttyf";
#elif defined(_TTY_HPUX_)
		filter="tty1p";
#elif defined(_TTY_SUN_)
		filter="tty";
#elif defined(_TTY_DIGITAL_)
		filter="tty0";
#elif defined(_TTY_FREEBSD_)
		filter="ttyd";
#else
		filter="ttyS";
#endif

		if( file.find(filter)!=std::string::npos )
			result.push_back(std::string("/dev/") + std::string(dir->d_name));
	}

	closedir(d);
  }

  return result;
}
