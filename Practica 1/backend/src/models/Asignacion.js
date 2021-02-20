const { Schema, model } = require('mongoose');

const asignacionSchema = new Schema({
    UsernameCoach: String,
	UsernameAtleta: String
});

module.exports = model('asignacion', asignacionSchema);