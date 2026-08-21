import serial

PORT = 'COM3'
BAUDRATE = 9600

pic = serial.Serial(PORT, BAUDRATE, timeout=1, write_timeout=0)

try:
    while True:
        # readline() reads until '\n' automatically, more robust than reading byte by byte
        raw = pic.readline()
        if not raw:
            continue

        # errors='ignore' prevents a UnicodeDecodeError
        command = raw.decode('ascii', errors='ignore').strip()
        if not command:
            continue

        print(command, flush=True)

        if command.startswith('Hello'):
            pic.write('unlock'.encode('ascii'))
            pic.flush()
            print('-> Sent unlock command to chip (2)', flush=True)

except KeyboardInterrupt:
    print('\nInterrupted by user')

finally:
    pic.close()