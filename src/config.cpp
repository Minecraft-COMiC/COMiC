#include <chrono>
#include <COMiC/core.hpp>

namespace COMiC::Config
{
    static inline std::string kebab_case(const std::string &in)
    {
        auto copy = in;
        std::transform(copy.begin(), copy.end(), copy.begin(), [](char c)
        { return std::tolower(c); });
        std::replace(copy.begin(), copy.end(), '_', '-');

        return copy;
    }

#define ASSERT_RANGE(variable, min, max)                                                                                                        \
    if (!((variable) >= (min) && (variable) <= (max)))                                                                                          \
    {                                                                                                                                           \
        std::cerr << "checkRange() failed: `" << kebab_case(#variable) << "` must be in range [" << (min) << ", " << (max) << "]" << std::endl; \
        return FAIL;                                                                                                                            \
    }

    static inline IfError checkRange()
    {
        ASSERT_RANGE(MAX_PLAYERS, 0, 0x7fffffff)
        ASSERT_RANGE(MAX_WORLD_SIZE, 1, 29999984)
        ASSERT_RANGE(NETWORK_COMPRESSION_THRESHOLD, -1, 0x20000)
        ASSERT_RANGE(OP_PERMISSION_LEVEL, 1, 4)
        ASSERT_RANGE(QUERY_PORT, 1, 65534)
        ASSERT_RANGE(RCON_PORT, 1, 65534)
        ASSERT_RANGE(SERVER_PORT, 1, 65534)
        ASSERT_RANGE(SPAWN_PROTECTION, -1, 0x7fffffff)
        ASSERT_RANGE(VIEW_DISTANCE, 3, 32)

        return SUCCESS;
    }

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

    static inline IfError writeDefaultConfig()
    {
        FILE *fp = fopen("server.properties", "w");
        if (fp == nullptr)
        {
            std::cerr << "writeDefaultConfig() failed: " << strerror(errno) << std::endl;
            return FAIL;
        }

        std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string config_header = "#COMiC server properties\n#";
        config_header.append(std::ctime(&time));
        if (config_header[config_header.length() - 1] == '\n')
            config_header.erase(config_header.length() - 1);

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
max-world-size=29999984)";

        fprintf(fp, "%s%s\n", config_header.c_str(), config_data);
        fclose(fp);

        return SUCCESS;
    }

#define PARSE_AUTO(file, name) parseValue<decltype(name)>(file, #name, name)
#define PARSE_NAME(file, name, true_name) parseValue<decltype(name)>(file, true_name, name)

    IfError loadConfig(const std::string &filename)
    {
        std::string file_str;
        if (readFile(filename, file_str))
        {
            std::cout << "Could not open config file, creating a default one." << std::endl;
            if (writeDefaultConfig())
            {
                std::cerr << "Could not write default config file" << std::endl;
                return FAIL;
            }

            if (readFile("server.properties", file_str))
            {
                std::cerr << "Could not read default config file" << std::endl;
                return FAIL;
            }
        }

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

        if (checkRange())
        {
            std::cerr << "Some of the config values were invalid" << std::endl;
            return FAIL;
        }

        return SUCCESS;
    }
}