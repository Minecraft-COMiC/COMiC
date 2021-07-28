from io import BytesIO

from netty_types import *


class C2S_Handshake:
    @property
    def protocol_version(self):
        return self._protocol_version

    @protocol_version.setter
    def protocol_version(self, v):
        self._protocol_version = VarInt(v)

    @property
    def server_address(self):
        return self._server_address

    @server_address.setter
    def server_address(self, v):
        self._server_address = String(v)

    @property
    def server_port(self):
        return self._server_port

    @server_port.setter
    def server_port(self, v):
        self._server_port = UShort(v)

    @property
    def next_state(self):
        return self._next_state

    @next_state.setter
    def next_state(self, v):
        self._next_state = VarInt(v)

    def __init__(self, protocol_version, server_address, server_port, next_state):
        self.protocol_version = protocol_version
        self.server_address = server_address
        self.server_port = server_port
        self.next_state = next_state

    def write(self, ostream):
        with BytesIO() as pck:
            VarInt(0).write(pck)
            self.protocol_version.write(pck)
            self.server_address.write(pck)
            self.server_port.write(pck)
            self.next_state.write(pck)
            pck.seek(0)
            pck = pck.read()

        VarInt(len(pck)).write(ostream)
        ostream.write(pck)


class C2S_LoginStart:
    @property
    def name(self):
        return self._name

    @name.setter
    def name(self, v):
        self._name = String(v)

    def __init__(self, name):
        self.name = name

    def write(self, ostream):
        with BytesIO() as pck:
            VarInt(0).write(pck)
            self.name.write(pck)
            pck.seek(0)
            pck = pck.read()

        VarInt(len(pck)).write(ostream)
        ostream.write(pck)


class S2C_EncryptionRequest:
    @property
    def server_id(self):
        return self._server_id

    @server_id.setter
    def server_id(self, v):
        self._server_id = String(v)

    @property
    def public_key_len(self):
        return self._public_key_len

    @public_key_len.setter
    def public_key_len(self, v):
        self._public_key_len = VarInt(v)

    @property
    def public_key(self):
        return self._public_key

    @public_key.setter
    def public_key(self, v):
        self._public_key = bytes(v)

    @property
    def verify_token_len(self):
        return self._verify_token_len

    @verify_token_len.setter
    def verify_token_len(self, v):
        self._verify_token_len = VarInt(v)

    @property
    def verify_token(self):
        return self._verify_token

    @verify_token.setter
    def verify_token(self, v):
        self._verify_token = bytes(v)

    def __init__(self, server_id, public_key_len, public_key, verify_token_len, verify_token):
        self.server_id = server_id
        self.public_key_len = public_key_len
        self.public_key = public_key
        self.verify_token_len = verify_token_len
        self.verify_token = verify_token

    @classmethod
    def read(cls, istream):
        ln = VarInt.read(istream)
        pck = istream.read(ln)
        if len(pck) != ln:
            raise EOFError

        with BytesIO(pck) as pck:
            assert VarInt.read(pck) == 0x01
            server_id = String.read(pck)
            public_key_len = VarInt.read(pck)
            public_key = pck.read(public_key_len)
            if len(public_key) != public_key_len:
                raise EOFError
            verify_token_len = VarInt.read(pck)
            verify_token = pck.read(verify_token_len)
            if len(verify_token) != verify_token_len:
                raise EOFError
        return cls(server_id, public_key_len, public_key, verify_token_len, verify_token)

    def write(self, ostream):
        with BytesIO() as pck:
            VarInt(0x01).write(pck)
            self.server_id.write(pck)
            self.public_key_len.write(pck)
            pck.write(self.public_key)
            self.verify_token_len.write(pck)
            pck.write(self.verify_token)
            pck.seek(0)
            pck = pck.read()

        VarInt(len(pck)).write(ostream)
        ostream.write(pck)


class C2S_EncryptionResponse:
    @property
    def shared_secret_length(self):
        return self._shared_secret_length

    @shared_secret_length.setter
    def shared_secret_length(self, v):
        self._shared_secret_length = VarInt(v)

    @property
    def shared_secret(self):
        return self._shared_secret

    @shared_secret.setter
    def shared_secret(self, v):
        self._shared_secret = bytes(v)

    @property
    def verify_token_length(self):
        return self._shared_secret_length

    @verify_token_length.setter
    def verify_token_length(self, v):
        self._verify_token_length = VarInt(v)

    @property
    def verify_token(self):
        return self._verify_token

    @verify_token.setter
    def verify_token(self, v):
        self._verify_token = bytes(v)

    def __init__(self, shared_secret_length, shared_secret, verify_token_length, verify_token):
        self.shared_secret_length = shared_secret_length
        self.shared_secret = shared_secret
        self.verify_token_length = verify_token_length
        self.verify_token = verify_token

    def write(self, ostream):
        with BytesIO() as pck:
            VarInt(0x01).write(pck)
            self.shared_secret_length.write(pck)
            pck.write(self.shared_secret)
            self.verify_token_length.write(pck)
            pck.write(self.verify_token)
            pck.seek(0)
            pck = pck.read()

        VarInt(len(pck) + 1).write(ostream)
        ostream.write(pck + b"\x00")


class S2C_SetCompression:
    @property
    def threshold(self):
        return self._threshold

    @threshold.setter
    def threshold(self, v):
        self._threshold = VarInt(v)

    def __init__(self, threshold):
        self.threshold = threshold

    @classmethod
    def read(cls, istream):
        ln = VarInt.read(istream)
        pck = istream.read(ln)
        if len(pck) != ln:
            raise EOFError

        with BytesIO(pck) as pck:
            assert VarInt.read(pck) == 0x03
            threshold = VarInt.read(pck)
        return cls(threshold)


class S2C_LoginSuccess:
    @property
    def uuid(self):
        return self._uuid

    @uuid.setter
    def uuid(self, v):
        self._uuid = UUID(v)

    @property
    def username(self):
        return self._username

    @username.setter
    def username(self, v):
        self._username = String(v)

    def __init__(self, uuid, username):
        self.uuid = uuid
        self.username = username

    @classmethod
    def read(cls, istream):
        ln = VarInt.read(istream)
        pck = istream.read(ln)
        if len(pck) != ln:
            raise EOFError

        with BytesIO(pck) as pck:
            assert VarInt.read(pck) == 0x02
            uuid = UUID.read(pck)
            userename = String.read(pck)
        return cls(uuid, userename)


class S2C__Login:
    @classmethod
    def read(cls, istream):
        ln = VarInt.read(istream)
        tp = istream.read(1)
        if len(tp) != 1:
            raise EOFError

        with BytesIO() as readed:
            ln.write(readed)
            readed.write(tp)
            readed.seek(0)
            readed = readed.read()

        tp = tp[0]
        def _read(n):
            nonlocal readed
            if readed:
                bb = readed[:n]
                readed = readed[n:]
                bb += istream.read(n - len(bb))
            else:
                bb = istream.read(n)
            return bb
        class reader:
            read = staticmethod(_read)

        if tp == (0x01):
            return S2C_EncryptionRequest.read(reader)
        elif tp == (0x02):
            return S2C_LoginSuccess.read(reader)
        elif tp == (0x03):
            return S2C_SetCompression.read(reader)

