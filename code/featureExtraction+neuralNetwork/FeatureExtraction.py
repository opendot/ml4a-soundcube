

from matplotlib import pyplot as plt
import matplotlib.cm as cm
import fnmatch
import os
import numpy as np
import librosa
from sklearn.manifold import TSNE
import json
import time
import progressbar as prog


soundClass = 'voci'
audio_folder = '../../data/data_segmented/'+soundClass+'/'
out_folder = '../../data/data_features/'+soundClass+'/'

ext = '*.wav'

    
def get_features(y, sr): 
    
    # Audio spectogram
    feat_stft = np.abs(librosa.stft(y))
    
    # Audio melspectrogram
    S = librosa.feature.melspectrogram(y, sr=sr, n_fft=1024,hop_length=512, n_mels=60)
    log_S = librosa.logamplitude(S, ref_power=np.max)
    
    # MFCC
    mfcc = librosa.feature.mfcc(S=log_S, n_mfcc=13)
    delta_mfcc = librosa.feature.delta(mfcc)
    delta2_mfcc = librosa.feature.delta(mfcc, order=2)

    feat_mfcc = np.vstack((mfcc,delta_mfcc,delta2_mfcc))

    feat_dict = {'stft': feat_stft, 'stft_mean': np.mean(feat_stft,1),'mel': log_S, 'mel_mean': np.mean(log_S,1), 'mfcc': feat_mfcc, 'mfcc_mean': np.mean(feat_mfcc,1)}
    
    return feat_dict






if __name__ == "__main__":

    files = []
    for root, dirnames, filenames in os.walk(audio_folder):
        for filename in fnmatch.filter(filenames, ext):
            files.append(os.path.join(root, filename))
    
    print("found "+ext+" %d files in %s"%(len(files),audio_folder))
    

    collectedFeat_MFCC = []  
    collectedFeat_Mel = [] 
    collectedFeat_STFT = []   
    
    widgets = [prog.Counter(),'/', str(len(files)),' | ',prog.Percentage(),' | ', prog.Bar(marker='='),' | ', prog.ETA(),' | ',prog.AbsoluteETA()]
    bar = prog.ProgressBar(widgets=widgets, maxval=len(files)).start()
    
    for i,f in enumerate(files):
        
        y, sr = librosa.load(f)
        if (np.sum(y) != 0):
            y *= 1.0 / np.max(np.abs(y))
            
        
        # Feature Extraction
            
        feat = get_features(y, sr)
        
        tail, filename = os.path.split(f)
        feat['fileFolder'] = tail+'/'
        feat['fileName'] = filename
        feat['class'] = soundClass
        
        # Convert numpy arrays into lists
        for k in feat.keys():
            if (type(feat[k]).__module__ == np.__name__):
                feat[k] = feat[k].tolist()
        
        # Save NPZ file
        np.savez(out_folder+filename+'.npz', feat)
        
            
        collectedFeat_MFCC.append({'class': feat['class'], 'fileFolder': feat['fileFolder'], 'fileName': feat['fileName'], 'mfcc_mean': feat['mfcc_mean'], 'mfcc': feat['mfcc']})
        collectedFeat_Mel.append({'class': feat['class'], 'fileFolder': feat['fileFolder'], 'fileName': feat['fileName'], 'mel_mean': feat['mel_mean'], 'mel': feat['mel']})
        collectedFeat_STFT.append({'class': feat['class'], 'fileFolder': feat['fileFolder'], 'fileName': feat['fileName'], 'stft_mean': feat['stft_mean'], 'stft': feat['stft']})
        
        if ((i%10)==0):
            bar.update(i)
    
    
    bar.finish()    
    
    np.savez(out_folder+soundClass+'_MFCC.npz', collectedFeat_MFCC)
    np.savez(out_folder+soundClass+'_Mel.npz', collectedFeat_Mel)
    np.savez(out_folder+soundClass+'_STFT.npz', collectedFeat_STFT)
            
            
            
            
            
            
            
            
            
            