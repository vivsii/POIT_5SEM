const express = require('express');
const session = require('express-session');
const bodyParser = require('body-parser');
const crypto = require('crypto');

const app = express();
const PORT = 3000;

// Генерируем случайный ключ длиной 32 байта и преобразуем его в строку в шестнадцатичном формате
const secret = crypto.randomBytes(32).toString('hex');

// Настройка сессий
app.use(session({
    secret: secret, // Используем сгенерированный секретный ключ
    resave: false,
    saveUninitialized: true
}));

// для парсинга JSON
app.use(bodyParser.json());

app.post('/web4a', (req, res) => {
    const { x, y } = req.body;

    // Проверка на положительные целые числа
    if (!Number.isInteger(x) || !Number.isInteger(y) || x < 0 || y < 0) {
        return res.status(400).json({ error: 'Both x and y must be positive integers' });
    }

    // Инициализация накопленных сумм
    if (!req.session.sN1) {
        req.session.sN1 = 0;
        req.session.sN2 = 0;
    }

    // Увеличение накопленных сумм
    req.session.sN1 += x;
    req.session.sN2 += y;

    // Подсчет количества запросов
    req.session.requestCount = (req.session.requestCount || 0) + 1;

    let response;

    // Проверка на кратность 5
    if (req.session.requestCount % 5 === 0) {
        response = { sx: req.session.sN1, sy: req.session.sN2 };
        // Сброс накопленных сумм
        req.session.sN1 = 0;
        req.session.sN2 = 0;
    } else {
        response = { sx: req.session.sN1, sy: req.session.sN2 };
    }

    res.json(response);
});

app.listen(PORT, () => {
    console.log(`Server running on http://localhost:${PORT}`);
});