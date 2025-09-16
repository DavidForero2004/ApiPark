const db = require("../config/db");

// Create vehicle type
const createVehicleType = async (req, res) => {
  try {
    const { nombre } = req.body;

    if (!nombre) return res.status(400).json({ error: "El nombre es obligatorio" });

    await db.query("CALL sp_createTipoVehiculo(?)", [nombre]);

    res.json({ message: "Tipo de vehículo creado correctamente" });
  } catch (error) {
    console.error(error);
    res.status(500).json({ error: error.message });
  }
};

// Get all vehicle types
const getVehicleTypes = async (req, res) => {
  try {
    const [rows] = await db.query("CALL sp_getTiposVehiculo()");
    res.json(rows[0]);
  } catch (error) {
    console.error(error);
    res.status(500).json({ error: error.message });
  }
};

// Get vehicle type by ID
const getVehicleTypeById = async (req, res) => {
  try {
    const { id } = req.params;
    const [rows] = await db.query("CALL sp_getTipoVehiculoById(?)", [id]);

    if (!rows[0].length) return res.status(404).json({ error: "Tipo de vehículo no encontrado" });

    res.json(rows[0][0]);
  } catch (error) {
    console.error(error);
    res.status(500).json({ error: error.message });
  }
};

// Update vehicle type
const updateVehicleType = async (req, res) => {
  try {
    const { id } = req.params;
    const { nombre } = req.body;

    if (!nombre) return res.status(400).json({ error: "El nombre es obligatorio" });

    await db.query("CALL sp_updateTipoVehiculo(?, ?)", [id, nombre]);

    res.json({ message: "Tipo de vehículo actualizado correctamente" });
  } catch (error) {
    console.error(error);
    res.status(500).json({ error: error.message });
  }
};

// Delete vehicle type
const deleteVehicleType = async (req, res) => {
  try {
    const { id } = req.params;

    await db.query("CALL sp_deleteTipoVehiculo(?)", [id]);

    res.json({ message: "Tipo de vehículo eliminado correctamente" });
  } catch (error) {
    console.error(error);
    res.status(500).json({ error: error.message });
  }
};

module.exports = {
  createVehicleType,
  getVehicleTypes,
  getVehicleTypeById,
  updateVehicleType,
  deleteVehicleType,
};
