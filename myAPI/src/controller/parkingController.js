const db = require("../config/db");

// Create parking
const createParking = async (req, res) => {
  try {
    const { nombre, direccion, telefono, email, codigo, legal } = req.body;

    await db.query(
      "CALL sp_createParqueadero(?, ?, ?, ?, ?, ?)",
      [nombre, direccion, telefono, email, codigo, legal]
    );

    res.json({ message: "Parking created successfully" });
  } catch (err) {
    res.status(500).json({ error: err.message });
  }
};

// Get all parkings
const getParkings = async (req, res) => {
  try {
    const [rows] = await db.query("CALL sp_getParqueaderos()");
    res.json(rows[0]);
  } catch (err) {
    res.status(500).json({ error: err.message });
  }
};

// Get parking by ID
const getParkingById = async (req, res) => {
  try {
    const { id } = req.params;
    const [result] = await db.query("CALL sp_getParqueaderoById(?)", [id]);

    if (result[0].length === 0) {
      return res.status(404).json({ error: "Parking not found" });
    }

    res.json(result[0][0]);
  } catch (err) {
    res.status(500).json({ error: err.message });
  }
};

// Update parking
const updateParking = async (req, res) => {
  try {
    const { id } = req.params;
    const { nombre, direccion, telefono, email, codigo, legal } = req.body;

    await db.query(
      "CALL sp_updateParqueadero(?, ?, ?, ?, ?, ?, ?)",
      [id, nombre, direccion, telefono, email, codigo, legal]
    );

    res.json({ message: "Parking updated successfully" });
  } catch (err) {
    res.status(500).json({ error: err.message });
  }
};

// Delete parking
const deleteParking = async (req, res) => {
  try {
    const { id } = req.params;
    await db.query("CALL sp_deleteParqueadero(?)", [id]);
    res.json({ message: "Parking deleted successfully" });
  } catch (err) {
    res.status(500).json({ error: err.message });
  }
};

module.exports = {
  createParking,
  getParkings,
  getParkingById,
  updateParking,
  deleteParking,
};
