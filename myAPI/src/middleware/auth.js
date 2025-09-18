const jwt = require("jsonwebtoken");

// Lista temporal de tokens inválidos (puedes guardarla en Redis o DB en producción)
let blacklistedTokens = [];

const verifyToken = (req, res, next) => {
  const authHeader = req.headers["authorization"];
  if (!authHeader) {
    return res.status(401).json({ message: "Token requerido" });
  }

  const token = authHeader.split(" ")[1]; // viene como "Bearer <token>"

  // Verificar si está en la lista negra
  if (blacklistedTokens.includes(token)) {
    return res.status(401).json({ message: "Token inválido, debes iniciar sesión otra vez" });
  }

  jwt.verify(token, process.env.JWT_SECRET, (err, decoded) => {
    if (err) {
      return res.status(401).json({ message: "Token inválido o expirado" });
    }
    req.user = decoded;
    next();
  });
};

// Agregar un token a la lista negra (logout)
const logout = (req, res) => {
  const authHeader = req.headers["authorization"];
  if (!authHeader) {
    return res.status(400).json({ message: "Token requerido para cerrar sesión" });
  }

  const token = authHeader.split(" ")[1];
  blacklistedTokens.push(token); // guardar token inválido
  return res.json({ message: "Sesión cerrada correctamente" });
};

module.exports = { verifyToken, logout };
