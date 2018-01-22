#include "logprintf_buf.h"

using namespace std;

logprintf_buf::logprintf_buf(logprintf_buf &&o) : logfunc_(o.logfunc_), buffer(std::move(o.buffer))
{

}

logprintf_buf& logprintf_buf::operator=(logprintf_buf &&o)
{
	logfunc_ = o.logfunc_;
	buffer = std::move(o.buffer);
	return *this;
}

streamsize logprintf_buf::xsputn(const char* s, streamsize n)
{
	streamsize start = 0;
	for (std::streamsize i = 0; i < n; i++)
	{
		if (s[i] == '\n')
		{
			buffer.append(s + start, s + i);
			sync();
			start = i + 1;
		}
	}
	buffer.append(s + start, s + n);
	return n;
}

int logprintf_buf::overflow(int c)
{
	char ch = traits_type::to_char_type(c);
	if (ch == '\n')
	{
		sync();
	} else {
		buffer.append(1, c);
	}
	return c;
}

int logprintf_buf::sync()
{
	logfunc_("%s", buffer.c_str());
	buffer.clear();
	return 0;
}
