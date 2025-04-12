from selenium import webdriver
from selenium.webdriver.edge.service import Service
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time

edge_service = Service("C:/driver/msedgedriver.exe")
driver = webdriver.Edge(service=edge_service)

wait = WebDriverWait(driver, 60)

driver.get("https://store.steampowered.com")
time.sleep(2)

def test_cart_functionality(game_name):
    try:
        print(f"Ищем игру '{game_name}'...")
        search_input = wait.until(
            EC.visibility_of_element_located((By.ID, "store_nav_search_term"))
        )
        search_input.clear()
        search_input.send_keys(game_name)
        search_input.submit()

        print("Ожидание загрузки результатов поиска...")
        wait.until(EC.visibility_of_element_located((By.XPATH, "//div[@class='search_results']")))

        print("Переходим на первую игру из результатов поиска...")
        first_game = wait.until(
            EC.element_to_be_clickable((By.XPATH, "//a[@data-ds-appid='1245620']"))
        )
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

            print("Переходим в корзину...")
            cart_button = wait.until(
                EC.element_to_be_clickable(
                    (By.XPATH, "//button[@class='DialogButton _DialogLayout Primary Focusable']"))
            )
            cart_button.click()

            # Проверка, что игра появилась в корзине
            print("Проверка, что игра добавлена в корзину...")
            cart_items = wait.until(
                EC.presence_of_all_elements_located((By.XPATH, f"//div[@class='EflKs0JjldhDSxbUBaiOp' and contains(text(), '{game_name}')]"))
            )

            assert len(cart_items) > 0, "Игра не найдена в корзине."
            print("Тест прошел успешно: Игра добавлена в корзину.")

        except Exception as e:
            print("Ошибка при выборе года рождения или добавлении в корзину:", e)
            return

    except Exception as e:
        print("Произошла ошибка:", e)
    finally:
        time.sleep(5)
        driver.quit()

game_name = "ELDEN RING"
test_cart_functionality(game_name)
