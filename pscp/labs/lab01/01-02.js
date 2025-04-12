const http = require('http');

const server = http.createServer((req, res) => {
    res.writeHead(200, { 'Content-Type': 'text/html' });

    let responseHTML = `
        <html>
        <head><title>Request Info</title></head>
        <body>
            <h1>Request Information</h1>
            <p><strong>Method:</strong> ${req.method}</p>
            <p><strong>URI:</strong> ${req.uri}</p>
            <p><strong>Protocol Version:</strong> ${req.httpVersion}</p>
            <h2>Headers:</h2>
            <pre>${JSON.stringify(req.headers, null, 2)}</pre>
    `;
    let body = '';
    req.on('data', chunk => {
        body += chunk;
    });
    req.on('end', () => {
        responseHTML += `
            <h2>Body:</h2>
            <pre>${body || 'No Body Content'}</pre>
            </body>
        </html>
        `;
        res.end(responseHTML);
    });
});

server.listen(3000, () => {
    console.log('Сервер запущен');
});
