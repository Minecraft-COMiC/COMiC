#ifndef COMIC_CORE_HPP
# define COMIC_CORE_HPP

# include <limits>
# include <algorithm>
# include <string>
# include <iostream>
# include <cstring>
#include <fstream>
#include <sstream>
# include "core/types.hpp"

namespace COMiC
{
    inline bool BROADCAST_RCON_TO_OPS;
    inline U16 VIEW_DISTANCE, MAX_BUILD_HEIGHT;
    inline std::string SERVER_IP, LEVEL_SEED;
    inline U16 RCON_PORT;
    inline std::string GAMEMODE;
    inline U16 SERVER_PORT;
    inline bool ALLOW_NETHER, ENABLE_COMMAND_BLOCK, ENABLE_RCON, ENABLE_QUERY;
    inline U8 OP_PERMISSION_LEVEL;
    inline bool PREVENT_PROXY_CONNECTIONS;
    inline std::string GENERATOR_SETTINGS, RESOURCE_PACK, LEVEL_NAME, RCON_PASSWORD;
    inline U32 PLAYER_IDLE_TIMEOUT;
    inline std::string MOTD;
    inline U32 QUERY_PORT;
    inline bool FORCE_GAMEMODE, HARDCORE, WHITE_LIST, BROADCAST_CONSOLE_TO_OPS, PVP, SPAWN_NPCS, GENERATE_STRUCTURES, SPAWN_ANIMALS, SNOOPER_ENABLED;
    inline std::string DIFFICULTY;
    inline U8 FUNCTION_PERMISSION_LEVEL;
    inline I32 NETWORK_COMPRESSION_THRESHOLD;
    inline std::string LEVEL_TYPE;
    inline bool SPAWN_MONSTERS;
    inline I32 MAX_TICK_TIME;
    inline bool ENFORCE_WHITELIST, USE_NATIVE_TRANSPORT;
    inline U32 MAX_PLAYERS;
    inline std::string RESOURCE_PACK_SHA1;
    inline I32 SPAWN_PROTECTION;
    inline bool ONLINE_MODE, ALLOW_FLIGHT;
    inline U32 MAX_WORLD_SIZE;

    static inline void readFile(const std::string &filename, std::string &out)
    {
        FILE *fp = fopen(filename.c_str(), "r");
        if (fp == nullptr)
        {
            std::cerr << "readFile() failed: " << strerror(errno) << std::endl;
            return;
        }

        char *buf = nullptr;
        size_t len = 0;
        while (getline(&buf, &len, fp) != EOF)
            out += buf;

        fclose(fp);
        free(buf);
    }

