const { DynamoDBClient, PutItemCommand } = require('@aws-sdk/client-dynamodb');
const { DynamoDBDocumentClient, PutCommand, GetCommand } = require('@aws-sdk/lib-dynamodb');

const fs = require('fs');
const {Utils} = require('./utils');

const client = new DynamoDBClient({ region: 'eu-west-2' });
const docClient = DynamoDBDocumentClient.from(client, {
    removeUndefinedValues: true, // 👈 Esta línea es crucial
});


async function getEntity(entityName, entityFilter) {

    const params3 = {
        TableName: entityName,
        Key: entityFilter,
    };

    try {
        const result = await docClient.send(new GetCommand(params3));
        if (result.Item) {
            console.log('✅ Objeto obtenido:', result.Item);
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

async function saveEntity(entityName, entity) {

    const cleanedItem = Utils.removeUndefinedDeep(entity);

    const command = new PutCommand({
        TableName: entityName,
        Item: cleanedItem,
    });

    try {
        const result = await docClient.send(command);
        console.log('✅ Insertado con éxito:', result);
    } catch (err) {
        console.error('❌ Error al insertar:', err);
    }
}


module.exports = {
    Repository: { saveEntity, getEntity }
};