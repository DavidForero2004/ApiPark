const pool = require("../config/db");

const validarHash = async (req, res) => {
  try {
    const { code } = req.body; 

    if (!code) {
      return res.status(400).json({ error: "Se requiere el código" });
    }

    const [rows] = await pool.query("CALL hash_Validate(?)", [code]);

    const resultado = rows[0][0].valido; 

    if (resultado === 1) {
      return res.json({ auth: true, message: "Código válido" });
    } else {
      return res.status(401).json({ auth: false, message: "Código inválido" });
    }

  } catch (error) {
    console.error("Error al validar hash:", error);
    return res.status(500).json({ error: "Error interno del servidor" });
  }
};

module.exports = { validarHash };
