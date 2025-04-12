const http = require('http');
const DB = require('./DB');
const url = require('url');
const path = require('path')
const fs = require('fs')

let db = new DB();

db.on('GET', async (req, res) =>{
    res.end(JSON.stringify(await db.select()))
})
db.on('POST', async (req, res) => {
    req.on('data', async (data) => {
        let body = JSON.parse(data)
        await db.insert(body)
        res.end(JSON.stringify(body))
    })
})
db.on('PUT', async (req,res) => {
    const parsedUrl = url.parse(req.url, true)
    const queryParams = parsedUrl.query

    req.on('data', async (data) => {
        let body = JSON.parse(data)
        await db.update(queryParams.id, body)
        res.end(JSON.stringify(body))
    })
})
db.on('DELETE', async (req, res) => {
    const parsedUrl = url.parse(req.url, true);
    const queryParams = parsedUrl.query;
    const deleted = await db.delete(queryParams.id);
    res.end(JSON.stringify(deleted));
});

const server = http.createServer((req, res) => {
    const parsedUrl = url.parse(req.url, true)
    const pathname = parsedUrl.pathname

    if(pathname == '/'){

        fs.readFile(path.join(__dirname, '04-02.html'), (err, html) => {
            if(err){
                res.writeHead(500, {'Content-Type': 'text/text'})
                res.end('Server error')
            } else {
                res.writeHead(200, {'Content-Type':'text/html; charset=utf-8'})
                res.end(html)
            }
        })
    } else if (pathname === '/api/db'){
        db.emit(req.method, req, res)
    }
})

server.listen(3000);