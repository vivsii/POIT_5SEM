const express = require("express");

const app = express();
const port = 3000;

app.use(express.json());

app.get("/", (req, res) => {
  res.sendFile(__dirname + "/WEB5C.html");
});
app.get('/favicon.ico', (req, res) => {
  res.status(204).end(); // Возвращаем статус 204 (Нет содержимого)
});

app.post("/sum", (req, res) => {
  const x = parseInt(req.headers["x-value-x"]);
  const y = parseInt(req.headers["x-value-y"]);

  const z = x + y;

  res.setHeader("X-Value-z", z.toString());
  setTimeout(() => {
    res.send("Сумма (А): " + z);
  }, 10000);
});

app.post("/generate", (req, res) => {
  const n = parseInt(req.headers["x-rand-n"]);

  const min = 5;
  const max = 10;
  const count = Math.floor(Math.random() * (max - min + 1)) + min;

  const numbers = [];
  for (let i = 0; i < count; i++) {
    const randomNumber = Math.floor(Math.random() * (2 * n + 1)) - n;
    numbers.push(randomNumber);
  }

  setTimeout(() => {
    res.json({ numbers });
  }, 1000);
});

app.listen(port, () => {
  console.log(`Server is running at http://localhost:${port}`);
});
