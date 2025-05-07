

const { Repository } = require('./repository');


// function registerContract(contractName , contractAddress){
//     console.log(`Adding ${contractName}: ${contractAddress} to the Registry`);
//     this.contracts.set(contractName , contractAddress);
//     console.log('Current Registry:', Array.from(this.contracts.entries()));
// }

async function getContractDetails(contractName) {
    const contractInfo = await Repository.getEntity('contracts', { contractName: contractName });
    return contractInfo;
}



async function getRegisteredContracts() {
    return await Repository.getAllEntities('contracts');
}


module.exports = {
    ContractService: { getRegisteredContracts, getContractDetails }
};