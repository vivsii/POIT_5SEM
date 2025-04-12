// Импортируем модуль express, который упрощает создание серверов на Node.js.
const express = require('express');

// Создаем экземпляр приложения express.
const app = express();

// Подключаем middleware express.json(), который парсит входящие запросы с JSON-форматом.
// Это позволяет серверу автоматически обрабатывать и извлекать данные из тела запросов в формате JSON.
app.use(express.json());

// Определяем обработчик GET-запроса для корневого маршрута ("/").
// Когда клиент обращается к корневому URL, будет отправлен HTML файл.
app.get("/", function(request, response) {
    // Отправляем файл "WEB5A.html", который должен находиться в текущем каталоге.
    response.sendFile(__dirname + "/WEB5A.html");
});

// Определяем обработчик POST-запроса для маршрута "/sum".
// Этот маршрут будет использоваться для получения данных и вычисления суммы.
app.post("/sum", function(request, response) {
    // Извлекаем значения "x-value-x" и "x-value-y" из заголовков запроса.
    // Преобразуем их в целые числа с помощью parseInt.
    const x = parseInt(request.headers["x-value-x"]);
    const y = parseInt(request.headers["x-value-y"]);

    // Вычисляем сумму x и y.
    const z = x + y;

    // Устанавливаем заголовок "X-Value-z" в ответе, чтобы передать результат вычисления.
    // Используем z.toString() для преобразования числа в строку.
    response.setHeader("X-Value-z", z.toString());

    // Отправляем текстовое сообщение клиенту с результатом суммы.
    response.send("результат: " + z);
});

// Запускаем сервер на заданном порту.
// В функции обратного вызова выводим сообщение в консоль о том, что сервер запущен.
app.listen(port, function(request, response) {
    console.log(`running`);
});


<!DOCTYPE html>
<!-- Объявляем тип документа, чтобы браузер знал, что это HTML5 -->
<html lang="en">
<!-- Начинаем HTML-документ и указываем язык страницы (английский) -->
<head>
    <!-- Заголовок документа -->
    <meta charset="UTF-8">
    <!-- Устанавливаем кодировку документа на UTF-8 для поддержки всех символов -->
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <!-- Настройки для мобильных устройств: масштабирование и ширина в соответствии с шириной устройства -->
    <title>Task A</title>
    <!-- Заголовок страницы, отображаемый на вкладке браузера -->
</head>
<body>
    <!-- Основное содержимое страницы -->
    <h3>Введите значения X и Y</h3>
    <!-- Заголовок, информирующий пользователя о необходимости ввода значений -->

    <form id="sumf">
        <!-- Форма для ввода значений X и Y. ID используется для доступа к форме в JavaScript -->
        
        <label for="valueX">значение X: <span id="xOutput">0</span></label>
        <!-- Метка для первого ползунка, отображает текущее значение X в элементе <span> -->
        
        <input type="range" id="valueX" name="valueX" min="0" max="100" value="0" 
            oninput="document.getElementById('xOutput').innerText = this.value">
        <!-- Ползунок для ввода значения X. 
             Параметры:
             - min: минимальное значение 0
             - max: максимальное значение 100
             - value: начальное значение 0
             - oninput: обработчик события, который обновляет текст в <span> с текущим значением ползунка -->
        <br/>
        <!-- Перенос на новую строку -->

        <label for="valueY">значение Y: <span id="yOutput">0</span></label>
        <!-- Метка для второго ползунка, отображает текущее значение Y в элементе <span> -->
        
        <input type="range" id="valueY" name="valueY" min="0" max="100" value="0" 
            oninput="document.getElementById('yOutput').innerText = this.value">
        <!-- Ползунок для ввода значения Y, аналогичный ползунку X -->
        <br/>
        <!-- Перенос на новую строку -->

        <button type="submit">сумма</button>
        <!-- Кнопка для отправки формы. При нажатии происходит вычисление суммы значений X и Y -->
    </form>
    
    <h3>результат:</h3>
    <!-- Заголовок для секции, где будет отображаться результат суммы -->

    <div id="result"></div>
    <!-- Элемент <div>, в который будет помещен текст с результатом суммы после обработки запроса -->

    <script>
        // Начинаем скрипт на JavaScript для обработки формы
        const form = document.getElementById('sumf');
        // Получаем элемент формы по ID для дальнейшего использования
        const resA = document.getElementById('result');
        // Получаем элемент для отображения результата по ID

        // Добавляем обработчик события на отправку формы
        form.addEventListener('submit', async(e) => {
            e.preventDefault(); // Предотвращаем стандартное поведение формы (перезагрузку страницы)

            // Извлекаем значения X и Y из ползунков
            const x = document.getElementById('valueX').value;
            const y = document.getElementById('valueY').value;

            // Создаем новый объект XMLHttpRequest для отправки HTTP-запроса
            const xhrA = new XMLHttpRequest();
            xhrA.open('POST', '/sum', false); // Открываем POST-запрос к маршруту /sum
            xhrA.setRequestHeader('Content-Type', 'text/plain'); // Устанавливаем заголовок типа содержимого
            xhrA.setRequestHeader('X-value-x', x); // Устанавливаем заголовок с значением X
            xhrA.setRequestHeader('X-value-y', y); // Устанавливаем заголовок с значением Y

            // Обработчик события onload для XMLHttpRequest
            xhrA.onload = function() {
                // Проверяем статус ответа
                if(xhrA.status === 200) {
                    // Если запрос успешен, отображаем результат в элементе <div>
                    resA.textContent = xhrA.responseText;
                }
            };

            // Отправляем запрос
            xhrA.send();
        });

    </script>
