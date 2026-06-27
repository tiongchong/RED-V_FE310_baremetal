#!/usr/bin/env python3
"""Tiny cross-platform UART terminal for the FE310 embedded-cli shell."""

from __future__ import annotations

import argparse
import os
import queue
import sys
import threading
import time


def die(message: str) -> None:
    print(message, file=sys.stderr)
    raise SystemExit(1)


try:
    import serial
    import serial.tools.list_ports
except ImportError:
    die("pyserial is required: python -m pip install pyserial")


def list_ports() -> None:
    ports = list(serial.tools.list_ports.comports())
    if not ports:
        print("No serial ports found")
        return
    for port in ports:
        print(f"{port.device:24} {port.description}")


def stdin_reader(out_q: "queue.Queue[bytes]", stop: threading.Event) -> None:
    if os.name == "nt":
        import msvcrt

        while not stop.is_set():
            if msvcrt.kbhit():
                ch = msvcrt.getwch()
                if ch == "\x03":
                    stop.set()
                    return
                out_q.put(ch.encode(errors="ignore"))
            else:
                time.sleep(0.01)
    else:
        import select
        import termios
        import tty

        fd = sys.stdin.fileno()
        old = termios.tcgetattr(fd)
        try:
            tty.setraw(fd)
            while not stop.is_set():
                readable, _, _ = select.select([sys.stdin], [], [], 0.05)
                if readable:
                    data = os.read(fd, 1)
                    if data == b"\x03":
                        stop.set()
                        return
                    out_q.put(data)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old)


def terminal(port: str, baud: int) -> None:
    stop = threading.Event()
    tx_q: "queue.Queue[bytes]" = queue.Queue()

    with serial.Serial(port, baudrate=baud, timeout=0.02) as ser:
        print(f"Connected to {port} at {baud}. Ctrl-C exits.")
        reader = threading.Thread(target=stdin_reader, args=(tx_q, stop), daemon=True)
        reader.start()

        try:
            while not stop.is_set():
                rx = ser.read(256)
                if rx:
                    sys.stdout.buffer.write(rx)
                    sys.stdout.buffer.flush()

                try:
                    tx = tx_q.get_nowait()
                except queue.Empty:
                    continue

                if tx == b"\n":
                    tx = b"\r"
                ser.write(tx)
        except KeyboardInterrupt:
            stop.set()
        finally:
            stop.set()
            print("\nDisconnected.")


def main() -> None:
    parser = argparse.ArgumentParser(description="FE310 UART CLI terminal")
    parser.add_argument("--port", help="Serial port, for example COM5 or /dev/ttyACM0")
    parser.add_argument("--baud", type=int, default=115200)
    parser.add_argument("--list", action="store_true", help="List serial ports")
    args = parser.parse_args()

    if args.list:
        list_ports()
        return

    if not args.port:
        parser.error("--port is required unless --list is used")

    terminal(args.port, args.baud)


if __name__ == "__main__":
    main()
