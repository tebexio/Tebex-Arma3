#ifndef EXTENSION_H
#define EXTENSION_H

#include <fstream>
#include <regex>
#include <thread>

#define TEBEX_EXTENSION_VERSION       "0.0.1"

namespace tebex {

    const std::string REQUEST_COMMAND_POISON = "poison";

    const std::string RESPONSE_TYPE_ERROR = "error";
    const std::string RESPONSE_TYPE_OK = "ok";

    const std::string EMPTY_SQF_DATA = "\"\"";

    struct Request {
        std::string command;
        std::vector<std::string> params;
    };

    struct Response {
        std::string type;
        std::string data;
    };

    namespace extension {

        bool initialize();
        void finalize();
        void call(char *output, int outputSize, const char *function);
        Request popRequest();

    }
}

#endif

