# Remote Relays

Web interface for remotely controlling relays on a LilyGo T-Relay
board.  It is based on the [LilyGo T-Relay InteractiveWebServer
example](https://github.com/Xinyuan-LilyGO/LilyGo-T-Relay/tree/main/examples/T-Relay/InteractiveWebServer).

## Configuring

Copy `WiFi-credentials-sample.h` to `WiFi-Credentials.h`, and edit
that file to grant access to your LAN.

## Building with PlatformIO

1. Install [VSCODE](https://code.visualstudio.com/) and [Python](https://www.python.org/)
2. Search for the PlatformIO plug-in in the VSCODE extension and install it.
3. After the installation is complete and the reload is completed, there will be a small house icon in the lower left corner. Click to display the Platformio IDE home page
4. Go to file - > Open folder - > Select the **Remote-Relays** folder. Click the (√) symbol in the lower left corner to compile (→) for upload.

## LED Status

- **Fast Blinking** attempting to connect to WiFi specified in `WiFi-credentials.h`
- **Heartbeat** connected

## Web Interface

The websocket interface uses either ws: or wss: depending on the http:/https: protocol used for the web page, allowing installation behind a reverse proxy with SSL.

## License

This project is derived from a LilyGo T-Relay example which is MIT Licensed,
but it embeded a copy of the GPLv3-licensed [ESP-DASH](https://github.com/ayushsharma82/ESP-DASH), so I suspect the original licensing was an oversight and the entirety *must* be GPLv3 licensed.
