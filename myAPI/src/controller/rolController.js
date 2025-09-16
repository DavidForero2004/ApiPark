const pool = require("../config/db");

// Crear uno o varios roles
const createRol = async (req, res) => {
  try {
    const { roles } = req.body; 
    // Se espera un array: [{nombre:"Admin"}, {nombre:"Usuario"}]

    if (!roles || !Array.isArray(roles) || roles.length === 0) {
      return res.status(400).json({ error: "Debes enviar un array con al menos un rol" });
    }

    for (let rol of roles) {
      await pool.query("CALL sp_createRol(?)", [rol.nombre]);
    }

    return res.status(201).json({
      message: "Roles creados correctamente",
      insertados: roles.length
    });
  } catch (error) {
    console.error("Error al crear roles:", error);
    return res.status(500).json({ error: "Error interno del servidor" });
  }
};

// Obtener todos los roles
const getRoles = async (req, res) => {
  try {
    const [rows] = await pool.query("CALL sp_getRoles()");
    return res.json(rows[0]);  // MySQL devuelve el primer set en rows[0]
  } catch (error) {
    console.error("Error al obtener roles:", error);
    res.status(500).json({ error: "Error al obtener roles" });
  }
};

// Obtener rol por ID
const getRolById = async (req, res) => {
  try {
    const { id } = req.params;
    const [rows] = await pool.query("CALL sp_getRolById(?)", [id]);

    if (rows[0].length === 0) {
      return res.status(404).json({ error: "Rol no encontrado" });
    }

    return res.json(rows[0][0]);
  } catch (error) {
    console.error("Error al obtener rol:", error);
    return res.status(500).json({ error: "Error interno del servidor" });
  }
};

// Actualizar rol
const updateRol = async (req, res) => {
  try {
    const { id } = req.params;
    const { nombre } = req.body;

    if (!nombre) return res.status(400).json({ error: "El nombre es requerido" });

    const [result] = await pool.query("CALL sp_updateRol(?, ?)", [id, nombre]);

    if (result.affectedRows === 0) {
      return res.status(404).json({ error: "Rol no encontrado" });
    }

    return res.json({ id, nombre });
  } catch (error) {
    console.error("Error al actualizar rol:", error);
    return res.status(500).json({ error: "Error interno del servidor" });
  }
};

// Eliminar rol
const deleteRol = async (req, res) => {
  try {
    const { id } = req.params;

    const [result] = await pool.query("CALL sp_deleteRol(?)", [id]);

    if (result.affectedRows === 0) {
      return res.status(404).json({ error: "Rol no encontrado" });
    }

    return res.json({ message: "Rol eliminado correctamente" });
  } catch (error) {
    console.error("Error al eliminar rol:", error);
    return res.status(500).json({ error: "Error interno del servidor" });
  }
};

module.exports = {
  createRol,
  getRoles,
  getRolById,
  updateRol,
  deleteRol,
};
