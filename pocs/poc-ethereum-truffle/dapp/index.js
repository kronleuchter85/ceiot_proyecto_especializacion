const express = require("express");
const bodyParser = require("body-parser");

const app = express();
const port = 3000;

const BlockchainService = require('./blockchain_service'); 
const blockchainService = new BlockchainService(process.env.BLOCKCHAIN_URL);

app.use(bodyParser.json());

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

//
// Gets the transactions of a block
//
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

//
// Returns the ABI JSON specification of the smart contract
//
app.get('/api/abi/:contractName', async (req, res) => {
  const { contractName } = req.params;

  try {
    // Verifica si existe el archivo ABI correspondiente al contrato
    const abiPath = blockchainService.getAbi(contractName);

    res.json({ abiPath });
  } catch (error) {
    console.error("Contract ABI not found:", error);
    res.status(500).json({ error: "Contract ABI not found", details: error.message });
  }
});

//
// Invoke for reading
//
app.get('/api/contracts/call/:contractName/:contractAddress/:method', async (req, res) => {
  const { contractName , contractAddress , method } = req.params;

  try {
    // Verifica si existe el archivo ABI correspondiente al contrato
    const result = await blockchainService.call(contractName , contractAddress , method);

    res.json({ result });
  } catch (error) {
    console.error("Contract ABI not found:", error);
    res.status(500).json({ error: "Contract ABI not found", details: error.message });
  }
});

//
// Invoke for writing
//
app.post('/api/contracts/send/:contractName/:contractAddress/:method', async (req, res) => {
  const { contractName , contractAddress , method } = req.params;
  const { value, account, privateKey } = req.body;

  if (!value || !account || !privateKey) {
    return res.status(400).json({ error: "Todos los parámetros son obligatorios: param1, param2, param3, param4" });
  }

  try {
    const result = await blockchainService.send(contractName , contractAddress , method , value, account, privateKey);

    res.status(200).json({
      message: "Parámetros recibidos correctamente",
      receipt: result
    });

  } catch (error) {
    console.error("Contract ABI not found:", error);
    res.status(500).json({ error: "Contract ABI not found", details: error.message });
  }
});

app.post('/api/contracts/register', async (req, res) => {
  const { contractName, contractAddress } = req.body;

  try {
    await blockchainService.registerContract(contractName , contractAddress);
    res.status(200).json({
      message: "Contrato Registrado"
    });
  } catch (error) {
    console.error("Problem registering contract:", error);
    res.status(500).json({ error: "Problem registering contract", details: error.message });
  }
});


app.get('/api/contracts/register', async (req, res) => {
  const { contractName , contractAddress } = req.params;

  try {
    const result = await blockchainService.getRegisteredContracts();
    res.json({ result });
  } catch (error) {
    console.error("Problem registering contract:", error);
    res.status(500).json({ error: "Problem registering contract", details: error.message });
  }
});



app.listen(port, () => {
    console.log(`API listening on port ${port}`);
});