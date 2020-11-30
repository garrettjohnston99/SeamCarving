import numpy as np
import imageio as io
import os.path

# Animates successive frames of seam removal as saved by SeamCarving.cpp
# Assumes frames are enumerated [1..n] in the folder 'data/output/' 
#
# Garrett Johnston & Thanh Nguyen, CS73 20F


im_path = 'data/output/'
images = []

i = 1

path = im_path + str(i) + '.png'
while os.path.exists(path):
    images.append(path)
    i += 1
    path = im_path + str(i) + '.png'

im_list = []
for filename in images:
    im_list.append(io.imread(filename))

io.mimwrite('data/output/dali50.gif', im_list)
