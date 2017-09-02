#include "stdafx.h"
#include "spdlogm.h"

std::shared_ptr<spdlog::sinks::daily_file_sink_mt> cpdlog::dsink = nullptr;
std::shared_ptr<spdlog::async_logger> cpdlog::alogger = nullptr;
std::shared_ptr<spdlog::sinks::wincolor_stdout_sink_mt> cpdlog::console = nullptr;
cpdlog::loglevel cpdlog::loglevels;

cpdlog pdlog;
