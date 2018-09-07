#pragma once

#include <cassert>
#include <memory>
#include "print.h"

struct Cmd {
    /**
     * Конструктор.
     * @param block_size Размер блока.
     */
    explicit Cmd(const std::size_t block_size) : m_block_size{block_size} {
        assert(block_size != 0);

        m_printers.emplace_back(std::make_shared<PrintConsole>());
        m_printers.emplace_back(std::make_shared<PrintFile>());
    }

    /// Выводит оставшиеся команды.
    void end() {
        print_pool();
    }

    /**
     * Обрабатывает команду.
     * @param cmd Команда.
     */
    void add_cmd(const std::string &cmd) {
        if (cmd == "{") {
            print_pool();
            ++m_braces_num;
        } else if (cmd == "}") {
            if (m_braces_num) {
                --m_braces_num;
                print_pool();
            }
        } else {
            if (m_cmd_pool.empty()) {
                set_first_cmd_time(std::time(nullptr));
            }
            m_cmd_pool.emplace_back(cmd);
            if (m_cmd_pool.size() == m_block_size) {
                print_pool();
            }
        }
    }

private:
    void print_pool() {
        if (m_cmd_pool.empty() || m_braces_num) {
            return;
        }

        for (const auto &printer:m_printers) {
            printer->print_cmd_pool(m_cmd_pool);
        }

        m_cmd_pool.clear();
    }

    void set_first_cmd_time(std::time_t time) {
        for (const auto &printer:m_printers) {
            printer->set_first_cmd_time(time);
        }
    }

private:
    size_t                                 m_block_size{};
    size_t                                 m_braces_num{};
    std::vector<std::string>               m_cmd_pool{};
    std::vector<std::shared_ptr<IPrinter>> m_printers{};
};