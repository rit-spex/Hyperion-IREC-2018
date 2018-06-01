from socketIO_client import SocketIO
import serial
import json
from data_type_util import header_handler, payload_builder
import pprint as pp
from threading import Thread
import queue

current_milli_time = lambda: int(round(time.time() * 1000))

class HABnetSocket(Thread):
    def __init__(self, hostname, port, name):
        Thread.__init__(self)
        self.hostname = hostname
        self.port = port
        self.name = name
        self.socket = None
        self.running = True
        self.to_habnet = queue.Queue()
    
    def send(self,data):
        self.to_habnet.put(data)

    def clean_stop(self):
        self.running = False

    def run(self):
        self.socket = SocketIO(self.hostname, self.port)
        self.set_functions()
        self.socket.wait(seconds=10)
        print("socket waiting")

    def set_functions(self):
        self.socket.on('connect',self.on_connect)
        self.socket.on('reconnect',self.on_reconnect)
        self.socket.on('disconnect',self.on_disconnect)
        self.socket.on('joinedSuccessfully',self.on_joinedSuccessfully)

    def on_connect(self):
        print('[Connected]')
        payload = json.dumps({ 'name': self.name, 'type': 'dataSource' })
        self.socket.emit('join', payload)

    def on_reconnect(self):
        print('[Reconnected]')

    def on_disconnect(self):
        print('[Disconnected]')

    def on_joinedSuccessfully(self, *args):
        print('[joinedSuccessfully]')
        pp.pprint(args[0])
        self.name = args[0]['name']
        self.output_data()

    def send_data(self, packet):
        json_packet = json.dumps(packet)
        self.socket.emit('sensorData', json_packet)

    def output_data(self):
        print("socket output start")
        while self.running or not self.to_habnet.empty():
            data = self.to_habnet.get()

            header_lst = data[0:6]
            data_type, flags, time = header_handler(header_lst)
            values = data[6:]

            payload = payload_builder(data_type, time, values)

            packet = {
                'dateCreated': time, #expecting millisconds
                'payload': payload,
                'name': self.name,
            }

            # pp.pprint(packet)
            self.send_data(packet)
        print("socket output end")