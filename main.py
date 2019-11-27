from click._compat import raw_input
from picamera import PiCamera
from time import sleep
from fractions import Fraction
import os

import signal
from PIL import Image as PILImage

# Google Drive is setup here directly
# Credential Checked, Folder created if needed
import drive

# IMPORTANT
# Please make sure a client_id.json file is created in the same folder after 'git pull'.

photo_freq = 60
light_sensing = 4
wait_gap = 2

# Set to the current first photo
start_num = 1
target_color_1 = {(250, 50, 50), (200, 60, 60)}


def interrupted(signum, frame):
    signal.signal(signal.SIGALRM, interrupted)


def input():
    try:
        print('Now press any alphabet key in 2 seconds to break.')
        foo = raw_input()
        return foo
    except:
        # Timeout
        return


class Blob:
    def __init__(self, location, color):
        self.start = [location[0], location[1]]
        self.size = [1, 1]
        self.color = color

    def add(self, l):
        blob_tolerance = 50

        def helper_close_enough(s, point):
            end = [s.start[0] + s.size[0] - 1, s.start[1] + s.size[1] - 1]  # right down point
            temp_edge = [[s.start[0] - blob_tolerance, s.start[1] - blob_tolerance],
                         [end[0] + blob_tolerance, end[1] + blob_tolerance]]
            if temp_edge[0][0] < point[0] < temp_edge[1][0] and temp_edge[0][1] < point[1] < temp_edge[1][1]:
                return True
            else:
                return False

        if helper_close_enough(self, l):
            self.size[0] = max(l[0] - self.start[0] + 1, self.size[0])
            self.size[1] = max(l[1] - self.start[1] + 1, self.size[1])
        return

    def get_mid(self):
        end = [self.start[0] + self.size[0] - 1, self.start[1] + self.size[1] - 1]  # right down point
        return [(self.start[0] + end[0]) / 2, (self.start[1] + end[1]) / 2]


def color_distance(c1, c2):
    return round(((c1[0] - c2[0]) ** 2 + (c1[1] - c2[1]) ** 2 + (c1[2] - c2[2]) ** 2) ** 0.5, 3)


def if_color_targeted(c, target_set):
    for t in target_set:
        if color_distance(c, t) < 50:
            return True
    return False


def find_blob(p, w, h):
    """
    Blob detection.
    :param p: list of tuples
    :param w:
    :param h:
    :return:
    """
    for t in range(len(p)):
        if if_color_targeted(p[t], target_color_1):
            # TODO: More than one target color set
            if not blobs:
                # TODO: More than one blob
                blobs.add(Blob((t % w, t // w), target_color_1))
            else:
                for b in blobs:
                    b.add((t % w, t // w))
    # return the mid-point of the blob
    if not blobs:
        return
    for b in blobs:
        # TODO: Only one blob now in blobs set
        return b.get_mid()


def location_to_letter(location):
    pass


def analyze(file_path, n):
    if os.path.exists(f_path):
        with open(file_path, 'rb') as img_handle:
            img = PILImage.open(img_handle)
            w0, h0 = img.size  # The original width and height of the image
            img = img.crop(((w0-h0) / 2, 0, (w0+h0) / 2, h0))
            img_data = img.getdata()
            if img.mode.startswith('RGB'):
                pixels = [p for p in img_data]
            # elif img.mode == 'LA':
            #     pixels = [p[0] for p in img_data]
            # elif img.mode == 'L':
            #     pixels = list(img_data)
            else:
                raise ValueError('Unsupported image mode: %r' % img.mode)
            w, h = img.size
            new_file_path = 'crop_' + file_path
            new_file_name = new_f_path.split('/')[-1]
            img.save(new_f_path)
        mid_point = find_blob(pixels, w, h)
        if not mid_point:
            # mid_point is None
            return None, new_file_name, new_file_path
        else:
            return location_to_letter(mid_point), new_file_name, new_file_path
    else:
        print("PATH NOT EXISTS ERROR (analyze)")


def write(a):
    with open('string.txt', 'a') as f:
        f.write(a)


if __name__ == '__main__':
    num = start_num
    nowStop = ''
    blobs = set()
    last_letter = ''

    camera = PiCamera()
    # camera.framerate = Fraction(1, 15)
    camera.shutter_speed = 100000
    camera.exposure_mode = 'off'
    camera.iso = 800

    while not nowStop:
        camera.start_preview()
        sleep(light_sensing)
        f_path = 'goldfish_%s.jpg' % num  # '/home/pi/Desktop/goldfish_%s.jpg'
        f_name = f_path.split('/')[-1]
        # Take a photo
        camera.capture(f_path)
        camera.stop_preview()

        # Analyze the photo
        letter, new_f_name, new_f_path = analyze(f_path, num)
        # Write letter to file
        # TODO: Display on the screen
        if letter and letter != last_letter:
            # Upload file to Google Drive
            # Only write the letter and upload the photo when location changes
            drive.upload(f_name, f_path, 'image/jpeg')
            drive.upload(new_f_name, new_f_path, 'image/jpeg')
            write(letter)
            last_letter = letter

        sleep(photo_freq - light_sensing - wait_gap - 1)  # Take only one photo every minute

        # Remove local photo
        if os.path.exists(f_path):
            os.remove(new_f_path)
            os.remove(f_path)
        else:
            print("PATH NOT EXISTS ERROR (remove)")

        # Now wait for users to press a key, if they want to terminate
        signal.alarm(wait_gap)
        nowStop = input()
        signal.alarm(0)

        num += 1

    print('Finished')
