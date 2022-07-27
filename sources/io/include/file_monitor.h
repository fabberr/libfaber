#ifndef FABER_FILE_MONITOR_H
#define FABER_FILE_MONITOR_H

/********** Headers **********/

// C++ stdlib
#include <fstream>          // file streams
#include <string_view>      // std::string_view
#include <utility>          // std::pair
#include <memory>           // std:unique_ptr
#include <unordered_map>    // std::unordered_map
#include <initializer_list> // std::initializer_list

/********** file_monitor.h **********/

namespace faber { inline namespace v1_0_0 {

    namespace io {
        
        /** Manages and stores open files in a hash table. */
        class file_monitor {
        private:
            /********** Private Types **********/

            /** Type of initializer list for specific openmode constructor. */
            using ilist_entry = std::pair<const char*, std::fstream::openmode>;

            /** Mapped type of hash table. Destructuring is supported. */
            struct file_info {

                /********** RAII **********/
                
                file_info(
                    std::unique_ptr<std::fstream>&& stream  = nullptr, 
                    std::fstream::openmode          mode    = static_cast<std::fstream::openmode>(0), 
                    std::uint32_t                   size    = 0U
                ) : stream(std::move(stream)), mode(mode), size(size) { }
                
                /********** Rule of Five **********/

                file_info(const file_info&) = delete;
                file_info(file_info&&)      = delete;
                ~file_info()                = default;

                file_info& operator=(const file_info&)  = delete;
                file_info& operator=(file_info&&)       = delete;

                /********** Public Members **********/
                
                std::unique_ptr<std::fstream>   stream; /** Pointer to an open file stream on the heap. */
                std::fstream::openmode          mode;   /** Mode which the stream was opened with. */
                std::uint32_t                   size;   /** Size (in byes) of the file associated to the stream. */
                /** @todo add a timestamp of when the file was opened maybe? */
            };

            /**
             * The underlying data structure.
             * A hash table that maps an unique filename to an object containing a 
             * `std::fstream` associated to it and other pertinent info.
            */
            using hashtable = std::unordered_map<std::string_view, file_info>;
        
        public:
            /********** Forward Declared Public Types **********/
            
            typedef file_info file_info_t;
            typedef hashtable hashtable_t;

        public:
            /********** Constructors & Destructor **********/
            
            file_monitor() = default; // default constructor

            file_monitor(std::initializer_list<const char*> ilist); // opens 1..* files (in|out)
            file_monitor(std::initializer_list<ilist_entry> ilist); // opens 1..* files, specify openmode for each

            file_monitor(const file_monitor&) 	= delete; // copy constructor (deleted)
            file_monitor(const file_monitor&&) 	= delete; // move constructor (deleted)

            ~file_monitor() = default;

        private:
            /********** Private Member Functions **********/

            std::fstream& open_impl(const char* filename, std::fstream::openmode mode);

        public:
            /********** Public Member Functions **********/

            std::fstream& open(const char* filename); // open file (in|out) and register its stream on the table
            std::fstream& open(const char* filename, std::fstream::openmode mode); // open file (specify openmode)
            void close(std::string_view filename); // close file and remove its stream from the table

            /** Wrapper for unordered_map::find (const version only). */
            hashtable_t::const_iterator find(std::string_view filename) const;
            
            /** Wrapper for std::unordered_map::at. */
            const file_info_t& operator[](const std::string_view key) const;

            /** Wrapper for std::unordered_map::operator[]. */
            file_info_t& operator[](const std::string_view key);

            const hashtable_t& data() const; // return const reference to underlying data structure, non-const version below
            hashtable_t& data();
            
            file_monitor& operator=(const file_monitor&)    = delete; // copy assignment (deleted)
            file_monitor& operator=(file_monitor&&)         = delete; // move assignment (deleted)

        private:
            /********** Private Members **********/

            hashtable_t _opened_files{};
        }; // class file_monitor
        
    } // namespace io

} // inline namespace v1_0_0
} // namespace faber

#endif // FABER_FILE_MONITOR_H

/** @todo make file_monitor a singleton */
/** @todo accessors for ios_base::openmode */
