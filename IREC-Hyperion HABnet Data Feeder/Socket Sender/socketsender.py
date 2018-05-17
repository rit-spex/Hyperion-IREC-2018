from socketIO_client import SocketIO
import serial
import json
import pprint as pp
import time
import serial.tools.list_ports
import argparse


arg_parser = argparse.ArgumentParser(description='Hyperion Ground Station Software - relays data to a HABnet instance')
arg_parser.add_argument('hostname', help='HABnet instance hostname', default='127.0.0.1', type=str)
arg_parser.add_argument('port', help='HABnet instance port', default=3000, type=int)
cl_args = arg_parser.parse_args()

current_milli_time = lambda: int(round(time.time() * 1000))

name = 'testSocket'
payload = json.dumps({ 'name': name, 'type': 'dataSource' })
joined = False

def on_connect():
    print('[Connected]')
    socket.emit('join', payload)

def on_reconnect():
    print('[Reconnected]')

def on_disconnect():
    print('[Disconnected]')
    joined = False

def on_joinedSuccessfully(*args):
    print('[joinedSuccessfully]')
    pp.pprint(args[0])
    name = args[0]['name']
    joined = True

# def begin_sending():
#     time.sleep(15)
#     while(True):
#         sendData()
#         time.sleep(2)

def send_data(packet):
    json_packet = json.dumps(packet)
    socket.emit('sensorData', json_packet)

def serial_reading():
    ser = serpal.Serial('COM3', 9600, timeout=1.0)
    while(True):
        line = ser.readline()
        data = line.strip().split(',')
        data_type = data[0]
        flags = data[1:5]
        time = data[5]
        data_values = data[6:]
        if joined:
            packet = {
                'dateCreated': time, #expecting millisconds
                'payload': {
                    #data stuff here
                    'timeStamp': time
                },
                'name': name,
            }

            send_data(packet)

ports = list(serial.tools.list_ports.comports())
for p in ports:
    print(p)

socket = SocketIO(cl_args.hostname, cl_args.port)
socket.on('connect',on_connect)
socket.on('reconnect',on_reconnect)
socket.on('disconnect',on_disconnect)
socket.on('joinedSuccessfully',on_joinedSuccessfully)

socket.wait()
