import sys
import socket
import pck_loader
from hashlib import sha1

from packets import *

# server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# server_socket.bind(("localhost", 8080))
# server_socket.listen(10)

if 0:
    c = socket.create_server(("localhost", 8081))
    c = c.accept()[0]
    class proxy:
        def __init__(self, o):
            self.o = o

        def read(self, n):
            b = bytes()
            while len(b) < n:
                u = self.o.recv(1)
                if u:
                    b += u
            return b


    ostream = c.makefile("wb")
    istream = proxy(c)

    ln = VarInt.read(istream)
    print(istream.read(ln))
    ln = VarInt.read(istream)
    print(istream.read(ln))
    S2C_EncryptionRequest("", 1, b"1", 1, b"1").write(ostream)
    ostream.flush()

    ln = VarInt.read(istream)
    print(istream.read(ln))
else:
    c = socket.create_connection(("localhost", 8082))


    class proxy:
        def __init__(self, o):
            self.o = o

        def read(self, n):
            b = bytes()
            while len(b) < n:
                u = self.o.recv(1)
                if u:
                    b += u
            return b


    ostream = c.makefile("wb")
    istream = proxy(c)
    C2S_Handshake(340, "mc-worldofanarchy.ru", 55373, 2).write(ostream)
    print("handshake sent")
    C2S_LoginStart("dos_ataker").write(ostream)
    print("loginstart sent")
    ostream.flush()
    r = S2C__Login.read(istream)
    if isinstance(r, S2C_EncryptionRequest):
        b = sha1()
        b.update(str(r.server_id).encode("ascii"))
        b.update(b"0" * 16)
        b.update(r.public_key)
        b = hex(int.from_bytes(b.digest(), byteorder='big', signed=True))[2:].encode("ascii")
        bb = sha1()
        bb.update(r.verify_token)
        bb.update(b"0" * 16)
        bb.update(r.public_key)
        bb = hex(int.from_bytes(bb.digest(), byteorder='big', signed=True))[2:].encode("ascii")

        C2S_EncryptionResponse(len(b), b, len(bb), bb).write(ostream)
        print("encryption sent")
        ostream.flush()
        raise Exception("pizdec")

    if isinstance(r, S2C_SetCompression):
        print(f"compression set {int(r.threshold)}")
        r = S2C__Login.read(istream)
    print("login success")

    while True:
        ln = VarInt.read(istream)
        # print(istream.read(1)[0])
        print(istream.read(ln))
# while True:
#     b = c.recv(1)
#     if b:
#         print(b)
# c.bind(("localhost", 8080))
# c.listen(10)
# c.close()


# c = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# c.connect(("mc-worldofanarchy.ru", 25565))
# with c.makefile("wb") as ostream:
#     C2S_LoginStart("dos_ataker").write(ostream)
# c.close()
