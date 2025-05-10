// const axios = require('axios');

// const pretty = require('json-stringify-safe');
// const { getContractNames } = require('./deploy_utils');
const { DynamoRepository } = require('./dynamo-repository');
const { S3Repository } = require('./s3-repository');
const {Utils} = require('./utils');
const { format } = require("date-fns");

// const CONTRACTS_REGISTRY_URL = 'http://poc-ethereum-dapp:3000/api/contracts/register';

module.exports = async function (deployer, network) {

    try {

        let contractNames = Utils.getContractNames('../contracts');

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
                    contractBlockNumber: receipt.blockNumber,
                    network: network,
                    timestamp: format(new Date(), "yyyyMMdd-HHmmss")
                }

                console.log(`Registro de Storage en la Registry`);

                await DynamoRepository.saveEntity('contracts' ,contractInfo);
                await S3Repository.addObject('ceiot-exploratory-robot' , 'contracts' , contractInfo);


            } catch (error) {
                console.error(`Error desplegando el contrato ${contractName}:`, error);
            }
        }


    } catch (error) {
        console.error("Error durante el despliegue:", error);
    }

};