const express = require("express");
const router = express.Router();

const {
  createRol,
  deleteRol,
  getRolById,
  updateRol,
  getRoles
} = require("../controller/rolController");

const {verifyToken} = require("../middleware/auth");

router.post("/new",verifyToken, createRol);      
router.get("/", verifyToken, getRoles);            
router.get("/:id",verifyToken, getRolById);       
router.put("/update/:id", verifyToken,updateRol);       
router.delete("/delete/:id",verifyToken, deleteRol);    

module.exports = router;
