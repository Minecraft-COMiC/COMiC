#ifndef COMIC_NETWORK_H
#define COMIC_NETWORK_H

#include <COMiC/core.h>
#include <COMiC/util.h>
#include <openssl/crypto.h>

#define DEFAULT_SERVER_IP ("0.0.0.0")
#define DEFAULT_SERVER_PORT (25565)

# ifdef __cplusplus
extern "C" {
# endif

typedef enum COMiC_Network_NetworkState
{
    HANDSHAKING = -1, PLAY, STATUS, LOGIN
} COMiC_Network_NetworkState;

// Packet ID is prefixed with Network State(raw id + 1)
typedef enum COMiC_Network_PacketID
{
// HANDSHAKING:
    HANDSHAKE_C2S_PACKET_ID,
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
} PacketID;

typedef struct COMiC_Network_ServerNetInfo
{
    COMiC_InternetAddress address;
    COMiC_Socket socket;
} COMiC_Network_ServerNetInfo;

typedef struct COMiC_Network_ClientNetInfo
{
    COMiC_InternetAddress address;
    COMiC_Socket socket;
    COMiC_Network_NetworkState state;
    COMiC_String username;
    COMiC_Util_JUUID uuid;
} COMiC_Network_ClientNetInfo;

typedef struct COMiC_Network_Buffer
{
    COMiC_Int8 *bytes;
    COMiC_UInt32 index;
    COMiC_UInt32 size;
} COMiC_Network_Buffer;

COMiC_Constructor
COMiC_Network_Buffer COMiC_Network_Buffer_Init(
        COMiC_In COMiC_UInt32 capacity
);

COMiC_Constructor
COMiC_Network_Buffer COMiC_Network_Buffer_InitWithDefaultCapacity(void);

void COMiC_Network_Buffer_Prepare(
        COMiC_Out COMiC_Network_Buffer *self
);

void COMiC_Network_Buffer_SkipBytes(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_UInt32 count
);

COMiC_Int8 COMiC_Network_Buffer_ReadByte(
        COMiC_Out COMiC_Network_Buffer *self
);

void COMiC_Network_Buffer_WriteByte(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int8 byte
);

COMiC_Int32 COMiC_Network_Buffer_ReadVarInt(
        COMiC_Out COMiC_Network_Buffer *self
);

void COMiC_Network_Buffer_WriteVarInt(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int32 value
);

COMiC_Int64 COMiC_Network_Buffer_ReadVarLong(
        COMiC_Out COMiC_Network_Buffer *self
);

void COMiC_Network_Buffer_WriteVarLong(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int64 value
);

COMiC_Bool COMiC_Network_Buffer_ReadBool(
        COMiC_Out COMiC_Network_Buffer *self
);

void COMiC_Network_Buffer_WriteBool(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int32 value
);

COMiC_Int16 COMiC_Network_Buffer_ReadShort(
        COMiC_Out COMiC_Network_Buffer *self
);

void COMiC_Network_Buffer_WriteShort(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int16 value
);

COMiC_Int32 COMiC_Network_Buffer_ReadInt(
        COMiC_Out COMiC_Network_Buffer *self
);

void COMiC_Network_Buffer_WriteInt(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int32 value
);

COMiC_Int64 COMiC_Network_Buffer_ReadLong(
        COMiC_Out COMiC_Network_Buffer *self
);

void COMiC_Network_Buffer_WriteLong(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int64 value
);

COMiC_Float COMiC_Network_Buffer_ReadFloat(
        COMiC_Out COMiC_Network_Buffer *self
);

void COMiC_Network_Buffer_WriteFloat(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Float value
);

COMiC_Double COMiC_Network_Buffer_ReadDouble(
        COMiC_Out COMiC_Network_Buffer *self
);

void COMiC_Network_Buffer_WriteDouble(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Double value
);

void COMiC_Network_Buffer_ReadString(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_Out COMiC_String out,
        COMiC_In COMiC_UInt32 max_length
);

void COMiC_Network_Buffer_WriteString(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_String str,
        COMiC_In COMiC_UInt32 max_length
);

COMiC_Int32 COMiC_Network_Buffer_ReadEnum(
        COMiC_Out COMiC_Network_Buffer *self
);

void COMiC_Network_Buffer_WriteEnum(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int32 value
);

void COMiC_Network_Buffer_ReadByteArray(
        COMiC_In COMiC_Network_Buffer *self,
        COMiC_Out COMiC_Int8 *out,
        COMiC_In COMiC_UInt32 count
);

void COMiC_Network_Buffer_WriteByteArray(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int8 *arr,
        COMiC_In COMiC_UInt32 count
);

void COMiC_Network_Buffer_WriteRSAKey(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In EVP_PKEY *key
);

COMiC_Int32 COMiC_Network_Buffer_ReadPacketID(
        COMiC_In COMiC_Network_Buffer *self
);

void COMiC_Network_Buffer_WritePacketID(
        COMiC_Out COMiC_Network_Buffer *self,
        COMiC_In COMiC_Int32 id
);

void COMiC_Network_Init(
        COMiC_Out COMiC_Network_ServerNetInfo *server
);

void COMiC_Network_ListenToConnections(
        COMiC_In COMiC_Network_ServerNetInfo server,
        COMiC_In COMiC_Network_ClientNetInfo *client,
        COMiC_In void (*onPacketReceive)(COMiC_Network_ClientNetInfo *, COMiC_Network_Buffer *)
);

void COMiC_Network_SendPacket(
        COMiC_In COMiC_Network_ClientNetInfo *connection,
        COMiC_In COMiC_Network_Buffer *buf
);

/*
void COMiC_Network_SendRequestEncryptionPacket(
        COMiC_In COMiC_Network_ClientNetInfo *connection
);
*/

void COMiC_Network_SendLoginSuccessPacket(
        COMiC_In COMiC_Network_ClientNetInfo *connection
);

void COMiC_Network_SendGameJoinPacket(
        COMiC_In COMiC_Network_ClientNetInfo *connection
);

void COMiC_Network_SendHeldItemChangePacket(
        COMiC_In COMiC_Network_ClientNetInfo *connection
);

void COMiC_Network_ReceivePacket(
        COMiC_In COMiC_Network_ClientNetInfo *connection,
        COMiC_In COMiC_Network_Buffer *buf
);

void COMiC_Network_Finalize(
        COMiC_In COMiC_Network_ServerNetInfo server
);

# ifdef __cplusplus
};
# endif

#endif /* COMiC_NETWORK_H */