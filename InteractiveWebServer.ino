/*
Web server for remote relay control with LilyGo T-Relay board

Copyright 2022 James Tittsler

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Original example: Copyright 2022 Xinyuan-LilyGO
*/
#if defined(RELAY8)
    #define RELAY_PIN_1 33
    #define RELAY_PIN_2 32
    #define RELAY_PIN_3 13
    #define RELAY_PIN_4 12
    #define RELAY_PIN_5 21
    #define RELAY_PIN_6 19
    #define RELAY_PIN_7 18
    #define RELAY_PIN_8 5
#else
    #define RELAY_PIN_1 21
    #define RELAY_PIN_2 19
    #define RELAY_PIN_3 18
    #define RELAY_PIN_4 5
#endif

#define LED_PIN     25


#define BLINK_FAST          0
#define BLINK_HEARTBEAT     1
#define BLINK_PATTERN_LEN   4

#define WIFI_CHECK_INTERVAL 3000

#include <Arduino.h>
/* ESP32 Dependencies */
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>
/* Your WiFi Credentials */
#include "WiFi-credentials.h"

unsigned long now = 0;
unsigned long timestamp = 0;
unsigned long wifiTime = 0;

int blinkPattern = BLINK_FAST;
int blinkCycle = 0;
int wifiStatus = WL_IDLE_STATUS;
unsigned long blinks[2][4] = {
    {101, 100, 101, 100},   /* odd -> LED on phase */
    { 75, 150,  75, 700}
};

#if defined(RELAY8)
    #define NUMBER_OF_RELAYS 8

    unsigned long ontime[8] = {     /* momentary relay on time in millis*/
        0, 0, 0, 400, 0, 0, 0, 800
    };

    unsigned long offtime[8] = {    /* when to turn off momentary switch*/
        0, 0, 0, 0, 0, 0, 0, 0
    };
#else
    #define NUMBER_OF_RELAYS 4
    unsigned long ontime[4] = {     /* momentary relay on time in millis*/
        0, 0, 0, 400
    };

    unsigned long offtime[4] = {    /* when to turn off momentary switch*/
        0, 0, 0, 0
    };
#endif


/* Start Webserver */
AsyncWebServer server(80);

/* Attach ESP-DASH to AsyncWebServer */
ESPDash dashboard(&server);

/*
  Button Card
  Format - (Dashboard Instance, Card Type, Card Name)
*/
Card Relay_1(&dashboard, BUTTON_CARD, "Relay_1");
Card Relay_2(&dashboard, BUTTON_CARD, "Relay_2");
Card Relay_3(&dashboard, BUTTON_CARD, "Relay_3");
Card Relay_4(&dashboard, BUTTON_CARD, "Relay_4_momentary");

#if defined(RELAY8)
Card Relay_5(&dashboard, BUTTON_CARD, "Relay_5");
Card Relay_6(&dashboard, BUTTON_CARD, "Relay_6");
Card Relay_7(&dashboard, BUTTON_CARD, "Relay_7");
Card Relay_8(&dashboard, BUTTON_CARD, "Relay_8_momentary");
#endif

