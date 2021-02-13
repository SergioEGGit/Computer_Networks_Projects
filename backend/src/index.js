const app = require('./app');
const { PORT } = require('./config');
const { connect } = require('./database');

app.set('port', PORT);


async function main(){

   //Database connection
   await connect();
   //Express application
   await app.listen(app.get('port'));
   console.log(`Server on port ${app.get('port')}: connected! :D :)`)
};

main();