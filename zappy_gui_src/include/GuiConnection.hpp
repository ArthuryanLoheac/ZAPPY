#pragma once

/**
 * @brief Establishes a connection to the client.
 *
 * @param sockfd Reference to the socket file descriptor.
 * @return int Status of the connection (0 for success, non-zero for failure).
 */
int client_connection(int &sockfd);

/**
 * @brief Handles the graphical interface logic.
 */
void graphic();

/**
 * @brief Main loop for the client-side operations.
 *
 * @param sockfd Socket file descriptor for communication.
 * @return int Status of the loop execution (0 for success, non-zero for failure).
 */
int loopClient(int sockfd);
