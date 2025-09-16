const express = require("express");
const cors = require("cors");

//* Import routes  */
const userRoutes = require("./routes/userRoutes");
const rolRoutes = require("./routes/rolRoutes");
const parkingRoutes = require("./routes/parkingRoutes");
const vehicleTypeRoutes = require("./routes/vehicleTypeRoutes");

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


module.exports = app;
