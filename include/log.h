#ifndef LOG_H
#define LOG_H

#include <cstdio>

#define log_(category, ...) printf(category ":%s %d: ", __FILE__, __LINE__); printf(__VA_ARGS__); printf("\n");

#define logv(...) log_("v", __VA_ARGS__);
#define logi(...) log_("\033[42mi\033[0m", __VA_ARGS__);
#define logw(...) log_("\033[43mw\033[0m", __VA_ARGS__);
#define loge(...) log_("\033[41me\033[0m", __VA_ARGS__);

#endif
