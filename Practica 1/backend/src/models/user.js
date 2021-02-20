const { Schema, model } = require('mongoose');

const userSchema = new Schema({
    Nombre: String,
    Apellido: String,
	Edad: Number,
	Genero: String, 
	Peso: Number, 
	Estatura: Number,
	Username: String,
	Password: String,
	Rol: String
});

module.exports = model('User', userSchema);