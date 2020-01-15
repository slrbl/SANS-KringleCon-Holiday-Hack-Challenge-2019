import requests

s = requests.Session()

s.get("https://studentportal.elfu.org/check.php")

response=s.get("https://studentportal.elfu.org/validator.php",headers={'Referer': "https://studentportal.elfu.org/check.php",'Host':'studentportal.elfu.org'})
token=response.text
print (token)
import time
time.sleep(1)
response=s.get(url="https://studentportal.elfu.org/application-check.php",headers={'Referer': "https://studentportal.elfu.org/check.php",'Host':'studentportal.elfu.org'},data={'elfmail':"test@test.com",'token':str(token)})
print ("Bingo")
print (response.text)
