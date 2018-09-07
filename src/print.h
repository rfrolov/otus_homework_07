#pragma once

#include <vector>
#include <fstream>
#include <ctime>

/// Интерфейс принтера.
struct IPrinter {
    /**
     * Выводит пулл команд.
     * @param cmd_pool Пулл команд.
     */
    virtual void print_cmd_pool(const std::vector<std::string> &cmd_pool) = 0;

    /**
     * Устанавливает время первой команды.
     * @param time Время первой команды.
     */
    virtual void set_first_cmd_time(std::time_t time) = 0;
};

/// Класс печати пула команд в консоль.
struct PrintConsole : public IPrinter {

    /**
     * Конструктор.
     * @param os Ссылка на поток вывода.
     */
    explicit PrintConsole(std::ostream &os = std::cout) : m_os{&os} {}

    /**
     * Выводит пулл команд.
     * @param cmd_pool Пулл команд.
     */
    void print_cmd_pool(const std::vector<std::string> &cmd_pool) override {
        *m_os << "Bulk: ";
        for (const auto &cmd : cmd_pool) {
            *m_os << cmd << (&cmd != &cmd_pool.back() ? ", " : "");
        }
        *m_os << std::endl;
    }

    /**
     * Устанавливает время первой команды (заглушка).
     * @param time Время первой команды.
     */
    void set_first_cmd_time(std::time_t time) override {}

private:
    std::ostream *m_os{};
};


/// Класс печати пула команд в файл.
struct PrintFile : public IPrinter {

    ~PrintFile() {
        fs_.close();
    }

    /**
     * Выводит пулл команд.
     * @param cmd_pool Пулл команд.
     */
    void print_cmd_pool(const std::vector<std::string> &cmd_pool) override {
        if (!fs_.is_open()) { return; }

        fs_ << "Bulk: ";
        for (const auto &cmd : cmd_pool) {
            fs_ << cmd << (&cmd != &cmd_pool.back() ? ", " : "");
        }
        fs_ << std::endl;
    }

    /**
     * Устанавливает время первой команды.
     * @param time Время первой команды.
     */
    void set_first_cmd_time(std::time_t time) override {
        std::string file_name = "bulk" + std::to_string(time) + ".log";
        if (fs_.is_open()) { return; }
        fs_.open(file_name, std::ios::out);
    }

private:
    std::fstream fs_{};
};