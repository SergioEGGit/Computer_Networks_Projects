const { Router } = require('express');
const router = Router();
const User = require('../models/user');


router.get('/users', async (req, res) => {

    const users = await User.find();
    res.json({users});
	
});

//peticion para eliminar TODOS los usuarios registrados en la base de datos
router.get('/delete_all', async (req, res) => {
    
	await User.deleteMany({})
    
	res.json({'Resultado': 'Usuarios borrados con exito! :D'});

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

module.exports = router; 