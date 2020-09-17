from flask import Flask
from flask_cors import CORS
from flask_jsonpify import jsonify
import serial
from threading import Thread

port = "/dev/cu.usbmodem14101" # 14101
comm = serial.Serial(port, timeout=1)

app = Flask(__name__)
CORS(app)

@app.route("/")
def arduino():
    count = 0
    comm.write("A".encode("utf-8"))

    result = {}
    
    while (count < 3):
        r = comm.readline().decode("ascii").rstrip()

        if (not(r == "" or r == "A")):
            result[count] = r
            count += 1
    
    return jsonify(result)
