# SOUNDCUBE

![Soundcube](/images/soundbox.jpg)


## Concept

The project has two main objectives: the first is to use convolutional neural networks to classify audio samples according to a predefined set of categories, with a focus on environmental sounds. The second objective is to build a 3d immersive virtual environment where sounds are placed based on their similarity, allowing users to explore them and play them as he "walks" inside the space.

The first part of the project could be useful for sound designers: they could build their own dataset and obtain an automatic classification of new sounds they want to use. It could also be useful to listen to the environment and detect special events happening in the "soundscape" surrounding the user.   

The second part of the project, on the other hand, can be especially useful for exhibitions, or for music performances. The way in which the 3d environment can be explored are many: for example, a composer could define a logic to walk inside the 3d space in a semi automated way, or devices like a Kinect or a Leap Motion could be used to move the focus on a particular zone in the environment.   


## Realization

We based our implementation on a [paper](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&cad=rja&uact=8&ved=0ahUKEwjgyOSm3s3QAhWHCsAKHf0qDaYQFggjMAA&url=http%3A%2F%2Fkarol.piczak.com%2Fpapers%2FPiczak2015-ESC-ConvNet.pdf&usg=AFQjCNEiXav5RG38ouVrn5HxrdeDfPOCnw&sig2=T41gwBtoQJ3glYS2WU5G3Q) by Karol J. Piczak, describing a suitable approach for what we wanted to do. 
A dataset of approximately 4000 sounds divided in 4 categories (wood, water, fans and voices) was built in order to have a training set for the machine learning step. The idea is to use the last layer of the convnet as feature vector for each sound, and to use [t-SNE](https://lvdmaaten.github.io/tsne/) in order to reduce the dimenions of the feature vector to 3 and be able to plot each audio sample on our 3d environment.

