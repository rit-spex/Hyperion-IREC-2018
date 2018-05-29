## relays data to a HABnet instance
--- 

**Requirements:**

* socketIO-client  ([github](https://github.com/invisibleroads/socketIO-client))

        pip install socketIO-client

* pyserial ([github](https://github.com/pyserial/pyserial))

        pip install pyserial


---

**Usage:**

positional arguments:

  1. hostname 

        HABnet instance hostname

  2. port
  
        HABnet instance port

optional arguments:
* -h, --help  show this help message and exit

---

**Example:**

    python socketsender.py 127.0.0.1 3000