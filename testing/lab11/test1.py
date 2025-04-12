from selenium import webdriver
from selenium.webdriver.edge.service import Service
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time

class BasePage:
    def __init__(self, driver):
        self.driver = driver
        self.wait = WebDriverWait(driver, 80)

class LoginPage(BasePage):
    USERNAME_INPUT = (By.XPATH, "//input[@class='_2GBWeup5cttgbTw8FM3tfx' and @type='text']")
    PASSWORD_INPUT = (By.XPATH, "//input[@class='_2GBWeup5cttgbTw8FM3tfx' and @type='password']")
    LOGIN_BUTTON = (By.CLASS_NAME, "DjSvCZoKKfoNSmarsEcTS")

    def enter_username(self, username):
        username_input = self.wait.until(EC.presence_of_element_located(self.USERNAME_INPUT))
        username_input.clear()
        username_input.send_keys(username)

    def enter_password(self, password):
        password_input = self.wait.until(EC.presence_of_element_located(self.PASSWORD_INPUT))
        password_input.clear()
        password_input.send_keys(password)

    def click_login_button(self):
        login_button = self.wait.until(EC.element_to_be_clickable(self.LOGIN_BUTTON))
        login_button.click()

class HomePage(BasePage):

    def is_loaded(self):
        return self.wait.until(EC.url_contains("store.steampowered.com"))

class TestSteamLogin:
    def setup(self):
        edge_service = Service("C:/driver/msedgedriver.exe")
        self.driver = webdriver.Edge(service=edge_service)
        self.driver.get("https://store.steampowered.com/login/")

    def teardown(self):
        time.sleep(5)
        self.driver.quit()

    def test_login(self, username, password):
        try:
            login_page = LoginPage(self.driver)
            login_page.enter_username(username)
            login_page.enter_password(password)
            login_page.click_login_button()

            home_page = HomePage(self.driver)
            if home_page.is_loaded():
                print("Авторизация прошла успешно!")
            else:
                print("Не удалось перейти на главную страницу.")
        except Exception as e:
            print("Произошла ошибка при авторизации:", e)

if __name__ == "__main__":
    username = "1"
    password = "1"

    test = TestSteamLogin()
    test.setup()
    try:
        test.test_login(username, password)
    finally:
        test.teardown()
