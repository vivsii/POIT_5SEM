const { user, DB, database } = require("./DB.js");
const { EventEmitter } = require('events');
const http = require('http');
const fs = require('fs');
const url = require('url');

class MyEmitter extends EventEmitter {}
const myEmitter = new MyEmitter();

myEmitter.on("GET", async (req, res) => {
    const data = await DB.select(database);
    res.writeHead(200, {'Content-Type': 'application/json'});
    res.end(JSON.stringify(data));
});

myEmitter.on("POST", async (req, res) => {
    let body = '';
    req.on('data', chunk => {
        body += chunk.toString();
    });
    req.on('end', async() => {
        const newRow = await DB.insert(JSON.parse(body));
        res.writeHead(201, {'Content-Type': 'application/json'});
        res.end(JSON.stringify(newRow));
    });
});

myEmitter.on("PUT", async (req, res) => {
    let body = '';
    req.on('data', chunk => {
        body += chunk.toString();
    });
    req.on('end', async () => {
        const newData = JSON.parse(body);
        const updateRow = await DB.update(newData);
        if (updateRow) {
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify(updateRow));
        } else {
            res.writeHead(404, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ message: 'Row not found' }));
        }
    });
});


myEmitter.on("DELETE", async (req, res) => {
    let body = '';
    req.on('data', chunk => {
        body += chunk.toString();
    });
    req.on('end', async () => {
        const { id } = JSON.parse(body);  
        const deletedRow = await DB.delete(id);  
        if (deletedRow) {
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify(deletedRow));
        } else {
            res.writeHead(404);
            res.end('Row not found');
        }
    });
});


const server = http.createServer((req, res) => {
    const reqUrl = url.parse(req.url, true);
    if (reqUrl.pathname === "/api/db") {
        switch (req.method) {
            case "GET": {
                myEmitter.emit("GET", req, res);
                break;
            }
            case "PUT": {
                myEmitter.emit("PUT", req, res);
                break;
            }

            case "POST": {
                myEmitter.emit("POST", req, res);
                break;
            }
            case "DELETE": {
                myEmitter.emit("DELETE", req, res);
                break;
            }
        }
    }
    else {
        res.writeHead(404);
        res.end('Page not found');
    }
})

server.listen(5000, () => {
    console.log('Server running at http://localhost:5000/api/db');
});