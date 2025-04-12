const http = require('http');
const url = require('url');

function factorial(n, callback) {
  if (n === 0 || n === 1) {
    return callback(null, 1);
  }
  
  setImmediate(() => {
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
      const startTime = process.hrtime(); // Начало замера времени

      factorial(k, (err, fact) => {
        const endTime = process.hrtime(startTime); // Конец замера времени
        const durationInMs = (endTime[0] * 1e3 + endTime[1] / 1e6).toFixed(3); // Время в миллисекундах

        if (err) {
          res.writeHead(500, { 'Content-Type': 'application/json' });
          res.end(JSON.stringify({ error: "Error calculating factorial" }));
        } else {
          res.writeHead(200, { 'Content-Type': 'application/json' });
          res.end(JSON.stringify({ k: k, fact: fact, duration: durationInMs + ' ms' }));
        }
      });
    }
  } else {
    res.writeHead(404, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ error: "Invalid request" }));
  }
});

server.listen(5000, () => {
  console.log('Server running at http://localhost:5000/');
});