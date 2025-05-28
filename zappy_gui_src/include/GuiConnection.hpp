#pragma once

int client_connection(int &sockfd);
void graphic(int sockfd);
int loopClient(int sockfd);
void handleCommand(std::string &command);