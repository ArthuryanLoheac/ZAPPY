/*
** EPITECH PROJECT, 2025
** server.h
** File description:
** Header file for server functions
*/

#ifndef SERVER_H
    #define SERVER_H

typedef struct server_s {
    int port;
    int server_fd;
    int nb_fds;
    struct pollfd *fds;
} server_t;

server_t *create_server(int port);
void destroy_server(server_t *server);

#endif /* !SERVER_H */
