import sys
import glob
import matplotlib.pyplot as plt
import matplotlib.animation as animation

ims = []
fig = plt.figure(figsize=(12, 12))
frames = len(glob.glob(sys.argv[1] + 'anim/*.png'))
for indx in range(frames):
	fid = sys.argv[1] + 'anim/{}.png'.format(indx)
	img = plt.imread(fid)
	im = plt.imshow(img, animated=True)
	ims.append([im])

ani = animation.ArtistAnimation(fig, ims, interval=50, blit=True)
ani.save(sys.argv[1] + "animation.mp4")