# -*- coding: utf-8 -*-

import random
import wave
import struct

import matplotlib.pyplot as plt
import numpy as np


def load_wave(file_path, stereo=False):
    with wave.open(file_path, 'r') as wave_file:
        len_ = wave_file.getnframes()
        data = wave_file.readframes(len_)
        if stereo:
            data = struct.unpack('{n}h'.format(n=len_*2), data)
        else:
            data = struct.unpack('{n}h'.format(n=len_), data)

    return np.array(data)


def save_wave(wave_, file_path, amplitude, sampling_rate, comtype='NONE', compname='not compressed', sampwidth=2, num_channels=1):
    n_frames = len(wave_)
    with wave.open(file_path, 'w') as wave_file:
        wave_file.setparams(
            (num_channels, sampwidth, sampling_rate, n_frames, comtype, compname))
        for signal in wave_:
            value = int(signal) if amplitude == 0 else int(signal * amplitude)
            wave_file.writeframes(struct.pack('h', value))


def signalwave(frequency, num_samples, sampling_rate, time, noisy=False):

    def noise():
        return random.uniform(0, 4) if noisy else 0

    if time is not None:
        num_samples = sampling_rate * time

    return [
        np.sin(2 * np.pi * frequency * t / sampling_rate + noise())
        for t in range(int(num_samples))]
