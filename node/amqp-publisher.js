var amqp = require('amqp');  
var url  = 'amqp://localhost:5672';

var payload = {  
  deviceId : '8675308'
};

var connection = amqp.createConnection({url: url},  { defaultExchangeName: 'amq.topic' });

connection.on('ready', function () {  
  connection.publish('ToiletHack.Node.13646069', payload);

  setTimeout(function(){
    connection.disconnect();
    process.exit();
  }, 500);
});