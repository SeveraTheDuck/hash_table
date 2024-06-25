import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys

PRORAM_NAME_INDEX    = 0
OUTPUT_FOLDER_INDEX  = 1
DATA_FILE_NAME_INDEX = 2

data_file_name = sys.argv[DATA_FILE_NAME_INDEX]
hash_function_name = data_file_name[data_file_name.find('/') + 1 : data_file_name.find('.')]

data = pd.read_csv(data_file_name, sep='\s+', header=None)

data['bucket index'].hist()
plt.xlabel('Bucket index')
plt.ylabel('Elements number')
plt.title(hash_function_name)
