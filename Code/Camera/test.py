# Untitled - By: zhangwei - 周一 3月 4 2019

import sensor, image, time

colorList = []

flagR = 1
flagG = 1
flagB = 1

# 红绿蓝 阈值 要根据实际颜色调试
red_threshold = (93, 24, 98, 29, -54, 90)
green_threshold = (34, 13, -14, -124, -106, 34)
blue_threshold = (48, 0, -68, 87, -128, -18)

# RGB 阈值的组合  对应code 1  2  4
thresholds = [(93, 24, 98, 29, -54, 90),
              (34, 13, -14, -124, -106, 34),
             (48, 0, -68, 87, -128, -18)]


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(True) # 自动增益打开
sensor.set_auto_whitebal(False) # 白平衡关闭

clock = time.clock()





while(True):
    clock.tick()
    img = sensor.snapshot()
    for blob in img.find_blobs(thresholds, pixels_threshold=2000,merge=False):
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy())
        print("Pixel = ")
        print(blob.pixels())
        print("\r\n")
        print("Area = ")
        print(blob.area())
        print("\r\n")
