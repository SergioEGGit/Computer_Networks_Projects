const { Schema, model } = require('mongoose');

const medicionesSchema = new Schema({
    Fecha: String,
	PulsoOxigeno: Number, 
	RitmoCardiaco: Number,
	Temperatura: Number,
	Username: String
});

module.exports = model('Medicion', medicionesSchema);