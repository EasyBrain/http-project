//
// Created by vadim on 17.04.2020.
//

#include "RequestHandlerFactory.hpp"

RequestHandlerFactory::RequestHandlerFactory() = default;

#include "RequestHandlers.hpp"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/URI.h>

PN::HTTPRequestHandler *RequestHandlerFactory::createRequestHandler(const PN::HTTPServerRequest &request) {
    std::string command = "bad req";
    Poco::URI uri;

    try {
        uri = Poco::URI(request.getURI());
    }
    catch (Poco::SyntaxException& ex) {
        return new BadRequestHandler;
    }

    const auto& params = uri.getQueryParameters();

    if(request.getMethod() == "GET") {
        return new GetRequestHandler;
    }
    else if(request.getMethod() == "PUT") {
        return new PutRequestHandler;
    }
    else if(request.getMethod() == "POST") {
        return new PostRequestHandler;
    }
    else if(request.getMethod() == "DELETE") {
        return new DeleteRequestHandler;
    }
    else if(request.getMethod() == "PATCH") {
        return new PatchRequestHandler;
    }
    else {
        return new BadRequestHandler;
    }
}
