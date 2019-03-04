 # Untitled - By: zxs - 周日 9月 2 2018

import sensor, image, time, lcd, json,math,ujson
from pyb import LED
from pyb import UART

led=LED(4)

obj = [[12,0],[10,12],[22,10],[99,11]]

threshold_index = 0 # 0 for red, 1 for green, 2 for blue

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
# The below thresholds track in general red/green/blue things. You may wish to tune them...
#(31, 61, 28, 56, -4, 42)

#(24, 66, 15, 74, -63, 71)
#(32, 65, -32, -6, -3, 21)
#(51, 71, -21, -4, -35, -19)
thresholds = [(19, 73, 36, 65, 3, 42), # generic_red_thresholds
              (26, 76, -50, -18, -12, 33), # generic_green_thresholds
              (24, 77, -31, 13, -50, -17)] # generic_blue_thresholds

objthresholds = [(47, 80, 20, 46, -10, 24), # generic_red_thresholds
            (65, 82, -13, -9, -4, 8), # generic_green_thresholds
            (58, 72, 1, 5, -19, -9)] # generic_blue_thresholds
#(47, 82, -26, -5, -7, 7)
#(66, 75, -20, -7, -3, 1)
#(50, 65, -1, 13, -18, -1)

#(50, 72, 2, 19, -30, -5)

graythreshold=[(100,255)]

K=5000

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA) # Special 128x160 framesize for LCD Shield.
sensor.skip_frames(time = 2000)
#sensor.set_auto_gain(False) # must turn this off to prevent image washout...
uart = UART(3, 115200)
uart.init(115200, bits=8, parity=None, stop=1)

# 比较两个色块大小的函数
def compareBlob(blob1, blob2):
    # 这里我们选择了pixels作为指标比对二者的代码
    # 你也可以换用其它指标 例如 blob.area()
    tmp = blob1.pixels() - blob2.pixels()
    if tmp == 0:
        return 0;
    elif tmp > 0:
        return 1;
    else:
        return -1;

