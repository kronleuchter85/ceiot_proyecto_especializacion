const { DynamoDBClient, PutItemCommand } = require('@aws-sdk/client-dynamodb');

const client = new DynamoDBClient({
  region: 'eu-west-2', 
});

const params = {
  TableName: 'contracts',
  Item: {
    userId: { S: '123' },
    name: { S: 'Juan Pérez' },
    age: { N: '30' },
    active: { BOOL: true }
  }
};

async function save(contract) {
  try {
    const command = new PutItemCommand(params);
    const result = await client.send(command);
    console.log('✅ Insertado con éxito:', result);
  } catch (err) {
    console.error('❌ Error al insertar:', err);
  }
}

insertarDato();
