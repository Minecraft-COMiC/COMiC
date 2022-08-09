#include <chrono>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <map>
#include <any>
#include <COMiC/core.hpp>
#include <COMiC/io.hpp>

namespace COMiC
{
    void writeDefaultConfig()
    {
        std::string config_header = "#COMiC server properties\n#";

        // Get local time:
        char strtime[100];
        std::time_t t = std::time(nullptr);
        std::strftime(strtime, sizeof(strtime), "%a %b %d %T %Y", std::localtime(&t));
        config_header.append(strtime);

        auto config_data = R"(
broadcast-rcon-to-ops=false
view-distance=10
max-build-height=256
server-ip=127.0.0.1
level-seed=
rcon.port=25575
gamemode=survival
server-port=25565
allow-nether=true
enable-command-block=true
enable-rcon=false
enable-query=false
op-permission-level=4
prevent-proxy-connections=false
generator-settings=
resource-pack=
level-name=world
rcon.password=
player-idle-timeout=0
motd=A COMiC Server
query.port=25565
force-gamemode=false
hardcore=false
white-list=false
broadcast-console-to-ops=false
pvp=true
spawn-npcs=true
generate-structures=true
spawn-animals=true
snooper-enabled=true
difficulty=normal
function-permission-level=2
network-compression-threshold=256
level-type=default
spawn-monsters=true
max-tick-time=60000
enforce-whitelist=false
use-native-transport=true
max-players=20
resource-pack-sha1=
spawn-protection=0
online-mode=true
allow-flight=true
max-world-size=29999984
)";

        IO::writeFile("server.properties", config_header + config_data);
    }

    template<typename T>
    T VanillaConfig::parseValue(const std::string &file, const std::string &name)
    {
        auto line_start = file.find(name);
        auto line_end = file.find('\n', line_start);
        auto value_start = file.find('=', line_start);

        if (line_start == std::string::npos || line_end == std::string::npos || value_start == std::string::npos)
            throw std::logic_error("COMiC: Could not find config parameter `" + name + "`");

        std::string to_parse = file.substr(value_start + 1, line_end - value_start - 1);

        if constexpr(std::is_same_v<T, bool>)
        {
            if (to_parse == "true")
                return true;
            else if (to_parse == "false")
                return false;
            else
                throw std::invalid_argument(
                        "COMiC: Config parameter `" + name + "` must be either `true` or `false` but `" +
                        to_parse + "` was provided"
                );
        }
        else if constexpr(std::is_integral_v<T>)
        {
            I64 value;
            try
            {
                value = std::stoll(to_parse);
            }
            catch (std::invalid_argument &)
            {
                throw std::invalid_argument(
                        "COMiC: Received `" + to_parse + "` (for config parameter `" + name +
                        "`) that is not a valid integer"
                );
            }
            catch (std::out_of_range &)
            {
                throw std::out_of_range(
                        "COMiC: Value provided for config parameter `" + name + "` does not fit in any integral type"
                );
            }

            if ((std::is_unsigned_v<T> && value >= 0 && value <= std::numeric_limits<T>::max()) ||
                (std::is_signed_v<T> && value >= std::numeric_limits<T>::min() &&
                 value <= std::numeric_limits<T>::max()))
                return (T) value;
            else
                throw std::out_of_range(
                        "COMiC: Config parameter `" + name + "` must at least be in range [" +
                        std::to_string(std::numeric_limits<T>::min()) + ", " +
                        std::to_string(std::numeric_limits<T>::max()) + ']'
                );
        }
        else if constexpr(std::is_same_v<T, std::string>)
            return to_parse;
        else
            throw std::invalid_argument("COMiC: Unexpected type for config parameter `" + name + '`');
    }

    template<typename T>
    void VanillaConfig::loadValue(const std::string &file, const std::string &name)
    {
        this->values[name] = std::make_any<T>(parseValue<T>(file, name));
    }

    template<typename T>
    void VanillaConfig::loadValueRange(const std::string &file, const std::string &name, T min, T max)
    {
        auto tmp = parseValue<T>(file, name);
        if (tmp < min || tmp > max)
            throw std::out_of_range(
                    "COMiC: Config parameter `" + name + "` must be in range [" +
                    std::to_string(min) + ", " + std::to_string(max) + ']'
            );

        this->values[name] = std::make_any<T>(tmp);
    }

    VanillaConfig::VanillaConfig(const std::string &filename)
    {
        std::string contents;
        try
        {
            contents = IO::readFile(filename);
        }
        catch (IO::IOError &) // Most likely configuration file was not created, so try a default one instead
        {
            std::cout << "Could not open config file, creating a default one" << std::endl;
            writeDefaultConfig();
            contents = IO::readFile("server.properties");
        }

        loadValue<bool>(contents, "broadcast-rcon-to-ops");
        loadValueRange<U16>(contents, "view-distance", 2, 32);
        loadValue<U16>(contents, "max-build-height");
        loadValue<std::string>(contents, "server-ip");
        loadValue<std::string>(contents, "level-seed");
        loadValueRange<U16>(contents, "rcon.port", 1, 65534);
        loadValue<std::string>(contents, "gamemode");
        loadValueRange<U16>(contents, "server-port", 1, 65534);
        loadValue<bool>(contents, "allow-nether");
        loadValue<bool>(contents, "enable-command-block");
        loadValue<bool>(contents, "enable-rcon");
        loadValue<bool>(contents, "enable-query");
        loadValueRange<U8>(contents, "op-permission-level", 0, 4);
        loadValue<bool>(contents, "prevent-proxy-connections");
        loadValue<std::string>(contents, "generator-settings");
        loadValue<std::string>(contents, "resource-pack");
        loadValue<std::string>(contents, "level-name");
        loadValue<std::string>(contents, "rcon.password");
        loadValue<U32>(contents, "player-idle-timeout");
        loadValue<std::string>(contents, "motd");
        loadValueRange<U16>(contents, "query.port", 1, 65534);
        loadValue<bool>(contents, "force-gamemode");
        loadValue<bool>(contents, "hardcore");
        loadValue<bool>(contents, "white-list");
        loadValue<bool>(contents, "broadcast-console-to-ops");
        loadValue<bool>(contents, "pvp");
        loadValue<bool>(contents, "spawn-npcs");
        loadValue<bool>(contents, "generate-structures");
        loadValue<bool>(contents, "spawn-animals");
        loadValue<bool>(contents, "snooper-enabled");
        loadValue<std::string>(contents, "difficulty");
        loadValueRange<U8>(contents, "function-permission-level", 1, 4);
        loadValueRange<I32>(contents, "network-compression-threshold", std::numeric_limits<I32>::min(), 0x200000);
        loadValue<std::string>(contents, "level-type");
        loadValue<bool>(contents, "spawn-monsters");
        loadValue<I32>(contents, "max-tick-time");
        loadValue<bool>(contents, "enforce-whitelist");
        loadValue<bool>(contents, "use-native-transport");
        loadValueRange<U32>(contents, "max-players", 0, 0x7fffffff);
        loadValue<std::string>(contents, "resource-pack-sha1");
        loadValue<I32>(contents, "spawn-protection");
        loadValue<bool>(contents, "online-mode");
        loadValue<bool>(contents, "allow-flight");
        loadValueRange<U32>(contents, "max-world-size", 1, 29999984);
    }
}