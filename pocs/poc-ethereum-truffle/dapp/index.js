const express = require("express");
const {Web3} = require("web3");

const app = express();
const port = 3000;

// Configura el proveedor y dirección del contrato
const web3 = new Web3(process.env.BLOCKCHAIN_URL);
// const contractABI = require("./contractABI.json");
// const contractAddress = process.env.CONTRACT_ADDRESS;
// const contract = new web3.eth.Contract(contractABI, contractAddress);


app.get("/api/accounts", async (req, res) => {
    try {
        accounts = await web3.eth.getAccounts()
        res.json({ accounts });
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
});

// Rutas REST
// app.get("/getData", async (req, res) => {
//     try {
//         const data = await contract.methods.getData().call();
//         res.json({ data });
//     } catch (err) {
//         res.status(500).json({ error: err.message });
//     }
// });

// app.post("/setData", async (req, res) => {
//     try {
//         const { value } = req.body; // valor enviado por el cliente
//         const accounts = await web3.eth.getAccounts();
//         const receipt = await contract.methods.setData(value).send({ from: accounts[0] });
//         res.json({ receipt });
//     } catch (err) {
//         res.status(500).json({ error: err.message });
//     }
// });

app.listen(port, () => {
    console.log(`API listening on port ${port}`);
});