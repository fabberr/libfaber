#ifndef FABER_IO_H
#define FABER_IO_H

/********** Headers **********/

// C++ stdlib
#include <filesystem> 	// filesystem utilities
#include <iostream> 	// streams
#include <fstream> 		// file streams
#include <string> 		// std::string
#include <functional> 	// function objects
#include <concepts> 	// concepts library

/********** io.h **********/

namespace faber { inline namespace v1_0_0 {

	/** Input/output utilities and std::filesystem wrappers. */
	namespace io {

		// filesystem namespace alias
		namespace fs = std::filesystem;

		/** Implementation details, see API section for user-facing interface. */
		namespace impl_details {

			/**
			 * Implements logic for opening and closing files with basic error checking 
			 * and invokes a callback function to operate on the stream associated o it.
			 * 
			 * See `io::read_file`, `io::write_file` and `io::open_file_then` functions
			 * on the API section of this header for more details about functionality.
			*/
			template<typename Invocable>
			requires std::invocable<Invocable, std::fstream&>
			static bool file_io_cllbck_impl(const std::string& filename, Invocable callback, std::ios_base::openmode mode) {
				if (std::fstream file{filename, mode}; file.is_open()) {
					auto ret = std::invoke(callback, file);
					file.close();
					return ret;
				}
				std::cerr << "[ERROR] Failed to open file `" << filename << "`\n";
				return false;
			}

		} // namespace impl_details
		
		/********** API  **********/

		/**
		 * Implements boilerplate code for opening and closing a file for reading with 
		 * basic error handling then invokes a user-defined callable that performs the 
		 * read operations on the file. This callable must take a reference to a file 
		 * stream object and return a boolean.
		 * 
		 * Example usage: `
		 *     using namespace std::string_literals;
		 *     using namespace faber;
		 * 
		 *     const auto print_to_stdout = [](std::fstream& file) -> bool {
		 *         for (std::string line{}; std::getline(file, line); ) {
		 *             std::cout << line << "\n";
		 *         }
		 *         return true; // ideally, you should check if the function executed successfuly or not instead
		 *     }
		 * 
		 *     // last argument is optional, `std::fstream::in` in this case
		 *     io::read_file("path/to/file"s, print_to_stdout);
		 * `
		 * 
		 * @throws May throw an exception caused by the constructor of `std::fstream` or
		 *         any caused by the provided callback function.
		 *
		 * @param filename Path to a file. The file will be created if it doesn't exist.
		 * @param callback A callable object or function that takes a reference to an 
		 *        already opened `std::fstream` object as its only argument. This 
		 *        callable must implement all read operations to be perfomed on the 
		 *        stream and return `true` if they succeed or `false` otherwise.
		 *        Note that this function is responsible for both opening and closing 
		 *        the file stream and, therefore, the callback fanction must NOT close 
		 *        or otherwise invalidate the stream passed as its argument.
		 * @param mode Bitmask type, file open mode. Refer to `std::ios_base::openmode`
		 *        documentation for more info. The openmode passed to this function will 
		 *        be appended to `ios_base::in` before opening the file.
		 *
		 * @returns A boolean, the same value returned by the callback function.
		*/
		template<typename Invocable>
		requires std::invocable<Invocable, std::fstream&>
		bool read_file(const std::string& filename, Invocable callback, std::ios_base::openmode mode = std::fstream::in) {
			return std::invoke(impl_details::file_io_cllbck_impl, filename, callback, mode |= std::fstream::in);
		}

		/**
		 * Implements boilerplate code for opening and closing a file for writing with 
		 * basic error handling then invokes a user-defined callable that performs the 
		 * write operations on the file. This callable must take a reference to a file 
		 * stream object and return a boolean.
		 * 
		 * Example usage: `
		 *     using namespace std::string_literals;
		 *     using namespace faber;
		 * 
		 *     const auto example = [](std::fstream& file) -> bool {
		 *         file << "This is an example usage of the `faber::io::write_file utility`.\n";
		 *         return file.good();
		 *     }
		 * 
		 *     // last argument is optional, `std::fstream::trunc` in this case
		 *     io::write_file("path/to/file"s, example);
		 * `
		 * 
		 * @throws May throw an exception caused by the constructor of `std::fstream` or
		 *         any caused by the provided callback function.
		 *
		 * @param filename Path to a file. By default, the file will be created if it 
		 *        doesn't exist and if it does, its contents will be discarded before 
		 *        writing.
		 * @param callback A callable object or function that takes a reference to an 
		 *        already opened `std::fstream` object as its only argument. This 
		 *        callable must implement all write operations to be perfomed on the 
		 *        stream and return `true` if they succeed or `false` otherwise.
		 *        Note that this function is responsible for both opening and closing 
		 *        the file stream and, therefore, the callback fanction must NOT close 
		 *        or otherwise invalidate the stream passed as its argument.
		 * @param mode Bitmask type, file open mode. Refer to `std::ios_base::openmode`
		 *        documentation for more info. The openmode passed to this function will 
		 *        be appended to `ios_base::trunc` before opening the file.
		 *
		 * @returns A boolean, the same value returned by the callback function.
		*/
		template<typename Invocable>
		requires std::invocable<Invocable, std::fstream&>
		bool write_file(const std::string& filename, Invocable callback, std::ios_base::openmode mode = std::fstream::trunc) {
			return std::invoke(impl_details::file_io_cllbck_impl, filename, callback, mode |= std::fstream::trunc);
		}

