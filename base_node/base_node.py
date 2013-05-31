#!/usr/bin/env python

import os
import logging
import xively
import time
import datetime
import requests
import random

import serial
from threading import Thread, RLock


# extract feed_id and api_key from environment variables
FEED_ID = 1526802309
API_KEY = 'tUqYMiYfWsU1bAve2wQp61HscIQFcvg3tYmTnXJtU8bzFIcx'

# initialize api client
api = xively.XivelyAPIClient(API_KEY)

logging.basicConfig(level=logging.DEBUG,
                    format='(%(threadName)-10s) %(message)s', )

# function to return a datastream object. This either creates a new datastream,
# or returns an existing one


def get_datastream(feed, id, tags='', max_v=None, min_v=None):
    try:
        datastream = feed.datastreams.get(id)
        logging.debug('Found existing datastream')
        datastream.max_value = max_v
        datastream.min_value = min_v
        return datastream
    except:
        logging.debug('Creating new datastream')
        datastream = feed.datastreams.create(id, tags=tags)
        return datastream


# main program entry point - runs continuously updating our datastream with the
# current 1 minute load average


def update_datastream(datastream, value):
    datastream.current_value = value
    datastream.at = datetime.datetime.utcnow()
    try:
        datastream.update()
    except requests.HTTPError as e:
        print "HTTPError({0}): {1}".format(e.errno, e.strerror)


class serial_thread(Thread):

    def __init__(self,  lock):
        Thread.__init__(self)
        self.on = True
        self.lock = lock
        self.s = serial.Serial('/dev/ttyUSB0', 9600)
        self.buffer = []

    def run(self):
        logging.debug('Serial Thread Started')
        while self.on:

            d = ord(self.s.read())
            frame = []
            if d == 0x7E:
                for i in range(5):
                    frame.append(ord(self.s.read()))
            if frame[-1] == 0xE7:
                with self.lock:
                    self.buffer.insert(0, frame[:-1])
                    logging.debug('{0} Recived'.format(str(frame[:-1])))
            else:
                logging.debug('Frame Droped')


def main():
    print 'PID ', os.getpid()
    feed = api.feeds.get(FEED_ID)

    room1_light = get_datastream(feed, 'room1_light')
    room1_temp = get_datastream(feed, 'room1_temperature')
    room2_light = get_datastream(feed, 'room2_light')

    data_streams = {
        0x33: {0x13: room1_light, 0x31: room1_temp},
        0x34: {0x13: room2_light},
    }

    lock = RLock()
    st = serial_thread(lock)
    st.start()

    while True:

        if st.buffer:
            f = []
            with lock:
                f = st.buffer.pop()

            stream = data_streams[f[0]][f[1]]
            update_datastream(stream, (f[2] * 255 + f[3]))


    # while True:
    # load_avg = read_loadavg()
    #     v = random.uniform(0, 100)
    #     logging.debug('Updating feed {0} with {1}'.format('Random', v))

    #     update_datastream(datastream, v)

    #     time.sleep(3)


if __name__ == '__main__':
    main()
    raw_input()
    raw_input()
    os._exit(1)