</body>
</html>



// Импортируем модуль express, который позволяет легко создавать серверы на Node.js.
const express = require("express");

// Создаем экземпляр приложения express.
const app = express();

// Определяем порт, на котором будет слушать сервер. В данном случае это 3000.
const port = 3000;

// Подключаем middleware express.json(), который позволяет серверу обрабатывать входящие запросы с JSON-форматом.
// Это позволяет автоматически парсить тело запроса в формате JSON.
app.use(express.json());

// Определяем обработчик GET-запроса для корневого маршрута ("/").
// Когда клиент обращается к корневому URL, будет отправлен HTML файл.
app.get("/", (req, res) => {
  // Отправляем файл "WEB5B.html", который должен находиться в текущем каталоге.
  res.sendFile(__dirname + "/WEB5B.html");
});

// Определяем обработчик POST-запроса для маршрута "/generate".
// Этот маршрут будет использоваться для генерации массива случайных чисел.
app.post("/generate", (req, res) => {
  // Извлекаем значение x-rand-n из заголовков запроса и преобразуем его в целое число.
  const n = parseInt(req.headers["x-rand-n"]);

  // Устанавливаем минимальное и максимальное значение для количества случайных чисел.
  const min = 5; // Минимальное количество случайных чисел.
  const max = 10; // Максимальное количество случайных чисел.
  
  // Генерируем случайное количество чисел в диапазоне от min до max (включительно).
  const count = Math.floor(Math.random() * (max - min + 1)) + min;

  // Создаем массив для хранения случайных чисел.
  const numbers = [];
  
  // Генерируем count случайных чисел в диапазоне от -n до n.
  for (let i = 0; i < count; i++) {
    // Генерируем случайное число в диапазоне от -n до n.
    const randomNumber = Math.floor(Math.random() * (2 * n + 1)) - n;
    // Добавляем сгенерированное число в массив numbers.
    numbers.push(randomNumber);
  }

  // Отправляем JSON-ответ с массивом случайных чисел.
  res.json({ numbers });
});

// Запускаем сервер на заданном порту.
// В функции обратного вызова выводим сообщение в консоль о том, что сервер запущен.
app.listen(port, () => {
  console.log(`Server is running at http://localhost:${port}`);
});





<!DOCTYPE html>
<html lang="en">
<head>
  <!-- Указываем кодировку документа, чтобы браузер правильно интерпретировал символы -->
  <meta charset="UTF-8">
  
  <!-- Устанавливаем мета-тег для управления адаптивностью (responsive) страницы -->
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  
  <!-- Устанавливаем заголовок страницы, который будет отображаться на вкладке браузера -->
  <title>B</title>
