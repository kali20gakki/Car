import sensor, image, time, pyb
from pyb import UART

# 红绿蓝 阈值 要根据实际颜色调试
red_threshold = (93, 24, 98, 29, -54, 90)
green_threshold = (18, 72, -21, -65, 47, 1)
blue_threshold = (5, 67, 76, -101, -27, -128)

# RGB 阈值的组合
thresholds = [(93, 24, 98, 29, -54, 90),
              (18, 72, -21, -65, 47, 1),
             (5, 67, 76, -101, -27, -128)]


# R G B x轴横坐标 用于排序
color_dist = {'R':0,
              'G':0,
              'B':0}


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000) # 跳过一些帧
sensor.set_auto_gain(True) # 自动增益打开
sensor.set_auto_whitebal(False) # 白平衡关闭

led = pyb.LED(3) #蓝灯
uart = UART(3, 9600,timeout = 1000)
clock = time.clock()


# 二维码识别
def qrcode_recognition():
    for code in img.find_qrcodes():
        img.draw_rectangle(code.rect(), color = (255, 0, 0)) # 脱机时注释提高性能
        if(code[4]):
            uart.write(code[4]+"\r\n")
            print(code[4]+"\r\n")


#寻找最大的色块 可以避免干扰
def find_max(blobs):
    max_size=0
    for blob in blobs:
        if blob.pixels() > max_size:
            max_blob=blob
            max_size = blob.pixels()
    return max_blob


# 寻找 红色 物料
def find_redBlob():
     img = sensor.snapshot()
     img.lens_corr(strength = 1.8, zoom = 1.0) # 畸变校正
     blobs = img.find_blobs([red_threshold],pixels_threshold=400, area_threshold=400)
     if blobs:
        max_blob=find_max(blobs) # 找出最大的红色
        # 脱机运行时注释掉下面两行 提高性能
        img.draw_rectangle(max_blob[0:4]) # 用矩形圈出目标颜色
        img.draw_cross(max_blob[5], max_blob[6]) # 在目标颜色区域的中心画十字形标记

        color_dist['R'] = max_blob[5] # 将色块的横坐标存入字典 为排序做准备


# 寻找 绿色 物料
def find_greenBlob():
     img = sensor.snapshot()
     img.lens_corr(strength = 1.8, zoom = 1.0) # 畸变校正
     blobs = img.find_blobs([green_threshold],pixels_threshold=400, area_threshold=400)
     if blobs:
        max_blob=find_max(blobs) # 找出最大的绿色
        # 脱机运行时注释掉下面两行 提高性能
        img.draw_rectangle(max_blob[0:4]) # 用矩形圈出目标颜色
        img.draw_cross(max_blob[5], max_blob[6]) # 在目标颜色区域的中心画十字形标记

        color_dist['G'] = max_blob[5] # 将色块的横坐标存入字典 为排序做准备


# 寻找 蓝色 物料
def find_blueBlob():
     img = sensor.snapshot()
     img.lens_corr(strength = 1.8, zoom = 1.0) # 畸变校正
     blobs = img.find_blobs([blue_threshold],pixels_threshold=400, area_threshold=400)
     if blobs:
        max_blob=find_max(blobs) # 找出最大的蓝色
        # 脱机运行时注释掉下面两行 提高性能
        img.draw_rectangle(max_blob[0:4]) # 用矩形圈出目标颜色
        img.draw_cross(max_blob[5], max_blob[6]) # 在目标颜色区域的中心画十字形标记

        color_dist['B'] = max_blob[5] # 将色块的横坐标存入字典 为排序做准备


# 色块排序
def color_sort():
    find_redBlob()
    find_blueBlob()
    find_greenBlob()
    sorted_value_list = sorted(color_dist, key=lambda x:color_dist[x])
    list2str = "".join(sorted_value_list) #转换为字符串
    print(list2str)
    print("\r\n")
    print(color_dist)
    uart.write(list2str+"\r\n")
    #led_flash()


# 灯闪 用来观察程序是否运行正常
def led_flash():
     led.on() # 灯亮
     time.sleep(100)
     led.off()


# 色块合并识别
def find_colorBlobs():
    img = sensor.snapshot()
    img.lens_corr(strength = 1.8, zoom = 1.0) # 畸变校正
    for blob in img.find_blobs(thresholds, pixels_threshold=400, area_threshold=100, merge=True):
        if blob.code() == 7: # r/g/b code
            color_sort()
        else:
            #print("can not find rgb blobs!!!")
            pass



while(True):
    clock.tick()
    img = sensor.snapshot()
    img.lens_corr(1.0) # strength of 1.8 is good for the 2.8mm lens.
    qrcode_recognition()
