//
// Created by vadim on 17.04.2020.
//

#ifndef HTTP_SERVER_SERVERAPP_H
#define HTTP_SERVER_SERVERAPP_H

#include <Poco/Util/ServerApplication.h>

namespace PU = Poco::Util;

class ServerApp: public PU::ServerApplication {
protected:
    int main(const std::vector<std::string>&) override;
};


#endif //HTTP_SERVER_SERVERAPP_H
