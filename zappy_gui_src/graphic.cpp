#include <iostream>

int graphic(int sockfd) {
    std::cout << "Graphic thread started with sockfd: " << sockfd << std::endl;
    return 0;
}