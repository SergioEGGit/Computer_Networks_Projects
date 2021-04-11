const { Router } = require('express');
const router = Router();


router.get('/', (req, res) => {

    
    res.json({'resultado': "Arqui2 - Practica 2 :D"});
	
});


module.exports = router; 