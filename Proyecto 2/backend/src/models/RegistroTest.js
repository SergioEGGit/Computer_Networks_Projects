const { Schema, model } = require('mongoose');

const registrosSchema = new Schema({
    Fecha: String,
	Periodo: Number,
	Estado: String,
	Username: String
});

module.exports = model('Registro', registrosSchema);