import requests
from datetime import datetime

def send_post(heart_rate, spo2, id = 1):

        header = {'X-Api-Key': 'OQi9CrIx.7jme4I0wAtgIplIp3JOWtGa5pW37AFDA', 
        'Content-Type': 'application/json', 
        'Content-Length': '106'}
        
        jso = {'heart_rate': str(heart_rate),
                'sp02': str(spo2),
                'temp': '103.1',
                'recorded_time': datetime.now().strftime('%Y-%m-%dT%H:%M:%SZ'),
                'patient_rec': str(id)}

        r = requests.post('http://72.134.81.31:25346/api/v1/patientdata/', json=jso, headers = header)