void setup()
{
    Serial.begin(115200);

    // Onboard LED
    pinMode(RELAY_PIN_1, OUTPUT);
    pinMode(RELAY_PIN_2, OUTPUT);
    pinMode(RELAY_PIN_3, OUTPUT);
    pinMode(RELAY_PIN_4, OUTPUT);
    
#if defined(RELAY8)
    pinMode(RELAY_PIN_5, OUTPUT);
    pinMode(RELAY_PIN_6, OUTPUT);
    pinMode(RELAY_PIN_7, OUTPUT);
    pinMode(RELAY_PIN_8, OUTPUT);
#endif
    pinMode(LED_PIN, OUTPUT);
    delay(100);

    digitalWrite(RELAY_PIN_1, LOW);
    digitalWrite(RELAY_PIN_2, LOW);
    digitalWrite(RELAY_PIN_3, LOW);
    digitalWrite(RELAY_PIN_4, LOW);
    
#if defined(RELAY8)
    digitalWrite(RELAY_PIN_5, LOW);
    digitalWrite(RELAY_PIN_6, LOW);
    digitalWrite(RELAY_PIN_7, LOW);
    digitalWrite(RELAY_PIN_8, LOW);
    digitalWrite(LED_PIN, LOW);
#endif

    /* Connect WiFi */
    Serial.print("Connecting to: ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    timestamp = millis();
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - timestamp > blinks[blinkPattern][blinkCycle]) {
            digitalWrite(LED_PIN, ((blinks[blinkPattern][blinkCycle] & 1) == 1));
            timestamp = millis();
            blinkCycle = (blinkCycle + 1) % BLINK_PATTERN_LEN;
        }
    }
    digitalWrite(LED_PIN, HIGH);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());

    /* Attach Relay_1 Callback */
    Relay_1.attachCallback([&](bool value) {
        /* Print our new button value received from dashboard */
        Serial.println("Relay_1 Triggered: " + String((value) ? "true" : "false"));
        digitalWrite(RELAY_PIN_1, value);
        /* Make sure we update our button's value and send update to dashboard */
        Relay_1.update(value);
        dashboard.sendUpdates();
        /* if momentary switch turning on, start timer */
        if (value && (ontime[0] > 0)) {
            offtime[0] = millis() + ontime[0];
        }
    });

    /* Attach Relay_2 Callback */
    Relay_2.attachCallback([&](bool value) {
        /* Print our new button value received from dashboard */
        Serial.println("Relay_2 Triggered: " + String((value) ? "true" : "false"));
        digitalWrite(RELAY_PIN_2, value);
        /* Make sure we update our button's value and send update to dashboard */
        Relay_2.update(value);
        dashboard.sendUpdates();
        /* if momentary switch turning on, start timer */
        if (value && (ontime[1] > 0)) {
            offtime[1] = millis() + ontime[1];
        }
    });

    /* Attach Button Callback */
    Relay_3.attachCallback([&](bool value) {
        /* Print our new button value received from dashboard */
        Serial.println("Relay_3 Triggered: " + String((value) ? "true" : "false"));
        digitalWrite(RELAY_PIN_3, value);
        /* Make sure we update our button's value and send update to dashboard */
        Relay_3.update(value);
        dashboard.sendUpdates();
        /* if momentary switch turning on, start timer */
        if (value && (ontime[2] > 0)) {
            offtime[2] = millis() + ontime[2];
        }
    });

    /* Attach Button Callback */
    Relay_4.attachCallback([&](bool value) {
        /* Print our new button value received from dashboard */
        Serial.println("Relay_4 Triggered: " + String((value) ? "true" : "false"));
        digitalWrite(RELAY_PIN_4, value);
        /* Make sure we update our button's value and send update to dashboard */
        Relay_4.update(value);
        dashboard.sendUpdates();
        /* if momentary switch turning on, start timer */
        if (value && (ontime[3] > 0)) {
            offtime[3] = millis() + ontime[3];
        }
    });
    
