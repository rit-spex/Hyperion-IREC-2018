from socketIO_client import SocketIO
import serial
import json
from data_type_util import header_handler, payload_builder
import pprint as pp


current_milli_time = lambda: int(round(time.time() * 1000))

class HABnetSocket():
    def __init__(self, args):
        self.hostname = args.hostname
        self.port = args.port
        self.serial_in = args.serial_in
        self.name = args.name

        self.socket = SocketIO(self.hostname, self.port)

        self.set_functions()

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
        self.serial_reading()
    
    def wait(self):
        self.socket.wait()

    def send_data(self, packet):
        json_packet = json.dumps(packet)
        self.socket.emit('sensorData', json_packet)

    def serial_reading(self):
        ser = serial.Serial(self.serial_in, 9600, timeout=1.0)
        pp.pprint(ser)
        while(True):
            line = ser.readline()
            s = line.decode('UTF-8')
            data = line.decode('UTF-8').strip().split(',')

            header_lst = data[0:6]
            data_type, flags, time = header_handler(header_lst)
            values = data[6:]

            payload = payload_builder(data_type, time, values)

            packet = {
                'dateCreated': time, #expecting millisconds
                'payload': payload,
                'name': self.name,
            }

            pp.pprint(packet)
            self.send_data(packet)