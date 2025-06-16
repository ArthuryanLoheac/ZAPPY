#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "Interface/Interface.hpp"
#include "Exceptions/Commands.hpp"
#include "Data/Data.hpp"

/**
 * @file Communication.cpp
 * @brief Implementation of communication-related command handlers
 *
 * This file contains the implementation of commands that involve
 * communicating with other players through broadcasts and message handling.
 */

namespace AI {

/**
 * @brief Handles the BROADCAST command response
 *
 * Processes the response from the server after attempting to broadcast a message.
 *
 * @param args Server response arguments
 * @param command Original command sent
 */
void Interface::commandBROADCAST(std::vector<std::string> &args,
    std::vector<std::string> &command) {
    if (args.size() < 2) {
        throw AI::CommandArgumentsException("BROADCAST",
            "Expected at least one argument, got " +
            std::to_string(args.size() - 1));
    }
    (void)command;
}

/**
 * @brief Processes received broadcast messages
 *
 * Filters and processes messages received from other players.
 * Adds valid messages to the message queue with their direction.
 *
 * @param args The message arguments including direction
 */
void Interface::receiveMessage(std::vector<std::string> &args) {
    if (args.size() != 3) {
        throw AI::CommandArgumentsException("MESSAGE",
            "Expected 3 arguments, got " +
            std::to_string(args.size() - 1));
    }

    std::string message = args[1];
    int direction = std::stoi(args[2]);

    if (needFilter(message)) {
        return;
    }

    message = message.substr(Data::i().magicKey.length());

    message = decrypt(message, Data::i().magicKey);

    Data::i().messageQueue.push({message, direction});
}

/**
 * @brief Initializes the list of filtered broadcast messages
 *
 * Sets up a list of message patterns that should be ignored when received.
 */
void Interface::initializeFilteredStrings() {
    filteredStrings = {
        "Salut c'est Frank Leboeuf, vous voulez vendre votre voiture ?"
    };
}

/**
 * @brief Checks if a message should be filtered
 *
 * Determines if a received message should be ignored based on filter patterns.
 * This function filters out unwanted or spam messages.
 *
 * @param arg The message to check
 * @return true if the message should be filtered, false otherwise
 */
bool Interface::needFilter(const std::string &arg) {
    if (std::find(filteredStrings.begin(), filteredStrings.end(), arg) !=
        filteredStrings.end()) {
        return true;
    }

    if (!arg.starts_with(Data::i().magicKey))
        return true;

    return false;
}

/**
 * @brief Encrypts a string using a simple XOR-based method
 *
 * This method applies an XOR operation with a key to each character of the input
 * and maps the result to a printable ASCII range (32-126).
 *
 * @param input The string to encrypt
 * @param key The encryption key
 * @return The encrypted string
 */
std::string Interface::encrypt(const std::string& input,
    const std::string& key) {
    std::string output;
    output.reserve(input.length());

    for (size_t i = 0; i < input.length(); ++i) {
        unsigned char c = input[i] ^ key[i % key.length()];
        unsigned char printable = (c % 95) + 32;
        output.push_back(printable);
    }

    return output;
}

/**
 * @brief Decrypts a string that was encrypted using the encrypt method
 *
 * This method reverses the encryption by applying the XOR operation with the key
 * and normalizing the result back to its original character.
 *
 * @param encrypted The encrypted string to decrypt
 * @param key The decryption key
 * @return The decrypted string
 */
std::string Interface::decrypt(const std::string& encrypted,
    const std::string& key) {
    std::string output;
    output.reserve(encrypted.length());

    for (size_t i = 0; i < encrypted.length(); ++i) {
        unsigned char c = encrypted[i];
        unsigned char normalized = c - 32;
        unsigned char keyChar = key[i % key.length()];
        unsigned char original = normalized ^ keyChar;
        output.push_back(original);
    }

    return output;
}

/**
 * @brief Sends a message to the other players by using the broadcast command
 *
 * This method encrypts the message and sends it to the server using the
 * broadcast command.
 *
 * @param message The message string to send
 */
void Interface::sendMessage(const std::string &message) {
    if (message.empty()) {
        return;
    }

    std::string encryptedMessage = encrypt(message, Data::i().magicKey);
    encryptedMessage = Data::i().magicKey + encryptedMessage;
    sendCommand("BROADCAST " + encryptedMessage);
}

}  // namespace AI
