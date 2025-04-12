const http = require('http');
const url = require('url');

function factorial(n, callback) {
  if (n === 0 || n === 1) {
    return callback(null, 1);
  }
  
  process.nextTick(() => {
    factorial(n - 1, (err, result) => {
      if (err) {
        return callback(err);
      }
      callback(null, n * result);
    });
  });
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
      const startTime = Date.now();
      factorial(k, (err, fact) => {
        const endTime = Date.now(); 
        const duration = endTime - startTime; 

        if (err) {
          res.writeHead(500, { 'Content-Type': 'application/json' });
          res.end(JSON.stringify({ error: "Internal server error" }));
        } else {
          res.writeHead(200, { 'Content-Type': 'application/json' });
          res.end(JSON.stringify({ k: k, fact: fact, duration: duration }));
        }
      });
    }
  } else {
    res.writeHead(404, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ error: "Invalid request" }));
  }
});

server.listen(5000, () => {
  console.log('Сервер запущен по адресу: http://localhost:5000/');
});