const { Router } = require('express');
const router = Router();
const User = require('../models/user');
const Medicion = require('../models/analisis');

// Raiz
router.get('/', (req, res) => {

    
    res.json({'resultado': "Arqui2 - Practica 2 :D"});
	
});

// Todos Los Usuarios 
router.get('/users', async (req, res) => {

    const users = await User.find();
    res.json({users});
	
});

// Agregar Medicion
router.post("/create_medicion_VO2MAX", async (req, res) => {
    
	try {

        const data = req.body;            
        
		var ArrayAux = data.Fecha.split(" ");
		
		await Medicion.create({
        
			Fecha: ArrayAux[0],
			Hora: ArrayAux[1],
			FechaCompleta: data.Fecha,
			Volumen: data.Volumen,
			Tipo: data.Tipo,
			Username: data.Username,
			Peso: data.Peso
        
		});   
		
        res.json({message : 'Medicion creada'});

    } catch (error) {
		
        res.json({ message : error });
    
	}

});

// Agregar Usuario 
router.post("/create_user", async (req, res) => {
    
	try {

        const data = req.body;            
        
		await User.create({
        
		Nombre: data.Nombre,
			Apellido: data.Apellido,
			Edad: data.Edad,
			Genero: data.Genero, 
			Peso: data.Peso, 
			Estatura: data.Estatura,
			Username: data.Username,
			Password: data.Password,
			Rol: data.Rol
        
		});   
		
        res.json({message : 'Usuario creado'});

    } catch (error) {
		
        res.send({ message : error });
    
	}

});

// Query 1
router.get('/GetOneUser/:Us/:Pass', async (req, res) => {
    
	const users = await User.find().where('Username').equals(req.params.Us).where('Password').equals(req.params.Pass);
    
	res.send(users[0]);

});

// Query 2 
router.get('/GetVO2MAXMediciones/:Us/:Fe', async (req, res) => {
    
	const Mediciones = await Medicion.find().where('Username').equals(req.params.Us).where('FechaCompleta').equals(req.params.Fe.replace(/_/gi,"/")).select('FechaCompleta Volumen Peso Tipo Username');
    
	res.send(Mediciones);

});

// Query 3 
router.get('/FechasUserVO2MAX/:Us', async (req, res) => {
    
	const Mediciones = await Medicion.find().distinct('FechaCompleta').where('Username').equals(req.params.Us);
    
	res.send(Mediciones);

});

// Query 4 
router.get('/FechasUserLiveVO2MAX', async (req, res) => {
    
	const Mediciones = await Medicion.find().limit(1).sort({FechaCompleta: -1}).select('FechaCompleta');
    
	res.send(Mediciones[0]);

});

module.exports = router; 