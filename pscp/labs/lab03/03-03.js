const http = require('http');
const url = require('url');
const fs = require('fs');
const path = require('path');

function factorial(n) {
    if (n === 0 || n === 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

const server = http.createServer((req, res) => {
    const parsedUrl = url.parse(req.url, true);
    const queryObject = parsedUrl.query;

    if (parsedUrl.pathname === '/fact' && queryObject.k) {
        const k = parseInt(queryObject.k);

        if (isNaN(k)) {
            res.writeHead(400, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: "Parameter 'k' must be a valid number" }));
        } else {
            const fact = factorial(k);
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ k: k, fact: fact }));
        }
    } else if (parsedUrl.pathname === '/') {
        fs.readFile(path.join(__dirname, '03-03.html'), 'utf8', (err, data) => {
            if (err) {
                res.writeHead(500, { 'Content-Type': 'text/plain' });
                res.end('Ошибка при чтении HTML файла.');
                return;
            }
            res.writeHead(200, { 'Content-Type': 'text/html' });
            res.end(data);
        });
    } else {
        res.writeHead(404, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: "Invalid request" }));
    }
});

server.listen(5000, () => {
    console.log('Сервер запущен по адресу: http://localhost:5000/');
});