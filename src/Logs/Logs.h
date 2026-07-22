#ifndef LOGS
#define LOGS

// #define DO_LOGS

#ifdef DO_LOGS

#include "spdlog/spdlog.h"
#include <spdlog/sinks/basic_file_sink.h>
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <string>

class Logs
{
public:
    Logs(const Logs &) = delete;
    Logs(Logs &&) = delete;
    Logs &operator=(const Logs &) = delete;
    Logs &operator=(Logs &&) = delete;

    static Logs &GetInstance()
    {
        static Logs instance;

        return instance;
    }

    void print_pc_status()
    {
        pc_status->info("{} {} {}", instruction, adressMode, message);
        instruction.clear();
        adressMode.clear();
        message.clear();
    }

    std::shared_ptr<spdlog::logger> pc_status;
    std::shared_ptr<spdlog::logger> registers;
    std::shared_ptr<spdlog::logger> ram;

    std::string instruction;
    std::string adressMode;
    std::string message;

private:
    Logs()
    {
        spdlog::init_thread_pool(8192, 3);
        pc_status = spdlog::basic_logger_mt<spdlog::async_factory>("pc status", "logs/instructions.txt");
        registers = spdlog::basic_logger_mt<spdlog::async_factory>("registers", "logs/registers.txt");
        ram = spdlog::basic_logger_mt<spdlog::async_factory>("ram", "logs/ram.txt");
        spdlog::flush_on(spdlog::level::err);
        spdlog::set_pattern("[%f us] [%l] %v");
        message.reserve(60);
    }
    ~Logs() = default;
};

#endif

#endif