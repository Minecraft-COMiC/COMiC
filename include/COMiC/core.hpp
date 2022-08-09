#ifndef COMIC_CORE_HPP
# define COMIC_CORE_HPP

# include <limits>
# include <algorithm>
# include <string>
# include <iostream>
# include <cstring>
# include <fstream>
# include <sstream>
# include <memory>
# include <map>
# include <any>
# include "core/types.hpp"

namespace COMiC
{
    inline bool alive = true;

    class VanillaConfig
    {
    private:
        std::map<std::string, std::any> values;

        template<typename T>
        T parseValue(const std::string &file, const std::string &name);

        template<typename T>
        void loadValue(const std::string &file, const std::string &name);

        template<typename T>
        void loadValueRange(const std::string &file, const std::string &name, T min, T max);

    public:
        explicit VanillaConfig(const std::string &filename = "server.properties");

        [[nodiscard]] inline bool broadcastRCONToOps() noexcept
        {
            return std::any_cast<bool>(this->values["broadcast-rcon-to-ops"]);
        }

        [[nodiscard]] inline U16 viewDistance() noexcept
        {
            return std::any_cast<U16>(this->values["view-distance"]);
        }

        [[nodiscard]] inline U16 maxBuildHeight() noexcept
        {
            return std::any_cast<U16>(this->values["max-build-height"]);
        }

        [[nodiscard]] inline std::string serverIP() noexcept
        {
            return std::any_cast<std::string>(this->values["server-ip"]);
        }

        [[nodiscard]] inline std::string levelSeed() noexcept
        {
            return std::any_cast<std::string>(this->values["level-seed"]);
        }

        [[nodiscard]] inline U16 RCONPort() noexcept
        {
            return std::any_cast<U16>(this->values["rcon.port"]);
        }

        [[nodiscard]] inline std::string gamemode() noexcept
        {
            return std::any_cast<std::string>(this->values["gamemode"]);
        }

        [[nodiscard]] inline U16 serverPort() noexcept
        {
            return std::any_cast<U16>(this->values["server-port"]);
        }

        [[nodiscard]] inline bool allowNether() noexcept
        {
            return std::any_cast<bool>(this->values["allow-nether"]);
        }

        [[nodiscard]] inline bool enableCommandBlock() noexcept
        {
            return std::any_cast<bool>(this->values["enable-command-block"]);
        }

        [[nodiscard]] inline bool enableRCON() noexcept
        {
            return std::any_cast<bool>(this->values["enable-rcon"]);
        }

        [[nodiscard]] inline bool enableQuery() noexcept
        {
            return std::any_cast<bool>(this->values["enable-query"]);
        }

        [[nodiscard]] inline U8 OpPermissionLevel() noexcept
        {
            return std::any_cast<U8>(this->values["op-permission-level"]);
        }

        [[nodiscard]] inline bool preventProxyConnections() noexcept
        {
            return std::any_cast<bool>(this->values["prevent-proxy-connections"]);
        }

        [[nodiscard]] inline std::string generatorSettings() noexcept
        {
            return std::any_cast<std::string>(this->values["generator-settings"]);
        }

        [[nodiscard]] inline std::string resourcePack() noexcept
        {
            return std::any_cast<std::string>(this->values["resource-pack"]);
        }

        [[nodiscard]] inline std::string levelName() noexcept
        {
            return std::any_cast<std::string>(this->values["level-name"]);
        }

        [[nodiscard]] inline std::string RCONPassword() noexcept
        {
            return std::any_cast<std::string>(this->values["rcon.password"]);
        }

        [[nodiscard]] inline U32 playerIdleTimeout() noexcept
        {
            return std::any_cast<U32>(this->values["player-idle-timeout"]);
        }

        [[nodiscard]] inline std::string motd() noexcept
        {
            return std::any_cast<std::string>(this->values["motd"]);
        }

        [[nodiscard]] inline U16 queryPort() noexcept
        {
            return std::any_cast<U16>(this->values["query.port"]);
        }

        [[nodiscard]] inline bool forceGamemode() noexcept
        {
            return std::any_cast<bool>(this->values["force-gamemode"]);
        }

        [[nodiscard]] inline bool hardcore() noexcept
        {
            return std::any_cast<bool>(this->values["hardcore"]);
        }

        [[nodiscard]] inline bool whitelist() noexcept
        {
            return std::any_cast<bool>(this->values["white-list"]);
        }

        [[nodiscard]] inline bool broadcastConsoleToOps() noexcept
        {
            return std::any_cast<bool>(this->values["broadcast-console-to-ops"]);
        }

        [[nodiscard]] inline bool pvp() noexcept
        {
            return std::any_cast<bool>(this->values["pvp"]);
        }

        [[nodiscard]] inline bool spawnNPCs() noexcept
        {
            return std::any_cast<bool>(this->values["spawn-npcs"]);
        }

        [[nodiscard]] inline bool generateStructures() noexcept
        {
            return std::any_cast<bool>(this->values["generate-structures"]);
        }

        [[nodiscard]] inline bool spawnAnimals() noexcept
        {
            return std::any_cast<bool>(this->values["spawn-animals"]);
        }

        [[nodiscard]] inline bool snooperEnabled() noexcept
        {
            return std::any_cast<bool>(this->values["snooper-enabled"]);
        }

        [[nodiscard]] inline std::string difficulty() noexcept
        {
            return std::any_cast<std::string>(this->values["difficulty"]);
        }

        [[nodiscard]] inline U8 functionPermissionLevel() noexcept
        {
            return std::any_cast<U8>(this->values["function-permission-level"]);
        }

        [[nodiscard]] inline I32 networkCompressionThreshold() noexcept
        {
            return std::any_cast<I32>(this->values["network-compression-threshold"]);
        }

        [[nodiscard]] inline std::string levelType() noexcept
        {
            return std::any_cast<std::string>(this->values["level-type"]);
        }

        [[nodiscard]] inline bool spawnMonsters() noexcept
        {
            return std::any_cast<bool>(this->values["spawn-monsters"]);
        }

        [[nodiscard]] inline I32 maxTickTime() noexcept
        {
            return std::any_cast<I32>(this->values["max-tick-time"]);
        }

        [[nodiscard]] inline bool enforceWhitelist() noexcept
        {
            return std::any_cast<bool>(this->values["enforce-whitelist"]);
        }

        [[nodiscard]] inline bool useNativeTransport() noexcept
        {
            return std::any_cast<bool>(this->values["use-native-transport"]);
        }

        [[nodiscard]] inline U32 maxPlayers() noexcept
        {
            return std::any_cast<U32>(this->values["max-players"]);
        }

        [[nodiscard]] inline std::string resourcePackSHA1() noexcept
        {
            return std::any_cast<std::string>(this->values["resource-pack-sha1"]);
        }

        [[nodiscard]] inline I32 spawnProtection() noexcept
        {
            return std::any_cast<I32>(this->values["spawn-protection"]);
        }

        [[nodiscard]] inline bool onlineMode() noexcept
        {
            return std::any_cast<bool>(this->values["online-mode"]);
        }

        [[nodiscard]] inline bool allowFlight() noexcept
        {
            return std::any_cast<bool>(this->values["allow-flight"]);
        }

        [[nodiscard]] inline U32 maxWorldSize() noexcept
        {
            return std::any_cast<U32>(this->values["max-world-size"]);
        }
    };

    inline VanillaConfig CONFIG;

    namespace OS
    {
        void setCtrlCHandler();
    }
}

#endif /* COMIC_CORE_HPP */
