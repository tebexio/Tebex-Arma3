#include "extension.h"
#include "Config.h"

#include <fstream>
#include <regex>
#include <thread>
#include <chrono>
#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "Api.h"

#ifdef _WIN32
#include "shlobj.h"
#endif



namespace tebex {
    namespace extension {

        namespace {
            Config *config;
            std::string requestParamSeparator = ":";
        }

        void respond(char* output, const std::string& type, const std::string& data) {
            std::string message = "" + type + ":" + data + "";
            message.copy(output, message.length());
            output[message.length()] = '\0';
        }

        void split(const std::string &str, const std::string& separator, std::vector<std::string> &elems) {
            std::regex separatorRegex(separator);
            elems = { std::sregex_token_iterator(str.begin(), str.end(), separatorRegex, -1), std::sregex_token_iterator() };
        }

        bool initialize() {
            config = new Config();
            //Do we have a valid secret? If not, prompt for one


            //Set up a timer to check for commands

            return true;
        }

        void finalize() {
        }

        void setInterval(auto function,int interval) {
            std::thread th([&]() {
                while(true) {
                    std::this_thread::sleep_for(std::chrono::seconds(interval));
                    function();
                }
            });
            th.detach();
        }

        rapidjson::Value addnvp(rapidjson::Document& document, const char *key, const char *value) {
            rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
            rapidjson::Value nvp(rapidjson::kArrayType);
            nvp.PushBack(rapidjson::Value(key, allocator), allocator);
            nvp.PushBack(rapidjson::Value(value, allocator), allocator);
            return nvp;

        }

        void call(char* output, int outputSize, const char* function) {
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
            else if (request.command == "secret") {
                config->secret = request.params[1];
                config->writeFile();

                auto api = new Api(config);
                try {
                    auto information = api->getServerInformation();
                    respond(output, RESPONSE_TYPE_OK, "Secret set to " + request.params[1] = "");
                } catch (std::runtime_error& e) {
                    char* error = (char *)"";
                    strcpy(error, "Could not validate secret key: ");
                    strcpy(error, e.what());
                    respond(output, RESPONSE_TYPE_ERROR,  + error);
                }

                respond(output, RESPONSE_TYPE_OK, "Secret set to " + request.params[1] + "");
                return;
            }
            else if (request.command == "info") {
                auto api = new Api(config);
                auto info = api->getServerInformation();

                rapidjson::Document response;
                response.SetArray();

                rapidjson::Document::AllocatorType& allocator = response.GetAllocator();


                addnvp(response, "account_id", info["account"]["id"].GetString());
                addnvp(response, "domain", info["account"]["domain"].GetString());
                addnvp(response, "account_name", info["account"]["name"].GetString());

                addnvp(response, "game_type", info["account"]["game_type"].GetString());

                addnvp(response, "server_id", info["server"]["id"].GetString());
                addnvp(response, "server_name", info["server"]["name"].GetString());

                rapidjson::StringBuffer buffer;
                rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                response.Accept(writer);
                respond(output, RESPONSE_TYPE_OK, buffer.GetString());
                buffer.Clear();
                return;
            }

            respond(output, RESPONSE_TYPE_ERROR, "Unkown command");
        }

    }
}