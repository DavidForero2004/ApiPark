const express = require("express");
const router = express.Router();

const { createUser } = require("../controller/userController");

router.get("/new", createUser);

module.exports = router;
