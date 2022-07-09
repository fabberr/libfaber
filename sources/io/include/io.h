#ifndef FABER_IO_H
#define FABER_IO_H

/********** Headers **********/

// C++ stdlib
#include <filesystem> 	// filesystem utilities
#include <iostream> 	// file streams
#include <functional> 	// function objects

/********** io.h **********/

namespace faber { inline namespace v1_0_0 {

	// filesystem namespace alias
	namespace fs = std::filesystem;

	/** Input/output utilities and std::filesystem wrappers. */
	namespace io {

		/********** Internally Linked Free Functions **********/
		
		/** Implements `read_file` and `write_file`. */
		template<typename Callable>
		static bool invoke_read_or_write(const fs::path& filename, Callable fileio_fn, std::ios_base::openmode mode) {
			bool success = false;
			if (std::fstream file{filename, mode}; file.is_open()) {
				success = fileio_fn(file);
				file.close();
			} else {
				std::cerr << "[ERROR] Failed to open file `" << filename << "`\n";
			}
			return success;
		}
		
		/********** API  **********/

		/**
		 * Implements boilerplate code for opening and closing regular files for reading
		 * with error handling. If the file is opened successfully, calls a user-defined
		 * function that implements all reading logic.
		 *
		 * @param filename Path to file for reading.
		 * @param read_fn Function that implements the whole read operation upon an 
		 *        already opened std::ifstream object. This function must take a 
		 *        reference to the stream as its only parameter and return `true` if it
		 *        succeeds in reading its contents. Note that this function (read_file)
		 *        is responsible for opening and closing the file and, therefore, 
		 *        `read_fn` should NOT attempt to open, close or otherwhise invalidate
		 *        the stream.
		 *
		 * @returns `true` if the file could be opened and if `read_fn` returned 
		 *          successfully, `false` otherwise.
		*/
		const auto read_file = [](const fs::path& filename, auto& read_fn) -> bool {
			return invoke_read_or_write(filename, read_fn, std::ios_base::in);
		};

		/**
		 * Implements boilerplate code for opening and closing regular files for writing
		 * with error handling. If the file is opened successfully, calls a user-defined
		 * function that implements all writing logic.
		 *
		 * @param filename Path to file for writing.
		 * @param write_fn Function that implements the whole write operation upon an 
		 *        already opened std::ofstream object. This function must take a 
		 *        reference to the stream as its only parameter and return `true` if it 
		 *        succeeds in writing its contents. Note that this function (write_file)
		 *        is responsible for opening and closing the file and, therefore, 
		 *        `write_fn` should NOT attempt to open, close or otherwhise invalidate 
		 *        the stream.
		 *
		 * @returns `true` if the file could be opened and if `write_fn` returned 
		 *          successfully, `false` otherwise.
		*/
		const auto write_fn = [](const fs::path& filename, auto& write_fn) -> bool {
			return invoke_read_or_write(filename, write_fn, std::ios_base::out);
		};

		/**
		 * Attempts to create a given directory. Parent directories will be created as 
		 * needed.
		 * If the function fails, logs an error message to stderr.
		 *
		 * @param dir Path to a directory.
		 *
		 * @returns `true` if the directory(ies) were created, `false` otherwise.
		*/
		const auto try_mkdir = [](const fs::path& dir) -> bool {
			std::error_code ec{};
			if (not fs::create_directories(dir, ec)) {
				std::cerr <<
					"[ERROR] `faber::io::try_mkdirs`: Failed to create directory(ies)\n"
					"\tReason: " << ec.message() << "\n"
				<< std::endl;
				return false;
			}
			return true;
		};

	} // namespace io
} // namespace v1_0_0
} // namespace faber

#endif // FABER_IO_H
