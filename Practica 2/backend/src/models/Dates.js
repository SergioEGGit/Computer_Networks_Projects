const { Schema, model } = require('mongoose');

const datesSchema = new Schema({
    Fecha: String, 
	Username: String
});

module.exports = model('dates', datesSchema);