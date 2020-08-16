"""
@author：陈章韶
@organization：华南理工大学

与MCU进行串口通信，发送测试集到MCU上运行，接收MCU返回的预测结果并保存到文件
"""
import serial
import time
import numpy as np

ser = serial.Serial(port='COM11', baudrate=115200)
print(ser)
testX = np.load('testData.npy')
testX = testX.reshape(testX.shape[0], 200, 6)
ofp = open(file='res.csv', mode='w')
for i in range(0, testX.shape[0]):
    while ser.in_waiting < 5:
        pass
    time.sleep(0.2)
    recv = ser.read(size=ser.in_waiting).decode(encoding='utf8')
    ser.reset_input_buffer()
    if recv.strip() == 'begin':
        for j in range(0, testX.shape[1]):
            for k in range(0, testX.shape[2]):
                send_str = str(testX[i][j][k])+' '
                ser.write(send_str.encode(encoding='utf8'))
                time.sleep(0.01)
        while ser.in_waiting < 2:
            pass
        time.sleep(0.01)
        recv = ser.read(size=ser.in_waiting).decode(encoding='utf8')
        ser.reset_input_buffer()
        if recv.strip() == 'ok':
            time.sleep(0.02)
            send_str = '200 '
            ser.write(send_str.encode(encoding='utf8'))
            time.sleep(0.01)
        while ser.in_waiting < 108:
            time.sleep(0.01)
            pass
        recv = ser.read(size=108).decode(encoding='utf8')
        ofp.write(recv+'\r\n')
    print("%d / %d" % (i, testX.shape[0]))
ofp.close()
