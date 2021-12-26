import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('openMPStats', header=None)
#Data preprocessing
thread1 = []
thread2 = []
thread3 = []
thread4 = []
for index, row in df.iterrows():
	if index % 4 == 0:
		thread1.append(row[2])
	elif index % 4 == 1:
		thread2.append(row[2])
	elif index % 4 == 2:
		thread3.append(row[2])
	else:
		thread4.append(row[2])
matrixSizeList = list(dict.fromkeys(df[0]))
serialTime = list(dict.fromkeys(df[3]))

#Plotting bar chart
plotData = pd.DataFrame({"thread1": thread1, "thread2": thread2, 
						  "thread3": thread3, "thread4": thread4, "Serial Time": serialTime}, matrixSizeList)

#Plotting bar chart
#plotData = pd.DataFrame({"thread1": thread1, "thread2": thread2, 
#						  "thread3": thread3, "thread4": thread4}, matrixSizeList)


plotData.plot.bar(width=0.5)
plt.grid(color='#95a5a6', linestyle='--', linewidth=1, axis='y', alpha=0.7)
plt.title("open MP Parallel v/s Serial")
plt.xlabel("Matrix Size")
plt.ylabel("Execution Time")
plt.show()
