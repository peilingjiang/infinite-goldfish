from picamera import PiCamera
from time import sleep
import os

# Google Drive is setup here directly
# Credential Checked, Folder created if needed
import drive

photo_freq = 60
light_sensing = 3

if __name__ == 'main':

    camera = PiCamera()
    i = 1
    while True:
        # TODO: Are there any more elegant ways to stop?
        camera.start_preview()
        sleep(light_sensing)
        f_path = 'goldfish_%s.jpg' % i  # '/home/pi/Desktop/goldfish_%s.jpg'
        f_name = f_path.split('/')[-1]
        camera.capture(f_path)
        camera.stop_preview()
        drive.upload(f_name, f_path, 'image/jpeg')
        sleep(photo_freq - light_sensing)  # Take one photo every minute
        if os.path.exists(f_path):
            os.remove(f_path)
        else:
            print("PATH NOT EXISTS ERROR")
        i += 1

    print('Finished')
