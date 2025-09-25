const db = require("../config/db");

// Create tariff
const createTariff = async (req, res) => {
  try {
    const { parqueadero_id, tipo_vehiculo_id, tarifa_dia, tarifa_mes } = req.body;

    if (!parqueadero_id || !tipo_vehiculo_id || !tarifa_dia || !tarifa_mes) {
      return res.status(400).json({ error: "Todos los campos son obligatorios" });
    }

    await db.query("CALL sp_createTarifa(?, ?, ?, ?)", [parqueadero_id, tipo_vehiculo_id, tarifa_dia, tarifa_mes]);

    res.json({ message: "Tarifa creada correctamente" });
  } catch (error) {
    console.error(error);
    res.status(500).json({ error: error.message });
  }
};

// Get all tariffs
const getTariffs = async (req, res) => {
  const {parqueadero_id} = req.body;
  try {
    const [rows] = await db.query("CALL sp_getTarifas(?)", [parqueadero_id]);
    res.json(rows[0]);
  } catch (error) {
    console.error(error);
    res.status(500).json({ error: error.message });
  }
};

// Get tariff by ID
const getTariffById = async (req, res) => {
  try {
    const { id } = req.params;
    const [rows] = await db.query("CALL sp_getTarifaById(?)", [id]);

    if (!rows[0].length) return res.status(404).json({ error: "Tarifa no encontrada" });

    res.json(rows[0][0]);
  } catch (error) {
    console.error(error);
    res.status(500).json({ error: error.message });
  }
};

// Update tariff
const updateTariff = async (req, res) => {
  try {
    const { id } = req.params;
    const { parqueadero_id, tipo_vehiculo_id, tarifa_dia, tarifa_mes } = req.body;

    if (!parqueadero_id || !tipo_vehiculo_id || !tarifa_dia || !tarifa_mes) {
      return res.status(400).json({ error: "Todos los campos son obligatorios" });
    }

    await db.query("CALL sp_updateTarifa(?, ?, ?, ?, ?)", [id, parqueadero_id, tipo_vehiculo_id, tarifa_dia, tarifa_mes]);

    res.json({ message: "Tarifa actualizada correctamente" });
  } catch (error) {
    console.error(error);
    res.status(500).json({ error: error.message });
  }
};

// Delete tariff
const deleteTariff = async (req, res) => {
  try {
    const { id } = req.params;
    await db.query("CALL sp_deleteTarifa(?)", [id]);
    res.json({ message: "Tarifa eliminada correctamente" });
  } catch (error) {
    console.error(error);
    res.status(500).json({ error: error.message });
  }
};

module.exports = {
  createTariff,
  getTariffs,
  getTariffById,
  updateTariff,
  deleteTariff,
};
