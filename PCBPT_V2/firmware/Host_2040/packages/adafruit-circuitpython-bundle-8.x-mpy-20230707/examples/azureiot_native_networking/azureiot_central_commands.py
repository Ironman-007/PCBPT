# SPDX-FileCopyrightText: 2021 ladyada for Adafruit Industries
# SPDX-License-Identifier: MIT

import time

import rtc
import socketpool
import wifi

import adafruit_ntp
from adafruit_azureiot import IoTCentralDevice
from adafruit_azureiot.iot_mqtt import IoTResponse

# Get wifi details and more from a secrets.py file
try:
    from secrets import secrets
except ImportError:
    print("WiFi secrets are kept in secrets.py, please add them there!")
    raise

print("Connecting to WiFi...")
wifi.radio.connect(secrets["ssid"], secrets["password"])

print("Connected to WiFi!")

if time.localtime().tm_year < 2022:
    print("Setting System Time in UTC")
    pool = socketpool.SocketPool(wifi.radio)
    ntp = adafruit_ntp.NTP(pool, tz_offset=0)

    # NOTE: This changes the system time so make sure you aren't assuming that time
    # doesn't jump.
    rtc.RTC().datetime = ntp.datetime
else:
    print("Year seems good, skipping set time.")

# To use Azure IoT Central, you will need to create an IoT Central app.
# You can either create a free tier app that will live for 7 days without an Azure subscription,
# Or a standard tier app that will last for ever with an Azure subscription.
# The standard tiers are free for up to 2 devices
#
# If you don't have an Azure subscription:
#
# If you are a student, head to https://aka.ms/FreeStudentAzure and sign up, validating with your
#  student email address. This will give you $100 of Azure credit and free tiers of a load of
#  service, renewable each year you are a student
#
# If you are not a student, head to https://aka.ms/FreeAz and sign up to get $200 of credit for 30
#  days, as well as free tiers of a load of services
#
# Create an Azure IoT Central app by following these instructions:
# https://aka.ms/CreateIoTCentralApp
# Add a device template with telemetry, properties and commands, as well as a view to visualize the
# telemetry and execute commands, and a form to set properties.
#
# Next create a device using the device template, and select Connect to get the device connection
# details.
# Add the connection details to your secrets.py file, using the following values:
#
# 'id_scope' - the devices ID scope
# 'device_id' - the devices device id
# 'device_sas_key' - the devices primary key
#
# The adafruit-circuitpython-azureiot library depends on the following libraries:
#
# From the Adafruit CircuitPython Bundle https://github.com/adafruit/Adafruit_CircuitPython_Bundle:
# * adafruit-circuitpython-minimqtt
# * adafruit-circuitpython-requests


# Create an IoT Hub device client and connect
esp = None
pool = socketpool.SocketPool(wifi.radio)
device = IoTCentralDevice(
    pool, esp, secrets["id_scope"], secrets["device_id"], secrets["device_sas_key"]
)


# Subscribe to commands
# Commands can be sent from the devices Dashboard in IoT Central, assuming
# the device template and view has been set up with the commands
# Command handlers need to return a response to show if the command was handled
# successfully or not, returning an HTTP status code and message
def command_executed(command_name: str, payload) -> IoTResponse:
    print("Command", command_name, "executed with payload", str(payload))
    # return a status code and message to indicate if the command was handled correctly
    return IoTResponse(200, "OK")


# Subscribe to the command execute event
device.on_command_executed = command_executed


print("Connecting to Azure IoT Central...")
device.connect()

print("Connected to Azure IoT Central!")

message_counter = 60

while True:
    try:
        # Poll every second for messages from the cloud
        device.loop()
    except (ValueError, RuntimeError) as e:
        print("Connection error, reconnecting\n", str(e))
        # If we lose connectivity, reset the wifi and reconnect
        wifi.reset()
        wifi.connect()
        device.reconnect()
        continue

    time.sleep(1)
