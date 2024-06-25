import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys
from matplotlib.pyplot import figure


PRORAM_NAME_INDEX    = 0
OUTPUT_FOLDER_INDEX  = 1
DATA_FILE_NAME_INDEX = 2

X_PLOT_PADDING = 20
Y_PLOT_PADDING = 5

figure(figsize=(15, 10), dpi=240, edgecolor='black')

data_file_name = sys.argv[DATA_FILE_NAME_INDEX]
hash_function_name = data_file_name[data_file_name.find('/') + 1 : data_file_name.find('.')]

data = pd.read_csv(data_file_name, sep='\\s+', header=None)
plt.bar (data[0], data[1], color='blue')

plt.xlim([-X_PLOT_PADDING, max(data[0]) + X_PLOT_PADDING])
plt.ylim([0, max(data[1]) + Y_PLOT_PADDING])

plt.title(hash_function_name)
plt.xlabel("Bucket index")
plt.ylabel("Elements number")
plt.savefig(sys.argv[OUTPUT_FOLDER_INDEX] + hash_function_name + ".png")