		/**
		 * Implements boilerplate code for opening and closing a file with basic error 
		 * handling then invokes a user-defined callable that performs I/O operations on
		 * the file. This callable must take a reference to a file stream object and 
		 * return a boolean.
		 * 
		 * Example usage: `
		 *     using namespace std::string_literals;
		 *     using namespace faber;
		 * 
		 *     const auto dup_first_line = [](std::fstream& file) -> bool {
		 *         std::string line{};
		 *         std::getline(file, line);
		 *         file << line;
		 *         return (not file.fail());
		 *     };
		 * 
		 *     // last argument is optional, `(std::fstream::in | std::fstream::out)` in this case
		 *     io::open_file_then("path/to/file"s, dup_first_line);
		 * `
		 * 
		 * @throws May throw an exception caused by the constructor of `std::fstream` or
		 *         any caused by the provided callback function.
		 *
		 * @param filename Path to a file. The file will be created if it doesn't exist.
		 * @param callback A callable object or function that takes a reference to an 
		 *        already opened `std::fstream` object as its only argument. This 
		 *        callable must implement all the I/O operations to be perfomed on the 
		 *        stream and return `true` if they succeed or `false` otherwise.
		 *        Note that this function is responsible for both opening and closing 
		 *        the file stream and, therefore, the callback fanction must NOT close 
		 *        or otherwise invalidate the stream passed as its argument.
		 * @param mode Bitmask type, file open mode. Refer to `std::ios_base::openmode`
		 *        documentation for more info. The openmode passed to this function will 
		 *        be appended to `(std::fstream::in | std::fstream::out)` before opening
		 *        the file.
		 *
		 * @returns A boolean, the same value returned by the callback function.
		*/
		template<typename Invocable>
		requires std::invocable<Invocable, std::fstream&>
		bool open_file_then(const std::string& filename, Invocable callback, std::fstream::openmode mode = (std::fstream::in | std::fstream::out)) {
			return std::invoke(impl_details::file_io_cllbck_impl, filename, callback, mode |= (std::fstream::in | std::fstream::out));
		}

		/**
		 * Attempts to create a directory. Parent direcotires will be created as needed.
		 *
		 * @param dir Path to a non-existant directory.
		 *
		 * @returns `true` if the directory(ies) were created, `false` otherwise.
		*/
		bool try_mkdirs(const fs::path& dir) {
			std::error_code ec{};
			if (not fs::create_directories(dir, ec)) {
				std::cerr <<
					"[ERROR] Failed to create directory(ies)\n"
					"\tReason: " << ec.message() << "\n"
				<< std::endl;
				return false;
			}
			return true;
		};

		/**
		 * Returns the size (in bytes) of a file currently on memory. The file should've
		 * been opened with `std::fstream::binary`.
		 *
		 * @param file File stream associated with a file.
		 *
		 * @returns The size of the file in bytes.
		*/
		std::size_t filesize(std::fstream& file) {
			using namespace std;

			// save current positions
			const auto cur_g = file.tellg(), 
			           cur_p = file.tellp();
			// get size
			file.seekg(ios_base::end);
			const auto size = file.tellg();

			// rewind positions
			file.seekg(cur_g, ios_base::beg);
			file.seekp(cur_p, ios_base::beg);

			return size;
		};

		/**
		 * Returns the size (in bytes) of a file on disk. Does not take into account the
		 * real size occupied on disk, only the contents are counted.
		 *
		 * @param filename Path to a file.
		 *
		 * @returns The size of the file in bytes or 0 if the file doesn't exist.
		*/
		std::size_t filesize(const std::string& filename) {
			using namespace std;
			using namespace faber::io;

			// prevents std::fstream constructor from creating file down the road
			if ( not fs::exists(filename) ) { return 0; }

			size_t size = 0;
			read_file(filename, [&size](fstream& file) -> bool {
				size = file.tellg();
				return true;
			}, (ios_base::ate | ios_base::binary));
			return size;
		};

		const auto copy_to_stream = [](const fs::path& filename, std::ostream& os = std::cout) -> void {
		};

	} // namespace io
} // inline namespace v1_0_0
} // namespace faber

#endif // FABER_IO_H
