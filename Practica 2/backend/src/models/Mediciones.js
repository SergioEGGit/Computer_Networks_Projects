const { Schema, model } = require('mongoose');

const medicionesSchema = new Schema({
    Fecha: String,
	Volumen: number,
	Tipo: String,
	Username: String
});

module.exports = model('Medicion', medicionesSchema);