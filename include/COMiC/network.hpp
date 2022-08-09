#ifndef COMIC_NETWORK_HPP
#define COMIC_NETWORK_HPP

#include <cstdlib>
#include <cstring>
#include <map>
#include <vector>
#include <COMiC/core.hpp>
#include <COMiC/util.hpp>
#include <COMiC/crypto.hpp>
#include <COMiC/compression.hpp>

namespace COMiC::Network
{
    using Socket = U64;

    class NetworkError : public Error
    {
    private:
        static std::string getNativeError();

    protected:
        std::string message;

    public:
        explicit NetworkError(const std::string &str) : NetworkError(str.c_str())
        {
        }

        explicit NetworkError(const char *str)
        {
            auto err = getNativeError();

            this->message = "COMiC::Network: ";
            this->message.append(str);
            this->message.append(" (");
            if (err.empty())
                this->message.append("Unknown system error");
            else
                this->message.append(err);
            this->message.append(")");
        }
    };

    enum NetworkState
    {
        HANDSHAKING = -1,
        PLAY = 0,
        STATUS = 1,
        LOGIN = 2
    };

// Packet ID is prefixed with Network State (raw id + 1)
    enum PacketID
    {
// HANDSHAKING:
        HANDSHAKE_C2S_PACKET_ID = 0x000,
// PLAY:
        ENTITY_SPAWN_S2C_PACKET_ID = 0x100,
        EXPERIENCE_ORB_SPAWN_S2C_PACKET_ID,
        ENTITY_SPAWN_GLOBAL_S2C_PACKET_ID,
        MOB_SPAWN_S2C_PACKET_ID,
        PAINTING_SPAWN_S2C_PACKET_ID,
        PLAYER_SPAWN_S2C_PACKET_ID,
        ENTITY_ANIMATION_S2C_PACKET_ID,
        STATISTICS_S2C_PACKET_ID,
        BLOCK_BREAKING_PROGRESS_S2C_PACKET_ID,
        BLOCK_ENTITY_UPDATE_S2C_PACKET_ID,
        BLOCK_ACTION_S2C_PACKET_ID,
        BLOCK_UPDATE_S2C_PACKET_ID,
        BOSS_BAR_S2C_PACKET_ID,
        DIFFICULTY_S2C_PACKET_ID,
        CHAT_MESSAGE_S2C_PACKET_ID,
        CHUNK_DELTA_UPDATE_S2C_PACKET_ID,
        COMMAND_SUGGESTIONS_S2C_PACKET_ID,
        COMMAND_TREE_S2C_PACKET_ID,
        CONFIRM_GUI_ACTION_S2C_PACKET_ID,
        CLOSE_CONTAINER_S2C_PACKET_ID,
        INVENTORY_S2C_PACKET_ID,
        CONTAINER_PROPERTY_UPDATE_S2C_PACKET_ID,
        CONTAINER_SLOT_UPDATE_S2C_PACKET_ID,
        COOLDOWN_UPDATE_S2C_PACKET_ID,
        CUSTOM_PAYLOAD_S2C_PACKET_ID,
        PLAY_SOUND_ID_S2C_PACKET_ID,
        DISCONNECT_S2C_PACKET_ID,
        ENTITY_STATUS_S2C_PACKET_ID,
        EXPLOSION_S2C_PACKET_ID,
        UNLOAD_CHUNK_S2C_PACKET_ID,
        GAME_STATE_CHANGE_S2C_PACKET_ID,
        OPEN_HORSE_CONTAINER_S2C_PACKET_ID,
        KEEP_ALIVE_S2C_PACKET_ID,
        CHUNK_DATA_S2C_PACKET_ID,
        WORLD_EVENT_S2C_PACKET_ID,
        PARTICLE_S2C_PACKET_ID,
        LIGHT_UPDATE_S2C_PACKET_ID,
        GAME_JOIN_S2C_PACKET_ID,
        MAP_UPDATE_S2C_PACKET_ID,
        SET_TRADE_OFFERS_S2C_PACKET_ID,
        ENTITY_S2C_PACKET$MOVE_RELATIVE_PACKET_ID,
        ENTITY_S2C_PACKET$ROTATE_AND_MOVE_RELATIVE_PACKET_ID,
        ENTITY_S2C_PACKET$ROTATE_PACKET_ID,
        ENTITY_S2C_PACKET_ID,
        VEHICLE_MOVE_S2C_PACKET_ID,
        OPEN_WRITTEN_BOOK_S2C_PACKET_ID,
        OPEN_CONTAINER_S2C_PACKET_ID,
        SIGN_EDITOR_OPEN_S2C_PACKET_ID,
        CRAFT_FAILED_RESPONSE_S2C_PACKET_ID,
        PLAYER_ABILITIES_S2C_PACKET_ID,
        COMBAT_EVENT_S2C_PACKET_ID,
        PLAYER_LIST_S2C_PACKET_ID,
        LOOK_AT_S2C_PACKET_ID,
        PLAYER_POSITION_LOOK_S2C_PACKET_ID,
        UNLOCK_RECIPES_S2C_PACKET_ID,
        ENTITIES_DESTROY_S2C_PACKET_ID,
        REMOVE_ENTITY_STATUS_EFFECT_S2C_PACKET_ID,
        RESOURCE_PACK_SEND_S2C_PACKET_ID,
        PLAYER_RESPAWN_S2C_PACKET_ID,
        ENTITY_SET_HEAD_YAW_S2C_PACKET_ID,
        SELECT_ADVANCEMENT_TAB_S2C_PACKET_ID,
        WORLD_BORDER_S2C_PACKET_ID,
        SET_CAMERA_ENTITY_S2C_PACKET_ID,
        HELD_ITEM_CHANGE_S2C_PACKET_ID,
        CHUNK_RENDER_DISTANCE_CENTER_S2C_PACKET_ID,
        CHUNK_LOAD_DISTANCE_S2C_PACKET_ID,
        SCOREBOARD_DISPLAY_S2C_PACKET_ID,
        ENTITY_TRACKER_UPDATE_S2C_PACKET_ID,
        ENTITY_ATTACH_S2C_PACKET_ID,
        ENTITY_VELOCITY_UPDATE_S2C_PACKET_ID,
        ENTITY_EQUIPMENT_UPDATE_S2C_PACKET_ID,
        EXPERIENCE_BAR_UPDATE_S2C_PACKET_ID,
        HEALTH_UPDATE_S2C_PACKET_ID,
        SCOREBOARD_OBJECTIVE_UPDATE_S2C_PACKET_ID,
        ENTITY_PASSENGERS_SET_S2C_PACKET_ID,
        TEAM_S2C_PACKET_ID,
        SCOREBOARD_PLAYER_UPDATE_S2C_PACKET_ID,
        PLAYER_SPAWN_POSITION_S2C_PACKET_ID,
        WORLD_TIME_UPDATE_S2C_PACKET_ID,
        TITLE_S2C_PACKET_ID,
        PLAY_SOUND_FROM_ENTITY_S2C_PACKET_ID,
        PLAY_SOUND_S2C_PACKET_ID,
        STOP_SOUND_S2C_PACKET_ID,
        PLAYER_LIST_HEADER_S2C_PACKET_ID,
        TAG_QUERY_RESPONSE_S2C_PACKET_ID,
        ITEM_PICKUP_ANIMATION_S2C_PACKET_ID,
        ENTITY_POSITION_S2C_PACKET_ID,
        ADVANCEMENT_UPDATE_S2C_PACKET_ID,
        ENTITY_ATTRIBUTES_S2C_PACKET_ID,
        ENTITY_STATUS_EFFECT_S2C_PACKET_ID,
        SYNCHRONIZE_RECIPES_S2C_PACKET_ID,
        SYNCHRONIZE_TAGS_S2C_PACKET_ID,
        PLAYER_ACTION_RESPONSE_S2C_PACKET_ID,
        TELEPORT_CONFIRM_C2S_PACKET_ID = 0x100,
        QUERY_BLOCK_NBT_C2S_PACKET_ID,
        UPDATE_DIFFICULTY_C2S_PACKET_ID,
        CHAT_MESSAGE_C2S_PACKET_ID,
        CLIENT_STATUS_C2S_PACKET_ID,
        CLIENT_SETTINGS_C2S_PACKET_ID,
        REQUEST_COMMAND_COMPLETIONS_C2S_PACKET_ID,
        CONFIRM_GUI_ACTION_C2S_PACKET_ID,
        BUTTON_CLICK_C2S_PACKET_ID,
        CLICK_WINDOW_C2S_PACKET_ID,
        GUI_CLOSE_C2S_PACKET_ID,
        CUSTOM_PAYLOAD_C2S_PACKET_ID,
        BOOK_UPDATE_C2S_PACKET_ID,
        QUERY_ENTITY_NBT_C2S_PACKET_ID,
        PLAYER_INTERACT_ENTITY_C2S_PACKET_ID,
        KEEP_ALIVE_C2S_PACKET_ID,
        UPDATE_DIFFICULTY_LOCK_C2S_PACKET_ID,
        PLAYER_MOVE_C2S_PACKET$POSITION_ONLY_PACKET_ID,
        PLAYER_MOVE_C2S_PACKET$BOTH_PACKET_ID,
        PLAYER_MOVE_C2S_PACKET$LOOK_ONLY_PACKET_ID,
        PLAYER_MOVE_C2S_PACKET_ID,
        VEHICLE_MOVE_C2S_PACKET_ID,
        BOAT_PADDLE_STATE_C2S_PACKET_ID,
        PICK_FROM_INVENTORY_C2S_PACKET_ID,
        CRAFT_REQUEST_C2S_PACKET_ID,
        UPDATE_PLAYER_ABILITIES_C2S_PACKET_ID,
        PLAYER_ACTION_C2S_PACKET_ID,
        CLIENT_COMMAND_C2S_PACKET_ID,
        PLAYER_INPUT_C2S_PACKET_ID,
        RECIPE_BOOK_DATA_C2S_PACKET_ID,
        RENAME_ITEM_C2S_PACKET_ID,
        RESOURCE_PACK_STATUS_C2S_PACKET_ID,
        ADVANCEMENT_TAB_C2S_PACKET_ID,
        SELECT_VILLAGER_TRADE_C2S_PACKET_ID,
        UPDATE_BEACON_C2S_PACKET_ID,
        UPDATE_SELECTED_SLOT_C2S_PACKET_ID,
        UPDATE_COMMAND_BLOCK_C2S_PACKET_ID,
        UPDATE_COMMAND_BLOCK_MINECART_C2S_PACKET_ID,
        CREATIVE_INVENTORY_ACTION_C2S_PACKET_ID,
        UPDATE_JIGSAW_C2S_PACKET_ID,
        UPDATE_STRUCTURE_BLOCK_C2S_PACKET_ID,
        UPDATE_SIGN_C2S_PACKET_ID,
        HAND_SWING_C2S_PACKET_ID,
        SPECTATOR_TELEPORT_C2S_PACKET_ID,
        PLAYER_INTERACT_BLOCK_C2S_PACKET_ID,
        PLAYER_INTERACT_ITEM_C2S_PACKET_ID,
// STATUS:
        QUERY_REQUEST_C2S_PACKET_ID = 0x200,
        QUERY_RESPONSE_S2C_PACKET_ID = 0x200,
        QUERY_PING_C2S_PACKET_ID,
        QUERY_PONG_S2C_PACKET_ID = 0x201,
// LOGIN:
        LOGIN_DISCONNECT_S2C_PACKET_ID = 0x300,
        LOGIN_HELLO_S2C_PACKET_ID,
        LOGIN_SUCCESS_S2C_PACKET_ID,
        LOGIN_COMPRESSION_S2C_PACKET_ID,
        LOGIN_QUERY_REQUEST_S2C_PACKET_ID,
        LOGIN_HELLO_C2S_PACKET_ID = 0x300,
        LOGIN_KEY_C2S_PACKET_ID,
        LOGIN_QUERY_RESPONSE_C2S_PACKET_ID
    };

