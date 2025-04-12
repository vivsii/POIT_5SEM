const WebSocket = require('ws')
const ws = new WebSocket('ws://localhost:4000');
const fs = require('fs')
let k = 0;
ws.on('open',() =>{
    const duplex = WebSocket.createWebSocketStream(ws,{encoding: 'utf8'});
    let wfile = fs.createWriteStream(`./download/MyFile.txt`);
    duplex.pipe(wfile);
})