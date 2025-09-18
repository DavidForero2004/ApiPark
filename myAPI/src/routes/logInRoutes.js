const express = require("express");
const { login } = require("../controller/logInController");

const router = express.Router();


router.post("/", login);

module.exports = router;