    struct ClientNetInfo
    {
    private:
        std::string address;
        Socket socket;

    public:
        NetworkState state = HANDSHAKING;
        std::string username;
        Util::UUID uuid;
        bool encrypted = false;
        Crypto::AES cipher;
        bool compressed = false;
        bool open = true;

        ClientNetInfo(Socket socket, std::string address);

        [[nodiscard]] const std::string &getIP() const;

        [[nodiscard]] Socket getSocket() const;

        bool operator==(const ClientNetInfo &other) const
        {
            return this->getSocket() == other.getSocket();
        }

        bool operator<(const ClientNetInfo &other) const
        {
            return this->getSocket() < other.getSocket();
        }

        bool operator>(const ClientNetInfo &other) const
        {
            return this->getSocket() > other.getSocket();
        }
    };

    class RecvBuf
    {
    private:
        USize index = 0;
        ByteVector bytes;

        void skip(USize count);

    public:
        RecvBuf(Byte *data, USize len, const ClientNetInfo &connection)
        {
            if (connection.encrypted)
                connection.cipher.decrypt(data, len, data);

            bytes.assign(data, data + std::min(10ULL, len));

            if (connection.compressed)
            {
                I32 data_length = this->readVarInt(); // Uncompressed data length
                if (data_length > 0)
                {
                    this->bytes = Compression::INFLATER.decompress(
                        data + this->index,
                        len - this->index
                    );

                    this->index = 0;
                    return;
                }
            }

            if (len > 10)
                this->bytes.insert(this->bytes.end(), data + 10, data + len);
        }

