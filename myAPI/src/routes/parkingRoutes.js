const express = require("express");
const router = express.Router();

const {
  createParking,
  getParkings,
  getParkingById,
  updateParking,
  deleteParking
} = require("../controller/parkingController");

const {verifyToken} = require("../middleware/auth");

router.post("/new",verifyToken, createParking);          
router.get("/", verifyToken,getParkings);                
router.get("/:id", verifyToken, getParkingById);          
router.put("/update/:id",verifyToken, updateParking);   
router.delete("/delete/:id", verifyToken, deleteParking); 

module.exports = router;
