from selenium import webdriver
from selenium.webdriver.edge.service import Service
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time
import json

edge_service = Service("C:/driver/msedgedriver.exe")
driver = webdriver.Edge(service=edge_service)

driver.get("https://store.steampowered.com/login/")

wait = WebDriverWait(driver, 80)


def test_login(username, password):
    try:
        username_input = wait.until(
            EC.presence_of_element_located((By.XPATH, "//input[@class='_2GBWeup5cttgbTw8FM3tfx' and @type='text']")))
        username_input.clear()
        username_input.send_keys(username)

        password_input = wait.until(EC.presence_of_element_located(
            (By.XPATH, "//input[@class='_2GBWeup5cttgbTw8FM3tfx' and @type='password']")))
        password_input.clear()
        password_input.send_keys(password)

        login_button = wait.until(EC.element_to_be_clickable((By.CLASS_NAME, "DjSvCZoKKfoNSmarsEcTS")))
        login_button.click()
        time.sleep(30)

        wait.until(EC.url_contains("store.steampowered.com"))
        print("Авторизация прошла успешно!")

        cookies = driver.get_cookies()
        print("Куки после авторизации:")
        print(json.dumps(cookies, indent=4))
        with open("cookies.json", "w") as file:
            json.dump(cookies, file)

    except Exception as e:
        print("Произошла ошибка при авторизации:", e)
    finally:
        time.sleep(5)
        driver.quit()


username = "11"
password = "11"
test_login(username, password)
