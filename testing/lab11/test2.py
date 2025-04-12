import pytest
from selenium import webdriver
from selenium.webdriver.edge.service import Service
from pages.main import MainPage
from pages.search import SearchResultsPage
from pages.game import GamePage
import time


def create_driver():
    edge_service = Service("C:/driver/msedgedriver.exe")
    driver = webdriver.Edge(service=edge_service)
    return driver


@pytest.mark.parametrize("game_name", ["ELDEN RING", "Одни из нас", "Counter-Strike 2"])
def test_cart_functionality(game_name):
    driver = create_driver()

    try:
        main_page = MainPage(driver)
        search_results_page = SearchResultsPage(driver)
        game_page = GamePage(driver)

        print(f"Тестируем игру: {game_name}")
        main_page.open()
        main_page.search_game(game_name)

        print("Переход к первой игре из результатов поиска...")
        search_results_page.select_first_game()

        print("Заполнение данных и добавление игры в корзину...")
        try:
            game_page.select_birth_year()
            game_page.open_product_page()
        except Exception as e:
            print("Не требуется ввод даты рождения:", e)

        game_page.add_to_cart()
        print(f"Тест прошел успешно для игры: {game_name}. Игра добавлена в корзину.")

    except Exception as e:
        print("Произошла ошибка:", e)
    finally:
        time.sleep(15)
        driver.quit()
