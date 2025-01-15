import serial

pic = serial.Serial('COM3', 9600, timeout = 1, write_timeout = 0)


command = ""
print('----------------------')
while True:
    if pic.in_waiting > 0:
        data = pic.read().decode('ascii')
        print(f"{data}", end = "", flush = True)
        
        if data != '\n':    command += data
        else:
            if command[:5] == 'Hello':
                pic.write('unlock'.encode('ascii') + b'\n')
                pic.flush()
            command = ""
            