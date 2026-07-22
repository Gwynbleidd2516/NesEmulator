#ifndef LOGS
#define LOGS

#include "spdlog/spdlog.h"
#include <spdlog/sinks/basic_file_sink.h>
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

    std::string instruction;
    std::string adressMode;
    std::string message;

private:
    Logs()
    {
        pc_status = spdlog::basic_logger_mt("pc status", "logs/cpu.txt");
        registers = spdlog::basic_logger_mt("registers", "logs/cpu.txt");
    }
    ~Logs() = default;
};

// Logs *Logs::instance = nullptr;

#endif