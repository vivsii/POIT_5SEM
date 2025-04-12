import time
from selenium import webdriver
from selenium.webdriver.common.by import By

driver = webdriver.Chrome()

# Открытие сайта
driver.get("https://goldapple.by")
driver.implicitly_wait(10)
#1 CSS
try:
    button = driver.find_element(By.CSS_SELECTOR, 'main.yePjw div.kPAjx')
    print("Найден элемент по CSS-селектору 'main.yePjw div.kPAjx':", button.text)
except:
    print("Элемент по CSS-селектору 'main.yePjw div.kPAjx' не найден")
#2
try:
    city = driver.find_element(By.CSS_SELECTOR, 'div.GIR6f span._0wJZ2')
    print("Найден элемент по CSS-селектору 'div.GIR6f span._0wJZ2':", city.text)
except:
    print("Элемент по CSS-селектору 'div.GIR6f span._0wJZ2' не найден")
#3
try:
    us = driver.find_element(By.CSS_SELECTOR, 'div.RO9gi div.Vohfc')
    print("Найден элемент по CSS-селектору 'div.jkQ8K div.cIfzJ':", us.text)
except:
    print("Элемент по CSS-селектору 'div.jkQ8K div.cIfzJ' не найден")

#1 XPath
try:
    button_xpath1 = driver.find_element(By.XPATH, "//div[@class='ga-ribbon__marquee']//div[@class='ga-ribbon__marquee-text']")
    print("Найден элемент по XPath '//div[@class='ga-ribbon__marquee']//div[@class='ga-ribbon__marquee-text']':", button_xpath1.text)
except:
    print("Элемент по XPath для кнопки с классами '//div[@class='tJcjV']//div[@class='_6ipOx' не найден")

#2
try:
    button_xpath = driver.find_element(By.XPATH, "//div[@class='Dp-mm']//div[@class='RO9gi']//h3[@class='aS6PU']")
    print("Найден элемент по XPath '//div[@class='jkQ8K']//div[@class='cIfzJ']':", button_xpath.text)
except:
    print("Элемент по XPath для кнопки с классами '//div[@class='jkQ8K']//div[@class='cIfzJ']")

#3
try:
    button_xpath = driver.find_element(By.XPATH, "//div[@class='GIR6f']//span[@class='_0wJZ2']")
    print("Найден элемент по XPath '//div[@class='GIR6f']//span[@class='_0wJZ2']':", button_xpath.text)
except:
    print("Элемент по XPath для кнопки с классами '//div[@class='GIR6f']//span[@class='_0wJZ2']' не найден")

#тэг
try:
    element_by_tag = driver.find_element(By.TAG_NAME, "h2")
    print("Найден элемент по тегу 'h1':", element_by_tag.text)
except:
    print("Элемент по тегу 'span' не найден")

# частичный текст
try:
    element_by_partial_link = driver.find_element(By.PARTIAL_LINK_TEXT, "политикой")
    print("Найден элемент по частичному тексту ссылки 'политикой':", element_by_partial_link.get_attribute("href"))
except:
    print("Элемент по частичному тексту ссылки 'политикой' не найден")
driver.quit()
