class Boolean(int):
    @classmethod
    def read(cls, istream):
        raw = istream.read(1)
        if len(raw) != 1:
            raise EOFError
        if raw == b"\x01":
            return cls(True)
        elif raw == b"\x00":
            return cls(False)
        raise ValueError

    def write(self, ostream):
        if self:
            ostream.write(b"\x01")
        else:
            ostream.write(b"\x00")


class VarInt(int):
    @classmethod
    def read(cls, istream):
        decoded_int = 0
        bit_offset = 0

        while True:
            current_byte = istream.read(1)
            if len(current_byte) != 1:
                raise EOFError
            current_byte = current_byte[0]

            decoded_int |= (current_byte & 0b01111111) << bit_offset

            if bit_offset == 35:
                raise ValueError("VarInt is too big");

            bit_offset += 7
            if (current_byte & 0b10000000) == 0:
                break
        return cls(decoded_int)

    def write(self, ostream):
        value = self

        while True:
            curren_byte = value & 0b01111111

            value >>= 7
            if value != 0:
                curren_byte |= 0b10000000
            ostream.write(bytes([curren_byte & 0xff]))

            if value == 0:
                break


class UShort(int):
    @classmethod
    def read(cls, istream):
        raw = istream.read(2)

        if len(raw) != 2:
            raise EOFError

        return cls((raw[0] << 8) | raw[1])

    def write(self, ostream):
        ostream.write(bytes([(self // 256) % 256, self % 256]))


class String(str):
    @classmethod
    def read(cls, istream):
        ln = VarInt.read(istream)
        raw = istream.read(ln)
        if len(raw) != ln:
            raise EOFError
        return cls(raw.decode("utf-8"))

    def write(self, ostream):
        raw = self.encode("utf-8")
        VarInt(len(raw)).write(ostream)
        ostream.write(raw)


class UUID(int):
    @classmethod
    def read(cls, istream):
        raw = istream.read(16)
        if len(raw) != 16:
            raise EOFError

        return int.from_bytes(raw, "big", signed=False)

    def write(self, ostream):
        ostream.write(self.to_bytes(16, "big", signed=False))


class BytesBase(bytes):
    @classmethod
    def read(cls, istream, n):
        raw = istream.read(n)
        if len(raw) != n:
            raise EOFError
        return cls(raw)

    def write(self, ostream):
        ostream.write(self)


class Bytes(type):
    def __new__(cls, field_name):
        return super().__new__(f"Bytes[.{field_name}]", (BytesBase,), {"field_name": field_name})

    def __init__(self, field_name):
        pass
