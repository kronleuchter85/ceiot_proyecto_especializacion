import json
import urllib.request

import boto3
import uuid
from datetime import datetime

s3 = boto3.client('s3')
sqs = boto3.client('sqs')


def lambda_handler(event, context):
    
    try:
            
        for record in event['Records']:
            
            body = record['body']

            if isinstance(body, str):
                payload = json.loads(body)
            else:
                payload = body  

            print('Payload: ', payload)

            #
            # tx
            #
            tx_message = payload['tx']
            tx_hash = tx_message['txReceipt']['transactionHash']

            s3.put_object(
                Bucket='ceiot-exploratory-robot',  
                Key=f"transactions/tx-{tx_hash}.json",
                Body=json.dumps(tx_message),
                ContentType='application/json'
            )

            #
            # reading
            #
            reading_message = payload['reading']
            reading_id = str(uuid.uuid4())
            reading_message['id'] = reading_id
            reading_message['tx_id'] = tx_hash

            s3.put_object(
                Bucket='ceiot-exploratory-robot',  
                Key=f"readings/rd-{reading_id}.json",
                Body=json.dumps(reading_message),
                ContentType='application/json'
            )

        return {
            'statusCode': 200,
            'body': json.dumps('All good!')
        }
        
    except Exception as e:
        print(e)
        
        return {
            'statusCode': 500,
            'body': json.dumps(f"Error: {e}")
        }

