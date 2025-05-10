

const { DynamoRepository } = require('./dynamo-repository');


// function registerContract(contractName , contractAddress){
//     console.log(`Adding ${contractName}: ${contractAddress} to the Registry`);
//     this.contracts.set(contractName , contractAddress);
//     console.log('Current Registry:', Array.from(this.contracts.entries()));
// }

async function getContractDetails(contractName) {
    const contractInfo = await DynamoRepository.getEntity('contracts', { contractName: contractName });
    return contractInfo;
}



async function getRegisteredContracts() {
    return await DynamoRepository.getAllEntities('contracts');
}


module.exports = {
    ContractService: { getRegisteredContracts, getContractDetails }
};