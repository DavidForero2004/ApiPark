const jwt = require("jsonwebtoken");
const pool = require("../config/db");
const { comparePassword } = require("./bcrypt");

const JWT_SECRET = process.env.JWT_SECRET || "default_secret"; 

const login = async (req, res) => {
  const { email, password } = req.body;

  try {
    // Llamar al procedure
    const [rows] = await pool.query("CALL sp_login_usuario(?)", [email]);

    const userData = rows[0][0]; 

    if (!userData) {
      return res.status(401).json({ message: "Usuario o contraseña incorrecto" });
    }

    // Validar contraseña
    const validPassword = await comparePassword(password, userData.password);
    if (!validPassword) {
      return res.status(401).json({ message: "Usuario o contraseña incorrecto" });
    }

    // Crear token
    const token = jwt.sign(
      { id: userData.id, rol: userData.rol },
      JWT_SECRET,
      { expiresIn: "1h" }
    );
    
    res.json({
      message: "Login exitoso",
      token,
      user: {
        i: userData.id,
        user: userData.nombre,
        email: userData.email,
        r: userData.rol,
        p: userData.parqueadero_id
       
      },
    });
  } catch (error) {
    console.error("Error en login:", error);
    res.status(500).json({ message: "Error en el servidor" });
  }
};

module.exports = { login };
