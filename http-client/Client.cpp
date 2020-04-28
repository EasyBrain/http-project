//
// Created by vadim on 21.04.2020.
//

#include "Client.h"

#include <fstream>

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/NetException.h>

namespace PN = Poco::Net;

int Client::main(const std::vector<std::string> &args) {
    session.setHost("127.0.0.1");
    session.setPort(Poco::UInt16(2112));

    running = true;

    while(running) {
        std::cout << "Enter command: ";
        std::getline(std::cin, input);
        runCmd();
    }

    return PU::Application::EXIT_OK;
}

void Client::runCmd() {
    std::stringstream ss(input);
    std::string cmd{};
    PN::HTTPRequest request;
    PN::HTTPResponse response;

    if(!(ss >> cmd)) {
        std::cout << "Expected command...\n";
        return;
    }

    if(cmd == "exit") {
        running = false;
        return;
    }
    else if(cmd == "cd") {
        std::string path{};

        if(!(ss >> path)) {
            std::cout << "Too law args... U need to add path arg!\n";
            return;
        }

        request.setMethod(PN::HTTPRequest::HTTP_GET);
        request.setURI(current_dir.string() + '/' + path);

        try {
            session.sendRequest(request);
        } catch (Poco::Exception& exception) {
            std::cout << exception.what() << '\n';
            return;
        }

        auto& rs = session.receiveResponse(response);

        Poco::StreamCopier::copyStream(rs, std::cout);

        if(response.getStatus() == PN::HTTPResponse::HTTP_OK) {
            current_dir /= path;
        }
    }
    else if(cmd == "get") {
        std::string filename{};

        if(!(ss >> filename)) {
            std::cout << "Too law args... U need to add filename arg!\n";
            return;
        }

        request.setMethod(PN::HTTPRequest::HTTP_GET);
        request.setURI(current_dir.string() + "?filename=" + filename);

        try {
            session.sendRequest(request);
        } catch (Poco::Exception& exception) {
            std::cout << exception.what() << '\n';
            return;
        }

        auto& rs = session.receiveResponse(response);

        if(response.getStatus() == PN::HTTPResponse::HTTP_OK) {
            std::ofstream file(filename);
            Poco::StreamCopier::copyStream(rs, file);
            file.close();
        }
        else {
            Poco::StreamCopier::copyStream(rs, std::cout);
        }
    }
    else if (cmd == "put") {
        std::filesystem::path path{};

        if(!(ss >> path)) {
            std::cout << "Too law args... U need to add path arg!\n";
            return;
        }

        std::ifstream file(path, std::ios::binary);
        if(!file) {
            std::cout << "Invalid path or arg cannot be a dir path\n";
            return;
        }

        request.setMethod(PN::HTTPRequest::HTTP_PUT);
        request.setURI((current_dir / path.filename()).string());

        try {
            auto& os = session.sendRequest(request);
            Poco::StreamCopier::copyStream(file, os);
        } catch (Poco::Exception& exception) {
            std::cout << exception.what() << '\n';
            return;
        }

        auto& rs = session.receiveResponse(response);

        Poco::StreamCopier::copyStream(rs, std::cout);

        if(response.getStatus() != PN::HTTPResponse::HTTP_OK) {
            std::cout << "Some problems... Command was not completed\n";
        }
    }
    else if(cmd == "post") {
        std::filesystem::path path{};
        std::string data{};

        if(!(ss >> path >> data)) {
            std::cout << "Too law args... U need to add path arg!\n";
            return;
        }


        request.setMethod(PN::HTTPRequest::HTTP_POST);
        request.setURI((current_dir / path.filename()).string());

        try {
            session.sendRequest(request) << data;
        } catch (Poco::Exception& exception) {
            std::cout << exception.what() << '\n';
            return;
        }

        auto& rs = session.receiveResponse(response);

        Poco::StreamCopier::copyStream(rs, std::cout);

        if(response.getStatus() != PN::HTTPResponse::HTTP_OK) {
            std::cout << "Some problems... Command was not completed\n";
        }
    }
    else if (cmd == "delete") {
        std::string filename{};

        if(!(ss >> filename)) {
            std::cout << "Too law args... U need to add filename arg!\n";
            return;
        }

        request.setMethod(PN::HTTPRequest::HTTP_DELETE);
        request.setURI(current_dir.string() + "?filename=" + filename);

        try {
            session.sendRequest(request);
        } catch (Poco::Exception& exception) {
            std::cout << exception.what() << '\n';
            return;
        }

        auto& rs = session.receiveResponse(response);

        Poco::StreamCopier::copyStream(rs, std::cout);

        if(response.getStatus() != PN::HTTPResponse::HTTP_OK) {
            std::cout << "Some problems... Command was not completed\n";
        }
    }
    else if(cmd == "copy" || cmd == "move") {
        std::string filename{};
        std::string dst{};

        if(!(ss >> filename >> dst)) {
            std::cout << "Too law args... U need to add filename arg!\n";
            return;
        }

        request.setMethod(PN::HTTPRequest::HTTP_PATCH);
        request.setURI(current_dir.string() + "?cmd=" + cmd
                + "&filename=" + filename + "&dst=" + dst);

        try {
            session.sendRequest(request);
        } catch (Poco::Exception& exception) {
            std::cout << exception.what() << '\n';
            return;
        }

        auto& rs = session.receiveResponse(response);

        Poco::StreamCopier::copyStream(rs, std::cout);

        if(response.getStatus() != PN::HTTPResponse::HTTP_OK) {
            std::cout << "Some problems... Command was not completed\n";
        }
    }
}
