#pragma once
#include <spdlog/spdlog.h>
#include <memory>
#include <limits>
#include <fmt/format.h>
#include <fmt/printf.h>

#if !defined(PDLOG_DISABLE_TRACE)
#define PDLOG_TRACE
#endif

#if !defined(PDLOG_DISABLE_DEBUG)
#define PDLOG_DEBUG
#endif

#if !defined(PDLOG_DISABLE_INFO)
#define PDLOG_INFO
#endif

#if !defined(PDLOG_DISABLE_WARN)
#define PDLOG_WARN
#endif

#if !defined(PDLOG_DISABLE_ERR)
#define PDLOG_ERR
#endif

#if !defined(PDLOG_DISABLE_CRITICAL)
#define PDLOG_CRITICAL
#endif


class cpdlog
{
public:
	cpdlog()
	{

	}
	struct loglevel
	{
		bool _bTrace = true;
        bool _bDebug = true;
        bool _bInfo = true;
		bool _bWarn = true;
		bool _bErr = true;
		bool _bCritical = true;
	};

	static std::shared_ptr<spdlog::sinks::daily_file_sink_mt> dsink;
	static std::shared_ptr<spdlog::async_logger> alogger;
	static std::shared_ptr<spdlog::sinks::wincolor_stdout_sink_mt> console;

	static loglevel loglevels;

};

extern cpdlog pdlog;

static std::string extract_name(std::string file)
{
	std::string name = file;
	std::string::size_type pos = name.find_last_of("/\\");

	return (pos != std::string::npos) ? name.substr(pos + 1) : name;
}

template<typename... Args>
static void spdlog_low_level_formatter(spdlog::level::level_enum LogLevel, const std::string func, std::string file, size_t line, std::string fmt, const Args & ... args)
{
	try
	{
		//check if log level enabled
		switch (LogLevel)
		{
		case spdlog::level::trace:
		{
			if (!pdlog.loglevels._bTrace) return;
			break;
		}
		case spdlog::level::debug:
		{
			if (!pdlog.loglevels._bDebug) return;
			break;
		}
		case spdlog::level::info:
		{
			if (!pdlog.loglevels._bInfo) return;
			break;
		}
		case spdlog::level::warn:
		{
			if (!pdlog.loglevels._bWarn) return;
			break;
		}
		case spdlog::level::err:
		{
			if (!pdlog.loglevels._bErr) return;
			break;
		}
		case spdlog::level::critical:
		{
			if (!pdlog.loglevels._bCritical) return;
			break;
		}
		case spdlog::level::off:
		{
			return;
			break;
		}
		default:
			break;
		}

		fmt::MemoryWriter w;
		if ((LogLevel == spdlog::level::trace) ||
			(LogLevel == spdlog::level::debug) ||
			(LogLevel == spdlog::level::critical))
			w.write("[{}] [{}:{}] ", func, extract_name(file), line);
		if (LogLevel == spdlog::level::err)
			w.write("[{}:{}] ", extract_name(file), line);
		w.write(fmt, args...);
		pdlog.alogger->log(LogLevel, w.str().c_str());
	}
	catch (...)
	{
		//exception in log function, probably something is going wrong
	}
}

#ifdef PDLOG_TRACE
#define LOG_TRACE(message) spdlog_low_level_formatter(spdlog::level::trace, __FUNCSIG__, __FILE__, __LINE__, "{}", message)
#define LOG_TRACEF(message, ...) spdlog_low_level_formatter(spdlog::level::trace, __FUNCSIG__, __FILE__, __LINE__, message, __VA_ARGS__)
#define LOG_TRACEC LOG_TRACE(".")
#else
#define LOG_TRACE(...)
#define LOG_TRACEF(message, ...)
#define LOG_TRACEC
#endif


#ifdef PDLOG_DEBUG
#define LOG_DEBUG(message) spdlog_low_level_formatter(spdlog::level::debug, __FUNCSIG__, __FILE__, __LINE__, "{}", message)
#define LOG_DEBUGF(message, ...) spdlog_low_level_formatter(spdlog::level::debug, __FUNCSIG__, __FILE__, __LINE__, message, __VA_ARGS__)
#else
#define LOG_DEBUG(...)
#define LOG_DEBUGF(message, ...)
#endif


#ifdef PDLOG_INFO
#define LOG_INFO(message) spdlog_low_level_formatter(spdlog::level::info, __FUNCSIG__, __FILE__, __LINE__, "{}", message)
#define LOG_INFOF(message, ...) spdlog_low_level_formatter(spdlog::level::info, __FUNCSIG__, __FILE__, __LINE__, message, __VA_ARGS__)
#else
#define LOG_INFO(...)
#define LOG_INFOF(message, ...)
#endif

#ifdef PDLOG_WARN
#define LOG_WARN(message) spdlog_low_level_formatter(spdlog::level::warn, __FUNCSIG__, __FILE__, __LINE__, "{}", message)
#define LOG_WARNF(message, ...) spdlog_low_level_formatter(spdlog::level::warn, __FUNCSIG__, __FILE__, __LINE__, message, __VA_ARGS__)
#else
#define LOG_WARN(...) 
#endif

#ifdef PDLOG_ERR
#define LOG_ERROR(message) spdlog_low_level_formatter(spdlog::level::err, __FUNCSIG__, __FILE__, __LINE__, "{}", message)
#define LOG_ERRORF(message, ...) spdlog_low_level_formatter(spdlog::level::err, __FUNCSIG__, __FILE__, __LINE__, message, __VA_ARGS__)
#else
#define LOG_ERROR(...) 
#define LOG_ERRORF(message, ...)
#endif

#ifdef PDLOG_CRITICAL
#define LOG_CRITICAL(message) spdlog_low_level_formatter(spdlog::level::critical, __FUNCSIG__, __FILE__, __LINE__, "{}", message)
#define LOG_CRITICALF(message, ...) spdlog_low_level_formatter(spdlog::level::critical, __FUNCSIG__, __FILE__, __LINE__, message, __VA_ARGS__)
#else
#define LOG_CRITICAL(...)
#endif
