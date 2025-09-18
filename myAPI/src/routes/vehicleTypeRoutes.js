const express = require("express");
const router = express.Router();
const {
  createVehicleType,
  getVehicleTypes,
  getVehicleTypeById,
  updateVehicleType,
  deleteVehicleType,
} = require("../controller/vehicleTypeController");
const {verifyToken} = require("../middleware/auth");
// Routes
router.post("/new", verifyToken, createVehicleType);
router.get("/", getVehicleTypes);
router.get("/:id", getVehicleTypeById);
router.put("/:id", updateVehicleType);
router.delete("/:id", deleteVehicleType);

module.exports = router;
