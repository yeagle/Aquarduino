PORT = /dev/ttyACM0

all: hex

hex:
	cd ..
	arduino-cli -v compile -v --fqbn arduino:megaavr:nona4809 rtc-setup.ino

do_upload:
	cd ..
	arduino-cli -v upload -p $(PORT) -v --fqbn arduino:megaavr:nona4809 rtc-setup.ino
