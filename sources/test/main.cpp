#include <file_monitor.h>
#include <string>

int main() {
    using namespace faber;
    using namespace std::string_literals;
    using namespace std::string_view_literals;

    io::file_monitor opened_files {
        { "path/to/file1", std::fstream::in                       },
        { "path/to/file2", (std::fstream::in | std::fstream::out) }
    };

    constexpr size_t BUF_SIZE{15};
    const auto buf = std::make_unique<char[]>(BUF_SIZE + 1);
    if (const auto& [stream, mode, size] = opened_files["path/to/file2"sv]; 
        size >= BUF_SIZE && mode == (std::fstream::in | std::fstream::out))
    {
        stream->read(buf.get(), BUF_SIZE);
        buf[BUF_SIZE] = '\0';
    }
}
