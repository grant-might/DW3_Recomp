"""A minimal client of the GDB remote serial protocol, for watching a game
in an emulator that runs a GDB server (tools/smoke.py).

Only what the smoke test needs: stop the target, read its memory and
registers, set breakpoints, let it run. Every packet is
$<payload>#<checksum>, acknowledged with +.
"""
import socket
import time

INTERRUPT = b"\x03"

# the registers that a g packet has for a MIPS target, in its order
REGISTERS = ("zero at v0 v1 a0 a1 a2 a3 t0 t1 t2 t3 t4 t5 t6 t7 s0 s1 s2 s3 s4 s5 s6 s7 "
             "t8 t9 k0 k1 gp sp fp ra sr lo hi badvaddr cause pc").split()


class GdbClient:
    def __init__(self, port, timeout):
        """Connects to the server on localhost, retrying until it listens."""
        deadline = time.monotonic() + timeout
        while True:
            try:
                self.sock = socket.create_connection(("127.0.0.1", port), timeout=10)
                break
            except OSError:
                if time.monotonic() > deadline:
                    raise
                time.sleep(0.5)
        self.buffer = b""

    def close(self):
        self.sock.close()

    def _send(self, payload):
        checksum = sum(payload) & 0xFF
        self.sock.sendall(b"$%s#%02x" % (payload, checksum))

    def _reply(self, timeout=None):
        """The next packet's payload, skipping the acknowledgements; None
        when TIMEOUT seconds pass without one."""
        self.sock.settimeout(timeout if timeout is not None else 60)
        while True:
            start = self.buffer.find(b"$")
            end = self.buffer.find(b"#", start)
            if start >= 0 and end >= 0 and len(self.buffer) >= end + 3:
                payload = self.buffer[start + 1 : end]
                self.buffer = self.buffer[end + 3 :]
                self.sock.sendall(b"+")
                return payload
            try:
                data = self.sock.recv(4096)
            except TimeoutError:
                if timeout is None:
                    raise
                return None
            if not data:
                raise EOFError("the GDB server closed the connection")
            self.buffer += data

    def halt(self):
        """Stops the target (a server stops it on connecting too)."""
        self.sock.sendall(INTERRUPT)
        return self._reply()

    def stop_reason(self):
        self._send(b"?")
        return self._reply()

    def read(self, address, size):
        self._send(b"m%x,%x" % (address, size))
        reply = self._reply()
        if reply.startswith(b"E") and len(reply) == 3:
            raise OSError(f"can't read {size:#x} bytes at {address:#010x}: {reply.decode()}")
        return bytes.fromhex(reply.decode())

    def registers(self):
        self._send(b"g")
        # the server may add registers it doesn't have, as xxxxxxxx
        reply = bytes.fromhex(self._reply()[: 8 * len(REGISTERS)].decode())
        return {name: int.from_bytes(reply[4 * i : 4 * i + 4], "little") for i, name in enumerate(REGISTERS)}

    def set_breakpoint(self, address):
        self._send(b"Z0,%x,4" % address)
        if self._reply() != b"OK":
            raise OSError(f"the GDB server can't break at {address:#010x}")

    def remove_breakpoint(self, address):
        self._send(b"z0,%x,4" % address)
        self._reply()

    def cont(self):
        """Lets the target run: the reply comes when it stops (halt, wait)."""
        self._send(b"c")

    def wait(self, timeout):
        """The reply of a target that runs, once it stops; None if it is
        still running after TIMEOUT seconds."""
        return self._reply(timeout)
