const express = require("express");

const app = express();
const port = 3000;

app.use(express.json());

app.get("/", (req, res) => {
  res.sendFile(__dirname + "/WEB5B.html");
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

  res.json({ numbers });
});

app.listen(port, () => {
  console.log(`Server is running at http://localhost:${port}`);
});
