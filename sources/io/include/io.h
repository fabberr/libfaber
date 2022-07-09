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

	// filesystem namespace alias
	namespace fs = std::filesystem;

	/** Input/output utilities and std::filesystem wrappers. */
	namespace io {

		/** Implementation details, see API section for user-facing interface. */
		namespace impl_details {

			template<typename Invocable>
			requires std::invocable<Invocable, std::fstream&>
			static bool read_or_write_file_impl(const std::string& filename, Invocable callback, std::ios_base::openmode mode) {
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

		template<typename Invocable>
		requires std::invocable<Invocable, std::fstream&>
		bool read_file(const std::string& filename, Invocable callback, std::ios_base::openmode mode = std::ios_base::in) {
			return impl_details::read_or_write_file_impl(filename, callback, mode |= std::ios_base::in);
		}

		template<typename Invocable>
		requires std::invocable<Invocable, std::fstream&>
		bool write_file(const std::string& filename, Invocable callback, std::ios_base::openmode mode = std::ios_base::out) {
			return impl_details::read_or_write_file_impl(filename, callback, mode |= std::ios_base::out);
		}

		bool try_mkdir(const fs::path& dir) {
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

		decltype(auto) filesize(const std::string& filename) {
			// std::char_traits<std::fstream>::pos_type size;
			// read_file(filename, [&size](std::fstream& file) {
			// 	size = file.tellg();
			// }, std::ios_base::in | std::ios_base::ate);
			// return size;
		};

		const auto dump_file_to_stream = [](const fs::path& filename, std::ostream& os = std::cout) -> void {
		};

	} // namespace io
} // namespace v1_0_0
} // namespace faber

#endif // FABER_IO_H
