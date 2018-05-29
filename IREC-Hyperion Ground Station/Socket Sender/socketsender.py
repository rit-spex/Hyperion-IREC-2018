import serial
import pprint as pp
import serial.tools.list_ports
import argparse
from habnet_socket import HABnetSocket


def display_ports():
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        print(p)

def arg_handler():
    arg_parser = argparse.ArgumentParser(description='Hyperion Ground Station Software - relays data to a HABnet instance')
    arg_parser.add_argument('-H', '--host', dest='hostname', help='HABnet instance hostname', default='127.0.0.1', nargs='?', type=str)
    arg_parser.add_argument('-P', '--port', dest='port', help='HABnet instance port', default=3000, nargs='?', type=int)
    arg_parser.add_argument('-S', '--serial', dest='serial_in', help='Serial input port', default='COM3', nargs='?', type=str)
    arg_parser.add_argument('-N', '--name', dest='name', help='HABnet socket name', default='testSocket', nargs='?', type=str)
    return arg_parser.parse_args()

def main():
    args = arg_handler()
    print(args)
    socket = HABnetSocket(args)
    socket.wait()

if __name__ == '__main__':
    main()
