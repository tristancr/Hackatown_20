# Arduino Server

## Installation

```
pip install Flask
pip install flask-jsonpify
pip install flask-serial
```

## Configuration

On ```macOS```, the serial will listen in ```/dev/cu.``` as follow
```
port = "/dev/cu.usbmodem14101"
```

On ```Windows```, the serial will listen in ```COM``` as follow
```
port = "COM6"
```

## Run the server
```
env FLASK_APP=arduino_server.py flask run
```