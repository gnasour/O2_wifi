import random
import requests
from datetime import datetime

def send_post(heart_rate, spo2, id = 1):
        spo2 = random.randrange(97,100)
        api_file = open("API_INFO")
        header = {'X-Api-Key': api_file.readline(), 
        'Content-Type': 'application/json', 
        'Content-Length': '106'}
        
        jso = {'heart_rate': str(heart_rate),
                'sp02': str(spo2),
                'temp': '103.1',
                'recorded_time': datetime.now().strftime('%Y-%m-%dT%H:%M:%SZ'),
                'patient_rec': str(id)}

        r = requests.post(api_file.readline(), json=jso, headers = header)

