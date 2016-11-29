

from matplotlib import pyplot as plt
import matplotlib.cm as cm
import fnmatch
import os
import numpy as np
import librosa
from sklearn.manifold import TSNE
import json


soundClass = 'voci'
path = '/Users/MaX/Desktop/ML4A/Project/data/'+soundClass+'/'
path_seg = '/Users/MaX/Desktop/ML4A/Project/data_seg/'+soundClass+'/'
path_out = '/Users/MaX/Desktop/ML4A/Project/data_out/'+soundClass+'/'
SET_TIME = 1
ext = '*.wav'
numSeg = 5      # Number of segment to produce for each file



def segment(y, sr):
   
    time = SET_TIME
    seg_qnt = sr*time
    flen = np.shape(y)
    flen = flen[0]
    
    seg = []    
    for i in range(0, flen, seg_qnt):
        seg_temp = y[i:i+seg_qnt]
        if np.shape(seg_temp)[0] < sr*time:
            l = (sr*time) - np.shape(seg_temp)[0]
            a = seg_temp
            seg_temp = np.hstack((a,np.zeros(l)))
    
        seg.append(seg_temp) 
            
    return seg



if __name__ == "__main__":

    files = []
    for root, dirnames, filenames in os.walk(path):
        for filename in fnmatch.filter(filenames, ext):
            files.append(os.path.join(root, filename))
    
    print("found "+ext+" %d files in %s"%(len(files),path))
    
    
    feature_vectors_MFCC = []
    feature_vectors_MEL = []
    feature_vectors_STFT = []    
    
    for i,f in enumerate(files):
        
        data = []
        
        print ("get %d of %d = %s"%(i+1, len(files), f))
        
        y, sr = librosa.load(f)
        if (np.sum(y) != 0):
            y *= 1.0 / np.max(np.abs(y))
            
        seg_list = segment(y,sr)
        
        
        # Select segments
        if np.shape(seg_list)[0]>numSeg:
            seg_list  = seg_list[0:numSeg]
        
        
        # Save segments
        for i,seg in enumerate(seg_list):
            tail, filename = os.path.split(f)
            librosa.output.write_wav(path_seg+filename+'_'+str(i)+ '.wav', seg, sr, norm=True)
            
            
            
            
            
            
            
            
            