</head>
<body>
  <!-- Заголовок первого уровня, вводимый для указания пользователю о необходимости ввести значение n -->
  <h1>Введите значение n:</h1>
  
  <!-- Создаем форму для ввода значения n, назначая ей уникальный идентификатор -->
  <form id="generateForm">
    <!-- Поле ввода для числа n с атрибутами для идентификации и заполнения -->
    <input type="number" id="valueN" name="valueN" placeholder="Введите значение n">
    <br/> <!-- Перенос строки для визуального разделения элементов формы -->
    
    <!-- Кнопка для отправки формы и инициирования генерации случайных чисел -->
    <button type="submit">Сгенерировать</button>
  </form>
  
  <!-- Заголовок второго уровня для отображения результата генерации -->
  <h1>Результат:</h1>
  
  <!-- Элемент div, в который будет помещаться результат генерации случайных чисел -->
  <div id="result"></div>

  <script>
    // Получаем элемент формы по ID
    const form = document.getElementById('generateForm');
    // Получаем элемент для отображения результата по ID
    const resultB = document.getElementById('result');

    // Добавляем обработчик события на отправку формы
    form.addEventListener('submit', async (e) => {
      // Предотвращаем стандартное поведение формы (перезагрузка страницы)
      e.preventDefault();

      // Извлекаем значение n из поля ввода
      const n = document.getElementById('valueN').value;

      // Создаем новый объект XMLHttpRequest для отправки HTTP-запроса
      const xhrB = new XMLHttpRequest();
      // Открываем POST-запрос к маршруту /generate (обработчик на сервере)
      xhrB.open('POST', '/generate', false);
      
      // Устанавливаем заголовок Content-Type для указания формата данных
      xhrB.setRequestHeader('Content-Type', 'application/json');
      // Устанавливаем пользовательский заголовок с введенным значением n
      xhrB.setRequestHeader('X-rand-n', n);

      // Обработчик события onload для XMLHttpRequest
      xhrB.onload = () => {
        // Проверяем статус ответа от сервера
        if (xhrB.status === 200) {
          // Парсим ответ сервера из JSON-формата
          const dataB = JSON.parse(xhrB.responseText);
          // Отображаем результат генерации в элементе <div> с ID result
          resultB.textContent = `Генерация чисел (B): ${dataB.numbers}`;
        }
      };

      // Отправляем запрос на сервер
      xhrB.send();
    });
  </script>
</body>
</html>





<!DOCTYPE html>
<html lang="en">
<head>
  <!-- Указываем кодировку документа, что позволяет браузеру корректно отображать текст -->
  <meta charset="UTF-8">
  
  <!-- Устанавливаем мета-тег для управления адаптивностью (responsive) страницы -->
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  
  <!-- Заголовок страницы, который будет отображаться на вкладке браузера -->
  <title>C</title>
</head>
<body>
  <!-- Заголовок первого уровня для обозначения формы -->
  <h1>Форма A и B</h1>
  
  <!-- Форма с уникальным идентификатором для сбора данных от пользователя -->
  <form id="formAB">
    <!-- Метка для поля ввода значения X, отображающая текущее значение ползунка -->
    <label for="valueX">значение X: <span id="xOutput">0</span></label>
    <!-- Ползунок для ввода значения X, с диапазоном от 0 до 100 -->
    <input type="range" id="valueX" name="valueX" min="0" max="100" value="0" 
           oninput="document.getElementById('xOutput').innerText = this.value">
    <br/> <!-- Перенос строки для визуального разделения элементов формы -->
    
    <!-- Метка для поля ввода значения Y, отображающая текущее значение ползунка -->
    <label for="valueY">значение Y: <span id="yOutput">0</span></label>
    <!-- Ползунок для ввода значения Y, с диапазоном от 0 до 100 -->
    <input type="range" id="valueY" name="valueY" min="0" max="100" value="0" 
           oninput="document.getElementById('yOutput').innerText = this.value">
    <br/> <!-- Перенос строки для визуального разделения элементов формы -->

    <!-- Поле ввода для значения N, позволяет пользователю вводить текст -->
    <input type="text" id="valueN" name="valueN" placeholder="N">
    <br/> <!-- Перенос строки для визуального разделения элементов формы -->

    <!-- Кнопка для отправки формы, инициирует обработку введенных значений -->
    <button type="submit">Отправить</button>
  </form>

  <!-- Заголовок второго уровня для отображения результатов обработки -->
  <h1>Результат:</h1>
  
  <!-- Элемент для отображения результата A -->
  <div id="resultA"></div>
  <!-- Элемент для отображения результата B -->
  <div id="resultB"></div>

  <script>
    // Получаем элемент формы по ID
    const formAB = document.getElementById('formAB');
    // Получаем элементы для отображения результатов по их ID
    const resultA = document.getElementById('resultA');
    const resultB = document.getElementById('resultB');

    // Добавляем обработчик события на отправку формы
    formAB.addEventListener('submit', async (e) => {
      // Предотвращаем стандартное поведение формы (перезагрузка страницы)
      e.preventDefault();

      // Извлекаем значения X, Y и N из соответствующих полей ввода
      const x = document.getElementById('valueX').value;
      const y = document.getElementById('valueY').value;
      const n = document.getElementById('valueN').value;

      // Создаем новый объект XMLHttpRequest для отправки первого HTTP-запроса
      const xhrA = new XMLHttpRequest();
      // Открываем POST-запрос к маршруту /sum (обработчик на сервере для суммирования)
      xhrA.open('POST', '/sum', false);
      // Устанавливаем заголовок Content-Type для указания формата данных
      xhrA.setRequestHeader('Content-Type', 'text/plain');
      // Устанавливаем пользовательские заголовки с введенными значениями X и Y
      xhrA.setRequestHeader('X-value-x', x);
      xhrA.setRequestHeader('X-value-y', y);

      // Обработчик события onload для XMLHttpRequest
      xhrA.onload = () => {
        // Проверяем статус ответа от сервера
        if (xhrA.status === 200) {
          // Если статус 200 (успех), отображаем ответ в элементе <div> для результата A
          resultA.textContent = xhrA.responseText;
        }
      };

      // Отправляем первый запрос на сервер
      xhrA.send();

      // Создаем новый объект XMLHttpRequest для отправки второго HTTP-запроса
      const xhrB = new XMLHttpRequest();
      // Открываем POST-запрос к маршруту /generate (обработчик на сервере для генерации чисел)
      xhrB.open('POST', '/generate', false);
      // Устанавливаем заголовок Content-Type для указания формата данных
      xhrB.setRequestHeader('Content-Type', 'application/json');
      // Устанавливаем пользовательский заголовок с введенным значением N
      xhrB.setRequestHeader('X-rand-n', n);

      // Обработчик события onload для второго XMLHttpRequest
      xhrB.onload = () => {
        // Проверяем статус ответа от сервера
        if (xhrB.status === 200) {
          // Если статус 200 (успех), парсим ответ сервера из JSON-формата
          const dataB = JSON.parse(xhrB.responseText);
          // Отображаем результат генерации в элементе <div> для результата B
          resultB.textContent = `Генерация чисел (B): ${dataB.numbers}`;
        }
      };

      // Отправляем второй запрос на сервер
      xhrB.send();
    });
  </script>
