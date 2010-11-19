#!/usr/bin/python

import os

res = list()
maxLines = 0
for filename in os.listdir('data'):
  file = open('data/' + filename, 'r')
  
  line = file.readline().replace('.',',')
  lineId = 0
  fileres = list()
  while line:
    lineId = lineId + 1
    line = file.readline().strip().replace('.',',').split(';')[0]
    fileres.append(line)
  res.append(fileres)
  if lineId > maxLines:
    maxLines = lineId - 1

for i in range(0, maxLines):
  resLine = ' '
  for j in res:
    if i < len(j):
      resLine += j[i]
    else:
      resLine += ' '
    resLine += ';'
  print resLine