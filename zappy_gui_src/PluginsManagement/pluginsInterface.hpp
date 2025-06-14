#pragma once

class pluginsInterface {
 public:
    virtual ~pluginsInterface() = default;

    /**
    * @brief Initialize the plugin.
    * @return true if initialization was successful, false otherwise.
    */
    virtual bool init() = 0;

    /**
    * @brief Get the name of the plugin.
    * @return The name of the plugin.
    */
    virtual const char* getName() const = 0;

    /**
    * @brief Get the version of the plugin.
    * @return The version of the plugin.
    */
    virtual const char* getVersion() const = 0;
};