</body>
</html>






// Импортируем фреймворк express для создания сервера
const express = require("express");

// Создаем экземпляр приложения express
const app = express();

// Устанавливаем порт, на котором будет запущен сервер
const port = 3000;

// Middleware для парсинга JSON-данных в теле запросов
app.use(express.json());

// Обработка GET-запроса на корневой маршрут ("/")
// При обращении к корневому маршруту отправляется HTML-файл
app.get("/", (req, res) => {
  // Отправляем файл WEB5C.html из текущей директории
  res.sendFile(__dirname + "/WEB5C.html");
});

// Обработка GET-запроса на маршрут для иконки сайта (favicon)
// Этот маршрут используется для обработки запросов к favicon.ico
app.get('/favicon.ico', (req, res) => {
  // Возвращаем статус 204 (Нет содержимого), чтобы не загружать иконку
  res.status(204).end(); // 204 No Content
});

// Обработка POST-запроса на маршрут "/sum"
// Этот маршрут обрабатывает запросы на получение суммы двух чисел
app.post("/sum", (req, res) => {
  // Извлекаем значения x и y из заголовков запроса и преобразуем их в целые числа
  const x = parseInt(req.headers["x-value-x"]);
  const y = parseInt(req.headers["x-value-y"]);

  // Вычисляем сумму x и y
  const z = x + y;

  // Устанавливаем заголовок ответа с результатом суммы
  res.setHeader("X-Value-z", z.toString());

  // Имитация длительной обработки, задерживаем ответ на 10 секунд
  setTimeout(() => {
    // Отправляем текстовое сообщение с результатом суммы
    res.send("Сумма (А): " + z);
  }, 10000);
});

// Обработка POST-запроса на маршрут "/generate"
// Этот маршрут генерирует массив случайных чисел на основе переданного значения n
app.post("/generate", (req, res) => {
  // Извлекаем значение n из заголовков запроса и преобразуем его в целое число
  const n = parseInt(req.headers["x-rand-n"]);

  // Устанавливаем минимальное и максимальное значение для количества генерируемых чисел
  const min = 5;
  const max = 10;
  
  // Генерируем случайное количество чисел в диапазоне от min до max
  const count = Math.floor(Math.random() * (max - min + 1)) + min;

  // Создаем массив для хранения сгенерированных чисел
  const numbers = [];
  
  // Генерируем случайные числа и добавляем их в массив
  for (let i = 0; i < count; i++) {
    // Генерируем случайное число в диапазоне от -n до n
    const randomNumber = Math.floor(Math.random() * (2 * n + 1)) - n;
    numbers.push(randomNumber); // Добавляем число в массив
  }

  // Имитация задержки в 1 секунду перед отправкой ответа
  setTimeout(() => {
    // Отправляем сгенерированный массив чисел в формате JSON
    res.json({ numbers });
  }, 1000);
});

// Запускаем сервер и начинаем прослушивать указанный порт
app.listen(port, () => {
  // Выводим в консоль сообщение о том, что сервер запущен
  console.log(`Server is running at http://localhost:${port}`);
});
