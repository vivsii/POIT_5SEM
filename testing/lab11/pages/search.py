from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC


class SearchResultsPage:
    def __init__(self, driver):
        self.driver = driver
        self.first_game_locator = (By.XPATH, "//div[@class='responsive_search_name_combined']")

    def select_first_game(self):
        first_game = WebDriverWait(self.driver, 60).until(
            EC.element_to_be_clickable(self.first_game_locator)
        )
        first_game.click()