#if defined(RELAY8)
    /* Attach Relay_5 Callback */
    Relay_5.attachCallback([&](bool value) {
        /* Print our new button value received from dashboard */
        Serial.println("Relay_5 Triggered: " + String((value) ? "true" : "false"));
        digitalWrite(RELAY_PIN_5, value);
        /* Make sure we update our button's value and send update to dashboard */
        Relay_5.update(value);
        dashboard.sendUpdates();
        /* if momentary switch turning on, start timer */
        if (value && (ontime[4] > 0)) {
            offtime[4] = millis() + ontime[4];
        }
    });

    /* Attach Relay_6 Callback */
    Relay_6.attachCallback([&](bool value) {
        /* Print our new button value received from dashboard */
        Serial.println("Relay_6 Triggered: " + String((value) ? "true" : "false"));
        digitalWrite(RELAY_PIN_6, value);
        /* Make sure we update our button's value and send update to dashboard */
        Relay_6.update(value);
        dashboard.sendUpdates();
        /* if momentary switch turning on, start timer */
        if (value && (ontime[5] > 0)) {
            offtime[5] = millis() + ontime[5];
        }
    });

    /* Attach Button Callback */
    Relay_7.attachCallback([&](bool value) {
        /* Print our new button value received from dashboard */
        Serial.println("Relay_7 Triggered: " + String((value) ? "true" : "false"));
        digitalWrite(RELAY_PIN_7, value);
        /* Make sure we update our button's value and send update to dashboard */
        Relay_7.update(value);
        dashboard.sendUpdates();
        /* if momentary switch turning on, start timer */
        if (value && (ontime[6] > 0)) {
            offtime[6] = millis() + ontime[6];
        }
    });

    /* Attach Button Callback */
    Relay_8.attachCallback([&](bool value) {
        /* Print our new button value received from dashboard */
        Serial.println("Relay_8 Triggered: " + String((value) ? "true" : "false"));
        digitalWrite(RELAY_PIN_8, value);
        /* Make sure we update our button's value and send update to dashboard */
        Relay_8.update(value);
        dashboard.sendUpdates();
        /* if momentary switch turning on, start timer */
        if (value && (ontime[7] > 0)) {
            offtime[7] = millis() + ontime[7];
        }
    });

#endif

    /* Start AsyncWebServer */
    server.begin();
}

void loop()
{
    int i = 0;
    bool needUpdate = false;

    now = millis();
    wifiStatus = WiFi.status();

    /* turn off momentary switches */
    for (i=0; i<NUMBER_OF_RELAYS; i++) {
        if ((offtime[i] > 0) && (now > offtime[i])) {
            offtime[i] = 0;
            needUpdate = true;
            switch (i) {
                case 0:
                    digitalWrite(RELAY_PIN_1, false);
                    Relay_1.update(false);
                    break;
                case 1:
                    digitalWrite(RELAY_PIN_2, false);
                    Relay_2.update(false);
                    break;
                case 2:
                    digitalWrite(RELAY_PIN_3, false);
                    Relay_3.update(false);
                    break;
                case 3:
                    digitalWrite(RELAY_PIN_4, false);
                    Relay_4.update(false);
                    break;
#if defined(RELAY8)
                case 4:
                    digitalWrite(RELAY_PIN_5, false);
                    Relay_5.update(false);
                    break;
                case 5:
                    digitalWrite(RELAY_PIN_6, false);
                    Relay_6.update(false);
                    break;
                case 6:
                    digitalWrite(RELAY_PIN_7, false);
                    Relay_7.update(false);
                    break;
                case 7:
                    digitalWrite(RELAY_PIN_8, false);
                    Relay_8.update(false);
                    break;
#endif
            }
            if (needUpdate) {
                dashboard.sendUpdates();
            }
        }
    }

    if ((wifiStatus != WL_CONNECTED) && (now-wifiTime > WIFI_CHECK_INTERVAL)) {
        Serial.println("Reconnecting to WiFi...");
        WiFi.disconnect();
        WiFi.reconnect();
        wifiTime = now;
        timestamp = now;
        blinkPattern = BLINK_FAST;
        blinkCycle = 0;
    } else if ((wifiStatus == WL_CONNECTED) && (blinkPattern == BLINK_FAST)) {
        timestamp = now;
        blinkPattern = BLINK_HEARTBEAT;
        blinkCycle = 0;
    }

    /* show LED heartbeat */
    if (now - timestamp > blinks[blinkPattern][blinkCycle]) {
        digitalWrite(LED_PIN, ((blinks[blinkPattern][blinkCycle] & 1) == 1));
        timestamp = now;
        blinkCycle = (blinkCycle + 1) % BLINK_PATTERN_LEN;
    }
}
