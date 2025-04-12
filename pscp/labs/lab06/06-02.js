const http = require('http');
const nodemailer = require('nodemailer');
const fs = require('fs');
const path = require('path');
const querystring = require('querystring');

const port = 3000;

const transporter = nodemailer.createTransport({
    host: 'smtp.ethereal.email',
    port: 587,
    auth: {
        user: 'benedict63@ethereal.email',
        pass: 'P1yezcUyxVrdmfFSf7'
    }
});

const sendFile = (res, filePath) => {
    fs.readFile(filePath, (err, data) => {
        if (err) {
            res.writeHead(404, { 'Content-Type': 'text/plain' });
            res.end('404 Not Found');
            return;
        }
        res.writeHead(200, { 'Content-Type': 'text/html' });
        res.end(data);
    });
};


const server = http.createServer((req, res) => {
    if (req.method === 'GET' && req.url === '/') {

        sendFile(res, path.join(__dirname,  'index2.html'));
    } else if (req.method === 'POST' && req.url === '/send') {
        let body = '';


        req.on('data', chunk => {
            body += chunk.toString(); 
        });

        req.on('end', () => {
            const { sender, receiver, message } = querystring.parse(body);

            transporter.sendMail({
                from: sender,
                to: receiver,
                subject: 'Сообщение от Node.js',
                text: message,
            }, (err, info) => {
                if (err) {
                    res.writeHead(500, { 'Content-Type': 'text/plain' });
                    res.end('Error: ' + err.toString());
                    return;
                }
                res.writeHead(200, { 'Content-Type': 'text/plain' });
                res.end('Email was sent: ' + info.response);
            });
        });
    } else {
        res.writeHead(404, { 'Content-Type': 'text/plain' });
        res.end('404 Not Found');
    }
});


server.listen(port, () => {
    console.log(`Сервер запущен на http://localhost:${port}`);
 
});
