const { Schema, model } = require('mongoose');

const analisisSchema = new Schema({
    Fecha: String,
	Hora: String,
	FechaCompleta: String,
	Volumen: number,
	Peso: number,
	Tipo: String,
	Username: String
});

module.exports = model('analisis, analisisSchema);