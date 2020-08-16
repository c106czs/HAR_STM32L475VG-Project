"""
@author：陈章韶
@organization：华南理工大学

搭建神经网络，读取数据集，训练得到h5模型文件
"""
import numpy as np
import pandas as pd
import time
from keras.models import Sequential
from keras.layers import Input, Dense, Conv2D, MaxPooling2D, Flatten, Dropout, Activation
from keras.models import Model
from keras import optimizers
from keras.utils import plot_model

ws = 200

# 读取原始csv数据集，并根据窗口大小整理成适合于网络输入层的向量
def readData(input_file, window_size=ws):
    fp = open(file=input_file, mode='r')
    dataset_x = []  # 存储训练的参数
    tmp_x = []
    dataset_y = []  # 存储标签
    cur_y = -1
    # 需要处理完文件所有的行
    for line in fp.readlines():
        str_list = line.split(',')
        if(len(str_list) < 7):
            continue
        # 得到一行x，6个数据
        single_x = [float(str_list[0]), float(str_list[1]), float(
            str_list[2]), float(str_list[3]), float(str_list[4]), float(str_list[5])]
        # 得到一个标签
        single_y = int(str_list[6])
        if cur_y == -1:
            cur_y = single_y
        if cur_y == single_y:
            if len(tmp_x) < window_size:
                tmp_x.append(single_x)
            else:
                dataset_x.append(tmp_x)
                dataset_y.append(single_y)
                tmp_x = []
                cur_y = -1
        else:
            tmp_x = []
            cur_y = -1
    return np.array(dataset_x), np.array(dataset_y)


# the neural network model
def NN_model(class_num=12, window_size=ws):
    activation = 'relu'
    inp = Input((window_size, 6,1))
    H = Conv2D(filters=18, kernel_size=(12, 2))(inp)
    H = Activation(activation)(H)
    H = MaxPooling2D(pool_size=(2, 1))(H)

    H = Conv2D(filters=36, kernel_size=(12, 1))(H)
    H = Activation(activation)(H)
    H = MaxPooling2D(pool_size=(2, 1))(H)

    H = Conv2D(filters=24, kernel_size=(12, 1))(H)
    H = Activation(activation)(H)
    H = MaxPooling2D(pool_size=(2, 1))(H)

    H = Flatten()(H)
    H = Dense(class_num)(H)
    H = Activation('softmax')(H)

    model = Model([inp], H)
    model.compile(loss='categorical_crossentropy', metrics=[
                  'accuracy'], optimizer='Adadelta')
    return model

#读取原始数据集的X和Y
data_X, data_Y = readData('USC-HAD_All.csv')
# 对标签进行独热编码（one-hot encoding）
labels = np.asarray(pd.get_dummies(data_Y), dtype=np.int8)

# 设定一个最大分割率，用于切割出训练集和测试集
trainSplitRatio = 0.8
# 对输入Xreshape使其适合于输入层
reshapedData_X = data_X.reshape(
    data_X.shape[0], data_X.shape[1], data_X.shape[2], 1)
# 随机生成一个不大于trainSplitRatio的分割率
trainSplit = np.random.rand(len(reshapedData_X)) < trainSplitRatio
# 分割出训练集和测试集
trainX = reshapedData_X[trainSplit]
testX = reshapedData_X[~trainSplit]
trainX = np.nan_to_num(trainX)
testX = np.nan_to_num(testX)
trainY = labels[trainSplit]
testY = labels[~trainSplit]
# 生成训练用的模型
model = NN_model()
print('\r\nmodel structure:')
for layer in model.layers:
    print(layer.name)
print('\r\n')
# 将模型结构保存成png
plot_model(model, to_file='model_structure.png', show_shapes='True')
# 开始训练模型
model.fit(x=trainX, y=trainY, batch_size=128, epochs=20,
          verbose=2, validation_split=1-trainSplitRatio)
# 获取模型训练结果
score = model.evaluate(x=testX, y=testY, verbose=2)
print('\r\nloss = %f\r\n'%score[0])
print('accuracy = %f\r\n'%score[1])
print('Baseline Error = %.2f%%\r\n' % (100-score[1]*100))
# 保存模型文件.h5
model.save('model_'+str(int(round(time.time()*1000)))+'.h5')
# 保存测试集
np.save('groundTruth.npy',testY)
np.save('testData.npy',testX)