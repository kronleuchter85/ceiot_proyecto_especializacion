
const {Web3} = require("web3");
const fs = require('fs');
const path = require('path');
const ABI_DIR = '/app/abi';

class BlockchainService {

    constructor(providerUrl){
        this.web3 = new Web3(providerUrl);
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

    async invoke(contractName, contractAddress, methodName){
        const abi = this.getAbi(contractName);


        console.log("ABI:", abi); // Debería imprimir un array
        console.log("Tipo de ABI:", typeof abi); // Debería ser 'object'
        console.log("Es un array:", Array.isArray(abi)); // Debería ser true

        const contract = new this.web3.eth.Contract(abi, contractAddress);
        // const account = await getAccounts()[0];
        const result = await contract.methods[methodName].call();

        return result;
    }

}


module.exports = BlockchainService;

// Configura el proveedor y dirección del contrato
// const contractABI = require("./contractABI.json");
// const contractAddress = process.env.CONTRACT_ADDRESS;
// const contract = new web3.eth.Contract(contractABI, contractAddress);



