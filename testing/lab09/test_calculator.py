import unittest
from calculator import Calculator

class MockLogger:
    def __init__(self):
        self.messages = []

    def log(self, message):
        #Заглушка
        self.messages.append(message)

class TestCalculator(unittest.TestCase):

    def setUp(self):
        #заглушку для Logger
        self.mock_logger = MockLogger()
        self.calculator = Calculator(logger=self.mock_logger)

    def test_add(self):
        result = self.calculator.add(1, 2)
        self.assertEqual(result, 3)
        self.assertIn("Adding 1 + 2 = 3", self.mock_logger.messages)

    def test_subtract(self):
        result = self.calculator.subtract(5, 3)
        self.assertEqual(result, 2)
        self.assertIn("Subtracting 5 - 3 = 2", self.mock_logger.messages)

    def test_multiply(self):
        result = self.calculator.multiply(3, 4)
        self.assertEqual(result, 12)
        self.assertIn("Multiplying 3 * 4 = 12", self.mock_logger.messages)

    def test_divide(self):
        result = self.calculator.divide(10, 2)
        self.assertEqual(result, 5)
        self.assertIn("Dividing 10 / 2 = 5.0", self.mock_logger.messages)

    def test_divide_by_zero(self):
        with self.assertRaises(ValueError) as context:
            self.calculator.divide(10, 0)
        self.assertEqual(str(context.exception), "Cannot divide by zero.")

if __name__ == '__main__':
    unittest.main()
