#pragma once

#include <iostream>
#include <format>
#include <fstream>
#include <sstream>
#include <memory>
#include <mutex>


template<bool Enabled, int32_t Num>
struct BaseLogger {
    static void set_stream_to_file(std::string filename) { 
        auto fs = std::make_unique<std::ofstream>(filename);
        if (fs->is_open()) {
            file = std::move(fs);
            os = file.get();
        } else {
            std::cerr << "Fail to open file: " << filename << '\n';
            os = &std::cout;
            file.reset();
        }
    }

    template<typename... Args>
    static inline void log(std::format_string<Args...> fmt, Args&&... args) {
        if constexpr (Enabled) {
            std::ostringstream msg;
            for (int32_t i = 0; i < indent; ++i) msg << "    ";
            msg << "> "
                << std::format(fmt, std::forward<Args>(args)...)
                << '\n';

            std::ostream& out = (is_buffering ? buffer : *os);
            out << msg.str();

            if (indent < cout_limit) std::cout << msg.str();
        }
    }

    static void in () { indent++; }
    static void out() { indent > 0 ? indent-- : indent; }

    static int32_t get_indent() { return indent; }
    static void    set_indent(int32_t new_indent) { new_indent >= 0 ? indent = new_indent : indent; }

    static int32_t get_cout_limit() { return cout_limit; }
    static void    set_cout_limit(int32_t new_limit) { new_limit >= 0 ? cout_limit = new_limit : cout_limit; }
    
    static void flush() {
        if constexpr (Enabled) {
            if (buffer.tellp() < 1) return;
            std::unique_lock<std::mutex> lock(flush_mutex);
            std::ostream& out = *os;
            out << buffer.str() << std::flush;
            buffer.str("");
            buffer.clear();
        }
    }

    static void buff_on()  {            is_buffering = true;  }
    static void buff_off() { flush(); is_buffering = false; }

private:
    static inline thread_local int32_t indent = 0;
    static inline thread_local bool is_buffering = false;

    static inline thread_local std::ostringstream buffer;

    static inline int32_t cout_limit = 1;

    static inline std::mutex flush_mutex;

    static inline std::ostream* os = &std::cout;
    static inline std::unique_ptr<std::ofstream> file;
};

using Logger     = BaseLogger<true,1>;
using DrawLogger = BaseLogger<true,2>;
