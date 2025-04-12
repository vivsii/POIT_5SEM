const express = require('express');
const fs = require('fs');
const path = require('path');

const app = express();
const PORT = 3000;

// Функция для добавления заголовков кеширования
function setCacheHeaders(res, cacheParam) {
  const now = new Date();
  
  switch (cacheParam) {
    case 'no-store':
      res.set('Cache-Control', 'no-store');
      break;
    case 'max-age':
      res.set('Cache-Control', 'max-age=60'); // 1 минута
      break;
    case 'etag':
      const etag = `"${now.getTime()}"`;
      res.set('ETag', etag);
      res.set('Cache-Control', 'private, must-revalidate');
      break;
    case 'last-modified':
      res.set('Last-Modified', now.toUTCString());
      res.set('Cache-Control', 'private, must-revalidate');
      break;
    case 'expires':
      res.set('Expires', new Date(now.getTime() + 60000).toUTCString()); // Через 1 минуту
      break;
    default:
      res.set('Cache-Control', 'no-cache');
  }
}
app.get('/', (req, res) => {
    const filePath = path.join(__dirname, 'index.html');
    res.sendFile(filePath);
  });
  
// 1. PNG-изображение
app.get('/image', (req, res) => {
  const cacheParam = req.query.cache_param || 'no-cache';
  setCacheHeaders(res, cacheParam);

  const imagePath = path.join(__dirname, 'public', 'cat.png');
  fs.readFile(imagePath, (err, data) => {
    if (err) return res.status(500).send('Ошибка загрузки изображения');
    res.set('Content-Type', 'image/png');
    res.send(data);
  });
});

// 2. JavaScript
app.get('/script', (req, res) => {
  const cacheParam = req.query.cache_param || 'no-cache';
  setCacheHeaders(res, cacheParam);

  const scriptContent = `
    console.log('Скрипт загружен с учетом кеширования: ${cacheParam}');
  `;
  res.set('Content-Type', 'application/javascript');
  res.send(scriptContent);
});

// 3. CSS
app.get('/styles', (req, res) => {
  const cacheParam = req.query.cache_param || 'no-cache';
  setCacheHeaders(res, cacheParam);

  const cssContent = `
    body {
      background-color: purple;
      font-family: Arial, sans-serif;
    }
  `;
  res.set('Content-Type', 'text/css');
  res.send(cssContent);
});

// Запуск сервера
app.listen(PORT, () => {
  console.log(`Сервер запущен на http://localhost:${PORT}`);
});
