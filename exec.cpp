#include "exec.h"

std::string exec(const std::string cmd)
{
    std::array<char, 128> buffer;
    std::string result;

    const char* cmdstr = cmd.c_str();
    std::shared_ptr<FILE> pipe(popen(cmdstr, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    std::cout<<result;
    return result;
}


