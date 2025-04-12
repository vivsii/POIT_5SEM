const http = require('http');
const url = require('url');

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
  } else {
    res.writeHead(404, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({ error: "Invalid request" }));
  }
});

server.listen(5000, () => {
  console.log('Server running at http://localhost:5000/');
});
