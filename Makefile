all: hex

hex:
	cd ..
	arduino-cli -v compile -v --fqbn arduino:megaavr:nona4809 AquArduino.ino

do_upload:
	cd ..
	arduino-cli -v upload -p /dev/ttyACM0 -v --fqbn arduino:megaavr:nona4809 AquArduino.ino
