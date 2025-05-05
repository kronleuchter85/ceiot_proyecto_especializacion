



const {Repository} = require('./repository');


// function registerContract(contractName , contractAddress){
//     console.log(`Adding ${contractName}: ${contractAddress} to the Registry`);
//     this.contracts.set(contractName , contractAddress);
//     console.log('Current Registry:', Array.from(this.contracts.entries()));
// }

// function getRegisteredContracts(){
//     console.log('Retrieving Registry:', Array.from(this.contracts.entries()));
//     return Array.from(this.contracts.entries());
// }

async function getContractDetails(contractName){

    let contractInfo = await Repository.getEntity('contracts' , {contractName: contractName });

    return contractInfo;
}
