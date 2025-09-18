const express = require("express");
const { logout } = require("../middleware/auth");

const router = express.Router();



// Ruta para logout
router.post("/", logout);

module.exports = router;