        [[nodiscard]] static inline I32 getPacketSize(const ClientNetInfo &connection, Byte *&data, USize len)
        {
            if (connection.encrypted)
                connection.cipher.decrypt(data, std::min(5ULL, len), data);

            I32 res = 0;
            Byte byte;
            for (I32 i = 0; i < std::min(6ULL, len); i++)
            {
                byte = data[i];
                data++;
                res |= (byte & 0x7F) << (i * 7);

                if ((byte & 0x80) != 0x80) break;
            }

            return res;
        }

        Byte read();

        I32 readVarInt();

        I64 readVarLong();

        bool readBool();

        I16 readShort();

        I32 readInt();

        I64 readLong();

        float readFloat();

        double readDouble();

        std::string readString(USize maxlen);

        I32 readEnum();

        ByteVector readByteArray();

        PacketID readPacketID(NetworkState state);

        [[nodiscard]] inline Byte *position() noexcept
        {
            return this->bytes.data() + this->index;
        }

        [[nodiscard]] inline const ByteVector &getBytes() const noexcept
        {
            return this->bytes;
        }
    };

    class SendBuf
    {
    private:
        enum
        {
            DATA_START = 5
        };

        ByteVector bytes;
        USize index = DATA_START;
        USize size;

        void prependSize();

        void skip(USize count);

