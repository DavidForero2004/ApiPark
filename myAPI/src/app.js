const express = require("express");
const cors = require("cors");

//* Import routes  */
const userRoutes = require("./routes/userRoutes");
const rolRoutes = require("./routes/rolRoutes");
const parkingRoutes = require("./routes/parkingRoutes");
const vehicleTypeRoutes = require("./routes/vehicleTypeRoutes");
const tariffRoutes = require("./routes/tariffRoutes");
const subscriptionRoutes = require("./routes/subscriptionRoutes");
const transactions = require("./routes/transactionRoutes");


const app = express();

// Middlewares
app.use(cors());
app.use(express.json());

//routes web
app.get("/", (req, res) => {
  res.send("Bienvenido a mi API");
});

app.use("/user", userRoutes);
app.use("/rol", rolRoutes);
app.use("/parking", parkingRoutes);
app.use("/vehicle-types", vehicleTypeRoutes);
app.use("/tariffs", tariffRoutes);
app.use("/subscriptions", subscriptionRoutes);
app.use("/transactions", transactions);


module.exports = app;
