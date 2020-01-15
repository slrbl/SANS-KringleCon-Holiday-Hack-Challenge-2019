# Author: walid.daboubi[at]gmail.com

import json
import sys
ips=[]

def check(str,cpm):
    if str in cpm.lower():
        return True
    return False

checks = ['/bin/','<script>','</script>','/etc/passwd','select','union','/bin','python','ruby','/../../','0>&1']

lfi = ['/../../','/etc/passwd']
xss = ['<script>','</script>'] 
shell = ['/bin/bash','/bin/']
sqli = [' select ',' union ', '1=1']

checks =  xss + shell + sqli

user_agents=[]

def append(ip,list):
    if ip not in list:list.append(ip)

#components = [p['uri'],p['user_agent'],p['username'],p['host']]
ips_count = {}
ips = []
with open('http_after_jq_2.json') as json_file:
    data = json.load(json_file)
    for p in data:
        if p['id.orig_h'] not in ips_count:
            ips_count[p['id.orig_h']] = 1
        else:
            ips_count[p['id.orig_h']] = +1

        if 1==1:    
            if check('/etc/passwd',p['uri']):
                append(p['id.orig_h'],ips)
                append(p['user_agent'],user_agents)
                print p['user_agent']

            if check('<script>',p['uri']):
                append(p['id.orig_h'],ips)
                print p['user_agent']
                append(p['user_agent'],user_agents)

            if check('</script>',p['uri']):
                append(p['id.orig_h'],ips)
                print p['user_agent']
                append(p['user_agent'],user_agents)

            if check('union',p['uri']):
                print p['user_agent']
                append(p['id.orig_h'],ips)
                append(p['user_agent'],user_agents)

            if check(' select ',p['uri']):
                print p['user_agent']
                append(p['id.orig_h'],ips)
                append(p['user_agent'],user_agents)

            if check('/bin/cat',p['uri']):
                print p['user_agent']
                append(p['id.orig_h'],ips)
                append(p['user_agent'],user_agents)

            # HOST
            if check('/etc/passwd',p['host']):
                append(p['id.orig_h'],ips)
                append(p['user_agent'],user_agents)
                print p['user_agent']

            if check('<script>',p['host']):
                append(p['id.orig_h'],ips)
                print p['user_agent']
                append(p['user_agent'],user_agents)

            if check('</script>',p['host']):
                append(p['id.orig_h'],ips)
                print p['user_agent']
                append(p['user_agent'],user_agents)

            if check('union',p['host']):
                print p['user_agent']
                append(p['id.orig_h'],ips)
                append(p['user_agent'],user_agents)

            if check(' select ',p['host']):
                print p['user_agent']
                append(p['id.orig_h'],ips)
                append(p['user_agent'],user_agents)

            if check('/bin/cat',p['host']):
                print p['user_agent']
                append(p['id.orig_h'],ips)
                append(p['user_agent'],user_agents)



            # USERNAME
            if check('or ',p['username']):
                print "%%%%%%%%%%%%%%%%%%%%%%%"
                print p['user_agent']
                print "%%%%%%%%%%%%%%%%%%%%%%%"
                append(p['id.orig_h'],ips)
                #append(p['user_agent'],user_agents)

            # USER_AGENT
            if check('/etc/passwd',p['user_agent']):
                append(p['id.orig_h'],ips)
                #append(p['user_agent'],user_agents)
                print p['user_agent']

            if check('<script>',p['user_agent']):
                append(p['id.orig_h'],ips)
                print p['user_agent']
                #append(p['user_agent'],user_agents)

            if check('</script>',p['user_agent']):
                append(p['id.orig_h'],ips)
                print p['user_agent']
                #append(p['user_agent'],user_agents)

            if check('union',p['user_agent']):
                print p['user_agent']
                append(p['id.orig_h'],ips)
                #append(p['user_agent'],user_agents)

            if check(' select ',p['user_agent']):
                print p['user_agent']
                append(p['id.orig_h'],ips)
                #append(p['user_agent'],user_agents)

            if check('/bin/',p['user_agent']):
                print p['user_agent']
                append(p['id.orig_h'],ips)
                #append(p['user_agent'],user_agents)


print "============================"
for a in user_agents:
    print a
print "============================"

with open('http_after_jq_2.json') as json_file:
    data = json.load(json_file)
    for p in data:
        if p['user_agent'] in user_agents:
            append(p['id.orig_h'],ips)


print len(ips)
print ",".join(ips)
