# Driver Style

Each IP driver has one public header in `include/drivers/` and one
implementation in `src/drivers/`.

Keep drivers:

- Register-level and polling-first.
- Free of CLI dependencies.
- Free of board pinmux assumptions.
- Small enough that hardware bring-up can inspect each register sequence.

Tests are allowed to depend on board helpers and CLI printing.

## Error Handling

Bus drivers return explicit status codes where transactions can fail:

```c
fe310_i2c_status_t rc = fe310_i2c_probe(&i2c0, 0x50);
```

Simple register drivers use direct functions and leave validation to the test.

## Adding a Driver Test

1. Add `tests/<ip>/test_<ip>_<case>.c`.
2. Expose the test function prototype in `tests/test_registry.c`.
3. Add a `{ "ip.case", "help text", test_ip_case }` registry entry.
4. Document external wiring in `docs/test_matrix.md`.
