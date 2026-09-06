"""Exercise heap file parsing and storage under address/undefined sanitizers."""
import os
from pathlib import Path
import subprocess
import tempfile
import unittest

SOURCE = Path(__file__).resolve().parents[1] / 'Heap' / 'heap_main.c'


class HeapInputTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.build = tempfile.TemporaryDirectory()
        cls.binary = Path(cls.build.name) / 'heap'
        subprocess.run([os.environ.get('CC', 'cc'), '-std=c11', '-Wall', '-Wextra',
                        '-Werror', '-fsanitize=address,undefined', str(SOURCE),
                        '-o', str(cls.binary)], check=True)

    @classmethod
    def tearDownClass(cls):
        cls.build.cleanup()

    def run_input(self, data):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root / 'in.txt').write_text(data)
            result = subprocess.run([str(self.binary)], cwd=root, capture_output=True,
                                    text=True, timeout=5)
            self.assertNotIn('AddressSanitizer', result.stderr)
            self.assertNotIn('runtime error:', result.stderr)
            output = root / 'out.txt'
            return result, output.read_text() if output.exists() else ''

    def test_more_than_100_customers(self):
        result, output = self.run_input('150 1\nSam\n' + ''.join(
            f'0 C{i:03d} Sam 0 10\n' for i in range(150)))
        self.assertEqual(result.returncode, 0, result.stderr)
        rows = [line.split() for line in output.splitlines()]
        self.assertEqual(len(rows), 150)
        self.assertEqual({row[0] for row in rows}, {f'C{i:03d}' for i in range(150)})
        self.assertEqual([int(row[1]) for row in rows], list(range(10, 1501, 10)))
        self.assertTrue(all(row[2:] == ['1', 'Sam'] for row in rows))

    def test_empty_heaps(self):
        result, output = self.run_input('0 10\n' + ' '.join(f'S{i}' for i in range(10)))
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(output, '')

    def test_invalid_headers(self):
        for header in ['', '1', '-1 1', '0 0', '0 11', '2147483648 1', '1x 1']:
            with self.subTest(header=header):
                result, output = self.run_input(header)
                self.assertNotEqual(result.returncode, 0)
                self.assertEqual(output, '')

    def test_overlong_and_incomplete_records(self):
        for data in ['1 1\n' + 'S' * 20, '1 1\nSam\n0 ' + 'C' * 20 + ' Sam 0 1',
                     '1 1\nSam\n0 Alice Sam 0', '1 1\nSam\n0 Alice Sam 0 -1']:
            with self.subTest(data=data):
                result, output = self.run_input(data)
                self.assertNotEqual(result.returncode, 0)
                self.assertEqual(output, '')

    def test_maximum_name_length(self):
        name = 'A' * 19
        result, output = self.run_input(f'1 1\n{name}\n0 {name} {name} 0 10')
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(output, f'{name} 10 1 {name}\n')

    def test_result_overflow(self):
        for record in ['2147483647 Alice Sam 0 1', '0 Alice Sam 2147483647 10']:
            result, output = self.run_input('1 1\nSam\n' + record)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn('exceeds integer range', result.stderr)
            self.assertEqual(output, '')


if __name__ == '__main__':
    unittest.main()
