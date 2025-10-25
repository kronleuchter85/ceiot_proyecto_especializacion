const express = require("express");
const bodyParser = require("body-parser");

const app = express();
const port = 3000;

const BlockchainService = require('./blockchain-service');
const blockchainService = new BlockchainService();
const { ContractService } = require('./contract-service');


const { S3Repository } = require('./s3-repository');

// 
// open api generator 
// 
const swaggerUi = require('swagger-ui-express');
const fs = require('fs');
// const swaggerDocument = yaml.load(fs.readFileSync('./openapi.yml'));
const swaggerDocument = JSON.parse(fs.readFileSync('./openapi.json'));
app.use('/api-docs', swaggerUi.serve, swaggerUi.setup(swaggerDocument));



app.use(bodyParser.json());


// app.get("/api/greetings", async (req, res) => {
//   try {
//     message = ["hola", "como", "estas?"];
//     res.json({ message });
//   } catch (err) {
//     res.status(500).json({ error: err.message });
//   }
// });

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
app.get('/api/contracts/:contractName/details', async (req, res) => {
  const { contractName } = req.params;

  try {
    const contractInfo = await ContractService.getContractDetails(contractName);

    res.json({ contractInfo });
  } catch (error) {
    console.error("Contract contractInfo not found:", error);
    res.status(500).json({ error: "Contract contractInfo not found", details: error.message });
  }
});

//
// Invoke for reading
//
app.get('/api/contracts/:contractName/xView/:method', async (req, res) => {
  const { contractName, method } = req.params;

  try {
    const result = await blockchainService.call(contractName, method);

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
  const { contractName, method } = req.params;
  const payload = req.body;

  if (!payload ) {
    return res.status(400).json({ error: "Uno o mas parametros no se pudieron determinar payload" });
  }

  try {
    const result = await blockchainService.send(contractName, method, payload);

    res.status(200).json({
      message: "Transaccion realizada con exito",
      result: result
    });

  } catch (error) {
    console.error("Error writing contract:", error);
    res.status(500).json({ error: "Error writing contract", details: error.message });
  }
});

app.post('/api/contracts/:contractName/xWriteAsync/:method', async (req, res) => {
  const { contractName, method } = req.params;
  const payload = req.body;

  if (!payload ) {
    return res.status(400).json({ error: "Uno o mas parametros no se pudieron determinar payload" });
  }

  res.status(200).json({
    message: "Transaccion en processo"
  });

  //
  // codigo asincrono para invocar a blockchain y subir el resultado al bucket
  //
  (async () => {

    try {
      const result = await blockchainService.send(contractName, method, payload);

      await S3Repository.addObject('ceiot-exploratory-robot', 'transactions', result);

    } catch (error) {
      console.error("Error writing contract:", error);
      res.status(500).json({ error: "Error writing contract", details: error.message });
    }
  })();
});

app.get('/api/contracts', async (req, res) => {

  try {
    const result = await ContractService.getRegisteredContracts();
    res.json({ result });
  } catch (error) {
    console.error("Problem getting contract:", error);
    res.status(500).json({ error: "Problem getting contract", details: error.message });
  }
});


app.get('/api/contracts/:contractName/events/:eventName', async (req, res) => {
  const { contractName, eventName } = req.params;

  try {
    const result = await blockchainService.getAllEvents(contractName, eventName);
    res.json({ result });
  } catch (error) {
    console.error("Problem registering contract:", error);
    res.status(500).json({ error: "Problem registering contract", details: error.message });
  }
});


// app.get('/api/expenses', async (req, res) => {

//   try {
//     const result = blockchainService.getExpenses();
//     res.json({ result });
//   } catch (error) {
//     console.error("Problem registering contract:", error);
//     res.status(500).json({ error: "Problem registering contract", details: error.message });
//   }
// });



app.listen(port, () => {
  console.log(`API listening on port ${port}`);
});