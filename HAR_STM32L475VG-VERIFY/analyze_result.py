"""
参考原作者代码修改而成，原作者代码链接：https://github.com/Shahnawax/HAR-CNN-Keras/blob/master/evaluate_model.py
以下是原作者声明：

Created on Wed Oct  4 17:07:51 2017
This script is written to evaluate a pretrained model saved as  model.h5 using 'testData.npy' 
and 'groundTruth.npy'. This script reports the error as the cross entropy loss in percentage
and also generated a png file for the confusion matrix. 
@author:Muhammad Shahnawaz
"""
import numpy as np
import matplotlib.pyplot as plt

def plot_cm(cM, labels,title='Confusion Matrix STM32'):
    # normalizing the confusionMatrix for showing the probabilities
    cmNormalized = np.around((cM/cM.sum(axis=1)[:,None])*100,2)
    # creating a figure object
    fig = plt.figure()
    # plotting the confusion matrix
    plt.imshow(cmNormalized,interpolation=None,cmap = plt.cm.Blues)
    # creating a color bar and setting the limits
    plt.colorbar()
    plt.clim(0,100)
    # assiging the title, x and y labels
    plt.xlabel('Predicted Values')
    plt.ylabel('Ground Truth')
    plt.title(title + '\n% confidence')
    # defining the ticks for the x and y axis
    plt.xticks(range(len(labels)),labels,rotation = 60)
    plt.yticks(range(len(labels)),labels)
    # number of occurences in the boxes
    width, height = cM.shape 
    print('Accuracy for each class is given below.')
    for predicted in range(width):
        for real in range(height):
            color = 'black'
            if(predicted == real):
                color = 'white'
                print(labels[predicted].ljust(12)+ ':', cmNormalized[predicted,real], '%')
            plt.gca().annotate(
                    '{:d}'.format(int(cmNormalized[predicted,real])),xy=(real, predicted),
                    horizontalalignment = 'center',verticalalignment = 'center',color = color)
    # making sure that the figure is not clipped
    plt.tight_layout()
    # saving the figure
    fig.savefig(title +'.png')

trueX = np.load(file='groundTruth.npy')
print(trueX.shape)
testX = np.loadtxt(fname='res-backup.csv', delimiter=',')
print(testX.shape)
label_sum = np.zeros(shape=12, dtype=np.int)
correct_sum = np.zeros(shape=12, dtype=np.int)
cm = np.zeros(shape=(12, 12), dtype=np.int)
for i in range(0, testX.shape[0]):
    max_true_index = np.argmax(trueX[i])
    max_test_index = np.argmax(testX[i])
    cm[max_true_index][max_test_index] += 1
    label_sum[max_true_index] += 1
    if max_true_index == max_test_index:
        correct_sum[max_test_index] += 1

confusion_matrix = np.zeros(shape=(12, 12))
for i in range(0, 12):
    for j in range(0, 12):
        confusion_matrix[i][j] = cm[i][j]/label_sum[i]

print("Confusion Matrix:")
for i in range(0, 12):
    for j in range(0,12):
        print("%.2f %%,"%(confusion_matrix[i][j]*100),end='')
    print('\r\n')

labels = ['walking forward', 'walking left', 'walking right', 'walking upstairs', 'walking downstairs',
          'running forward', 'jumping', 'sitting', 'standing', 'sleeping', 'elevator up', 'elevator down']
correct_rate=np.zeros(shape=12)
print("The accuracy of each activity:\r\n")
for i in range(0, 12):
    correct_rate[i]=correct_sum[i]/label_sum[i]
    print("%s : %.2f %%"%(labels[i],correct_rate[i]*100))

plot_cm(cM=cm,labels=labels)