# Install Pandas Library
# !pip install pandas
import pandas
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--input_file_path", required=True, help="input file")
parser.add_argument("--output_file_path", required=True, help="output file")
args = parser.parse_args()

excelFilePath = args.input_file_path
csvFilePath = args.output_file_path

readFile = pandas.read_excel(excelFilePath)

string_col = readFile.select_dtypes(['object'])
readFile[string_col.columns] = string_col.applymap(lambda x: x.strip() if isinstance(x, str) else x)

readFile.to_csv(csvFilePath, index = False, date_format='%m-%d-%Y %H:%M')