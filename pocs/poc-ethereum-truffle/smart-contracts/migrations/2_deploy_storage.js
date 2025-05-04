const axios = require('axios');

const pretty = require('json-stringify-safe');
const { getContractNames } = require('./deploy_utils');

const CONTRACTS_REGISTRY_URL = 'http://poc-ethereum-dapp:3000/api/contracts/register';

module.exports = async function (deployer, network) {

    try {


        let contractNames = getContractNames('../contracts');

        for (const contractName of contractNames) {

            try {
                const Contract = artifacts.require(contractName);
                await deployer.deploy(Contract);
                console.log(`Contrato ${contractName} desplegado con éxito`);

                let contractInstance = await Contract.deployed();

                const receipt = await web3.eth.getTransactionReceipt(Contract.transactionHash);

                let contractInfo = {
                    contractName: contractName,
                    contractAddress: Contract.address,
                    contractABI: contractInstance.abi ,
                    contractTransactionHash: Contract.transactionHash,
                    contractBlock: receipt.blockNumber
                }

                console.log(`Detalles del Depliegue de`);
                console.log(contractInfo);
      

                // console.log(`Detalles del Depliegue de ${contractName}: ${contractInstance2}`);
                // console.log(`Depliegue de contrato ${contractName} en ${contractInstance.address}`);

                console.log(`Registro de Storage en la Registry`);

                await axios.post(CONTRACTS_REGISTRY_URL, {
                    contractName: contractName,
                    contractAddress: contractInstance.address
                });

            } catch (error) {
                console.error(`Error desplegando el contrato ${contractName}:`, error);
            }
        }


    } catch (error) {
        console.error("Error durante el despliegue:", error);
    }

};