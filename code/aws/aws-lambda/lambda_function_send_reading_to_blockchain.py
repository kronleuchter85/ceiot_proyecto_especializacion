import json
import urllib.request
import os
import boto3
import uuid
from datetime import datetime


QUEUE_URL = os.environ['QUEUE_URL']
DAPP_ENDPOINT_URL = os.environ['DAPP_ENDPOINT_URL']


s3 = boto3.client('s3')
sqs = boto3.client('sqs')


def lambda_handler(event, context):
    
    try:
            
        for record in event['Records']:
            
            body = record['body']

            if isinstance(body, str):
                outer_body = json.loads(body)
            else:
                outer_body = body  

            message_str = outer_body['Message']
            reading_message = json.loads(message_str)
            print(reading_message)

            payload = {
                "date": reading_message['date'],
                "time": reading_message['time'],
                "values": [
                    reading_message['date'],
                    reading_message['time'],                   
                    reading_message['deviceId'],
                    reading_message['geoLat'],
                    reading_message['geoLong'],
                    reading_message['type'],
                    reading_message['value']
                ]
            }
            data = json.dumps(payload).encode('utf-8')
            req = urllib.request.Request(
                DAPP_ENDPOINT_URL,
                data=data,
                headers={'Content-Type': 'application/json'},
                method='POST'
            )
            
            with urllib.request.urlopen(req) as response:
                response_data = response.read()
                print("Respuesta:", response_data)

                response_json = json.loads(response_data)
                tx_result_message = response_json['result']  

                message_body = {
                    "reading": reading_message,
                    "tx": tx_result_message
                }

                response = sqs.send_message(
                    QueueUrl=QUEUE_URL,
                    MessageBody=json.dumps(message_body)
                )


        return {
            'statusCode': 200,
            'body': json.dumps('Hello from Lambda!')
        }
        
    except Exception as e:
        print(e)
        
        return {
            'statusCode': 500,
            'body': json.dumps(f"Error: {e}")
        }

