#ifndef COMIC_NETWORK_HPP
#define COMIC_NETWORK_HPP

#include <cstdlib>
#include <cstring>
#include <COMiC/core.hpp>
#include <COMiC/util.hpp>
#include <COMiC/crypto.hpp>

namespace COMiC::Network
{
    const inline char *DEFAULT_SERVER_IP = "127.0.0.1";
    const inline I32 DEFAULT_SERVER_PORT = 25565;

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
        QUERY_PONG_S2C_PACKET_ID,
// LOGIN:
        LOGIN_DISCONNECT_S2C_PACKET_ID = 0x300,
        LOGIN_HELLO_S2C_PACKET_ID,
        LOGIN_SUCCESS_S2C_PACKET_ID,
        LOGIN_COMPRESSION_S2C_PACKET_ID,
        LOGIN_QUERY_REQUEST_S2C_PACKET_ID,
        LOGIN_HELLO_C2S_PACKET_ID = 0x300,
        LOGIN_KEY_C2S_PACKET_ID,
        LOGIN_QUERY_RESPONSE_C2S_PACKET_ID,
    };

    namespace OS
    {
        struct InetAddr;
        struct Socket;
    }

    struct ClientNetInfo
    {
        OS::InetAddr *address;
        OS::Socket *socket;
        NetworkState state;
        char *username;
        Util::UUID *uuid;
        bool encrypted;
        Crypto::AES *cipher;
    };

    struct Buffer
    {
    private:
        enum {DATA_START = 5};

    public:
        Byte *bytes;
        USize index;
        USize size;

        Buffer(const Byte *bytes, USize index, USize size) : index(index), size(size)
        {
            this->bytes = new Byte[size];
            memcpy(this->bytes, bytes, size);
        }

        explicit Buffer(USize capacity) : size(capacity)
        {
            this->index = DATA_START;
            this->bytes = new Byte[capacity];
        }

        Buffer() : Buffer(128)
        {
        }

        ~Buffer()
        {
            delete[] bytes;
        }

        void prepare();

        void skip(USize count);

        Byte read();

        void write(Byte byte);

        I32 readVarInt();

        void writeVarInt(I32 value);

        I64 readVarLong();

        void writeVarLong(I64 value);

        bool readBool();

        void writeBool(bool value);

        I16 readShort();

        void writeShort(I16 value);

        I32 readInt();

        void writeInt(I32 value);

        I64 readLong();

        void writeLong(I64 value);

        float readFloat();

        void writeFloat(float value);

        double readDouble();

        void writeDouble(double value);

        void readString(char *out, USize maxlen);

        void writeString(const char *str, USize maxlen);

        I32 readEnum();

        void writeEnum(I32 value);

        void readByteArray(Byte *out, I32 count);

        void writeByteArray(const Byte *arr, I32 count);

        PacketID readPacketID(NetworkState state);

        void writePacketID(PacketID id);
    };

    struct NetManager
    {
        OS::InetAddr *address;
        OS::Socket *socket;
        Crypto::RSA *rsa;

        void sendRequestEncryptionPacket(ClientNetInfo *connection) const;

        static void sendLoginSuccessPacket(ClientNetInfo *connection);

        static void sendGameJoinPacket(ClientNetInfo *connection);

        static void sendHeldItemChangePacket(ClientNetInfo *connection);

        void receivePacket(ClientNetInfo *connection, Buffer *buf) const;
    };

    void init(NetManager *server);

    void listenToConnections(NetManager server, ClientNetInfo *client);

    void sendPacket(ClientNetInfo *connection, Buffer *buf);

    void finalize(NetManager server);

    void sendHTTPGet(const std::string &server, const std::string &page, std::string &out);
}

#endif /* COMiC_NETWORK_HPP */