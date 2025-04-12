from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC


class MainPage:
    def __init__(self, driver):
        self.driver = driver
        self.search_input_locator = (By.ID, "store_nav_search_term")

    def open(self):
        self.driver.get("https://store.steampowered.com")

    def search_game(self, game_name):
        search_input = WebDriverWait(self.driver, 60).until(
            EC.visibility_of_element_located(self.search_input_locator)
        )
        search_input.clear()
        search_input.send_keys(game_name)
        search_input.submit()
