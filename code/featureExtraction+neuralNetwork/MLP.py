

# Sound Classification using Multilayer Perceptron


import os
import fnmatch
import json
import numpy as np
from sklearn import svm
from sklearn.metrics import accuracy_score
from sklearn.cross_validation import train_test_split
from keras.models import Sequential
from keras.layers.core import Dense, Dropout, Activation
from keras.optimizers import SGD

#soundClass = ['Legno', 'Voci','Metallici','Liquidi']
soundClass = ['Legno', 'Liquidi']
#path_features = '/Users/MaX/Work/Projects/SoundCube/data/data_features/'+soundClass+'/'
path_features = '/Users/MaX/Work/Projects/SoundCube/data/data_features/'
ext = '*.json'
test_size = 0.3    



# Initialize the MLP
def initialize_nn(frame_size):
    model = Sequential() # The Keras Sequential model is a linear stack of layers.
    model.add(Dense(100, init='uniform', input_dim=frame_size)) # Dense layer
    model.add(Activation('tanh')) # Activation layer
    model.add(Dropout(0.5)) # Dropout layer
    model.add(Dense(100, init='uniform')) # Another dense layer
    model.add(Activation('tanh')) # Another activation layer
    model.add(Dropout(0.5)) # Another dropout layer
    model.add(Dense(2, init='uniform')) # Last dense layer
    model.add(Activation('softmax')) # Softmax activation at the end
    
    sgd = SGD(lr=0.1, decay=1e-6, momentum=0.9, nesterov=True) # Using Nesterov momentum
    
    model.compile(loss='binary_crossentropy', optimizer=sgd, metrics=['accuracy']) # Using logloss
    
    return model
    
    
    
    


if __name__ == "__main__":

    print('load feature file')
    
    # Load features
    d = []
    feat = []          
    GT = []
    for i,c in enumerate(soundClass):
        print('Load class '+c)

        fileName = path_features+c+'/'+c+'_mean.json'
        with open(fileName) as json_data:
            feat_temp = json.load(json_data)
        
        feat = np.hstack((feat,feat_temp))
        GT = np.hstack((GT,np.zeros(np.shape(feat_temp)[0])+i))
    
    # Collect features in a numpy array
    X = []
    for i,f in enumerate(feat):
        if (X == []):
            X = f['mel']
        else:    
            X = np.vstack((X,f['mel']))
            
            
    # Generate trining and test dataset
    
    X_train, X_test, Y_train, Y_test = train_test_split(X, GT, test_size = test_size)
    
    
    
    # -----  Multilayer perceptron -----
    print('Initializing model')
    model = initialize_nn(frame_size)
    
    print('Training model')
    model.fit(X_train, y_train,
            batch_size=32, nb_epoch=100,
            verbose=1, callbacks=[],
            validation_split=0.2,
            validation_data=None,
            shuffle=True,
            class_weight=None,
            sample_weight=None)
    
    print('Predicting on test data')
    y_score = model.predict(X_test)
    
    print('Generating results')
    generate_results(y_test[:, 0], y_score[:, 0])
    
            
        
    # # ----- TRAIN -----            
    # clf = svm.SVC(C=10)     
    # clf.fit(X_train,Y_train)  
    #     
    # 
    # 
    # # ----- TEST -----
    # y_pred = clf.predict(X_test)
    
    
    
    # ----- EVALUATION -----
    acc = accuracy_score(Y_test,y_pred)                    
    print("The accuracy is: "+str(acc))     
            
            