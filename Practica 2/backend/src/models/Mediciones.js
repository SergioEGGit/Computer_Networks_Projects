const { Schema, model } = require('mongoose');

const medicionesSchema = new Schema({
    Fecha: String,
	Hora: String,
	FechaCompleta: String,
	Volumen: number,
	Peso: number,
	Tipo: String,
	Username: String
});

module.exports = model('Medicion', medicionesSchema);