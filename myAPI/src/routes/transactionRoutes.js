const express = require("express");
const router = express.Router();
const {createTransaction,deleteTransaction,getTransactionById,getTransactions,updateTransaction} = require("../controller/transactionController");

router.post("/", createTransaction);       // create
router.get("/", getTransactions);          // get all
router.get("/:id", getTransactionById);    // get by id
router.put("/:id", updateTransaction);     // update
router.delete("/:id", deleteTransaction);  // delete

module.exports = router;
