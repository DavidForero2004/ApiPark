// archivo: auth.js
const bcrypt = require('bcryptjs');

// Función para hashear una contraseña
async function hashPassword(password) {
  const salt = await bcrypt.genSalt(10);
  return await bcrypt.hash(password, salt);
}

// Función para comparar una contraseña con un hash
async function comparePassword(password, hashedPassword) {
  return await bcrypt.compare(password, hashedPassword); // devuelve true o false
}

// Exportamos las funciones para poder llamarlas desde otro archivo
module.exports = {
  hashPassword,
  comparePassword
};
