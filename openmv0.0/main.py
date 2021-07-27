#************************************ (C) COPYRIGHT 2019 ANO ***********************************#
import sensor, image, time, math, struct
import json
from pyb import LED,Timer
from struct import pack, unpack
import Message,LineFollowing,DotFollowing,ColorRecognition,QRcode,Photography,find_apriltags
#初始化镜头
sensor.reset()
sensor.set_vflip(True)
sensor.set_hmirror(True)
sensor.set_transpose(False)
sensor.set_pixformat(sensor.RGB565)#设置相机模块的像素模式
sensor.set_framesize(sensor.QQVGA)#设置相机分辨率240*160
sensor.skip_frames(30)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)#若想追踪颜色则关闭白平衡
clock = time.clock()#初始化时钟
Message.Ctr.WorkMode=2 #测试AprilTag用


#主循环
while(True):
    clock.tick()#时钟初始化
    #接收串口数据
    if Message.Ctr.WorkMode==0:#点检测
        DotFollowing.DotCheck()
    elif Message.Ctr.WorkMode==1:#线检测
        LineFollowing.LineCheck()
    elif Message.Ctr.WorkMode==2:#AprilTag检测
        find_apriltags.Find_Apriltags()
    elif Message.Ctr.WorkMode==3:#颜色识别
        ColorRecognition.ColorRecognition()
    elif Message.Ctr.WorkMode==4:#二维码识别
        QRcode.ScanQRcode()
    elif Message.Ctr.WorkMode==5:#拍照
        Photography.Photography('IMG.jpg',10)

    #Message.Ctr.WorkMode += 1
    #Message.Ctr.WorkMode = Message.Ctr.WorkMode %3


    #LastWorkMode = Message.Ctr.WorkMode
    #用户数据发送
    #Message.UartSendData(Message.UserDataPack(127,127,32767,32767,65536,65536,65536,65536,65536,65536))
    #计算程序运行频率
'''
    if Message.Ctr.IsDebug == 1:
        fps=int(clock.fps())
        Message.Ctr.T_ms = (int)(1000/fps)
        print('fps',fps,'T_ms',Message.Ctr.T_ms)
'''

#************************************ (C) COPYRIGHT 2019 ANO ***********************************#
