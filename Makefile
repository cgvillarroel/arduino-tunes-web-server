port := COM4

build: $(wildcard *)
	arduino-cli compile

flash: build
	arduino-cli upload

f: flash

monitor: flash
	arduino-cli monitor -p $(port)

m: monitor
