from flask import Flask
from flask_jsonpify import jsonify
import serial

comm = serial.Serial("/dev/cu.usbmodem14101", timeout=1)

app = Flask(__name__)

@app.route("/")
def arduino():
    result = {}
    for i in range(3):
        r = comm.readline()
        l = r.decode("ascii").rstrip().split("-")
        result[l[0]] = int(l[1])
    
    return jsonify(result)
