# movies=["Don Jon","Avengers","LUCY"];
# for movie in movies:
#   print movie

import httplib,time
ISOTIMEFORMAT="%Y-%m-%d %X"
conn=httplib.HTTPConnection('baike.baidu.com')
conn.request("GET","/subview/2321/5786291.htm")
r1=conn.getresponse()
print r1.status
data1=r1.read()
print data1
print time.strftime(ISOTIMEFORMAT,time.localtime())