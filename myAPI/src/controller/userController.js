// controllers/userController.js
const db = require("../config/db");
const { hashPassword } = require("./bcrypt");

const createUser = async (req, res) => {
  try {
    let { nombre, apellido, cedula, telefono, email, password, rol_id, parking_id } = req.body;

    if (!nombre || !apellido || !cedula || !password || !rol_id || !parking_id) {
      return res.status(400).json({ error: "Missing required fields" });
    }

    // Generar hash de la contraseña
    password = await hashPassword(password);

    await db.query("CALL sp_createUsuario(?, ?, ?, ?, ?, ?, ?, ?)", [
      nombre,
      apellido,
      cedula,
      telefono,
      email,
      password,
      rol_id,
      parking_id,
    ]);

    res.json({ message: "User created successfully" });
  } catch (error) {
    console.error(error); // Para ver errores en consola
    res.status(500).json({ error: error.message });
  }
};

// Get all users
const getUsers = async (req, res) => {
      let {parking_id } = req.query;
  try {
    const [rows] = await db.query("CALL sp_getUsuarios(?)", [parking_id]);
    res.json(rows[0]);
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
};

// Get user by ID
const getUserById = async (req, res) => {
  try {
    const { id } = req.params;
    const [rows] = await db.query("CALL sp_getUsuarioById(?)", [id]);

    if (!rows[0].length) return res.status(404).json({ error: "User not found" });

    res.json(rows[0][0]);
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
};

// Update user
const updateUser = async (req, res) => {
  try {
    const { id } = req.params;
    let { nombre, apellido, cedula, telefono, email, password, rol_id, parking_id } = req.body;

    // Hash de la contraseña solo si se envía nueva
    if (password) {
      password = await hashPassword(password);
    }

    await db.query("CALL sp_updateUsuario(?, ?, ?, ?, ?, ?, ?, ?, ?)", [
      id,
      nombre,
      apellido,
      cedula,
      telefono,
      email,
      password,
      rol_id,
      parking_id,
    ]);

    res.json({ message: "User updated successfully" });
  } catch (error) {
    console.error(error);
  }
};

// Delete user
const deleteUser = async (req, res) => {
  try {
    const { id } = req.params;

    await db.query("CALL sp_deleteUsuario(?)", [id]);

    res.json({ message: "User deleted successfully" });
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
};

module.exports = {
  createUser,
  getUsers,
  getUserById,
  updateUser,
  deleteUser,
};
