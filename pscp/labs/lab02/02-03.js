const http = require('http');
const path = require('path');

const server = http.createServer((req, res) => {
    if (req.url === '/api/name') {
        res.writeHead(200, { 'Content-Type': 'text/plain; charset=utf-8'});
        res.end('Евсеенко Викторрия Павловна');
    } else {
        res.writeHead(404, { 'Content-Type': 'text/plain' });
        res.end('Страница не найдена');
    }
});

server.listen(5000, () => {
    console.log('Сервер запущен на порту 5000');
});
