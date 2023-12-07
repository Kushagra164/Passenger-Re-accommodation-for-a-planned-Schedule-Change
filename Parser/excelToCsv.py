# Install Pandas Library
# !pip install pandas
import pandas
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("input_file_path", required=True)
parser.add_argument("output_file_path", required=True)
args = parser.parse_args()

excelFilePath = args.input_file_path
csvFilePath = args.output_file_path

readFile = pandas.read_excel(excelFilePath)
readFile.to_csv(csvFilePath)