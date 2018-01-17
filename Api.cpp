//
// Created by liam on 17/01/18.
//

#include "Api.h"
#include <curl/curl.h>
#include <fstream>
#include <regex>
#include <exception>

char* Api::buildEndpoint(char *url) {
    char * endpoint;
    strcat(endpoint, this->apiUrl);
    strcat(endpoint, "/information");

    return endpoint;
}

rapidjson::Document Api::get(char *url) {

    CURL *curl = curl_easy_init();
    struct curl_slist *list = NULL;


    curl_easy_setopt(curl, CURLOPT_URL, this->buildEndpoint(url));

    std::string secret = "X-Buycraft-Secret: " + this->config->secret;

    list = curl_slist_append(list, secret.c_str() );
    list = curl_slist_append(list, "Accept: application/json" );

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Tebex-Arma3");

    std::string response_string;
    std::string header_string;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->writeFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

    curl_easy_perform(curl);

    curl_slist_free_all(list); /* free the list again */
    curl_easy_cleanup(curl);
    curl = NULL;

    auto document = this->parseResponse(response_string);

    if (document.HasMember("error_code")) {
        char * error = (char *)"";
        strcpy(error, "Error from API:");
        strcpy(error, document["error_message"].GetString());
        throw std::runtime_error(error);
    }

    return document;
}

void Api::doDelete(char *url) {

    CURL *curl = curl_easy_init();
    struct curl_slist *list = NULL;


    curl_easy_setopt(curl, CURLOPT_URL, this->buildEndpoint(url));

    std::string secret = "X-Buycraft-Secret: " + this->config->secret;

    list = curl_slist_append(list, secret.c_str() );
    list = curl_slist_append(list, "Accept: application/json" );

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Tebex-Arma3");
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->writeFunction);

    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);


    curl_easy_perform(curl);

    curl_slist_free_all(list); /* free the list again */
    curl_easy_cleanup(curl);
    curl = NULL;

    if (response_code != 204) {
        throw std::runtime_error("DELETE failed");
    }

}

rapidjson::Document Api::getServerInformation() {
    return this->get((char *)"/information");
}

rapidjson::Document Api::retrieveOfflineQueue() {
    return this->get((char *)"/queue/offline-commands");
}

rapidjson::Document Api::retrieveDueQueue() {
    return this->get((char *)"/queue");
}

rapidjson::Document Api::getPlayerQueue(std::string id) {
    char * endpoint = (char *)"/queue/";
    strcpy(endpoint, id.c_str());
    return this->get(endpoint);
}

void Api::deleteCommand(std::vector<int> ids) {
    char * endpoint = (char *)"/queue?";
    char *amp = (char *) "";
    for (auto& id : ids) {
        strcpy(endpoint, amp);
        strcpy(endpoint, (char *) "ids[]=");
        strcpy(endpoint, std::to_string(id).c_str());
        amp = (char *) "&";
    }

    this->doDelete(endpoint);
}

size_t Api::writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

rapidjson::Document Api::parseResponse(std::string response) {
    rapidjson::Document document;
    document.Parse(response.c_str());

    return document;
}

