const express = require("express");
const router = express.Router();
const {
  createSubscription,
  getSubscriptions,
  getSubscriptionById,
  updateSubscription,
  deleteSubscription,
} = require("../controller/subscriptionController");
const {verifyToken} = require("../middleware/auth");

// Routes
router.post("/new",verifyToken, createSubscription);
router.get("/", verifyToken, getSubscriptions);
router.get("/:id", verifyToken,getSubscriptionById);
router.put("/:id", verifyToken,updateSubscription);
router.delete("/:id",verifyToken, deleteSubscription);

module.exports = router;