    public:
        explicit SendBuf(USize capacity) : size(capacity)
        {
            this->bytes.reserve(capacity);
            for (I32 i = 0; i < DATA_START; i++)
                this->bytes.push_back(0);
        }

        SendBuf() : SendBuf(128)
        {
        }

        void prepare(ClientNetInfo &connection);

        void write(Byte byte);

        void write(std::initializer_list<Byte> data);

        void writeVarInt(I32 value);

        void writeVarLong(I64 value);

        void writeBool(bool value);

        void writeShort(I16 value);

        void writeInt(I32 value);

        void writeLong(I64 value);

        void writeFloat(float value);

        void writeDouble(double value);

        void writeString(const std::string &str, USize maxlen);

        void writeEnum(I32 value);

        void writeByteArray(const ByteVector &arr);

        void writePacketID(PacketID id);

        [[nodiscard]] inline Byte *data() const
        {
            return const_cast<Byte *>(this->bytes.data()) + DATA_START;
        }

        [[nodiscard]] inline Byte *position() const
        {
            return const_cast<Byte *>(this->bytes.data()) + this->index;
        }

        [[nodiscard]] inline USize length() const
        {
            return this->index - DATA_START;
        }

        [[nodiscard]] inline USize getSize() const
        {
            return this->size;
        }

        [[nodiscard]] inline USize getIndex() const
        {
            return this->index;
        }

        [[nodiscard]] inline ByteVector &getBytes()
        {
            return this->bytes;
        }
    };

    class NetworkManager
    {
    private:
        Socket socket; // Server socket
        std::map<Socket, ClientNetInfo> clients; // Clients
        Crypto::RSA rsa = Crypto::RSA(false);

    public:
        NetworkManager();

        void start();

        void end();

        inline ClientNetInfo &addClient(Socket s, std::string ip) noexcept
        {
            return this->clients.try_emplace(s, s, ip).first->second;
        }

        void closeSocket(Socket s, bool connected);

        void disconnect(ClientNetInfo &connection, const std::string &reason = "");

        static std::string sendHTTPGet(const std::string &server, const std::string &page);

        ~NetworkManager();

        [[nodiscard]] inline Socket getSocket() const noexcept
        {
            return this->socket;
        }

        // S -> C:
        void sendPacket(ClientNetInfo &connection, SendBuf &buf);

        // Login:
        void sendRequestEncryptionPacket(ClientNetInfo &connection);

        void sendLoginSuccessPacket(ClientNetInfo &connection);

        void sendSetCompressionPacket(ClientNetInfo &connection, I32 threshold);

        // Status:
        void sendStatusResponsePacket(ClientNetInfo &connection);

        void sendPongPacket(ClientNetInfo &connection, I64 payload);

        void sendLegacyPong(ClientNetInfo &connection);

        // Play:
        void sendDisconnectPacket(ClientNetInfo &connection, const std::string &reason);

        void sendHeldItemChangePacket(ClientNetInfo &connection);

        // C -> S:
        void receivePacket(ClientNetInfo &connection, RecvBuf buf);

        // Login:
        void handleEncryptionResponsePacket(ClientNetInfo &connection, RecvBuf &buf);
    };

    inline NetworkManager NETSERVICE;
}

#endif /* COMiC_NETWORK_HPP */