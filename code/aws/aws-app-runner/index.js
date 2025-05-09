const express = require("express");
const bodyParser = require("body-parser");

const app = express();
const port = 3000;


app.use(bodyParser.json());


app.get("/api/greetings", async (req, res) => {
  try {
    message = ["hola", "como", "estas?"];
    res.json({ message });
  } catch (err) {
    res.status(500).json({ error: err.message });
  }
});


app.get("/api/secret", async (req, res) => {
  try {

    const TEST_SECRET = process.env.TEST_SECRET;
    // const secretObj = JSON.parse(TEST_SECRET);

    res.json({ TEST_SECRET });
  } catch (err) {
    res.status(500).json({ error: err.message });
  }
});


app.listen(port, () => {
  console.log(`API listening on port ${port}`);
});