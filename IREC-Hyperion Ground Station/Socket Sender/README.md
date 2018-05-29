## Hyperion Socket Sender
### Reads incoming data from a serial device (see:[System Forwarder](../System%20Forwarder)) and sends it to a [HABnet](https://github.com/RIT-Space-Exploration/HABnet) instance


--- 

**Requirements:**

* socketIO-client  ([github](https://github.com/invisibleroads/socketIO-client))

        pip install socketIO-client

* pyserial ([github](https://github.com/pyserial/pyserial))

        pip install pyserial

---

**Optional Arguments:**

* `-h`, `--help`

  show help message and exit

* `-H [HOSTNAME]`, `--host [HOSTNAME]`

  HABnet instance hostname

  default: `127.0.0.1`

* `-P [PORT]`, `--port [PORT]`

  HABnet instance port

  default: `3000`

* `-S [SERIAL_IN]`, `--serial [SERIAL_IN]`

  Serial input port

  default: `COM3`

* `-N` [NAME], `--name [NAME]`

  HABnet socket name

  default: `testSocket`

---

**Examples:**

    python socketsender.py 
    python socketsender.py --host 127.0.0.1 --port 3000
