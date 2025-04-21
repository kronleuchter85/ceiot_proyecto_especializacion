const express = require("express");
const bodyParser = require("body-parser");

const app = express();
const port = 3000;

// 
// open api generator 
// 
const swaggerUi = require('swagger-ui-express');
const fs = require('fs');
const swaggerDocument = yaml.load(fs.readFileSync('./openapi.yaml', 'utf8'));
// const swaggerDocument = JSON.parse(fs.readFileSync('./openapi.json'));
app.use('/api-docs', swaggerUi.serve, swaggerUi.setup(swaggerDocument));


const BlockchainService = require('./blockchain_service'); 
const blockchainService = new BlockchainService(process.env.BLOCKCHAIN_URL);

const wallet_private_key = process.env.WALLET_PRIVATE_KEY;

app.use(bodyParser.json());


app.get("/api/greetings", async (req, res) => {
  try {
      message = ["hola" , "como" , "estas?"];
      res.json({ message });
  } catch (err) {
      res.status(500).json({ error: err.message });
  }
});

app.get("/api/accounts", async (req, res) => {
  try {
      accounts = await blockchainService.getAccounts();
      res.json({ accounts });
  } catch (err) {
      res.status(500).json({ error: err.message });
  }
});

// 
// -----------------------------------------------------
// ABI
// -----------------------------------------------------
// 


app.get('/api/contracts/:contractName/abi', async (req, res) => {
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
// -----------------------------------------------------
// Invoking the contracts
// -----------------------------------------------------
// 


app.post('/api/contracts/xView', async (req, res) => {
  const { contractName ,  method } = req.body;
  const contractAddress = blockchainService.getContractAddress(contractName);

  try {
    // Verifica si existe el archivo ABI correspondiente al contrato
    const result = await blockchainService.call(contractName , contractAddress , method);

    res.status(200).json({ result });
  } catch (error) {
    console.error("Contract ABI not found:", error);
    res.status(500).json({ error: "Contract ABI not found", details: error.message });
  }
});


app.post('/api/contracts/xWrite', async (req, res) => {
  
  const { contractName ,  method, args, account } = req.body;
  const privateKey = wallet_private_key;
  const contractAddress = blockchainService.getContractAddress(contractName);

  if (!args || !account || !privateKey) {
    return res.status(400).json({ error: "Todos los parámetros son obligatorios: param1, param2, param3, param4" });
  }

  try {
    const result = await blockchainService.send(contractName , contractAddress , method , args, account, privateKey);

    res.status(200).json({
      message: "Parámetros recibidos correctamente",
      receipt: result
    });

  } catch (error) {
    console.error("Contract ABI not found:", error);
    res.status(500).json({ error: "Contract ABI not found", details: error.message });
  }
});

// 
// -----------------------------------------------------
// Registry
// -----------------------------------------------------
// 


app.post('/api/contracts/registry', async (req, res) => {
  const { contractName, contractAddress } = req.body;

  try {
    blockchainService.registerContract(contractName , contractAddress);
    res.status(200).json({
      message: "Contrato Registrado"
    });
  } catch (error) {
    console.error("Problem registering contract:", error);
    res.status(500).json({ error: "Problem registering contract", details: error.message });
  }
});


app.get('/api/contracts/registry', async (req, res) => {

  try {
    const result = blockchainService.getRegisteredContracts();
    res.json({ result });
  } catch (error) {
    console.error("Problem registering contract:", error);
    res.status(500).json({ error: "Problem registering contract", details: error.message });
  }
});

// 
// -----------------------------------------------------
// Info from the blockchain:
// - events log
// - blocks
// - transactions
// -----------------------------------------------------
// 


app.get('/api/contracts/events/:contractName/:eventName', async (req, res) => {
  const { contractName  , eventName } = req.params;
  const contractAddress = blockchainService.getContractAddress(contractName);

  try {
    const result = await blockchainService.getAllEvents(contractName,contractAddress , eventName);
    res.status(200).json({ result });
  } catch (error) {
    console.error("Problem registering contract:", error);
    res.status(500).json({ error: "Problem registering contract", details: error.message });
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

app.get("/api/blocks/:n/transactions", async (req, res) => {
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
// -----------------------------------------------------
// Expenses
// -----------------------------------------------------
// 

app.get('/api/expenses', async (req, res) => {

  try {
    const result = blockchainService.getExpenses();
    res.json({ result });
  } catch (error) {
    console.error("Problem registering contract:", error);
    res.status(500).json({ error: "Problem registering contract", details: error.message });
  }
});



app.listen(port, () => {
    console.log(`API listening on port ${port}`);
});