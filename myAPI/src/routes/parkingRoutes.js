const express = require("express");
const router = express.Router();

const {
  createParking,
  getParkings,
  getParkingById,
  updateParking,
  deleteParking
} = require("../controller/parkingController");

router.post("/new", createParking);          
router.get("/", getParkings);                
router.get("/:id", getParkingById);          
router.put("/update/:id", updateParking);   
router.delete("/delete/:id", deleteParking); 

module.exports = router;
