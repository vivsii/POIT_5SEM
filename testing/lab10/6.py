import pytest
from selenium import webdriver
from selenium.webdriver.edge.service import Service
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time

# Функция для настройки и запуска драйвера
def create_driver():
    edge_service = Service("C:/driver/msedgedriver.exe")
    driver = webdriver.Edge(service=edge_service)
    return driver

# Параметризация теста для различных игр
@pytest.mark.parametrize("game_name", ["ELDEN RING", "Одни из нас", "Counter-Strike 2"])
def test_cart_functionality(game_name):
    driver = create_driver()
    wait = WebDriverWait(driver, 60)

    try:
        print(f"Ищем игру '{game_name}'...")

        driver.get("https://store.steampowered.com")
        time.sleep(2)

        # Находим поле для ввода запроса поиска
        search_input = wait.until(
            EC.visibility_of_element_located((By.ID, "store_nav_search_term"))
        )
        search_input.clear()
        search_input.send_keys(game_name)
        search_input.submit()

        print("Ожидание загрузки результатов поиска...")
        wait.until(EC.visibility_of_element_located((By.XPATH, "//div[@class='search_results']")))

        print("Переходим на первую игру из результатов поиска...")

        # Находим первую ссылку игры в результатах поиска
        first_game = wait.until(
            EC.element_to_be_clickable((By.XPATH, "//div[@class='responsive_search_name_combined']"))
        )

        # Переход по ссылке первой игры
        first_game.click()

        try:
            print("Ожидание выпадающего списка для выбора года рождения...")
            birth_year_dropdown = wait.until(
                EC.visibility_of_element_located((By.XPATH, "//select[@id='ageYear']"))
            )
            birth_year_dropdown.click()
            time.sleep(1)
            year_option = wait.until(
                EC.element_to_be_clickable((By.XPATH, "//select[@id='ageYear']/option[@value='2005']"))
            )
            year_option.click()
            print("Год рождения выбран.")
            open_page = wait.until(
                EC.element_to_be_clickable(
                    (By.ID, "view_product_page_btn"))
            )
            open_page.click()

            print("Добавляем игру в корзину...")
            add_to_cart_button = wait.until(
                EC.element_to_be_clickable(
                    (By.ID, "btn_add_to_cart_440408"))
            )
            add_to_cart_button.click()

        except Exception as e:
            print("Ошибка при выборе года рождения:", e)
            return

        print(f"Тест прошел успешно для игры: {game_name}. Игра добавлена в корзину.")

    except Exception as e:
        print("Произошла ошибка:", e)
    finally:
        time.sleep(15)
        driver.quit()
