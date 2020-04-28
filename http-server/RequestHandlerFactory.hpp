//
// Created by vadim on 17.04.2020.
//

#ifndef HTTP_SERVER_REQUESTHANDLERFACTORY_HPP
#define HTTP_SERVER_REQUESTHANDLERFACTORY_HPP

#include <Poco/Net/HTTPRequestHandlerFactory.h>

namespace PN = Poco::Net;

class RequestHandlerFactory: public PN::HTTPRequestHandlerFactory {
public:
    RequestHandlerFactory();

    PN::HTTPRequestHandler* createRequestHandler(const PN::HTTPServerRequest& request) override;
};


#endif //HTTP_SERVER_REQUESTHANDLERFACTORY_HPP
