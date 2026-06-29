# FE310 RedBoard Bring-Up

1. Build:

   ```sh
   make
   ```

2. Flash:

   ```sh
   make flash
   ```

   This uses `openocd/redv_redboard.cfg`.  To use a different OpenOCD binary
   or config:

   ```sh
   make flash OPENOCD=/path/to/openocd OPENOCD_CFG=/path/to/board.cfg
   ```

3. Find the serial port:

   ```sh
   python3 scripts/serial_cli.py --list
   ```

4. Open the CLI:

   ```sh
   python3 scripts/serial_cli.py --port /dev/ttyACM0 --baud 115200
   ```

5. Smoke test:

   ```text
   info
   led toggle
   test list
   test run gpio.output pin=5 blink=3 ms=100
   test run timer.delay ms=100
   ```

If serial output is garbled, rebuild with the actual peripheral clock:

```sh
make clean
make CPU_HZ=<actual-hz>
```
