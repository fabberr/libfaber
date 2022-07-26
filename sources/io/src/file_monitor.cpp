/********** Headers **********/

// internal
#include <file_monitor.h>

using file_monitor  = faber::io::file_monitor;
using hash_map_t    = file_monitor::hash_map_t;
using file_info_t   = file_monitor::file_info_t;

/********** file_monitor.cpp **********/

/********** Constructors & Destructor **********/

file_monitor::file_monitor(const std::initializer_list<const std::string_view>&&) {
}

file_monitor::file_monitor(const std::initializer_list<ilist_entry>&&) {
}

/********** Private Member Functions **********/

std::fstream& file_monitor::open_impl(std::string_view filename, std::fstream::openmode mode) {
}

/********** Public Member Functions **********/

std::fstream& file_monitor::open(std::string_view filename) {
}

std::fstream& file_monitor::open(std::string_view filename, std::fstream::openmode mode) {
}

void file_monitor::close(std::string_view filename) {
}

hash_map_t::iterator file_monitor::find(std::string_view filename) const {
}

const file_info_t& file_monitor::operator[](const std::string_view key) const {
}

/** This description does not show up, IntelliSense prefers the display whatever comments it finds on the file that declares this. */
file_info_t& file_monitor::operator[](const std::string_view key) {
}

const hash_map_t& file_monitor::opened_files() const {
}

hash_map_t& file_monitor::opened_files() {
}
