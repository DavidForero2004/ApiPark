const express = require("express");
const router = express.Router();
const {
  createVehicleType,
  getVehicleTypes,
  getVehicleTypeById,
  updateVehicleType,
  deleteVehicleType,
} = require("../controller/vehicleTypeController");

// Routes
router.post("/new", createVehicleType);
router.get("/", getVehicleTypes);
router.get("/:id", getVehicleTypeById);
router.put("/:id", updateVehicleType);
router.delete("/:id", deleteVehicleType);

module.exports = router;
