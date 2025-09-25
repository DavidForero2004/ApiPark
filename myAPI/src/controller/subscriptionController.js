const pool = require("../config/db"); // mysql2/promise

// Get all subscriptions
const getSubscriptions = async (req, res) => {
  const {parqueadero_id} = req.body;
  try {
    const [rows] = await pool.query("CALL sp_getMensualidades(?)",[parqueadero_id]);
    res.json(rows[0]);
  } catch (error) {
    res.status(500).json({ message: error.message });
  }
};

// Get subscription by ID
const getSubscriptionById = async (req, res) => {
  try {
    const { id } = req.params;
    const [rows] = await pool.query("CALL sp_getMensualidadById(?)", [id]);
    res.json(rows[0][0]);
  } catch (error) {
    res.status(500).json({ message: error.message });
  }
};

// Create subscription
const createSubscription = async (req, res) => {
  try {
    const { usuario_id, tarifa_id, placa, fecha_inicio, fecha_fin, estado } = req.body;
    await pool.query(
      "CALL sp_createMensualidad(?,?,?,?,?,?)",
      [usuario_id, tarifa_id, placa, fecha_inicio, fecha_fin, estado]
    );
    res.status(201).json({ message: "Subscription created successfully" });
  } catch (error) {
    res.status(500).json({ message: error.message });
  }
};

// Update subscription
const updateSubscription = async (req, res) => {
  try {
    const { id } = req.params;
    const { usuario_id, tarifa_id, placa, fecha_inicio, fecha_fin, estado } = req.body;
    await pool.query(
      "CALL sp_updateMensualidad(?,?,?,?,?,?,?)",
      [id, usuario_id, tarifa_id, placa, fecha_inicio, fecha_fin, estado]
    );
    res.json({ message: "Subscription updated successfully" });
  } catch (error) {
    res.status(500).json({ message: error.message });
  }
};

// Delete subscription
const deleteSubscription = async (req, res) => {
  try {
    const { id } = req.params;
    await pool.query("CALL sp_deleteMensualidad(?)", [id]);
    res.json({ message: "Subscription deleted successfully" });
  } catch (error) {
    res.status(500).json({ message: error.message });
  }
};

module.exports = {
  getSubscriptions,
  getSubscriptionById,
  createSubscription,
  updateSubscription,
  deleteSubscription
};
