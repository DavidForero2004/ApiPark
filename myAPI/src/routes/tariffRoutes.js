const express = require("express");
const router = express.Router();
const {
  createTariff,
  getTariffs,
  getTariffById,
  updateTariff,
  deleteTariff,
} = require("../controller/tariffController");

router.post("/new", createTariff);
router.get("/", getTariffs);
router.get("/:id", getTariffById);
router.put("/:id", updateTariff);
router.delete("/:id", deleteTariff);

module.exports = router;
