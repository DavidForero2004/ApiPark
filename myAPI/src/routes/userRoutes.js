const express = require("express");
const router = express.Router();
const userController = require("../controller/userController");
const {verifyToken} = require("../middleware/auth");
router.post("/new", userController.createUser);
router.get("/", verifyToken, userController.getUsers);
router.get("/:id",verifyToken, userController.getUserById);
router.put("/:id", verifyToken, userController.updateUser);
router.delete("/:id", verifyToken, userController.deleteUser);

module.exports = router;
