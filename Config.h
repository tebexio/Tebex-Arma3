//
// Created by liam on 17/01/18.
//

#include <fstream>
#include <regex>

#ifndef TEBEX_ARMA3_CONFIG_H
#define TEBEX_ARMA3_CONFIG_H


class Config {
    public:
        std::string secret = "";
        bool verbose = false;
        Config();
        bool writeFile();

    private:
        FILE *file;
};


#endif //TEBEX_ARMA3_CONFIG_H
