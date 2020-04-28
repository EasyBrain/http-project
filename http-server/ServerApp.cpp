//
// Created by vadim on 17.04.2020.
//

#include "ServerApp.h"

#include <Poco/Net/HTTPServer.h>

#include "RequestHandlerFactory.hpp"

int ServerApp::main(const std::vector<std::string> &) {
    auto* params = new Poco::Net::HTTPServerParams;

    params->setMaxQueued(100);
    params->setMaxThreads(16);

    PN::HTTPServer srv(new RequestHandlerFactory, 2112);

    srv.start();
    std::cout << "Server started\n";

    waitForTerminationRequest();

    std::cout << "Shutting down...\n";
    srv.stop();

    return PU::Application::EXIT_OK;
}
