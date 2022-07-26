#ifndef FABER_FILE_MONITOR_H
#define FABER_FILE_MONITOR_H

/********** Headers **********/

// C++ stdlib
#include <fstream> // file streams
#include <string_view> // std::string_view
#include <utility> // std::pair
#include <memory> // std:unique_ptr
#include <unordered_map> // std::unordered_map
#include <initializer_list> // std::initializer_list

namespace faber { inline namespace v1_0_0 {

    namespace io {
        
        /** Manages open files. */
        class file_monitor {
        private:
            /********** Private Types **********/

            /** Type of initializer list for specific openmode constructor. */
            using ilist_entry = std::pair<const std::string_view, std::fstream::openmode>;

            /** Mapped-to type of hash map. Destructuring is supported. */
            struct file_info {
                std::unique_ptr<std::fstream> 	stream = nullptr;
                std::fstream::openmode 			mode = static_cast<std::fstream::openmode>(0);
                std::uint32_t 					size = 0;
                /** @todo add a timestap maybe? */
            };

            /**
             * The underlying data structure.
             * A hash map that associates an unique filename to an object containing a 
             * `std::fstream` associated to it and other pertinent info.
            */
            using hash_map = std::unordered_map<std::string_view, file_info>;
        
        public:
            /********** Forward Declared Public Types **********/
            
            typedef file_info   file_info_t;
            typedef hash_map    hash_map_t;

        public:
            /********** Constructors & Destructor **********/
            
            file_monitor() = default; // default-initializes members

            file_monitor(const std::initializer_list<const std::string_view>&&); 	/** opens 1..* files (in|out) */
            file_monitor(const std::initializer_list<ilist_entry>&&); 				// opens 1..* files, specify openmode for each

            file_monitor(const file_monitor&) 	= delete; // copy constructor (deleted)
            file_monitor(const file_monitor&&) 	= delete; // move constructor (deleted)

            ~file_monitor() = default;

        private:
            /********** Private Member Functions **********/

            std::fstream& open_impl(std::string_view filename, std::fstream::openmode mode);

        public:
            /********** Public Member Functions **********/

            std::fstream& open(std::string_view filename); // open file (in|out) and register its stream on the map
            std::fstream& open(std::string_view filename, std::fstream::openmode mode); // open file (specify openmode)
            void close(std::string_view filename); // close file and remove its stream from the map

            hash_map_t::iterator find(std::string_view filename) const;	// wrapper for unordered_map::find
            
            /** Wrapper for std::unordered_map::at. */
            const file_info_t& operator[](const std::string_view key) const;

            /** Wrapper for std::unordered_map::operator[]. */
            file_info_t& operator[](const std::string_view key);

            const hash_map_t& opened_files() const; // return const reference to underlying data structure, non-const version below
            hash_map_t& opened_files(); // warning: calling operator[] on this reference may insert a new default-constructed fstream not associeated with any file
            
            file_monitor& operator=(const file_monitor&)    = delete; // copy assignment (deleted)
            file_monitor& operator=(file_monitor&&)         = delete; // move assignment (deleted)

        private:
            /********** Private Members **********/

            hash_map_t _opened_files{};
        }; // class file_monitor
        
    } // namespace io

} // inline namespace v1_0_0
} // namespace faber

#endif // FABER_FILE_MONITOR_H

/** @todo accessors for ios_base::openmode */
