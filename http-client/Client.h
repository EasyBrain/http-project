//
// Created by vadim on 21.04.2020.
//

#ifndef HTTP_CLIENT_CLIENT_H
#define HTTP_CLIENT_CLIENT_H

#include <filesystem>
#include <memory>

#include <Poco/Util/Application.h>
#include <Poco/Net/HTTPClientSession.h>

namespace PU = Poco::Util;

class Client: public PU::Application {
public:
    int main(const std::vector<std::string>& args) override;

private:
    void runCmd();

    Poco::Net::HTTPClientSession session{};
    std::filesystem::path current_dir = "./";
    std::string input{};
    bool running = false;
};


#endif //HTTP_CLIENT_CLIENT_H
