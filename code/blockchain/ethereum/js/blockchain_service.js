// const axios = require('axios');

const { Repository } = require('./repository');
const { ContractService } = require('./contract_service');
const HDWalletProvider = require('@truffle/hdwallet-provider');
const { Web3 } = require("web3");
const ExpenseService = require('./expenses_service');


class BlockchainService {

    constructor(providerUrl) {

        console.log(`Mnemonic: ${process.env.MNEMONIC}`);
        console.log(`Provider URL: ${providerUrl}`);

        if (process.env.MNEMONIC == undefined) {
            console.log('Initializing Dev provider for Ganache...');
            this.web3 = new Web3(providerUrl);
        } else {

            console.log('Initializing Network provider for Test Network...');

            const provider = new HDWalletProvider({
                mnemonic: {
                    phrase: process.env.MNEMONIC
                },
                providerOrUrl: providerUrl
            });
            this.web3 = new Web3(provider);
        }

        this.contracts = new Map();
        this.expenseService = new ExpenseService(providerUrl);
    }



    async getFirstAccount() {
        const accounts = await this.getAllAccounts();
        return accounts[0];
    }

    async getAllAccounts() {
        const accounts = await this.web3.eth.getAccounts();
        return accounts;
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
                    block.transactions.map(async (tx) => {
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

    async call(contractName, methodName) {
        const contractInfo = await ContractService.getContractDetails(contractName);
        const contractAddress = contractInfo.contractAddress;
        const abi = contractInfo.contractABI;

        const contract = new this.web3.eth.Contract(abi, contractAddress);
        const result = await contract.methods[methodName]().call();
        const sanitizedResult = JSON.parse(JSON.stringify(result, (key, value) =>
            typeof value === 'bigint' ? value.toString() : value
        ));
        console.log(`Resultado:`, sanitizedResult);
        return sanitizedResult;
    }

    async send(contractName, methodName, values, account, privateKey) {

        const nonce = await this.web3.eth.getTransactionCount(account, 'pending');
        const contractInfo = await ContractService.getContractDetails(contractName);
        const contractAddress = contractInfo.contractAddress;
        const abi = contractInfo.contractABI;

        console.log('-----------------------------------------------------------------------------------------');
        console.log(`Network Nonce: ${nonce}`);
        console.log(`Invoking method: ${contractName}.${methodName}( ${values} )`);
        console.log(`Contract address: ${contractAddress}`);
        // console.log(`Contract account: ${account}`);
        console.log(`Contract ABI:`,abi);

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
            nonce: nonce
        };
        
        console.log(`TxData:`,txData);
        
        try {
            
            const signedTx = await this.web3.eth.accounts.signTransaction(txData, privateKey);
            console.log(`Signed TX:`,signedTx);
            
            let receipt = await this.web3.eth.sendSignedTransaction(signedTx.rawTransaction);
            
            receipt = JSON.parse(JSON.stringify(receipt, (key, value) =>
                typeof value === 'bigint' ? value.toString() : value
        ));
        
        console.log(`Receipt:`, receipt);
        
        const gasUsed = receipt['gasUsed'];
        const expenseInfo = this.expenseService.getExpenseInformation(contractName, methodName, gasUsed, gasPrice);
        
        this.expenseService.addExpenseInformation(expenseInfo);
        
        let result = {
            contractName: contractName,
            methodName: methodName,
            txReceipt: receipt,
            txExpense: expenseInfo
        }
        
        console.log(`Transacción finalizada con éxito:`, result);
        console.log('-----------------------------------------------------------------------------------------');
        return result;
        
    } catch (error) {
        console.error(`Error al invocar el método de escritura:`, error);
            throw error;
        }
    }
    
    
    
    async getAllEvents(contractName, eventName) {
        
        const contractInfo = await Repository.getEntity('contracts', { contractName: contractName });
        const contractABI = contractInfo.contractABI;
        const contractAddress = contractInfo.contractAddress;
        const contractDeploymentBlockNumber = contractInfo.contractBlockNumber;
        const myContract = new this.web3.eth.Contract(contractABI, contractAddress);
        
        const events = await myContract.getPastEvents(eventName, {
            fromBlock: contractDeploymentBlockNumber,
            toBlock: 'latest',
        });
        
        const sanitizedEvents = JSON.parse(JSON.stringify(events, (key, value) =>
            typeof value === 'bigint' ? value.toString() : value
        ));
        return sanitizedEvents;
    }


}


module.exports = BlockchainService;



