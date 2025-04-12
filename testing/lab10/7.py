import pytest
import logging
from selenium import webdriver
from selenium.webdriver.edge.service import Service
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time

logging.basicConfig(filename="test_log.log", level=logging.INFO)
logger = logging.getLogger()

@pytest.mark.smoke
@pytest.mark.cart
@pytest.mark.parametrize("game_name", ["ELDEN RING"])
def test_cart_functionality(game_name):
    edge_service = Service("C:/driver/msedgedriver.exe")
    driver = webdriver.Edge(service=edge_service)

    wait = WebDriverWait(driver, 60)
    driver.get("https://store.steampowered.com")
    time.sleep(2)

    try:
        logger.info(f"Ищем игру '{game_name}'...")

        search_input = wait.until(
            EC.visibility_of_element_located((By.ID, "store_nav_search_term"))
        )
        search_input.clear()
        search_input.send_keys(game_name)
        search_input.submit()

        logger.info("Ожидание загрузки результатов поиска...")
        wait.until(EC.visibility_of_element_located((By.XPATH, "//div[@class='search_results']")))

        logger.info("Переходим на первую игру из результатов поиска...")

        first_game = wait.until(
            EC.element_to_be_clickable((By.XPATH, "//div[@class='responsive_search_name_combined']"))
        )

        first_game.click()

        try:
            logger.info("Ожидание выпадающего списка для выбора года рождения...")
            birth_year_dropdown = wait.until(
                EC.visibility_of_element_located((By.XPATH, "//select[@id='ageYear']"))
            )
            birth_year_dropdown.click()
            time.sleep(1)
            year_option = wait.until(
                EC.element_to_be_clickable((By.XPATH, "//select[@id='ageYear']/option[@value='2005']"))
            )
            year_option.click()
            logger.info("Год рождения выбран.")
            open_page = wait.until(
                EC.element_to_be_clickable(
                    (By.ID, "view_product_page_btn"))
            )
            open_page.click()

            logger.info("Добавляем игру в корзину...")
            add_to_cart_button = wait.until(
                EC.element_to_be_clickable(
                    (By.ID, "btn_add_to_cart_440408"))
            )
            add_to_cart_button.click()

        except Exception as e:
            logger.error("Ошибка при выборе года рождения:", e)
            return

        logger.info("Тест прошел успешно: Игра добавлена в корзину.")

    except Exception as e:
        logger.error("Произошла ошибка:", e)
    finally:
        time.sleep(15)
        driver.quit()


@pytest.mark.smoke
@pytest.mark.search
def test_search_functionality():
    game_name = "Одни из нас"
    test_cart_functionality(game_name)


@pytest.mark.skip(reason="Тест пока не актуален.")
def test_some_other_functionality():
    logger.info("Этот тест не будет выполняться в данный момент.")


@pytest.mark.xfail(reason="Ожидаемое падение теста")
def test_failing_functionality():
    assert False, "Тест должен упасть, так как это ожидаемое поведение."


game_name = "ELDEN RING"
test_cart_functionality(game_name)
