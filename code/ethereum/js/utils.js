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



function removeUndefinedDeep(obj) {
    if (Array.isArray(obj)) {
        return obj
            .map(removeUndefinedDeep)
            .filter((item) => item !== undefined);
    } else if (obj !== null && typeof obj === 'object') {
        return Object.entries(obj).reduce((acc, [key, val]) => {
            const cleaned = removeUndefinedDeep(val);
            if (cleaned !== undefined) {
                acc[key] = cleaned;
            }
            return acc;
        }, {});
    } else if (obj !== undefined) {
        return obj;
    }
}


module.exports = {
    Utils: { getContractNames , removeUndefinedDeep }
};