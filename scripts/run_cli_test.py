#!/usr/bin/env python3
"""Run one CLI test command and watch for PASS/FAIL."""

from __future__ import annotations

import argparse
import re
import sys
import time


try:
    import serial
except ImportError:
    print("pyserial is required: python -m pip install pyserial", file=sys.stderr)
    raise SystemExit(1)


def main() -> None:
    parser = argparse.ArgumentParser(description="Run one FE310 CLI hardware test")
    parser.add_argument("--port", required=True)
    parser.add_argument("--baud", type=int, default=115200)
    parser.add_argument("--timeout", type=float, default=10.0)
    parser.add_argument("command", nargs="+", help='Command, for example: test run gpio.output pin=5 value=1')
    args = parser.parse_args()

    command = " ".join(args.command).strip()
    done = re.compile(rb"\b(PASS|FAIL)\b")

    with serial.Serial(args.port, args.baud, timeout=0.05) as ser:
        ser.reset_input_buffer()
        ser.write((command + "\r").encode())
        deadline = time.monotonic() + args.timeout
        captured = bytearray()

        while time.monotonic() < deadline:
            chunk = ser.read(256)
            if chunk:
                sys.stdout.buffer.write(chunk)
                sys.stdout.buffer.flush()
                captured.extend(chunk)
                match = done.search(captured)
                if match:
                    raise SystemExit(0 if match.group(1) == b"PASS" else 2)

    print("\nTimed out waiting for PASS/FAIL", file=sys.stderr)
    raise SystemExit(3)


if __name__ == "__main__":
    main()
