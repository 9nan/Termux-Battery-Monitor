#!/data/data/com.termux/files/usr/bin/bash
while true; do
    battery=$(termux-battery-status | jq .percentage)
    curl -X POST -H "Content-Type: application/json" -d "{\"percentage\":$battery}" "http://localhost:21132/update-battery"
    sleep 10  # Sends data every 5 minutes
done

