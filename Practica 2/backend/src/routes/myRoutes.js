const { Router } = require('express');
const router = Router();
const User = require('../models/user');
const Medicion = require('../models/Mediciones');

router.get('/', (req, res) => {

    
    res.json({'resultado': "Arqui2 - Practica 2 :D"});
	
});


router.get('/users', async (req, res) => {

    const users = await User.find();
    res.json({users});
	
});

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

router.get('/one_user/:Us/:Pass', async (req, res) => {
    
	const users = await User.find().where('Username').equals(req.params.Us).where('Password').equals(req.params.Pass);
    
	res.send(users);

});

router.post("/create_medicion", async (req, res) => {
    
	try {

        const data = req.body;            
        
		await Medicion.create({
        
			Fecha: data.Fecha,
			Hora: data.Hora,
			Volumen: data.Volumen,
			Tipo: data.Tipo,
			Username: data.Username
        
		});   
		
        res.json({message : 'Medicion creada'});

    } catch (error) {
		
        res.send({ message : error });
    
	}

});

router.get('/get/Mediciones/:Us/:Da', async (req, res) => {
    
	const Mediciones = await Medicion.find().where('Username').equals(req.params.Us).where('Fecha').equals(req.params.Da).sort({ Periodo: 1, Distancia: 1 });
    
	res.send(Mediciones);

});

module.exports = router; 