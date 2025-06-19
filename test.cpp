#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <stdexcept>

std::string getHostnameIP(const std::string& hostname) {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(hostname.c_str(), NULL, &hints, &res);
    if (status != 0) {
        throw std::runtime_error("Error while getting IP: " +
            std::string(gai_strerror(status)));
    }

    std::string ipStr;

    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    char ipBuffer[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ipv4->sin_addr), ipBuffer, sizeof(ipBuffer));
    ipStr = ipBuffer;
    freeaddrinfo(res);
    return ipStr;
}

int main() {
    try {
        std::string hostname = "localhost";
        std::cout << "Hostname: " << hostname << std::endl;
        std::string ipAddress = getHostnameIP(hostname);
        std::cout << "IP address: " << ipAddress << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
