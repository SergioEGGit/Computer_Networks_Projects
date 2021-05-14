const { Schema, model } = require('mongoose');

const medicionesSchema = new Schema({
    Fecha: String,
	Periodo: Number,
	Velocidad: Number,
    Distancia: Number,
	PulsoOxigeno: Number, 
	RitmoCardiaco: Number,
	Temperatura: Number,
	Username: String
});

module.exports = model('Medicion', medicionesSchema);