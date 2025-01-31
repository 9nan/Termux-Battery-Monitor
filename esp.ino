/**
 * WiFi Battery Status Monitor
 *
 * This sketch connects to a WiFi network, fetches battery status from a remote API,
 * and controls an LED and relay based on the battery level.
 *
 * Copyright (c) 2023 itznan. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// List of WiFi network SSIDs
const char* ssidList[] = {"ZTE_2.4G_G9Dcxd", "ナン0による自動充電", "ナン1で自動充電"};

// List of WiFi network passwords
const char* passwordList[] = {"DK9rqReX", "11223344", "11223344"};

// URL of the remote API for fetching battery status
const char* api_url = "http://localhost:21132/battery";

// GPIO pin connected to the LED
#define LED_PIN 2

// GPIO pin connected to the relay
#define RELAY_PIN 5

/**
 * Connects to a WiFi network from the ssidList. If connection fails,
 * attempts to connect to the next network. If all connections fail,
 * restarts the ESP.
 */
void connectToWiFi() {
    for (int i = 0; i < 3; i++) {
        WiFi.begin(ssidList[i], passwordList[i]);
        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 20) {
            delay(1000);
            attempts++;
        }
        if (WiFi.status() == WL_CONNECTED) {
            Serial.print("Connected to: ");
            Serial.println(ssidList[i]);
            if (checkInternet()) {
                return;
            }
        }
    }
    Serial.println("No internet on any network, restarting...");
    ESP.restart();
}

/**
 * Checks if the internet connection is working by making a GET request
 * to a known URL (http://clients3.google.com/generate_204).
 *
 * @return True if the internet connection is working, false otherwise.
 */
bool checkInternet() {
    HTTPClient http;
    http.begin("http://clients3.google.com/generate_204");
    int httpResponseCode = http.GET();
    http.end();
    return (httpResponseCode == 204);
}

/**
 * Ensures that the internet connection is working. If not, disconnects
 * from the current WiFi network and reconnects using connectToWiFi().
 */
void ensureInternet() {
    if (WiFi.status() != WL_CONNECTED || !checkInternet()) {
        Serial.println("No internet, switching WiFi...");
        WiFi.disconnect();
        connectToWiFi();
    }
}

/**
 * Fetches the battery status from the remote API using ensureInternet()
 * to ensure a working internet connection. If the battery level is below
 * 99%, turns on the LED and relay.
 */
void fetchBatteryStatus() {
    ensureInternet();
    if (WiFi.status() != WL_CONNECTED) return;
    
    HTTPClient http;
    http.begin(api_url);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode == 200) {
        String response = http.getString();
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, response);
        
        if (!error) {
            int battery = doc["percentage"];
            if (battery < 99) {
                digitalWrite(LED_PIN, HIGH);
                digitalWrite(RELAY_PIN, HIGH);
            } else {
                digitalWrite(LED_PIN, LOW);
                digitalWrite(RELAY_PIN, LOW);
            }
        }
    }
    http.end();
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(RELAY_PIN, LOW);
    
    connectToWiFi();
}

void loop() {
    fetchBatteryStatus();
    delay(5000);
}