#pragma once

#include <cstdarg>
#include <cstdio>

class Logger { 
   public:
    // Singleton access
    static Logger& Instance() {
        static Logger logger;
        return logger;
    }

    // printf-style logging method
    void Log(const char* format, ...) {
        constexpr size_t buffersize = 512;
        char buffer[buffersize];

        va_list args;
        va_start(args, format);
        std::vsnprintf(buffer, buffersize, format, args);
        va_end(args);

        printf("%s", buffer);
    }

   private:
    Logger() = default;
};
