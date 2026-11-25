# TTGO heat

TTGO T-Display PID heat controller: MAX6675 thermocouple, SSR PWM, rotary encoder for start time / cook temp / cook time / hold temp, NTP schedule.

Copy `TTGO_Dynamic/secrets.h.example` to `TTGO_Dynamic/secrets.h` and fill in Wi-Fi. `secrets.h` is gitignored.

Settings persist in ESP32 Preferences (`values`).

## Pins

| GPIO | Function |
|---|---|
| 26 | MAX6675 DO |
| 25 | MAX6675 CS |
| 33 | MAX6675 CLK |
| 27 | SSR PWM |
| 13 | Encoder A (DS1) |
| 15 | Encoder B (DS2) |
| 12 | Encoder push (DS3) |
| 19/18/5/16/23/4 | T-Display SPI / backlight (see `TTGO_Display.h`) |

Use TFT_eSPI User Setup `Setup25_TTGO_T_Display.h`.

## PID

`HeatPID.ino`: Kp 15, Ki 0.2, Kd 10, 5 s window, output via `PWMOutESP32` on the SSR pin. Heater runs when wall-clock time is between StartTime and StartTime+CookTime.

## Libraries

TFT_eSPI, PID, MAX6675, NTPClient_Generic, PWMOutESP32, EncoderButton, Preferences, Button2.

## Flash

Board: ESP32 (TTGO T-Display). Sketch folder: `TTGO_Dynamic`.
