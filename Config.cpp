//
// Created by liam on 17/01/18.
//

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "Config.h"

Config::Config() {
    //Do we have a config? If not, create a default one
    if (this->file = fopen("tebex_config.cfg", "r+")) {
    } else {
        this->file = fopen("tebex_config.cfg", "w+");
        this->writeFile();
    }

    char* jsonString = (char *)"";

    //read file
    fgets(jsonString, 10000, this->file);

    rapidjson::Document document;
    document.Parse<0>(jsonString);

    this->secret = document["secret"].GetString();
    this->verbose = document["verbose"].GetBool();
}

bool Config::writeFile() {
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

    rapidjson::Value secret;
    rapidjson::Value verbose;

    secret.SetString(this->secret.c_str(), allocator);
    verbose.SetBool(this->verbose);

    document.AddMember("secret", secret, allocator);
    document.AddMember("verbose", verbose, allocator);

    fputs(document.GetString(), this->file);

    return true;
}