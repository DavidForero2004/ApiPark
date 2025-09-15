const pool = require("../config/db");


const createUser = async (req, res) => {
  try {
    
    res.json({ message: "hola" }); 
    
  } catch (error) {
    console.error("Error en createUser:", error);
    res.status(500).json({ error: "Error en el servidor" });
  }
};



module.exports = { createUser };