
# Author: Massimiliano Zanoni
# Created on 11/12/2016

# Rename files of a specific format in a given folder using a progressive code


import fnmatch
import os
import json
from subprocess import call




# Rename Files

soundClass = 'legno'
path = '../data/data_segmented/'+soundClass+'/'
path_out = '../data/data_segmented/'+soundClass+'/'
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
    
    
    
    
    
    