import os
from pathlib import Path
import subprocess
import tempfile
import unittest


class InputTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.build = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.build.cleanup)
        cls.program = str(Path(cls.build.name) / 'bst')
        source = Path(__file__).resolve().parents[1] / 'BST' / 'bst_main.c'
        subprocess.run([os.environ.get('CC', 'cc'), '-std=c11', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=address,undefined', str(source), '-o', cls.program], check=True)

    def run_input(self, data):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory)
            (path / 'in1.txt').write_text(data)
            run = subprocess.run([self.program], cwd=path, capture_output=True, text=True,
                                 timeout=3, check=True)
            return run.stderr, (path / 'out1.txt').read_text()

    def test_duplicate_frequency_and_lookup(self):
        error, output = self.run_input('3\n1 kiwi\n1 kiwi\n2 kiwi\n')
        self.assertEqual(error, '')
        self.assertEqual(output, '2 0\nkiwi 2\n')

    def test_long_word_reports_error_without_splitting_into_commands(self):
        error, output = self.run_input('2\n1 ' + 'x' * 100 + '\n1 pear\n')
        self.assertIn('exceeds 19 characters', error)
        self.assertEqual(output, '')

    def test_incomplete_command(self):
        error, output = self.run_input('1\n1\n')
        self.assertIn('Incomplete command', error)
        self.assertEqual(output, '')

    def test_empty_input(self):
        self.assertEqual(self.run_input(''), ('', ''))


if __name__ == '__main__':
    unittest.main()
