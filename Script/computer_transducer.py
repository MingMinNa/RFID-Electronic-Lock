import serial

pic = serial.Serial('COM3', 9600, timeout=1)


command = ""
print('----------------------')
while True:
    if pic.in_waiting > 0:
        data = pic.read().decode('ascii')
        print(f"{data}", end = "", flush = True)

        if data != '\n':    command += data
        else:
            if command[:5] == 'Hello':
                for ele in "unlock\n".encode('ascii'):
                    pic.write(ele)
            command = ""
            