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
router.get("/",verifyToken, getVehicleTypes);
router.get("/:id",verifyToken, getVehicleTypeById);
router.put("/:id",verifyToken, updateVehicleType);
router.delete("/:id",verifyToken, deleteVehicleType);

module.exports = router;
