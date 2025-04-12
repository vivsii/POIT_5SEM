const http = require('http');
const DB = require('./DB');
const url = require('url');
const path = require('path');
const fs = require('fs');

DB.on('GET', async (req, res) => {
    try {
        const data = await DB.select();
        res.writeHead(200, 'OK', { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(data));
    } catch (error) {
        res.writeHead(500, 'Server Error', { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }
});

DB.on('POST', async (req, res) => {
    let body = '';
    req.on('data', (chunk) => {
        body += chunk; // Собираем данные запроса
    });
    req.on('end', async () => {
        try {
            const parsedBody = JSON.parse(body); // Парсим JSON
            await db.insert(parsedBody); // Добавляем в базу
            res.writeHead(200, {'Content-Type': 'application/json'}); // Указываем тип контента
            res.end(JSON.stringify(parsedBody)); // Возвращаем результат
        } catch (err) {
            res.writeHead(400, {'Content-Type': 'application/json'});
            res.end(JSON.stringify({ error: 'Invalid JSON format' }));
        }
    });
});


DB.on('PUT', async (req, res) => {
    const parsedUrl = url.parse(req.url, true);
    let body = '';

    req.on('data', (chunk) => {
        body += chunk;
    });

    req.on('end', async () => {
        try {
            if (!body) {
                res.writeHead(400, 'Bad Request', { 'Content-Type': 'application/json' });
                return res.end(JSON.stringify({ error: 'Body is empty' }));
            }

            const data = JSON.parse(body);
            await DB.update(data);

            res.writeHead(200, 'Updated', { 'Content-Type': 'application/json' });
            res.end(JSON.stringify(data));
        } catch (error) {
            res.writeHead(400, 'Bad Request', { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ error: 'Invalid JSON format' }));
        }
    });
});

DB.on('DELETE', async (req, res) => {
    const parsedUrl = url.parse(req.url, true);
    const queryParams = parsedUrl.query;

    try {
        if (!queryParams.id) {
            res.writeHead(400, 'Bad Request', { 'Content-Type': 'application/json' });
            return res.end(JSON.stringify({ error: 'ID is required' }));
        }

        const deleted = await DB.delete(queryParams.id);
        res.writeHead(204, 'Deleted', { 'Content-Type': 'application/json' });
        res.end(JSON.stringify(deleted));
    } catch (error) {
        res.writeHead(500, 'Server Error', { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Internal Server Error' }));
    }
});

const server = http.createServer((req, res) => {
    const parsedUrl = url.parse(req.url, true);
    const pathname = parsedUrl.pathname;

    if (pathname === '/') {
        fs.readFile(path.join(__dirname, '04-02.html'), (err, html) => {
            if (err) {
                res.writeHead(500, { 'Content-Type': 'text/plain' });
                res.end('Server error');
            } else {
                res.writeHead(200, { 'Content-Type': 'application/json' });
                res.end(html);
            }
        });
    } else if (pathname === '/api/db') {
        DB.emit(req.method, req, res);
    } else {
        res.writeHead(404, 'Not Found', { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Not found' }));
    }
});

server.listen(3000, () => {
    console.log('Server running on port 3000');
});
