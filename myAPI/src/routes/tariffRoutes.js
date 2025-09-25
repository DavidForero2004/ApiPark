const express = require("express");
const router = express.Router();
const {
  createTariff,
  getTariffs,
  getTariffById,
  updateTariff,
  deleteTariff,
} = require("../controller/tariffController");

const {verifyToken} = require("../middleware/auth");


router.post("/new", verifyToken,createTariff);
router.get("/", verifyToken, getTariffs);
router.get("/:id", verifyToken, getTariffById);
router.put("/:id", verifyToken, updateTariff);
router.delete("/:id", verifyToken, deleteTariff);

module.exports = router;
