const app = require('./src/app');
const { PORT } = require('./src/config');
const { connect } = require('./src/database');

app.set('port', PORT);


async function main(){

   //Database connection
   await connect();
   //Express application
   await app.listen(app.get('port'));
   console.log(`Server on port ${app.get('port')}: connected! :D :)`)
};

main();