#串口字符串指令切换功能：
#   {"RGB_blocks":"find_R"}   查找红色色块
#   {"RGB_blocks":"find_G"}   查找绿色色块
#   {"RGB_blocks":"find_B"}   查找蓝色色块
#   {"RGB_blocks":"find_RGB_blocks"} 查找最近色块颜色
#   {"Code_block":"find_Code"}  查找二维码
#   {"RGB_circles":"find_R"}   查找红色色环
#   {"RGB_circles":"find_G"}   查找绿色色环
#   {"RGB_circles":"find_B"}   查找蓝色色环
#   {"circle":"RGB"} 查找当前色环颜色
#   {"LED":"ON"}     打开LED
#   {"LED":"OFF"}     关闭LED
lcd.init() # Initialize the lcd screen.
A=5
B=0
while(True):

    exsit=0;
    data=[]
    img = sensor.snapshot()
    img.lens_corr(1.0) # strength of 1.8 is good for the 2.8mm lens.
    if(uart.any()):
        time.sleep(200)
        d=uart.read()
        da=str(d,"utf-8")

        jsobj=ujson.loads(da)
        for key in jsobj.keys():
            print('key:%s value:%s'%(key,jsobj.get(key)))
        if(jsobj.get("RGB_blocks")=="find_R"):
            A=1
        else:
            if(jsobj.get("RGB_blocks")=="find_G"):
                 A=2
            else:
                if(jsobj.get("RGB_blocks")=="find_B"):
                     A=3
                else:
                    if(jsobj.get("RGB_blocks")=="find_RGB_blocks"):
                         A=4
                    else:
                        if(jsobj.get("Code_block")=="find_Code"):
                             A=5
                        else:
                            if(jsobj.get("RGB_circles")=="find_R"):
                                 A=6
                                 B=0
                            else:
                                 if(jsobj.get("RGB_circles")=="find_G"):
                                      A=7
                                      B=0
                                 else:
                                     if(jsobj.get("RGB_circles")=="find_B"):
                                          A=8
                                          B0
                                     else:
                                         if(jsobj.get("circle")=="RGB"):
                                              A=6
                                              B=1
                                         else:
                                             if(jsobj.get("LED")=="ON"):
                                               led.on()
                                             else:
                                                 if(jsobj.get("LED")=="OFF"):
                                                   led.off()
                                                 else:
                                                     A=0
    if(A==1):
            blobs =img.find_blobs([thresholds[0]], pixels_threshold=2025, area_threshold=1600, merge=True)
            if len(blobs) == 0:
                continue
            bigBlob = blobs[0]
            for blob in blobs:
                if compareBlob(bigBlob, blob) == -1:
                    bigBlob = blob
            img.draw_rectangle(bigBlob.rect())
            img.draw_cross(bigBlob.cx(), bigBlob.cy()) #在目标颜色区域的中心画十字形标记
            Lm = bigBlob.h()
            length = 718.5*math.sin(0.003233*Lm+2.727)+131.9*math.sin(0.01649*Lm+4.406)
            if length<0:
                length=20
            output_str=json.dumps([bigBlob.cx(),bigBlob.cy(), int(length*10)])
            m=str(bigBlob.cx())
            m=m+'d'
            n=str(bigBlob.cy())
            m=m+n
            l=str(int(length*10))
            m=m+'L'
            m=m+l
            m=m+'sp'
            data_out ="{\"R\":"+output_str+"}\r\n"
            uart.write(m)
            print(m)

    if(A==2):
            blobs =img.find_blobs([thresholds[1]], pixels_threshold=2025, area_threshold=1600, merge=True)
            if len(blobs) == 0:
                continue
            bigBlob = blobs[0]
            for blob in blobs:
                if compareBlob(bigBlob, blob) == -1:
                    bigBlob = blob
            img.draw_rectangle(bigBlob.rect())
            img.draw_cross(bigBlob.cx(), bigBlob.cy())
            Lm = bigBlob.h()
            length = 718.5*math.sin(0.003233*Lm+2.727)+131.9*math.sin(0.01649*Lm+4.406)
            if length<0:
                length=20
            output_str=json.dumps([bigBlob.cx(),bigBlob.cy(), int(length*10)])
            m=str(bigBlob.cx())
            m=m+'d'
            n=str(bigBlob.cy())
            m=m+n
            l=str(int(length*10))
            m=m+'L'
            m=m+l
            m=m+'sp'
            data_out ="{\"G\":"+output_str+"}\r\n"
            uart.write(m)
            print(m)

    if(A==3):
            blobs=img.find_blobs([thresholds[2]], pixels_threshold=2025, area_threshold=1600, merge=True)
            if len(blobs) == 0:
                continue
            bigBlob = blobs[0]
            for blob in blobs:
                if compareBlob(bigBlob, blob) == -1:
                    bigBlob = blob
            img.draw_rectangle(bigBlob.rect())
            img.draw_cross(bigBlob.cx(), bigBlob.cy())
            Lm = bigBlob.h()
            length = 718.5*math.sin(0.003233*Lm+2.727)+131.9*math.sin(0.01649*Lm+4.406)
            if length<0:
                length=20
            output_str=json.dumps([bigBlob.cx(),bigBlob.cy(), int(length*10)])
            m=str(bigBlob.cx())
            m=m+'d'
            n=str(bigBlob.cy())
            m=m+n
            l=str(int(length*10))
            m=m+'L'
            m=m+l
            m=m+'sp'
            data_out ="{\"B\":"+output_str+"}\r\n"
            uart.write(m)
            print(m)

    if(A==4):
            blobs=img.find_blobs([thresholds[0],thresholds[1],thresholds[2]], pixels_threshold=2025, area_threshold=1600, merge=True)
            if len(blobs) == 0:
                continue
            bigBlob = blobs[0]
            for blob in blobs:
                if compareBlob(bigBlob, blob) == -1:
                    bigBlob = blob
            img.draw_rectangle(bigBlob.rect())
            img.draw_cross(bigBlob.cx(), bigBlob.cy())
            Lm = bigBlob.h()
            length = 718.5*math.sin(0.003233*Lm+2.727)+131.9*math.sin(0.01649*Lm+4.406)
            if length<0:
                length=20
            output_str=json.dumps([bigBlob.cx(),bigBlob.cy(), int(length*10)])
            if(bigBlob.code()==1):
                data_out ="{\"R\":"+output_str+"}\r\n"
            if(bigBlob.code()==2):
                data_out ="{\"G\":"+output_str+"}\r\n"
            if(bigBlob.code()==4):
                data_out ="{\"B\":"+output_str+"}\r\n"
                m=str(bigBlob.cx())
                m=m+'d'
                n=str(bigBlob.cy())
                m=m+n
                m=m+'sp'

            uart.write(m)
            print(m)

    if(A==5):
            for code in img.find_qrcodes():
                img.draw_rectangle(code.rect(), color = (0, 0, 255))
                output_str=json.dumps(code.payload())
                data_out="{\"code\":"+output_str+"}\r\n"
                m= code.payload()
                m='f'+m
                m=m+'sp'
                print(m)
                #uart.write('dsp')
                uart.write(m)

    if(A==6 ):
            img.binary([objthresholds[0]])
            img.dilate(2)

            blobs=img.find_blobs(graythreshold, pixels_threshold=2025, area_threshold=1600, merge=True)
            if len(blobs) == 0:
                continue
            bigBlob = blobs[0]
            for blob in blobs:
                if compareBlob(bigBlob, blob) == -1:
                    bigBlob = blob
                img.draw_rectangle(bigBlob.rect())
         #       print(bigBlob.cx(), bigBlob.cy())
                output_str=json.dumps([bigBlob.cx(),bigBlob.cy()])
                data_out ="{\"R\":"+output_str+"}\r\n"
            uart.write(data_out)
            print(data_out)

    if(A==7 ):
            img.binary([objthresholds[1]])
            img.dilate(4)
            blobs=img.find_blobs(graythreshold, pixels_threshold=2025, area_threshold=1600, merge=True)
            if len(blobs) == 0:
                continue
            bigBlob = blobs[0]
            for blob in blobs:
                if compareBlob(bigBlob, blob) == -1:
                    bigBlob = blob
                img.draw_rectangle(bigBlob.rect())
         #       print(bigBlob.cx(), bigBlob.cy())
                output_str=json.dumps([bigBlob.cx(),bigBlob.cy()])
                data_out ="{\"G\":"+output_str+"}\r\n"
            uart.write(data_out)
            print(data_out)
    if(A==8 ):
            img.binary([objthresholds[2]])
            img.dilate(4)

            blobs=img.find_blobs(graythreshold, pixels_threshold=900, area_threshold=900, merge=True)
            if len(blobs) == 0:
                continue
            bigBlob = blobs[0]
            for blob in blobs:
                if compareBlob(bigBlob, blob) == -1:
                    bigBlob = blob
                img.draw_rectangle(bigBlob.rect())
         #       print(bigBlob.cx(), bigBlob.cy())
                output_str=json.dumps([bigBlob.cx(),bigBlob.cy()])
                data_out ="{\"B\":"+output_str+"}\r\n"
            uart.write(data_out)
            print(data_out)



    lcd.display(img)
#    print(clock.fps())
