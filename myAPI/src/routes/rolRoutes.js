const express = require("express");
const router = express.Router();

const {
  createRol,
  deleteRol,
  getRolById,
  updateRol,
  getRoles
} = require("../controller/rolController");

router.post("/new", createRol);      
router.get("/", getRoles);            
router.get("/:id", getRolById);       
router.put("/update/:id", updateRol);       
router.delete("/delete/:id", deleteRol);    

module.exports = router;
