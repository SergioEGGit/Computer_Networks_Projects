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
            firstName: data.name,
            lastName: data.lastname
        });       
        res.json({message : 'Usuario creado'});

    } catch (error) {
        res.send({ message : error });
    }
});


module.exports = router; 