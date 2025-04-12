const em = require('events')
const { EventEmitter } = require('stream')

emmiter = new EventEmitter();

emmiter.on('sayhi',()=>{
    console.log("Вот ваше событие")
})

emmiter.emit('sayhi')

//создать экземпляр эммит, создать событие, создать обработчик и подписать на событие, вызвать собыьи