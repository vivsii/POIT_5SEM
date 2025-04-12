from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC


class GamePage:
    def __init__(self, driver):
        self.driver = driver
        self.birth_year_dropdown_locator = (By.XPATH, "//select[@id='ageYear']")
        self.year_option_locator = (By.XPATH, "//select[@id='ageYear']/option[@value='2005']")
        self.open_page_button_locator = (By.ID, "view_product_page_btn")
        self.add_to_cart_button_locator = (By.ID, "btn_add_to_cart_440408")

    def select_birth_year(self, year="2005"):
        dropdown = WebDriverWait(self.driver, 60).until(
            EC.visibility_of_element_located(self.birth_year_dropdown_locator)
        )
        dropdown.click()
        option = WebDriverWait(self.driver, 10).until(
            EC.element_to_be_clickable(
                (By.XPATH, f"//select[@id='ageYear']/option[@value='{year}']")
            )
        )
        option.click()

    def open_product_page(self):
        open_page_button = WebDriverWait(self.driver, 60).until(
            EC.element_to_be_clickable(self.open_page_button_locator)
        )
        open_page_button.click()

    def add_to_cart(self):
        add_to_cart_button = WebDriverWait(self.driver, 60).until(
            EC.element_to_be_clickable(self.add_to_cart_button_locator)
        )
        add_to_cart_button.click()
