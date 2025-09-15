const express = require("express");
const cors = require("cors");

//* Import routes  */
const userRoutes = require("./routes/userRoutes");

const app = express();

// Middlewares
app.use(cors());
app.use(express.json());

//routes web
app.get("/", (req, res) => {
  res.send("Bienvenido a mi API");
});

app.use("/user", userRoutes);

module.exports = app;
