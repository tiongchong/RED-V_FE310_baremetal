# Hardware Test Matrix

| Test | Requirement | Expected Result |
| --- | --- | --- |
| `gpio.output` | LED or probed GPIO pin | Pin changes or blinks |
| `gpio.input` | External level or pull-up | CLI prints sampled value |
| `uart.poll` | Host UART terminal | Message appears in terminal |
| `uart.rx-timeout` | Host sends one key after command | CLI prints received byte |
| `i2c.scan` | I2C/Qwiic device attached | One or more addresses found |
| `i2c.probe` | Known 7-bit device address | Probe returns `PASS` |
| `spi.loopback` | Wire SPI1 MOSI to MISO | RX pattern equals TX pattern |
| `spi.flash-id` | External SPI flash on SPI1 CS0 | JEDEC ID is not `00` or `ff` |
| `pwm.duty` | Optional scope/logic analyzer | PWM compare registers configured |
| `pwm.count` | None | Counter advances |
| `timer.delay` | None | `mtime` advances by expected ticks |
| `timer.irq` | Machine timer interrupt enabled by test | Callback fires |
| `otp.read` | None | OTP bytes are dumped |
| `rtc.tick` | AON RTC available | RTC count advances |
| `wdog.feed` | None | Watchdog starts, feeds, disables |
| `wdog.reset` | Explicit confirmation | Board resets |
| `plic.basic` | None | Priority/enable path executes |
| `aon.backup` | AON backup register writable | Readback matches |

## Guarded Tests

`wdog.reset` requires:

```text
confirm=YES_RESET
```

OTP programming is not implemented in the default driver.  Leave it disabled
unless you have a sacrificial device and a documented OTP address allocation.
