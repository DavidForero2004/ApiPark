const express = require("express");
const router = express.Router();
const {createTransaction,deleteTransaction,getTransactionById,getTransactions,updateTransaction} = require("../controller/transactionController");
const {verifyToken} = require("../middleware/auth");
router.post("/", verifyToken, createTransaction);       // create
router.get("/",verifyToken, getTransactions);          // get all
router.get("/:id", verifyToken, getTransactionById);    // get by id
router.put("/:id", verifyToken, updateTransaction);     // update
router.delete("/:id", verifyToken, deleteTransaction);  // delete

module.exports = router;
