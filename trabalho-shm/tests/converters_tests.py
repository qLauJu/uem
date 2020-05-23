import sys
sys.path.append('.')

from file_helpers import read_file
from converters import parse_morse_to_text, parse_text_to_morse
from numpy.testing import assert_allclose
import unittest


class TestConverters(unittest.TestCase):

    def test_morse_to_text_parser(self):
        TEST_INPUT, _ = read_file('./tests/samples/test_case1.morse')
        EXPECTED_RETURN, _ = read_file('./tests/samples/test_case1.txt')

        self.assertEqual(parse_morse_to_text(
            TEST_INPUT), EXPECTED_RETURN)

    def test_text_to_morse_parser(self):
        TEST_INPUT, _ = read_file('./tests/samples/test_case2.txt')
        EXPECTED_RETURN, _ = read_file('./tests/samples/test_case2.morse')

        self.assertEqual(parse_text_to_morse(TEST_INPUT), EXPECTED_RETURN)


if __name__ == '__main__':
    unittest.main()
