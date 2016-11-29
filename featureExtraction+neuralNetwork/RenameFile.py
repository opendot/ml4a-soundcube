from matplotlib import pyplot as plt
import matplotlib.cm as cm
import fnmatch
import os
import numpy as np
import librosa
from sklearn.manifold import TSNE
import json
from subprocess import call




# Rename Files

soundClass = 'legno'
path = '/Users/MaX/Desktop/ML4A/Project/data_seg/'+soundClass+'/'
path_out = '/Users/MaX/Desktop/ML4A/Project/data_seg/'+soundClass+'/'
SET_TIME = 1
ext = '.wav'



if __name__ == "__main__":

    files = []
    for root, dirnames, filenames in os.walk(path):
        for filename in fnmatch.filter(filenames, "*"+ext):
            files.append(os.path.join(root, filename))
    
    print("found "+ext+" %d files in %s"%(len(files),path))
    
    for i,f in enumerate(files):
        tail,name = os.path.split(f)
        newname = tail+"/"+soundClass+"_"+str(i)+ext
        print(newname)
        os.rename( f, os.path.join(tail, newname))
    
    
    
    
    
    