#ifndef COMIC_CORE_HPP
# define COMIC_CORE_HPP

# include <limits>
# include <algorithm>
# include <string>
# include <iostream>
# include <cstring>
# include "core/types.hpp"

namespace COMiC
{
    inline bool alive = true;

    static inline IfError readFile(const std::string &filename, std::string &out)
    {
        FILE *fp = fopen(filename.c_str(), "r");
        if (fp == nullptr)
        {
            std::cerr << "readFile() failed: " << strerror(errno) << std::endl;
            return FAIL;
        }

        char *buf = nullptr;
        size_t len = 0;
        while (getline(&buf, &len, fp) != EOF)
            out += buf;

        fclose(fp);
        free(buf);

        return SUCCESS;
    }

    namespace Config
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

        IfError loadConfig(const std::string &filename = "server.properties");
    }
}

#endif /* COMIC_CORE_HPP */
