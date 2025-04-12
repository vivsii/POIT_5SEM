from logger import Logger

class Calculator:
    def __init__(self, logger=None):
        self.logger = logger or Logger()

    def add(self, a, b):
        result = a + b
        self.logger.log(f"Adding {a} + {b} = {result}")
        return result

    def subtract(self, a, b):
        result = a - b
        self.logger.log(f"Subtracting {a} - {b} = {result}")
        return result

    def multiply(self, a, b):
        result = a * b
        self.logger.log(f"Multiplying {a} * {b} = {result}")
        return result

    def divide(self, a, b):
        if b == 0:
            raise ValueError("Cannot divide by zero.")
        result = a / b
        self.logger.log(f"Dividing {a} / {b} = {result}")
        return result
