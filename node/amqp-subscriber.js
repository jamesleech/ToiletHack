var amqp = require('amqp');  
var url  = 'amqp://localhost:5672';

var connection = amqp.createConnection(
  { url: url }, 
  { defaultExchangeName: 'amq.topic' });

connection.on('ready', function () {  
    connection.queue('testing-queue', { durable: false, autoDelete: false }, function (q) {
        q.bind('*');

        q.subscribe({ ack: true, prefetchCount: 1 }, function (message) {
            console.log('received message', message.toString());
            q.shift();
        });
    });
});