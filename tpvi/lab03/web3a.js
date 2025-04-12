const http = require('http');
const Cookies = require('cookies');

let accumulatedData = { sx: 0, sy: 0 };
let requestCount = 0;

http.createServer((req, res) => {
    const cookies = new Cookies(req, res);

    const cookieData = cookies.get('accumulated_data');
    if (cookieData) {
        accumulatedData = JSON.parse(cookieData);
    }

    if (req.method === 'POST') {
        let body = '';

        req.on('data', chunk => {
            body += chunk.toString(); 
        });

        req.on('end', () => {
            let data;
            try {
                data = JSON.parse(body);
            } catch (error) {
                res.writeHead(400, { 'Content-Type': 'application/json' });
                return res.end(JSON.stringify({ error: 'Invalid JSON' }));
            }

            const x = parseInt(data.x, 10);
            const y = parseInt(data.y, 10);

            if (isNaN(x) || isNaN(y) || x <= 0 || y <= 0) {
                res.writeHead(400, { 'Content-Type': 'application/json' });
                return res.end(JSON.stringify({ error: 'x and y must be positive integers.' }));
            }

            requestCount++;

            if (requestCount === 1) {
                accumulatedData.sx = x;
                accumulatedData.sy = y;
            } else {
                accumulatedData.sx += x;
                accumulatedData.sy += y;
            }

            if (requestCount % 5 === 0) {
                accumulatedData = { sx: 0, sy: 0 }; 
            }

            cookies.set('accumulated_data', JSON.stringify(accumulatedData), { maxAge: 60 * 60 * 1000 }); 

            res.writeHead(200, { 'Content-Type': 'application/json' });
            res.end(JSON.stringify(accumulatedData));
        });
    }
    else{
        
    res.setHeader('Content-Type', 'text/html');

    res.write('<h1>Hello world!</h1>');
    res.end();
    }
}).listen(3000, () => {
    console.log('localhost:3000 is active');
});