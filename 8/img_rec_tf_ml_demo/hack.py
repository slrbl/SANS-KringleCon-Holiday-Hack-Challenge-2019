import base64
import requests
import json

response = requests.post("https://fridosleigh.com/api/capteha/request")
response_js = json.loads(response.text)


requested_elements = response_js['select_type']

def convert_and_save_image(base46_data,id):
	imgdata = base64.b64decode(base46_data)
	filename = './unknown_images/'+str(id)+'.png'
	with open(filename, 'wb') as f:
	    f.write(imgdata)


for image in response_js['images']:
	print (image['uuid'])
	print (image['base64'])
	convert_and_save_image(image['base64'],image['uuid'])