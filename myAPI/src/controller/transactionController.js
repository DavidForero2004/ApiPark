const pool = require("../config/db"); // tu pool de mysql2

// CREATE
const createTransaction = async (req, res) => {
  try {
    const { usuario_id, mensualidad_id, tipo, monto, descripcion } = req.body;
    await pool.query(
      "CALL sp_createTransaccion(?,?,?,?,?)",
      [usuario_id, mensualidad_id, tipo, monto, descripcion]
    );
    res.status(201).json({ message: "Transaction created successfully" });
  } catch (error) {
    res.status(500).json({ message: error.message });
  }
};

// GET ALL
const getTransactions = async (req, res) => {
  try {
    const [rows] = await pool.query("CALL sp_getTransacciones()");
    res.json(rows[0]); // mysql2 devuelve un array de resultados por cada SELECT en el SP
  } catch (error) {
    res.status(500).json({ message: error.message });
  }
};

// GET BY ID
const getTransactionById = async (req, res) => {
  try {
    const { id } = req.params;
    const [rows] = await pool.query("CALL sp_getTransaccionById(?)", [id]);
    res.json(rows[0][0] || null); // si no existe, devolvemos null
  } catch (error) {
    res.status(500).json({ message: error.message });
  }
};

// UPDATE
const updateTransaction = async (req, res) => {
  try {
    const { id } = req.params;
    const { usuario_id, mensualidad_id, tipo, monto, descripcion } = req.body;
    await pool.query("CALL sp_updateTransaccion(?,?,?,?,?,?)", [
      id,
      usuario_id,
      mensualidad_id,
      tipo,
      monto,
      descripcion,
    ]);
    res.json({ message: "Transaction updated successfully" });
  } catch (error) {
    res.status(500).json({ message: error.message });
  }
};

// DELETE
const deleteTransaction = async (req, res) => {
  try {
    const { id } = req.params;
    await pool.query("CALL sp_deleteTransaccion(?)", [id]);
    res.json({ message: "Transaction deleted successfully" });
  } catch (error) {
    res.status(500).json({ message: error.message });
  }
};

module.exports = {
  createTransaction,
  getTransactions,
  getTransactionById,
  updateTransaction,
  deleteTransaction,
};
