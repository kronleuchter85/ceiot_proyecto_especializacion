// const axios = require('axios');
const {Web3} = require("web3");
const ExpenseService = require('./expenses_service'); 
const fs = require('fs');
const path = require('path');

const ABI_DIR = '/app/abi';




class BlockchainService {

    constructor(providerUrl){
        this.web3 = new Web3(providerUrl);
        this.contracts = new Map();
        this.expenseService = new ExpenseService(providerUrl);
    }

    async getAccounts(){
        return await this.web3.eth.getAccounts();
    }

    async getLastNBlocks(n) {
        const latestBlockNumber = await this.web3.eth.getBlockNumber(); // Obtiene el número del último bloque
        const blockNumbers = Array.from({ length: n }, (_, i) => Number(latestBlockNumber) - i).filter(num => num >= 0);

        const blocks = await Promise.all(
            blockNumbers.map(async (num) => {
                const block = await this.web3.eth.getBlock(num);
                return JSON.parse(
                    JSON.stringify(block, (key, value) => (typeof value === 'bigint' ? value.toString() : value))
                );
            })
        );
        return blocks;
    }

    async getTransactionsFromBlock(blockNumber) {
        try {
            const block = await this.web3.eth.getBlock(blockNumber, true);

            if (block && block.transactions) {
            
            const transactions = await Promise.all(
                block.transactions.map( async (tx) => {
                return JSON.parse(
                    JSON.stringify(tx, (key, value) => (typeof value === 'bigint' ? value.toString() : value))
                );
                }) 
            );
            return transactions;
            } else {
            console.log(`No se encontraron transacciones en el bloque ${blockNumber}`);
            return [];
            }
        } catch (error) {
            console.error(`Error al obtener las transacciones del bloque ${blockNumber}:`, error);
            return [];
        }
    }

    getAbi(contractName){
        return JSON.parse(fs.readFileSync(path.join(ABI_DIR, contractName + '.json'), 'utf8')).abi;
    }

    async call(contractName, contractAddress, methodName){
        const abi = this.getAbi(contractName);

        const contract = new this.web3.eth.Contract(abi, contractAddress);
        const result = await contract.methods[methodName]().call();
        const sanitizedResult = JSON.parse(JSON.stringify(result, (key, value) => 
            typeof value === 'bigint' ? value.toString() : value
          ));
        console.log(`Resultado:`, sanitizedResult);
        return sanitizedResult;
    }

    async send(contractName, contractAddress, methodName , values , account , privateKey){


        console.log(`Invoking method: ${contractName}.${methodName}(${values})`);
        console.log(`Contract address: ${contractAddress})`);
        console.log(`Contract account: ${account})`);


        const abi = this.getAbi(contractName);
        const contract = new this.web3.eth.Contract(abi, contractAddress);
        const tx = contract.methods[methodName](...values);
        const estimatedGas = await tx.estimateGas({ from: account });
        const gasPrice = await this.web3.eth.getGasPrice();
        
        const txData = {
          from: account,
          to: contractAddress,
          gas: estimatedGas,
          gasPrice: gasPrice,
          data: tx.encodeABI(), 
        };

        console.log(`TxData: ${txData}`);

        try{
            
            const signedTx = await this.web3.eth.accounts.signTransaction(txData, privateKey);
            const receipt = await this.web3.eth.sendSignedTransaction(signedTx.rawTransaction);
    
            const sanitizedReceipt = JSON.parse(JSON.stringify(receipt, (key, value) => 
                typeof value === 'bigint' ? value.toString() : value
              ));
            console.log(`Transacción enviada con éxito:`, sanitizedReceipt);

            const gasUsed = sanitizedReceipt['gasUsed'];

            this.expenseService.addExpense(contractName , methodName , gasUsed , gasPrice);

            return sanitizedReceipt;

        } catch (error) {
            console.error(`Error al invocar el método de escritura:`, error);
            throw error;
        }
    }






    registerContract(contractName , contractAddress){
        console.log(`Adding ${contractName}: ${contractAddress} to the Registry`);
        this.contracts.set(contractName , contractAddress);
        console.log('Current Registry:', Array.from(this.contracts.entries()));
    }

    getRegisteredContracts(){
        console.log('Retrieving Registry:', Array.from(this.contracts.entries()));
        return Array.from(this.contracts.entries());
    }

    getContractAddress(contractName){
        return this.contracts.get(contractName);
    }


    async getAllEvents(contractName , contractAddress , eventName){

        const contractABI = this.getAbi(contractName);
        const myContract = new this.web3.eth.Contract(contractABI, contractAddress);

        const events = await myContract.getPastEvents(eventName, {
            fromBlock: 0, 
            toBlock: 'latest', 
        });

        const sanitizedEvents = JSON.parse(JSON.stringify(events, (key, value) => 
            typeof value === 'bigint' ? value.toString() : value
          ));
        return sanitizedEvents;
    }


    getExpenses(){
        return this.expenseService.getExpenses();
    }
    
}


module.exports = BlockchainService;



