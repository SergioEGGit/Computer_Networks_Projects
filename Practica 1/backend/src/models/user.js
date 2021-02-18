const { Schema, model } = require('mongoose');

const userSchema = new Schema({
    Nombre: String,
    Apellido: String,
	Edad: int64,
	Genero: String, 
	Peso: int64, 
	Estatura: int64,
	Username: String,
	Password: String,
	Rol: String
});

module.exports = model('User', userSchema);