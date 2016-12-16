
# Author: Massimiliano Zanoni
# Created on 11/12/2016

# Extract a set of segments from audio in a folders.



from matplotlib import pyplot as plt
import matplotlib.cm as cm
import fnmatch
import os
import numpy as np
import librosa
from sklearn.manifold import TSNE
import json


soundClass = 'voci'
path = '../../data/data/'+soundClass+'/'
path_seg = '../../data/data_segmented/'+soundClass+'/'
ext = '*.wav'

seg_length = 1     # Lenght of each segment in seconds
numSeg = 5          # Number of segments to produce for each file


# Perfomers the segmentation. Segments are extracted senquentially from the beginning of the audio stream 
# y: audio stream
# sr: sample rate

def segment(y, sr):

    seg_qnt = sr*seg_length
    flen = np.shape(y)
    flen = flen[0]
    
    seg = []    
    for i in range(0, flen, seg_qnt):
        seg_temp = y[i:i+seg_qnt]
        if np.shape(seg_temp)[0] < sr*seg_length:
            l = (sr*seg_length) - np.shape(seg_temp)[0]
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
            
            
            
            
            
            
            
            
            