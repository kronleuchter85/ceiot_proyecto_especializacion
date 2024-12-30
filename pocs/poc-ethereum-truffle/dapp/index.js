const express = require("express");

const app = express();
const port = 3000;


const BlockchainService = require('./blockchain_service'); 
const blockchainService = new BlockchainService(process.env.BLOCKCHAIN_URL);

app.get("/api/accounts", async (req, res) => {
  try {
      accounts = await blockchainService.getAccounts();
      res.json({ accounts });
  } catch (err) {
      res.status(500).json({ error: err.message });
  }
});


app.get('/api/blocks/:n', async (req, res) => {
  const n = parseInt(req.params.n, 10); 

  if (isNaN(n) || n <= 0) {
    return res.status(400).send({ error: 'El parámetro N debe ser un número entero positivo.' });
  }
  try {
    const blocks = await blockchainService.getLastNBlocks(n);
    res.json({ blocks });
  }
  catch (err) {
      res.status(500).json({ error: err.message });
  }

});

app.get("/api/transactions/block/:n", async (req, res) => {
  const n = parseInt(req.params.n, 10); 

  if (isNaN(n) || n <= 0) {
    return res.status(400).send({ error: 'El parámetro N debe ser un número entero positivo.' });
  }
  
  try {
    const txs = await blockchainService.getTransactionsFromBlock(n);
    res.json({ txs });
  } catch (err) {
      res.status(500).json({ error: err.message });
  }
});


// Rutas REST
// app.get("/getDatad", async (req, res) => {
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