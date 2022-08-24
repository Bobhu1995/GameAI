#pragma once
#include <chrono>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>

enum class MY_COLOR
{
    BLACK = 0,
    WHITE,
    RED
};

static time_t get_millis()
{
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
    time_t timestamp = tmp.count();
    return timestamp;
}

static void get_cur_time(char* buf, int len)
{
    time_t now = time(nullptr);
    tm t; 
    localtime_s(&t, &now);
    sprintf_s(buf, len, "%04d-%02d-%02d %02d:%02d:%02d", 1900 + t.tm_year, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
}

static void simple_log(const char* str)
{
    char buf[4096] = { 0 };
    get_cur_time(buf, 4096);
    printf("%s %s\n", buf, str);
}