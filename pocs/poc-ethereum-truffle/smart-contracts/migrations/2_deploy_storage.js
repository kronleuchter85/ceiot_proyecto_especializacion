const axios = require('axios');

const { getContractNames } = require('./deploy_utils');

const CONTRACTS_REGISTRY_URL = 'http://poc-ethereum-dapp:3000/api/contracts/register';

module.exports = async function (deployer , network) {

    try{


        let contractNames = getContractNames('../contracts');

        for(const contractName of contractNames){

            try {
                const Contract = artifacts.require(contractName);
                await deployer.deploy(Contract); 
                console.log(`Contrato ${contractName} desplegado con éxito`);

                let contractInstance = await Contract.deployed();
                console.log(`Depliegue de contrato ${contractName} en ${contractInstance.address}`);

                console.log(`Registro de Storage en la Registry`);
                
                await axios.post( CONTRACTS_REGISTRY_URL , {
                    contractName: contractName,
                    contractAddress: contractInstance.address
                });

            } catch (error) {
                console.error(`Error desplegando el contrato ${contractName}:`, error);
            }
        }


    }catch(error){
        console.error("Error durante el despliegue:", error);
    }

};