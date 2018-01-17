//
// Created by liam on 17/01/18.
//

#include <fstream>
#include <regex>
#include "Config.h"
#include "rapidjson/document.h"

#ifndef TEBEX_ARMA3_API_H
#define TEBEX_ARMA3_API_H


class Api {

private:
    const char* apiUrl = "https://plugin.buycraft.net";
    Config* config;

public:
    rapidjson::Document getServerInformation();
    rapidjson::Document retrieveOfflineQueue();
    rapidjson::Document retrieveDueQueue();
    rapidjson::Document getPlayerQueue(std::string id);
    void deleteCommand(std::vector<int> ids);
    Api(Config* config) {
        this->config = config;
    }

private:
    rapidjson::Document get(char* url);
    void doDelete(char* url);
    char* buildEndpoint(char *url);
    static size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data);
    rapidjson::Document parseResponse(std::string response);

};



#endif //TEBEX_ARMA3_API_H
