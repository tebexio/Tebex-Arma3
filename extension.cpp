#include "extension.h"

#include <fstream>
#include <regex>
#include <thread>

#ifdef _WIN32
#include "shlobj.h"
#endif



namespace tebex {
    namespace extension {

        namespace {
            const std::string EXTENSION_FOLDER_ENV_VAR = "TEBEX_EXTENSION_HOME";
            const std::string EXTENSION_FOLDER = "TebexExtension";
            const std::string CONFIG_FILE = "config.properties";
            const std::string DEFAULT_REQUEST_PARAM_SEPARATOR = "`";

            std::thread sqlThread;
            std::string requestParamSeparator;
            std::string configError = "";
        }

        void respond(char* output, const std::string& type, const std::string& data) {
            std::string message = "{'type' : '" + type + "', 'message' : ' " + data + "'}";
            message.copy(output, message.length());
            output[message.length()] = '\0';
        }

        void split(const std::string &str, const std::string& separator, std::vector<std::string> &elems) {
            std::regex separatorRegex(separator);
            elems = { std::sregex_token_iterator(str.begin(), str.end(), separatorRegex, -1), std::sregex_token_iterator() };
        }

        std::string getExtensionFolder() {
            return "";
        }

        std::string getStringProperty(const std::string& key) {
            return "";
        }

        uint32_t getUIntProperty(const std::string& key) {
            return 0;
        }

        bool initialize() {
            return true;
        }

        void finalize() {
        }

        void call(char* output, int outputSize, const char* function) {
            if (!configError.empty()) {
                respond(output, RESPONSE_TYPE_ERROR, configError);
                return;
            }
            Request request{ "" };
            split(std::string(function), requestParamSeparator, request.params);
            if (!request.params.empty()) {
                request.command = request.params[0];
            }
            if (request.command == "version") {
                respond(output, RESPONSE_TYPE_OK, TEBEX_EXTENSION_VERSION);
                return;
            }
            else if (request.command == "separator") {
                respond(output, RESPONSE_TYPE_OK, requestParamSeparator);
                return;
            }
            respond(output, RESPONSE_TYPE_ERROR, "\"Unkown command\"");
        }
    }
}