/*
** EPITECH PROJECT, 2025
** client.h
** File description:
** Header file for client functions
*/

#ifndef CLIENT_H
    #define CLIENT_H

    #include <stdbool.h>

    #include "zappy.h"

/**
 * @brief Structure representing a queue of waiting commands for a client.
 */
typedef struct waitingCommands_s {
    struct waitingCommands_s *next; /**< Pointer to the next command. */
    char **command; /**< Command arguments. */
    int ticksLeft; /**< Ticks remaining before the command is executed. */
} waitingCommands_t;

/**
 * @brief Structure representing the inventory of a client.
 */
typedef struct inventory_s {
    int food; /**< Amount of food. */
    int linemate; /**< Amount of linemate. */
    int deraumere; /**< Amount of deraumere. */
    int sibur; /**< Amount of sibur. */
    int mendiane; /**< Amount of mendiane. */
    int phiras; /**< Amount of phiras. */
    int thystame; /**< Amount of thystame. */
} inventory_t;

/**
 * @brief Structure representing the stats of a client.
 */
typedef struct stats_s {
    int id; /**< Client ID. */
    int x; /**< X-coordinate of the client. */
    int y; /**< Y-coordinate of the client. */
    int level; /**< Level of the client. */
    int orientation; /**< Orientation of the client. */
    int tickLife; /**< Remaining life ticks of the client. */
    char *team_name; /**< Team name of the client. */
    inventory_t inventory; /**< Inventory of the client. */
    int tickRemainingElevation; /**< Ticks remaining for elevation. */
} stats_t;

/**
 * @brief Structure representing a client.
 */
typedef struct client_s {
    struct client_s *next; /**< Pointer to the next client in the list. */
    int fd; /**< File descriptor of the client. */
    char *in_buffer; /**< Input buffer for the client. */
    char *out_buffer; /**< Output buffer for the client. */
    bool is_connected; /**< Connection status of the client. */
    bool is_waiting_id; /**< Whether the client is waiting for an ID. */
    bool is_graphic; /**< Whether the client is a graphic client. */
    waitingCommands_t *waiting_commands; /**< Queue of waiting commands. */
    stats_t stats; /**< Stats of the client. */
} client_t;

/**
 * @brief Check the ticks for all clients and process their commands.
 * @param zappy Pointer to the zappy server structure.
 */
void check_ticks(zappy_t *zappy);

/**
 * @brief Add a new client to the server.
 * @param zappy Pointer to the zappy server structure.
 * @param fd File descriptor of the new client.
 */
void add_client(zappy_t *zappy, int fd);

/**
 * @brief Remove a client from the server.
 * @param zappy Pointer to the zappy server structure.
 * @param fd File descriptor of the client to remove.
 */
void remove_client(zappy_t *zappy, int fd);

/**
 * @brief Destroy all clients and free their resources.
 * @param clients Pointer to the list of clients.
 */
void destroy_clients(client_t *clients);

/**
 * @brief Send data to a specific client.
 * @param zappy Pointer to the zappy server structure.
 * @param c Pointer to the client.
 */
void send_data(zappy_t *zappy, client_t *c);

/**
 * @brief Send data to all graphic clients.
 * @param zappy Pointer to the zappy server structure.
 * @param data Data to send.
 */
void send_data_to_graphics(zappy_t *zappy, char *data);

/**
 * @brief Add data to a buffer.
 * @param buffer Pointer to the buffer.
 * @param data Data to add.
 */
void add_to_buffer(char **buffer, const char *data);

/**
 * @brief Get the size of a number in characters.
 * @param nbr The number to measure.
 * @return The size of the number in characters.
 */
int get_size(int nbr);

/**
 * @brief Push a command to the client's command queue.
 * @param args Command arguments.
 * @param client Pointer to the client.
 * @param zappy_ptr Pointer to the zappy server structure.
 */
void push_command_to_queue(char **args, client_t *client, zappy_t *zappy_ptr);

/**
 * @brief Send player data to a client.
 * @param zappy Pointer to the zappy server structure.
 * @param c Pointer to the client.
 */
void send_players_data(zappy_t *zappy, client_t *c);

/**
 * @brief Send egg data to a client.
 * @param zappy Pointer to the zappy server structure.
 * @param c Pointer to the client.
 */
void send_eggs_data(zappy_t *zappy, client_t *c);

/**
 * @brief Delete the first egg with the specified team name.
 * @param zappy_ptr Pointer to the zappy server structure.
 * @param team_name Name of the team whose egg to delete.
 */
void delete_egg_team_name(zappy_t *zappy_ptr, char *team_name);

/**
 * @brief Handle an incantation command
 * @param args Command arguments.
 * @param zappy_ptr Pointer to the zappy server structure.
 * @param client Pointer to the client.
 * @return Status of the incantation (success or failure).
 */
int handle_incantation(char **args, zappy_t *zappy_ptr, client_t *client);

/**
 * @brief Consume resources for an incantation.
 * @param zappy_ptr Pointer to the zappy server structure.
 * @param x X-coordinate of the incantation.
 * @param y Y-coordinate of the incantation.
 * @param level Level of the incantation.
 */
void consume_incantation(zappy_t *zappy_ptr, int x, int y, int level);

/**
 * @brief Check if an incantation is valid.
 * @param zappy_ptr Pointer to the zappy server structure.
 * @param client Pointer to the client.
 * @param level Level of the incantation.
 * @return True if valid, false otherwise.
 */
int check_incantation_valid(zappy_t *zappy_ptr, client_t *client, int level);

/**
 * @brief Add a command to the client's queue with a duration.
 * @param duration Duration of the command in ticks.
 * @param args Command arguments.
 * @param client Pointer to the client.
 * @return True if the command was added successfully, false otherwise.
 */
bool add_command(int duration, char **args, client_t *client);

/**
 * @brief Add a command to the second position client's queue with a duration.
 * @param duration Duration of the command in ticks.
 * @param args Command arguments.
 * @param client Pointer to the client.
 * @return True if the command was added successfully, false otherwise.
 */
bool add_command_second(int duration, char **args, client_t *client);

#endif /* !CLIENT_H */
