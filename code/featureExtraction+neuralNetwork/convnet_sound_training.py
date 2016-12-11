import argparse
import functools
import os
import sys
import fnmatch
import numpy as np
import keras
import json

from random import shuffle
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Convolution2D, MaxPooling2D
from keras.optimizers import SGD


def process_arguments(args):
	parser = argparse.ArgumentParser(description='json parser + convnet')
	parser.add_argument('--input_dir', action='store', help='path to the json file directories')
	parser.add_argument('--output_file', action='store', help='path to where to store the convnet training data')
	#parser.add_argument('--exc', action='store', help='number of excerpts to analyze')
	params = vars(parser.parse_args(args))
	return params

def label_def(label_class):
	label_value = []
	#label_string = os.path.basename(os.path.normpath(sample_path))
	if label_class == 'legno':
		label_value = [1, 0, 0, 0]
	elif label_class == 'liquidi':
		label_value = [0, 1, 0, 0]
	elif label_class == 'metallici':
		label_value = [0, 0, 1, 0]
	elif label_class == 'voci':
		label_value = [0, 0, 0, 1]
	else:
		print("No input specified!")
	return label_value

def uniform(scale):
    return functools.partial(keras.initializations.uniform, scale=scale)

def normal(stdev):
    return functools.partial(keras.initializations.normal, scale=stdev)

if __name__ == '__main__':

	X_train = []
	Y_train = []
	X_test = []
	Y_test = []

	model = Sequential()
	L2 = keras.regularizers.l2
	params = process_arguments(sys.argv[1:])

	json_dir = params['input_dir']
	#number_exc = int(params['exc'])
	print("parsing json files in directory %s"%json_dir)
	files = []
	extension = 'json'
	for root, dirnames, filenames in os.walk(json_dir):
	    for filename in fnmatch.filter(filenames, '*.'+extension):
	        files.append(os.path.join(root, filename))

	coeff_list = []
	labels = []
	for fil in files:
		excerpt_list = json.load(open(fil,'r'))
		#for j,excerpt in enumerate(excerpt_list):
		for j, excerpt in enumerate(excerpt_list):
			excerpt_class = excerpt['class']
			#excerpt_spectrum = excerpt['stft']
			excerpt_coeff = excerpt['mel']   # 120x44
			#print np.array(excerpt_coeff).shape
			coeff_list.append(excerpt_coeff)
			labels.append(label_def(excerpt_class))
			#if j==number_exc-1: 
			#	break

	X = coeff_list
	Y = labels
	npX = np.array(X)

	#print len(X), len(Y)
	idX = range(len(X))
	shuffle(idX)
	#print len(idX)

	print len(X), len(Y), len(idX)

	for j,index in enumerate(idX):
		if (j < 4000):
			X_train.append(X[index])
			Y_train.append(Y[index])
		else:
			X_test.append(X[index])
			Y_test.append(Y[index]) 

	X_train = np.array(X_train)
	X_test = np.array(X_test)
	#normalization
	xMax = np.min(np.min(np.min(npX,0), 0), 0)   #to correct with --> max(max(max(abs(npX))))
	print 'matrix maximum', xMax
	X_train = np.divide(X_train, xMax)
	X_test = np.divide(X_test, xMax)
	#reshaping to fit neural network input
	X_train = X_train.reshape(len(X_train), 120, 44, 1)
	X_test = X_test.reshape(len(X_test), 120, 44, 1)
	print 'X train shape', np.array(X_train).shape
	print 'X test shape', np.array(X_test).shape



	model.add(Convolution2D(80, 57, 6, init=uniform(0.001), W_regularizer=L2(0.001), input_shape=(120, 44, 1)))
	model.add(Activation('relu'))
	model.add(MaxPooling2D(pool_size=(4, 3), strides=(1,3)))
	model.add(Dropout(0.5))
	model.add(Convolution2D(80, 1, 3, init=uniform(0.1), W_regularizer=L2(0.001)))
	model.add(Activation('relu'))
	model.add(MaxPooling2D(pool_size=(1, 3), strides=(1,3)))
	model.add(Flatten())
	model.add(Dense(5000, init=normal(0.01), W_regularizer=L2(0.001)))
	model.add(Activation('relu'))
	model.add(Dropout(0.5))
	model.add(Dense(5000, init=normal(0.01), W_regularizer=L2(0.001)))
	model.add(Activation('relu'))
	model.add(Dropout(0.5))

	model.add(Dense(4, init=normal(0.01), W_regularizer=L2(0.001)))
	model.add(Activation('softmax'))

	sgd = SGD(lr=0.1, decay=1e-6, momentum=0.9, nesterov=True)
	model.compile(loss='categorical_crossentropy', optimizer=sgd)
	
	conv_weights = model.layers[0].get_weights()
	conv_weights[1][:] = 0.1
	model.layers[0].set_weights(conv_weights)

	print('\nTraining... (batch size of 1 000 | 30 batches per epoch)')
	model.fit(X_train, Y_train, batch_size=1000, nb_epoch=50, shuffle=True)
	#model.net.save('trainingML4A.h5')

	print("saving weights to file")
	model.save_weights("weights.h5")

	#print("prediction")
	#predictions = model.predict(X_train)
	#print predictions

	print("evaluation")
	score = model.evaluate(X_test, Y_test, verbose = 0)
	print score






