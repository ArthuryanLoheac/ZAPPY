#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstring>

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

    // Extract the actual encrypted message (remove magic key)
    message = message.substr(Data::i().magicKey.length());

    // Clean up any newlines from the message
    message.erase(std::remove(message.begin(), message.end(), '\n'),
        message.end());

    // Decrypt the message using our improved decryption method
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

const char Interface::base64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * @brief Encode a string using Base64
 *
 * @param input The binary data to encode
 * @return Base64 encoded string
 */
std::string Interface::base64Encode(const std::string& input) {
    std::string encoded;
    int val = 0;
    int valb = -6;

    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            encoded.push_back(base64Chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }

    if (valb > -6) {
        encoded.push_back(base64Chars[((val << 8) >> (valb + 8)) & 0x3F]);
    }

    while (encoded.size() % 4) {
        encoded.push_back('=');
    }

    return encoded;
}

/**
 * @brief Decode a Base64 encoded string
 *
 * @param encoded The Base64 encoded string
 * @return Decoded binary data
 */
std::string Interface::base64Decode(const std::string& encoded) {
    std::string decoded;
    int val = 0;
    int valb = -8;

    for (unsigned char c : encoded) {
        if (c == '=') break;

        const char* pos = std::strchr(base64Chars, c);
        if (pos == nullptr) continue;

        val = (val << 6) + static_cast<int>(pos - base64Chars);
        valb += 6;

        if (valb >= 0) {
            decoded.push_back(static_cast<char>((val >> valb) & 0xFF));
            valb -= 8;
        }
    }

    return decoded;
}

/**
 * @brief Encrypts a string using XOR with key and Base64 encoding
 *
 * This method applies an XOR operation with a key to each character of the input
 * and encodes the result using Base64, ensuring no control characters are present.
 *
 * @param input The string to encrypt
 * @param key The encryption key
 * @return The encrypted string
 */
std::string Interface::encrypt(const std::string& input,
    const std::string& key) {
    if (input.empty() || key.empty()) return "";

    std::string xored;
    xored.reserve(input.length());

    for (size_t i = 0; i < input.length(); ++i) {
        xored.push_back(input[i] ^ key[i % key.length()]);
    }

    return base64Encode(xored);
}

/**
 * @brief Decrypts a string that was encrypted using the encrypt method
 *
 * This method decodes the Base64 encrypted string and reverses the XOR operation
 * with the key to recover the original message.
 *
 * @param encrypted The encrypted string to decrypt
 * @param key The decryption key
 * @return The decrypted string
 */
std::string Interface::decrypt(const std::string& encrypted,
    const std::string& key) {
    if (encrypted.empty() || key.empty()) return "";

    std::string decoded = base64Decode(encrypted);
    std::string output;
    output.reserve(decoded.length());

    for (size_t i = 0; i < decoded.length(); ++i) {
        unsigned char c = decoded[i];
        unsigned char keyChar = key[i % key.length()];
        unsigned char original = c ^ keyChar;
        output.push_back(original);
    }

    return output;
}

/**
 * @brief Sends a message to the other players by using the broadcast command
 *
 * This method encrypts the message and sends it to the server using the
 * broadcast command. The message is prefixed with the magic key to identify
 * messages intended for our AI clients.
 *
 * @param message The message string to send
 */
void Interface::sendMessage(const std::string &message) {
    if (message.empty()) {
        return;
    }

    std::string encryptedMessage = encrypt(message, Data::i().magicKey);
    encryptedMessage = Data::i().magicKey + encryptedMessage;
    sendCommand("BROADCAST " + encryptedMessage + "\n");
}

}  // namespace AI
