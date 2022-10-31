import paho.mqtt.client as mqtt
import time
import RPi.GPIO as gpio

def on_message(client, userdata, message):
    a= str(message.payload.decode("utf-8"))
    print(a)
    seconds = 0.02
    time.sleep(seconds)
    if (a == "F"):
        print("forward")
        forward(seconds)
        gpio.cleanup()
    if (a == "B"):
        print("reverse")
        reverse(seconds)
        gpio.cleanup()
    if (a == "R"):
        print("right")
        right_turn(seconds)
        gpio.cleanup()
    if (a == "L"):
        print("left")
        left_turn(seconds)
        gpio.cleanup()

    #print("received message: " ,str(message.payload.decode("utf-8"))

def init():
    gpio.setmode(gpio.BCM)
    gpio.setup(17, gpio.OUT)
    gpio.setup(22, gpio.OUT)
    gpio.setup(23, gpio.OUT)
    gpio.setup(24, gpio.OUT)

def forward(sec):
    init()
    gpio.output(17, False)
    gpio.output(22, True)
    gpio.output(23, True)
    gpio.output(24, False)
    time.sleep(sec)
    gpio.cleanup()
    
def reverse(sec):
    init()
    gpio.output(17, True)
    gpio.output(22, False)
    gpio.output(23, False)
    gpio.output(24, True)
    time.sleep(sec)
    gpio.cleanup()
    
def left_turn(sec):
    init()
    gpio.output(17, True)
    gpio.output(22, False)
    gpio.output(23, True)
    gpio.output(24, False)
    time.sleep(sec)
    gpio.cleanup()
    
def right_turn(sec):
    init()
    gpio.output(17, False)
    gpio.output(22, True)
    gpio.output(23, False)
    gpio.output(24, True)
    time.sleep(sec)
    gpio.cleanup()
mqttBroker ="192.168.1.33"

client = mqtt.Client("Remote controll robot")
client.connect(mqttBroker) 

client.loop_start()

client.subscribe("outTopic")
client.on_message=on_message 

time.sleep(3000)
client.loop_stop()