    static inline std::string kebab_case(const std::string &in)
    {
        auto copy = in;
        std::transform(copy.begin(), copy.end(), copy.begin(), [](char c)
        { return std::tolower(c); });
        std::replace(copy.begin(), copy.end(), '_', '-');

        return copy;
    }

#define ASSERT_RANGE(variable, min, max) \
    if (!(variable >= min && variable <= max)) \
        std::cerr << "checkRange() failed: `" << kebab_case(#variable) << "` must be in range [" << min << ", " << max << "]" << std::endl

    static inline void checkRange()
    {
        ASSERT_RANGE(MAX_PLAYERS, 0, 0x7fffffff);
        ASSERT_RANGE(MAX_WORLD_SIZE, 1, 29999984);
        ASSERT_RANGE(NETWORK_COMPRESSION_THRESHOLD, -1, 0x20000);
        ASSERT_RANGE(OP_PERMISSION_LEVEL, 1, 4);
        ASSERT_RANGE(QUERY_PORT, 1, 65534);
        ASSERT_RANGE(RCON_PORT, 1, 65534);
        ASSERT_RANGE(SERVER_PORT, 1, 65534);
        ASSERT_RANGE(SPAWN_PROTECTION, -1, 0x7fffffff);
        ASSERT_RANGE(VIEW_DISTANCE, 3, 32);
    }

#undef ASSERT_RANGE

    // TODO: Pretty names instead of mangled
    template<typename T>
    static inline void parseValue(const std::string &file, const std::string &name, T &out)
    {
        auto copy = kebab_case(name);

        auto line_start = file.find(copy);
        auto line_end = file.find('\n', line_start);
        auto value_start = file.find('=', line_start);

        if (line_start == std::string::npos)
        {
            std::cerr << "parseValue() failed: could not find `" << copy << '`' << std::endl;
            return;
        }

        if (line_end == std::string::npos || value_start == std::string::npos)
        {
            std::cerr << "parseValue() failed: configuration file is of invalid format" << std::endl;
            return;
        }

        std::string to_parse = file.substr(value_start + 1, line_end - value_start - 1);

        if constexpr(std::is_same_v<T, bool>)
        {
            if (to_parse == "true")
                out = true;
            else if (to_parse == "false")
                out = false;
            else
                std::cerr << "parseValue() failed: received `" << to_parse << "` (for parameter `"
                          << copy << "`) that is not boolean" << std::endl;
        }
        else if constexpr(std::is_integral_v<T>)
        {
            U64 value;
            try
            {
                value = std::stoull(to_parse);
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << "parseValue() failed: received `" << to_parse << "` (for parameter `"
                          << copy << "`) that is not valid integer" << std::endl;
                return;
            }
            catch (const std::out_of_range &e)
            {
                std::cerr << "parseValue() failed: value provided for integer parameter `" << copy << "` is too big"
                          << std::endl;
                return;
            }

            if ((std::is_unsigned_v<T> && value <= std::numeric_limits<T>::max()) ||
                (std::is_signed_v<T> && (I64) value >= std::numeric_limits<T>::min() &&
                (I64) value <= std::numeric_limits<T>::max()))
                out = (T) value;
            else
                std::cerr << "parseValue() failed: `" << copy << "` = " << value << " does not fit in type `"
                          << typeid(T).name() << '`' << std::endl;
        }
        else if constexpr(std::is_same_v<T, std::string>)
            out = to_parse;
        else
            std::cerr << "parseValue() failed: unexpected type `" << typeid(T).name() << "` for parameter `"
                      << copy << '`' << std::endl;
    }

#define PARSE_AUTO(file, name) parseValue<decltype(name)>(file, #name, name)
#define PARSE_NAME(file, name, true_name) parseValue<decltype(name)>(file, true_name, name)

    static inline void loadConfig(const std::string &filename = "server.properties")
    {
        std::string file_str;
        readFile(filename, file_str);

        PARSE_AUTO(file_str, BROADCAST_RCON_TO_OPS);
        PARSE_AUTO(file_str, VIEW_DISTANCE);
        PARSE_AUTO(file_str, MAX_BUILD_HEIGHT);
        PARSE_AUTO(file_str, SERVER_IP);
        PARSE_AUTO(file_str, LEVEL_SEED);
        PARSE_NAME(file_str, RCON_PORT, "rcon.port");
        PARSE_AUTO(file_str, GAMEMODE);
        PARSE_AUTO(file_str, SERVER_PORT);
        PARSE_AUTO(file_str, ALLOW_NETHER);
        PARSE_AUTO(file_str, ENABLE_COMMAND_BLOCK);
        PARSE_AUTO(file_str, ENABLE_RCON);
        PARSE_AUTO(file_str, ENABLE_QUERY);
        PARSE_AUTO(file_str, OP_PERMISSION_LEVEL);
        PARSE_AUTO(file_str, PREVENT_PROXY_CONNECTIONS);
        PARSE_AUTO(file_str, GENERATOR_SETTINGS);
        PARSE_AUTO(file_str, RESOURCE_PACK);
        PARSE_AUTO(file_str, LEVEL_NAME);
        PARSE_NAME(file_str, RCON_PASSWORD, "rcon.password");
        PARSE_AUTO(file_str, PLAYER_IDLE_TIMEOUT);
        PARSE_AUTO(file_str, MOTD);
        PARSE_NAME(file_str, QUERY_PORT, "query.port");
        PARSE_AUTO(file_str, FORCE_GAMEMODE);
        PARSE_AUTO(file_str, HARDCORE);
        PARSE_AUTO(file_str, WHITE_LIST);
        PARSE_AUTO(file_str, BROADCAST_CONSOLE_TO_OPS);
        PARSE_AUTO(file_str, PVP);
        PARSE_AUTO(file_str, SPAWN_NPCS);
        PARSE_AUTO(file_str, GENERATE_STRUCTURES);
        PARSE_AUTO(file_str, SPAWN_ANIMALS);
        PARSE_AUTO(file_str, SNOOPER_ENABLED);
        PARSE_AUTO(file_str, DIFFICULTY);
        PARSE_AUTO(file_str, FUNCTION_PERMISSION_LEVEL);
        PARSE_AUTO(file_str, NETWORK_COMPRESSION_THRESHOLD);
        PARSE_AUTO(file_str, LEVEL_TYPE);
        PARSE_AUTO(file_str, SPAWN_MONSTERS);
        PARSE_AUTO(file_str, MAX_TICK_TIME);
        PARSE_AUTO(file_str, ENFORCE_WHITELIST);
        PARSE_AUTO(file_str, USE_NATIVE_TRANSPORT);
        PARSE_AUTO(file_str, MAX_PLAYERS);
        PARSE_AUTO(file_str, RESOURCE_PACK_SHA1);
        PARSE_AUTO(file_str, SPAWN_PROTECTION);
        PARSE_AUTO(file_str, ONLINE_MODE);
        PARSE_AUTO(file_str, ALLOW_FLIGHT);
        PARSE_AUTO(file_str, MAX_WORLD_SIZE);

        checkRange();
    }

#undef PARSE_AUTO
#undef PARSE_NAME


}

#endif /* COMIC_CORE_HPP */
