#pragma once

# include <stdexcept>

class MutexException : public std::runtime_error
{
public:
	MutexException(const char *msg) :
		runtime_error(msg) {}
	MutexException(const std::string &msg) :
		runtime_error(msg) {}
	virtual ~MutexException() throw() {}
};

class ThreadException : public std::runtime_error
{
public:
	ThreadException(const char *msg) :
		runtime_error(msg) {}
	ThreadException(const std::string &msg) :
		runtime_error(msg) {}
	virtual ~ThreadException() throw() {}
};