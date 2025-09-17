const express = require("express");
const router = express.Router();
const {
  createSubscription,
  getSubscriptions,
  getSubscriptionById,
  updateSubscription,
  deleteSubscription,
} = require("../controller/subscriptionController");

// Routes
router.post("/new", createSubscription);
router.get("/", getSubscriptions);
router.get("/:id", getSubscriptionById);
router.put("/:id", updateSubscription);
router.delete("/:id", deleteSubscription);

module.exports = router;
