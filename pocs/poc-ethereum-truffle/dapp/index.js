const express = require("express");
const bodyParser = require("body-parser");

const app = express();
const port = 3000;

const BlockchainService = require('./blockchain_service'); 
const blockchainService = new BlockchainService(process.env.BLOCKCHAIN_URL);

const wallet_private_key = process.env.WALLET_PRIVATE_KEY;

// 
// open api generator 
// 
const swaggerUi = require('swagger-ui-express');
const fs = require('fs');
// const swaggerDocument = yaml.load(fs.readFileSync('./openapi.yml'));
const swaggerDocument = JSON.parse(fs.readFileSync('./openapi.json'));
app.use('/api-docs', swaggerUi.serve, swaggerUi.setup(swaggerDocument));



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
      accounts = await blockchainService.getAllAccounts();
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
// Returns the ABI JSON specification of the smart contract
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
// Invoke for reading
//
app.get('/api/contracts/:contractName/xView/:method', async (req, res) => {
  const { contractName ,  method } = req.params;
  const contractAddress = blockchainService.getContractAddress(contractName);

  try {
    // Verifica si existe el archivo ABI correspondiente al contrato
    const result = await blockchainService.call(contractName , contractAddress , method);

    res.json({ result });
  } catch (error) {
    console.error("Error reading contract:", error);
    res.status(500).json({ error: "Error writing contract", details: error.message });
  }
});

//
// Invoke for writing
//
app.post('/api/contracts/:contractName/xWrite/:method', async (req, res) => {
  const { contractName ,  method } = req.params;
  const { value } = req.body;
  const privateKey = wallet_private_key;
  const account = await blockchainService.getFirstAccount();
  const contractAddress = blockchainService.getContractAddress(contractName);

  // console.log(`Account: ${account}`);
  // console.log(`PK: ${privateKey}`);
  // console.log(`Value: ${value}`);
  if (!value || !account || !privateKey) {
    return res.status(400).json({ error: "Uno o mas parametros no se pudieron determinar: Account, PK, o valor" });
  }

  try {
    const result = await blockchainService.send(contractName , contractAddress , method , value, account, privateKey);

    res.status(200).json({
      message: "Transaccion realizada con exito",
      result: result
    });

  } catch (error) {
    console.error("Error writing contract:", error);
    res.status(500).json({ error: "Error writing contract", details: error.message });
  }
});

app.post('/api/contracts/register', async (req, res) => {
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


app.get('/api/contracts/:contractName/events/:eventName', async (req, res) => {
  const { contractName  , eventName } = req.params;
  const contractAddress = blockchainService.getContractAddress(contractName);

  try {
    const result = await blockchainService.getAllEvents(contractName,contractAddress , eventName);
    res.json({ result });
  } catch (error) {
    console.error("Problem registering contract:", error);
    res.status(500).json({ error: "Problem registering contract", details: error.message });
  }
});


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