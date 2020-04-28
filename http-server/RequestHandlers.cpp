//
// Created by vadim on 17.04.2020.
//

#include "RequestHandlers.hpp"

#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/URI.h>

#include <filesystem.hpp>

#include <vector>
#include <fstream>

void GetRequestHandler::handleRequest(PN::HTTPServerRequest &request, PN::HTTPServerResponse &response) {
    Poco::URI uri(request.getURI());
    auto path = uri.getPath();
    std::string filename{};

    response.setChunkedTransferEncoding(true);
    response.setContentType("application/misc");

    const auto& params = uri.getQueryParameters();

    for(const auto& [key, value] : params) {
        if(key == "filename") {
            filename = value;
        }
    }

    path = "./" + path + '/' + filename;

    if(std::filesystem::exists(path)) {
        response.setStatus(PN::HTTPResponse::HTTPStatus::HTTP_OK);

        if(std::filesystem::is_directory("./" + path + '/' + filename)) {
            auto& os = response.send();

            std::vector<ext::filesystem::EntryInfo> entries{};

            ext::filesystem::dir_entries_info(path, std::back_inserter(entries));

            for (auto &e : entries) {
                 os << std::left << std::setw(40) << e.name << std::setw(12)
                   << ext::filesystem::perms_to_str(e.perms) << std::setw(15)
                   << ext::filesystem::size_to_str(e.size) << std::setw(3) << e.type << '\n';
            }

            os.flush();
        }
        else {
            response.sendFile(path, "application/misc");
        }
    }
    else {
        response.setStatus(PN::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        response.send() << "Can't open file\n";
    }
}

void BadRequestHandler::handleRequest(PN::HTTPServerRequest &request, PN::HTTPServerResponse &response) {
    response.setStatus(PN::HTTPResponse::HTTPStatus::HTTP_BAD_REQUEST);
    response.setChunkedTransferEncoding(true);
    response.setContentType("text/cvs");

    auto& os = response.send();
    std::cerr << "Uri: " << request.getURI() << '\n';

    os << "Bad request\n";
}

void PutRequestHandler::handleRequest(PN::HTTPServerRequest &request, PN::HTTPServerResponse &response) {
    const auto& path = Poco::URI(request.getURI()).getPath();
    std::ofstream file(path, std::ios::binary);

    response.setChunkedTransferEncoding(true);
    response.setContentType("text/cvs");

    if(file) {
        response.setStatus(PN::HTTPResponse::HTTPStatus::HTTP_OK);

        file << request.stream().rdbuf();

        response.send() << "Created file\n";
    }
    else {
        response.setStatus(PN::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);

        response.send() << "Can't create file\n";
    }

    file.close();
}

void PostRequestHandler::handleRequest(PN::HTTPServerRequest &request, PN::HTTPServerResponse &response) {
    const auto& path = Poco::URI(request.getURI()).getPath();
    std::ofstream file(path, std::ios::binary | std::ios::app);

    response.setChunkedTransferEncoding(true);
    response.setContentType("text/cvs");

    if(file) {
        response.setStatus(PN::HTTPResponse::HTTPStatus::HTTP_OK);

        file << request.stream().rdbuf();

        response.send() << "Appended\n";
    }
    else {
        response.setStatus(PN::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);

        response.send() << "Can't append in file\n";
    }

    file.close();
}

void DeleteRequestHandler::handleRequest(PN::HTTPServerRequest &request, PN::HTTPServerResponse &response) {
    Poco::URI uri(request.getURI());
    const auto& path = uri.getPath();
    std::string filename{};

    response.setChunkedTransferEncoding(true);
    response.setContentType("text/cvs");

    auto& os = response.send();

    const auto& params = uri.getQueryParameters();

    for(const auto& [key, value] : params) {
        if(key == "filename") {
            filename = value;
        }
    }

    std::cerr << "./" + path + '/' + filename << '\n';
    if(std::filesystem::exists("./" + path + '/' + filename)) {
        std::error_code ec{};

        std::filesystem::remove("./" + path + '/' + filename, ec);

        if(!ec) {
            response.setStatus(PN::HTTPResponse::HTTPStatus::HTTP_OK);
            os << "File deleted\n";
        }
        else {
            response.setStatus(PN::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
            os << "Some problems... File not deleted\n";
        }
    }
    else {
        response.setStatus(PN::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        os << "Can't open file\n";
    }
}

void PatchRequestHandler::handleRequest(PN::HTTPServerRequest &request, PN::HTTPServerResponse &response) {
    Poco::URI uri(request.getURI());
    const auto& path = uri.getPath();
    std::string filename{}, dst{}, cmd{};

    response.setChunkedTransferEncoding(true);
    response.setContentType("text/cvs");

    const auto& params = uri.getQueryParameters();

    for(const auto& [key, value] : params) {
        if(key == "filename") {
            filename = value;
        }
        else if(key == "dst") {
            dst = value;
        }
        else if(key == "cmd") {
            cmd = value;
        }
    }

    std::error_code ec;
    std::filesystem::copy("./" + path + '/' + filename, "./" + dst + '/' + filename, ec);

    if(!ec && cmd == "move") {
        std::filesystem::remove("./" + path + '/' + filename, ec);

        if(!ec) {
            response.setStatus(PN::HTTPResponse::HTTPStatus::HTTP_OK);
            response.send() << "File moved\n";
        }
        else {
            response.setStatus(PN::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
            response.send() << "Can't open file\n";
        }
    }
    else if(!ec) {
        response.setStatus(PN::HTTPResponse::HTTPStatus::HTTP_OK);
        response.send() << "File copied\n";
    }
    else {
        response.setStatus(PN::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
        response.send() << "Can't open file\n";
    }
}
