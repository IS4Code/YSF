#ifndef YSF_LOGPRINTF_BUF_H
#define YSF_LOGPRINTF_BUF_H

#include <string>
#include <sstream>

class logprintf_buf : public std::streambuf
{
	typedef void(*logprintf_t)(char* format, ...);
	logprintf_t logfunc_;

	std::string buffer;
public:
	logprintf_buf(logprintf_t logfunc) : logfunc_(logfunc)
	{

	}

	logprintf_buf(const logprintf_buf &) = delete;
	logprintf_buf(logprintf_buf &&);
	logprintf_buf& operator=(const logprintf_buf &) = delete;
	logprintf_buf& operator=(logprintf_buf &&);

	virtual std::streamsize xsputn(const char* s, std::streamsize n) override;
	virtual int overflow(int c) override;
	virtual int sync() override;
};

#endif
