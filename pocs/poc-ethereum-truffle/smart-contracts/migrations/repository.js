const { DynamoDBClient, PutItemCommand } = require('@aws-sdk/client-dynamodb');
const { DynamoDBDocumentClient, PutCommand , GetCommand} = require('@aws-sdk/lib-dynamodb');

const fs = require('fs');

const client = new DynamoDBClient({ region: 'eu-west-2'});
const docClient = DynamoDBDocumentClient.from(client, {
    removeUndefinedValues: true, // 👈 Esta línea es crucial
});


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

async function getEntity(entityName , entityFilter){

    const params3 = {
        TableName: entityName,
        Key: entityFilter,
    };

    try {
        const result = await docClient.send(new GetCommand(params3));
        if (result.Item) {
            console.log('✅ Objeto obtenido:', result.Item);
            // fs.writeFileSync('/app/build/contracts/from-dynamo.json', JSON.stringify(result.Item, null, 2), 'utf8');
            return result.Item;
        } else {
            console.log('⚠️ Objeto no encontrado');
            return null;
        }
    } catch (err) {
        console.error('❌ Error al obtener el objeto:', err);
        throw err;
    }
}

async function saveEntity(entityName , entity) {


    // const cleanAbi = JSON.parse(JSON.stringify(contractInformation.contractABI, (_, value) => {
    //     if (typeof value === 'bigint') return value.toString();
    //     if (typeof value === 'function' || typeof value === 'undefined') return undefined;
    //     return value;
    // }));


    // const params = {
    //     TableName: 'contracts',

        // Item: {
        //     name: { S: contractInformation.contractName},
        //     contractAddress: { S:contractInformation.contractName },
        //     contractTransactionHash: { S: contractInformation.contractTransactionHash },
        //     contractABI: { S: JSON.stringify(contractInformation.contractABI) },
        //     contractBlock: { N: contractInformation.contractBlockNumber }
        //   }
    //     Item: {
    //         name: contractInformation.contractName,
    //         contractAddress: contractInformation.contractAddress,
    //         contractABI: contractInformation.contractABI,
    //         contractTransactionHash: contractInformation.contractTransactionHash,
    //         contractBlock: contractInformation.contractBlockNumber,
    //     }
    // };

    // fs.writeFileSync('/app/build/contracts/original.json', JSON.stringify(contractInformation.contractABI, null, 2), 'utf8');

    // console.log('----------------------------------------------------------------------------------------------------------------')
    // const cleanAbi2 = JSON.parse(JSON.stringify(contractInformation.contractABI, (_, value) => {
    //     if (typeof value === 'bigint') return value.toString();
    //     if (typeof value === 'function' || typeof value === 'undefined') return undefined;
    //     return value;
    // }));

    // console.log(JSON.stringify(cleanAbi2, null, 2));
    // console.log('----------------------------------------------------------------------------------------------------------------')

    
    const cleanedItem = removeUndefinedDeep(entity);

    // fs.writeFileSync('/app/build/contracts/cleansed.json', JSON.stringify(cleanedItem.contractABI, null, 2), 'utf8');


    console.log('----------------------------------------------------------------------------------------------------------------')

    // const cleanAbi3 = JSON.parse(JSON.stringify(cleanedItem.contractABI, (_, value) => {
    //     if (typeof value === 'bigint') return value.toString();
    //     if (typeof value === 'function' || typeof value === 'undefined') return undefined;
    //     return value;
    // }));

    console.log(cleanedItem);
    console.log('----------------------------------------------------------------------------------------------------------------')

    const command = new PutCommand({
        TableName: entityName,
        Item: cleanedItem,
    });

    // console.log(params);

    try {
        // const command = new PutCommand(params);
        // const command = new PutItemCommand(params);
        const result = await docClient.send(command);
        console.log('✅ Insertado con éxito:', result);
    } catch (err) {
        console.error('❌ Error al insertar:', err);
    }



}


module.exports = { saveEntity , getEntity };