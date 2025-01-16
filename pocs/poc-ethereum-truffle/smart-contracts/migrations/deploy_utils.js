const path = require('path');
const fs = require('fs');

function getContractNames(contractsFolder) {
    const contractsDir = path.join(__dirname, contractsFolder); 
    const contractFiles = fs.readdirSync(contractsDir);

    let contracts = [];
    for (const file of contractFiles) {
        if (file.endsWith('.sol')) { 
            const contractName = path.basename(file, '.sol'); 
            console.log(`Contrato: ${contractName}`);

            contracts.push(contractName);
        }
    }

    return contracts;
}

module.exports = { getContractNames };