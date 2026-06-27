# CLI Commands

The firmware boots directly into the UART shell.

```text
help
info
led [on|off|toggle]
test list
test run <name> [key=value ...]
reboot
```

Examples:

```text
test run gpio.output pin=5 value=1
test run gpio.output pin=5 blink=5 ms=100
test run gpio.input pin=12 pullup=1
test run uart.poll message=hello
test run uart.rx-timeout ms=3000
test run i2c.scan hz=100000
test run i2c.probe addr=0x50
test run spi.loopback hz=1000000
test run spi.flash-id hz=1000000
test run pwm.duty channel=1 duty=500 period=1000
test run pwm.count ms=10
test run timer.delay ms=100
test run timer.irq ms=50
test run otp.read offset=0 len=16
test run rtc.tick ms=100
test run wdog.feed
test run wdog.reset confirm=YES_RESET
test run plic.basic irq=3 priority=1
test run aon.backup index=0 value=0x12345678
```

`embedded-cli` also provides command history and help/autocomplete behavior.
