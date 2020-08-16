import numpy as np

testX=np.load('testData.npy')
testY=np.load('groundTruth.npy')
print(testX.shape)
print(testY.shape)
testX=testX.reshape(testX.shape[0],testX.shape[1]*testX.shape[2]*testX.shape[3])
testY=testY.reshape(testY.shape[0],12)
np.savetxt(fname='x.csv',X=testX,fmt='%.06f',delimiter=',')
np.savetxt(fname='y.csv',X=testY,fmt='%d',delimiter=',')