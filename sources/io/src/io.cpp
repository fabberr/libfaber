/********** Headers **********/

// C++ stdlib
#include <iostream> 	// streams
#include <fstream> 		// file streams
#include <system_error> // std::error_code

// internal
#include <io.h>

/********** API Implementation **********/

bool faber::io::read_file(const fs::path& filename, std::function<bool(std::ifstream&)>& read_fn) {
	faber::io::invoke_read_or_write(filename, read_fn, std::ios_base::in);
}

bool faber::io::write_file(const fs::path& filename, std::function<bool(std::ofstream&)>& write_fn) {
	faber::io::invoke_read_or_write(filename, write_fn, std::ios_base::out);
}

bool faber::io::try_mkdirs(const fs::path& dir) {
	std::error_code ec{};
	if (not fs::create_directories(dir, ec)) {
		std::cerr <<
			"[ERROR] `faber::io::try_mkdirs`: Failed to create directory(ies)\n"
			"\tReason: " << ec.message() << "\n"
		<< std::endl;
		return false;
	}
	return true;
}
