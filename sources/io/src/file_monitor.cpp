/********** Headers **********/

// C++ stdlib
#include <iostream>     // std::cerr
#include <stdexcept>    // std::runtime_error

// C stdlib
#include <cstring> // std::memcpy

// internal
#include <file_monitor.h>
#include <io.h>

using file_monitor  = faber::io::file_monitor;
using hashtable_t   = file_monitor::hashtable_t;
using file_info_t   = file_monitor::file_info_t;

/********** file_monitor.cpp **********/

/********** Constructors & Destructor **********/

file_monitor::file_monitor(std::initializer_list<const char*> ilist) {
    for (const auto& entry : ilist) {
        open(entry);
    }
}

file_monitor::file_monitor(std::initializer_list<ilist_entry> ilist) {
    for (const auto& [filename, mode] : ilist) {
        open(filename, mode);
    }
}

/********** Private Member Functions **********/

std::fstream& file_monitor::open_impl(const char* filename, std::fstream::openmode mode) {

    if (const auto it = find(filename); it != _opened_files.cend()) {
        throw std::runtime_error{"error: file already opened on monitor."};
    }
    
    auto f_ptr = std::make_unique<std::fstream>(filename, mode);
    if (not *f_ptr) {
        throw std::runtime_error{"error: could not open file"};
    }

    // Transfer ownership of pointer to a file_info_t temporary which will be constructed
    // in place as a new value in the hash table with `filename` as its key.
    // `std::unordered_map::emplace` returns a pair containing an iterator to the possibly
    // newly inserted kv-pair and a boolean. This boolean will be set to true if the value
    // was actually inserted, and false if the key was already present.
    // Values `inserted` and `key` below are unused but required for destructuring.
    [[maybe_unused]] const auto& [iterator, inserted] = _opened_files.emplace(filename, file_info_t{ std::move(f_ptr), mode, faber::io::filesize(*f_ptr) });
    [[maybe_unused]] const auto& [key, info] = *iterator;
    return *info.stream;

    // The alternative is this cursed one-liner with poor readability. Performance losses are practically nonexistent.
    // return *_opened_files.emplace(filename, file_info_t{ std::move(f_ptr), mode, faber::io::filesize(*f_ptr) }).first->second.stream;
}

/********** Public Member Functions **********/

std::fstream& file_monitor::open(const char* filename) {
    return open_impl(filename, (std::fstream::in | std::fstream::out));
}

std::fstream& file_monitor::open(const char* filename, std::fstream::openmode mode) {
    return open_impl(filename, mode);
}

void file_monitor::close(std::string_view filename) {
}

hashtable_t::const_iterator file_monitor::find(std::string_view filename) const {
    return _opened_files.find(filename);
}

const file_info_t& file_monitor::operator[](const std::string_view key) const {
    return _opened_files.at(key);
}

file_info_t& file_monitor::operator[](const std::string_view key) {
}

const hashtable_t& file_monitor::data() const {
}

hashtable_t& file_monitor::data() {
}
