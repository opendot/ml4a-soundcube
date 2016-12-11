

from matplotlib import pyplot as plt
import matplotlib.cm as cm
import fnmatch
import os
import numpy as np
import librosa
from sklearn.manifold import TSNE
import json


soundClass = 'legno'
path = '/Users/MaX/Desktop/ML4A/Project/data_seg/'+soundClass+'/'
path_seg = '/Users/MaX/Desktop/ML4A/Project/data_seg/'+soundClass+'/'
path_out = '/Users/MaX/Desktop/ML4A/Project/data_out/'+soundClass+'/'
ext = '*.wav'

    
def get_features(y, sr): 
    #feat_stft = np.abs(librosa.stft(y))
    feat_stft = []
    
    S = librosa.feature.melspectrogram(y, sr=sr, n_fft=1024,hop_length=512, n_mels=120)
    log_S = librosa.logamplitude(S, ref_power=np.max)
    
    #mfcc = librosa.feature.mfcc(S=log_S, n_mfcc=13)
    #delta_mfcc = librosa.feature.delta(mfcc)
    #delta2_mfcc = librosa.feature.delta(mfcc, order=2)

    #feat_mfcc = np.concatenate((np.mean(mfcc,1), np.mean(delta_mfcc,1), np.mean(delta2_mfcc,1)))
    #feat_mfcc = (feat_mfcc-np.mean(feat_mfcc))/np.std(feat_mfcc)
    
    feat_dict = {'stft': feat_stft, 'mel': log_S}
    
    return feat_dict






if __name__ == "__main__":

    files = []
    for root, dirnames, filenames in os.walk(path):
        for filename in fnmatch.filter(filenames, ext):
            files.append(os.path.join(root, filename))
    
    print("found "+ext+" %d files in %s"%(len(files),path))
    

    data = []  
    data_mean = []
    
    for i,f in enumerate(files):
        
        print ("get %d of %d = %s"%(i+1, len(files), f))
        
        y, sr = librosa.load(f)
        if (np.sum(y) != 0):
            y *= 1.0 / np.max(np.abs(y))
            
        
        # Feature Extraction
            
        feat = get_features(y, sr)
            
        #a = feat['mfcc'].tolist()
        #b = feat['stft'].tolist()
        b = []
        c = feat['mel'].tolist()
        c_mean = np.mean(c,1).tolist()
    
        data.append({'path':files[i],'stft': b,'mel':c, 'class':soundClass})
        data_mean.append({'path':files[i],'stft': b,'mel':c_mean, 'class':soundClass})

    #tail, filename = os.path.split(f)
    #with open(path_out+soundClass+'.json', 'w') as f:
    #    json.dump(data, f) 
        
    with open(path_out+soundClass+'_mean.json', 'w') as f_mean:
        json.dump(data_mean, f_mean)    
        
            
            
            
            
            
            
            
            
            
            
            