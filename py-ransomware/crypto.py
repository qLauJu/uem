from cryptography.fernet import Fernet
import os


def encrypt(file_path, encryptor):
    output_file = file_path + '.enc'

    with open(file_path, 'rb') as f:
        data = f.read()

    try:
        encrypted = encryptor.encrypt(data)
    except:
        print('Failed to encrypt file: {}'.format(file_path))
        return

    with open(output_file, 'wb') as f:
        f.write(encrypted)

    os.remove(file_path)


def decrypt(file_path, decryptor):
    output_file = os.path.splitext(file_path)[0]

    with open(file_path, 'rb') as f:
        data = f.read()

    try:
        decrypted = decryptor.decrypt(data)
    except:
        print('Failed to decrypt file: {}'.format(file_path))
        return

    with open(output_file, 'wb') as f:
        f.write(decrypted)

    os.remove(file_path)
