# -*- coding: utf-8 -*-

from enum import Enum
import numpy as np
import audio.signalwave as signalwave
from audio.constants import AMPLITUDE, SAMPLING_RATE


class FileType(Enum):
    TEXT = "txt"
    MORSE = "morse"
    AUDIO = "wav"


def read_file(file_path):
    file_type = FileType(file_path.split('.')[-1])

    if file_type == FileType.AUDIO:
        return (signalwave.load_wave(file_path), file_type)

    with open(file_path) as f:
        content = f.read()

    return (content, file_type)


def dump(text, audio, morse, file_path):
    if text is not None:
        dump_text(text, file_path)

    if audio is not None:
        dump_audio(audio, file_path)

    if morse is not None:
        dump_morse(morse, file_path)


def dump_text(text, file_path_no_extension):
    output_path = '{}.{}'.format(file_path_no_extension, FileType.TEXT.value)
    with open(output_path, 'w') as f:
        f.write(text)


def dump_morse(morse, file_path_no_extension):
    output_path = '{}.{}'.format(file_path_no_extension, FileType.MORSE.value)
    with open(output_path, 'w') as f:
        f.write(morse)


def dump_audio(audio, file_path_no_extension):
    signalwave.save_wave(audio, '{}.wav'.format(
        file_path_no_extension), amplitude=AMPLITUDE, sampling_rate=SAMPLING_RATE)
