from netty_types import *
from pck_loader import packet_type


class C2S_Handshake(metaclass=packet_type[0x00]):
    protocol_version: VarInt
    server_address: String
    server_port: UShort
    next_state: VarInt


class C2S_LoginStart(metaclass=packet_type[0x00]):
    name: String


class S2C_EncryptionRequest(metaclass=packet_type[0x01]):
    server_id: String
    public_key_len: VarInt
    public_key: bytes
    verify_token_len: VarInt
    verify_token: bytes


class C2S_EncryptionResponse(metaclass=packet_type[0x01]):
    shared_secret_length: VarInt
    shared_secret: bytes
    verify_token_length: VarInt
    verify_token: bytes


class S2C_SetCompression(metaclass=packet_type[0x03]):
    threshold: VarInt


class S2C_LoginSuccess(metaclass=packet_type[0x02]):
    uuid: UUID
    username: String


class S2C__Login:
    @classmethod
    def read(cls, istream):
        ln = VarInt.read(istream)
        tp = istream.read(1)
        if len(tp) != 1:
            raise EOFError

        if tp == (0x01):
            return S2C_EncryptionRequest.read.no_type_check(istream)
        elif tp == (0x02):
            return S2C_LoginSuccess.read.no_type_check(istream)
        elif tp == (0x03):
            return S2C_SetCompression.read.no_type_check(istream)
