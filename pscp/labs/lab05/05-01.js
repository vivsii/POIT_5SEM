const { user, DB, database } = require("./db.js");
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
        const parsedBody = JSON.parse(body); 
        const deletedRow = await DB.delete(parsedBody.id); 
        if (deletedRow) {
            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify(deletedRow));
        } else {
            res.writeHead(404, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify({ message: 'Row not found' }));
        }
    });
});

let requestCount = 0;
let commitCount = 0;
let stats = { startTime: null, endTime: null, requestCount: 0, commitCount: 0 };
let statCollectionInterval = false;
let shutdownTimeout = null;

const commit = () => {
    console.log('Фиксация состояния БД выполнена.');
    commitCount++;
    myEmitter.emit('commit');
};

const handleCommand = (command) => {
    const parts = command.split(' ');
    const cmd = parts[0];
    const arg = parseInt(parts[1], 10);

    switch (cmd) {
        case 'sd':
            if (isNaN(arg)) {  
                if (shutdownTimeout) { 
                    clearTimeout(shutdownTimeout);  
                    shutdownTimeout = null;
                    console.log('Остановка сервера отменена.');
                } else {
                    clearTimeout(shutdownTimeout); 
                    shutdownTimeout = setTimeout(() => { 
                    console.log('Сервер остановлен.');
                    process.exit(0);
                }, 500);
                }
            } else {
                if (shutdownTimeout) clearTimeout(shutdownTimeout); 
                shutdownTimeout = setTimeout(() => { 
                    console.log('Сервер остановлен.');
                    process.exit(0);
                }, arg * 1000);
                console.log(`Сервер остановится через ${arg} секунд.`);
            }
            break;        
        case 'sc':
        if (isNaN(arg)) { 
            if (statCollectionInterval) {  
                clearInterval(statCollectionInterval);  
                statCollectionInterval = null;
                console.log('Периодическая фиксация остановлена.');
            } else {
                console.log('Периодическая фиксация не запущена.');
            }
        } else { 
            if (statCollectionInterval) clearInterval(statCollectionInterval); 
            statCollectionInterval = setInterval(commit, arg * 1000); 
            console.log(`Периодическая фиксация запущена с интервалом ${arg} секунд.`);
        }
        break;
        case 'ss':
        if (isNaN(arg)) {  
            if (statCollectionInterval && statsCollectionActive) {  
                clearInterval(statCollectionInterval);  
                statCollectionInterval = null;
                stats.endTime = new Date();
                stats.requestCount = requestCount;
                stats.commitCount = commitCount;
                console.log('Сбор статистики завершен.');
                console.log(stats);
                stats.startTime = null;
                statsCollectionActive = false; 
            } else {
                console.log('Сбор статистики не запущен.');
            }
        } else {  
            if (!stats.startTime) {  
                stats.startTime = new Date();
                stats.requestCount = 0; 
                stats.commitCount = 0;
                console.log(`Сбор статистики начат на ${arg} секунд.`);
                statsCollectionActive = true; 
                
                statCollectionInterval = setInterval(() => {
                    stats.requestCount = requestCount;
                    stats.commitCount = commitCount;
                }, 1000);

                setTimeout(() => {
                    if (statsCollectionActive) {  
                        stats.endTime = new Date();
                        clearInterval(statCollectionInterval);
                        statCollectionInterval = null;
                        stats.requestCount = requestCount;
                        stats.commitCount = commitCount;
                        console.log(`Сбор статистики завершен.`);
                        console.log(stats);
                        stats.startTime ;
                        statsCollectionActive = false; 
                    }
                }, arg * 1000);
            } else {
                console.log('Сбор статистики уже запущен.');
            }
        }
        break;
        default:
            console.log('Неизвестная команда.');
            break;
    }
};

const server = http.createServer((req, res) => {
    requestCount++;
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
    else if(req.url === "/"){
        fs.readFile("index.html", function (err, data){
            if(err)
            {
                res.writeHead(500, {"Content-Type": "text/plain"});
                res.end("Server error!");
            }

            else {
                res.writeHead(200, { "Content-Type": "text/html" });
                res.end(data);
            }
        })
    }
    else if (req.method === 'GET' && req.url === '/api/ss') {
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({
            startTime: stats.startTime,
            endTime: stats.endTime || null,
            requestCount,
            commitCount,
        }))
    }
    else {
        res.writeHead(404);
        res.end('Page not found');
    }
})

server.listen(5000, () => {
    console.log('Server running at http://localhost:5000/api/db');
    const readline = require('readline');
    const rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout,
    });

    rl.on('line', handleCommand);
});