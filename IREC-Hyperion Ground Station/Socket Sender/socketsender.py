import serial
import pprint as pp
import serial.tools.list_ports
import argparse
from habnet_socket import HABnetSocket
import threading
import queue
import data_type_util
import os

running = True
to_file = queue.Queue()
commands = queue.Queue()

def display_ports():
    ports = list(serial.tools.list_ports.comports())
    if ports:
        print('Connected Devices:')
        for p in ports:
            print('\t'+str(p))
    else:
        print("No connected devices detected.")

def arg_handler():
    arg_parser = argparse.ArgumentParser(description='Hyperion Ground Station Software - relays data to a HABnet instance')
    arg_parser.add_argument('-H', '--host', dest='hostname', help='HABnet instance hostname', default='127.0.0.1', nargs='?', type=str)
    arg_parser.add_argument('-P', '--port', dest='port', help='HABnet instance port', default=3000, nargs='?', type=int)
    arg_parser.add_argument('-S', '--serial', dest='serial_in', help='Serial input port', default='COM3', nargs='?', type=str)
    arg_parser.add_argument('-N', '--name', dest='name', help='HABnet socket name', default='testSocket', nargs='?', type=str)
    arg_parser.add_argument('-D', '--details', dest='details', help='Display various details and close', action='store_true')
    return arg_parser.parse_args()

def main():
    args = arg_handler()
    print(args)
    if args.details:
        display_ports()
        return
    
    socket_thread = HABnetSocket(args.hostname, args.port, args.name)
    serial_thread = threading.Thread(target=read_serial, args=(args.serial_in,socket_thread))
    write_thread = threading.Thread(target=write_files)
    input_thread = threading.Thread(target=take_input, args=(socket_thread,))
    
    socket_thread.start()
    serial_thread.start()
    write_thread.start()
    input_thread.start()

    socket_thread.join()
    serial_thread.join()
    write_thread.join()
    input_thread.join()


def read_serial(serial_in, socket):
    with serial.Serial(serial_in, 9600, timeout=1.0) as ser:
        print("ser:")
        pp.pprint(ser)
        ser.reset_input_buffer()

        while running or not commands.empty():

            # check if we need to send a command
            if not commands.empty():
                command = commands.get()
                print("COMMAND",command)
                ser.write(command.encode('utf-8'))
                print("COMMAND",command)

            # read in
            line = ser.readline().decode('UTF-8').strip()
            data = line.split(',')
            if len(data) == 1:
                continue
            data_type = int(data[0])
            to_file.put({'data_type':data_type, 'line':line})
            if data_type not in {0,7,8}:
                socket.send(data)
            if data_type in {8}:
                print(data)
            # print(data)
        print("serial thread done")

def write_files():
    fn_all = get_filename(-1)
    keys = data_type_util.files.keys()
    buffers = { key: [] for key in keys }
    
    while running or not to_file.empty():
        item = to_file.get()
        data_type = item['data_type']
        line = item['line']
        all_buffer = buffers[-1]
        single_buffer = buffers[data_type]

        all_buffer.append(line)
        single_buffer.append(line)

        if len(all_buffer) == 50:
            write_buffer(fn_all, all_buffer)
            del all_buffer[:]

        if len(buffers[data_type]) == 50:
            fn_single = get_filename(data_type)
            write_buffer(fn_single, single_buffer)
            del single_buffer[:]

    for key,buffer in buffers.items():
        fn = get_filename(key)
        write_buffer(fn, buffer)
        del buffer[:]
    print("file thread done")

def write_buffer(fn, buffer):
    if buffer:
        os.makedirs(os.path.dirname(fn), exist_ok=True)
        with open(fn, 'a') as fp:
            for line in buffer:
                fp.write(line)
                fp.write('\n')
                fp.flush()
                os.fsync(fp.fileno())
    
def get_filename(data_type):
    folder = 'data'
    suffix = '_data.csv'
    return os.path.join(folder, data_type_util.files[data_type] + suffix)

def take_input(socket):
    command_arm = 'ARM'
    command_disarm = 'DISARM'
    command_stop = 'STOP'
    global running
    while running:
        print('Commands:')
        print('\t[ARM] : Sends the ARM command')
        print('\t[DISARM] : Sends the DISARM command')
        print('\t[STOP] : Cleanly stops the program')
        line = input().upper()
        print()
        if line == command_arm:
            print('ARM command entered')
            commands.put(command_arm)
        elif line == command_disarm:
            print('DISARM command entered')
            commands.put(command_disarm)
        elif line == command_stop:
            print('STOP command entered')
            # global running
            running = False
            socket.clean_stop()
        else:
            print('invalid command')
        # return
        
    print("input thread done")

if __name__ == '__main__':
    main()
