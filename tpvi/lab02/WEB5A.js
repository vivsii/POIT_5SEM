const  express = require('express');
const app = express();

app.use(express.json());

app.get("/", function(request, response) {response.sendFile(__dirname + "/WEB5A.html");});

app.post("/sum", function(request, response){
    const x = parseInt(request.headers["x-value-x"]);
    const y = parseInt(request.headers["x-value-y"]);

    const z = x + y;

    response.setHeader("X-Value-z", z.toString());
    response.send("результат: " + z);
});
app.listen(3000, function(request, response){ console.log(`running`)});