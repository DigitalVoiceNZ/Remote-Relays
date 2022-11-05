#!/bin/bash

set -euo pipefail

pio lib -g install "ESP Async WebServer@1.2.3" "ArduinoJson@6.17.0"

pio ci --lib=. --board esp32dev "examples/Basic"
pio ci --lib=. --board esp32dev "examples/Chart"
pio ci --lib=. --board esp32dev "examples/Dynamic"
pio ci --lib=. --board esp32dev "examples/AccessPoint"
pio ci --lib=. --board esp32dev "examples/Interactive"
pio ci --lib=. --board esp12e "examples/Basic"
pio ci --lib=. --board esp12e "examples/Chart"
pio ci --lib=. --board esp12e "examples/Dynamic"
pio ci --lib=. --board esp12e "examples/AccessPoint"
pio ci --lib=. --board esp12e "examples/Interactive"
