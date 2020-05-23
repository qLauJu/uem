import audio.signalwave as signalwave
import numpy as np
from morse import SPACE_BETWEEN_WORDS, SPACE_BETWEEN_LETTERS, MORSE_TO_TEXT_TABLE, SUPPORTED_CHARS, TEXT_TO_MORSE_TABLE
from audio.constants import FREQUENCY, NUMBER_OF_SAMPLES, SAMPLING_RATE, TIME_UNIT
from functools import reduce

#####################
### MORSE -> TEXT ###
#####################


def parse_morse_to_text(morse):
    words = morse.split(SPACE_BETWEEN_WORDS)
    words_in_text = map(transform_word_to_text, words)
    return ' '.join(words_in_text)


def transform_word_to_text(word):
    letters = word.split(SPACE_BETWEEN_LETTERS)
    return ''.join(map(transform_letter_to_text, letters))


def transform_letter_to_text(letter):
    return MORSE_TO_TEXT_TABLE[letter]


#####################
### TEXT -> MORSE ###
#####################

def parse_text_to_morse(text):
    words = text.upper().split(' ')
    clean_words = map(filter_only_supported_chars, words)
    words_in_morse = map(transform_word_to_morse, clean_words)
    return SPACE_BETWEEN_WORDS.join(words_in_morse)


def filter_only_supported_chars(word):
    return filter(lambda c: c in SUPPORTED_CHARS, word)


def transform_word_to_morse(word):
    return SPACE_BETWEEN_LETTERS.join(map(transform_letter_to_morse, word))


def transform_letter_to_morse(letter):
    return TEXT_TO_MORSE_TABLE[letter]

######################
### MORSE -> AUDIO ###
######################


def parse_morse_to_audio(morse):
    frequencies = map(lambda c: FREQUENCY if c == '1' else 0, morse)
    audio = reduce(
        lambda waves, wave: waves + wave,
        map(
            lambda f: signalwave.signalwave(
                f, NUMBER_OF_SAMPLES, SAMPLING_RATE, TIME_UNIT),
            frequencies),
        [])

    return np.array(audio)

######################
### AUDIO -> MORSE ###
######################


def parse_audio_to_morse(audio):
    morse_len = len(audio) / NUMBER_OF_SAMPLES
    samples = np.array_split(audio, morse_len)
    return ''.join(map(lambda s: '0' if s.max() == 0 else '1', samples))


#####################
### TEXT -> AUDIO ###
#####################

def parse_text_to_audio(text):
    morse = parse_text_to_morse(text)
    return parse_morse_to_audio(morse)


#####################
### AUDIO -> TEXT ###
#####################

def parse_audio_to_text(audio):
    morse = parse_audio_to_morse(audio)
    return parse_morse_to_text(morse)
