# Untitled - By: zhangwei - 周一 3月 4 2019

import sensor, image, time, pyb
from pyb import UART

colorList = []

flagR = 1
flagG = 1
flagB = 1
flagEN = 1

counter = 0

# 晚上 日光灯条件 工训场地阈值
# (93, 24, 98, 29, -54, 90)    RED
# (1, 26, -9, -82, -6, 49)     GREEN
# (5, 67, 76, -101, -27, -128) BLUE

# (53, 89, -21, -44, -9, 59)   GREEN 纸片

# 红绿蓝 阈值 要根据实际颜色调试
red_threshold = (93, 24, 98, 29, -54, 90)
# green_threshold = (1, 26, -9, -82, -6, 49)
green_threshold = (53, 89, -21, -44, -9, 59) # 纸片
blue_threshold = (5, 67, 76, -101, -27, -128)

# RGB 阈值的组合  对应code 1  2  4
thresholds = [(93, 24, 98, 29, -54, 90),
              (53, 89, -21, -44, -9, 59),
             (5, 67, 76, -101, -27, -128)]


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(True) # 自动增益打开
sensor.set_auto_whitebal(False) # 白平衡关闭

led = pyb.LED(3) #蓝灯
uart = UART(3, 9600,timeout = 1000)
clock = time.clock()


# 灯闪 用来观察程序是否运行正常
def led_flash():
     led.on() # 灯亮
     time.sleep(100)
     led.off()


while(True):
    clock.tick()
    img = sensor.snapshot()
    img.lens_corr(1.0)
    if(flagEN):
        for blob in img.find_blobs(thresholds, pixels_threshold=5000,merge=False):
            img.draw_rectangle(blob.rect())
            img.draw_cross(blob.cx(), blob.cy())
            if(blob.code() == 1):
                if(flagR):
                    colorList.append('R')
                    flagR = 0
            elif(blob.code() == 2):
                if(flagG):
                    colorList.append('G')
                    flagG = 0
            elif(blob.code() == 4):
                if(flagB):
                    colorList.append('B')
                    flagB = 0

        if(len(colorList) == 3):
            colorStr = "".join(colorList) #转换为字符串
            print(colorStr)
            uart.write(colorStr+"\r\n")
            time.sleep(200)
            counter = counter+1
            if(counter > 3):flagEN = 0

    else:
        for code in img.find_qrcodes():
            img.draw_rectangle(code.rect(), color = (255, 0, 0)) # 脱机时注释提高性能
            if(code[4]):
                uart.write(code[4]+"\r\n")
                led_flash()
                print(code[4]+"\r\n")


