import json
import urllib.request

import boto3
import uuid
from datetime import datetime

s3 = boto3.client('s3')
contractName = 'EnvironmentalData2'
method = 'recordReading'
url = f"https://s4c8dgrvny.eu-west-2.awsapprunner.com/api/contracts/{contractName}/xWrite/{method}"

def lambda_handler(event, context):
    
    try:
            
        for record in event['Records']:
            
            body = record['body']

            if isinstance(body, str):
                outer_body = json.loads(body)
            else:
                outer_body = body  

            message_str = outer_body['Message']
            message_data = json.loads(message_str)
            print(message_data)

            payload = {
                "date": message_data['date'],
                "time": message_data['time'],
                "values": [
                    message_data['date'],
                    message_data['time'],
                    message_data['deviceId'],
                    message_data['geoLat'],
                    message_data['geoLong'],
                    message_data['temperature'],
                    message_data['humidity'],
                    message_data['light'],
                    message_data['pressure'] 
                ]
            }
            data = json.dumps(payload).encode('utf-8')
            req = urllib.request.Request(
                url,
                data=data,
                headers={'Content-Type': 'application/json'},
                method='POST'
            )
            
            with urllib.request.urlopen(req) as response:
                response_data = response.read()
                print("Respuesta:", response_data)

                response_json = json.loads(response_data)
                value = response_json['result']  
                
                filename = f"tx-{uuid.uuid4()}.json"

                s3.put_object(
                    Bucket='ceiot-exploratory-robot',  
                    Key=f"transactions/{filename}",
                    Body=json.dumps(value),
                    ContentType='application/json'
                )

        return {
            'statusCode': 200,
            'body': json.dumps('Hello from Lambda!')
        }
        
    except Exception as e:
        print(e)
        
        return {
            'statusCode': 500,
            'body': json.dumps('Error!', e)
        }

