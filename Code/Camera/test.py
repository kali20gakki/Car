# Untitled - By: zhangwei - 周一 3月 4 2019

import sensor, image, time

colorList = []

flagR = 1
flagG = 1
flagB = 1

# 红绿蓝 阈值 要根据实际颜色调试
red_threshold = (93, 24, 98, 29, -54, 90)
green_threshold = (37, 82, -17, -46, -11, 15)
blue_threshold = (5, 67, 76, -101, -27, -128)

# RGB 阈值的组合  对应code 1  2  4
thresholds = [(93, 24, 98, 29, -54, 90),
              (37, 82, -17, -46, -11, 15),
             (5, 67, 76, -101, -27, -128)]


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(True) # 自动增益打开
sensor.set_auto_whitebal(True) # 白平衡关闭

clock = time.clock()



# 二维码识别
def qrcode_recognition():
    for code in img.find_qrcodes():
        img.draw_rectangle(code.rect(), color = (255, 0, 0)) # 脱机时注释提高性能
        if(code[4]):
            uart.write(code[4]+"\r\n")
            print(code[4]+"\r\n")


while(True):
    clock.tick()
    img = sensor.snapshot()
    qrcode_recognition()
