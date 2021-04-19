const { Schema, model } = require('mongoose');

const analisisSchema = new Schema({
    Fecha: String,
	Hora: String,
	FechaCompleta: String,
	Volumen: Number,
	Peso: Number,
	Tipo: String,
	Username: String
});

module.exports = model('Analisis', analisisSchema);