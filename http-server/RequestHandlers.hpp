//
// Created by vadim on 17.04.2020.
//

#ifndef HTTP_SERVER_REQUESTHANDLERS_HPP
#define HTTP_SERVER_REQUESTHANDLERS_HPP

#include <Poco/Net/HTTPRequestHandler.h>

namespace PN = Poco::Net;

class GetRequestHandler: public PN::HTTPRequestHandler {
public:
    void handleRequest(PN::HTTPServerRequest& request, PN::HTTPServerResponse& response) override;
};

class DeleteRequestHandler: public PN::HTTPRequestHandler {
public:
    void handleRequest(PN::HTTPServerRequest& request, PN::HTTPServerResponse& response) override;
};

class PatchRequestHandler: public PN::HTTPRequestHandler {
public:
    void handleRequest(PN::HTTPServerRequest& request, PN::HTTPServerResponse& response) override;
};

class PutRequestHandler: public PN::HTTPRequestHandler {
public:
    void handleRequest(PN::HTTPServerRequest& request, PN::HTTPServerResponse& response) override;
};

class PostRequestHandler: public PN::HTTPRequestHandler {
public:
    void handleRequest(PN::HTTPServerRequest& request, PN::HTTPServerResponse& response) override;
};

class BadRequestHandler: public PN::HTTPRequestHandler {
public:
    void handleRequest(PN::HTTPServerRequest& request, PN::HTTPServerResponse& response) override;
};


#endif //HTTP_SERVER_REQUESTHANDLERS_HPP
