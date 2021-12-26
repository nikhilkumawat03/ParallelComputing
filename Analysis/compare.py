import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

df1 = pd.read_csv('stats', header=None)
df2 = pd.read_csv('openMPStats', header=None)

matrixSizeList = list(dict.fromkeys(df1[0]))

pThreadTime = []
openMPTime = []

serialTime = list(dict.fromkeys(df2[3]))

for index, row in df1.iterrows():
	if index % 4 == 3:
		pThreadTime.append(row[2])
		
for index, row in df2.iterrows():
	if index % 4 == 3:
		openMPTime.append(row[2])
		
plt.plot(matrixSizeList, serialTime, label="Serial Time")
plt.plot(matrixSizeList, pThreadTime, label="pthread Time")
plt.plot(matrixSizeList, openMPTime, label="openMP Time")

plt.title("Serial v/s pthread v/s openmp")
plt.xlabel("Matrix Size")
plt.ylabel("Execution Time")
plt.legend()
plt.show()
