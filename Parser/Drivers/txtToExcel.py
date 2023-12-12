import pandas
import argparse
import os

BookingHeader = ["RECLOC","CreationDate","ActionCD","ClassCD","SegSeq","PaxCnt","FlightNum","Orig_CD","Dest_CD","DepDate","DepTime","ArrDate","ArrTime"]
ScheduleHeader = ["ScheduleID","CarrierCD","FlightNum","AircraftType","AircraftTailNumber","DepartureAirport","ArrivalAirport","DepartureTime","ArrivalTime","StartDate","EndDate","Status","FrequencyPattern"]
InventoryHeader = ["InventoryID","ScheduleID","FlightNum","AircraftType","DepartureDate","ArrivalDate","DepartureAirport","ArrivalAirport","Status","RescheduledTo"]

parser = argparse.ArgumentParser()
parser.add_argument("--input_file_path", required=True, help="input file")
parser.add_argument("--output_folder", required=True, help="output file")
args = parser.parse_args()

outputFolderPath = args.output_folder
inputFilePath = args.input_file_path

inputFile = open(inputFilePath)

lines = []
for line in inputFile:
    lines.append(line)

T = int(lines[0])
def toFile(counter):
    outputFile = open("/tmp/file","w")
    while True:
        curLine = lines[counter]
        counter += 1
        if(curLine == "break\n"):
            break
        outputFile.write(curLine)
    outputFile.close()
    return counter

counter = 1
for i in range(T):

    curOutputFolderPath = outputFolderPath + "/Solution" + str(i+1)

    if not os.path.exists(curOutputFolderPath):
        os.makedirs(curOutputFolderPath)

    # Schedule

    counter = toFile(counter)
    csvFile = pandas.read_csv("/tmp/file",sep="\s+",header=None)
    csvFile.iloc[:,3]=csvFile.iloc[:,3].astype(str)+csvFile.iloc[:,4].astype(str)
    csvFile=csvFile.drop([4],axis=1)

    csvFile.to_csv("/tmp/intermediate.csv",header=ScheduleHeader,index=None)
    csvFile = pandas.read_csv("/tmp/intermediate.csv")

    csvFile.to_excel(curOutputFolderPath+"/schedule.xlsx",index=False)

    # Inventory

    counter = toFile(counter)
    csvFile = pandas.read_csv("/tmp/file",sep="\s+",header=None)
    csvFile.iloc[:,3]=csvFile.iloc[:,3].astype(str)+csvFile.iloc[:,4].astype(str)
    csvFile=csvFile.drop([4],axis=1)

    csvFile.to_csv("/tmp/intermediate.csv",header=InventoryHeader,index=None)
    csvFile = pandas.read_csv("/tmp/intermediate.csv")

    csvFile.to_excel(curOutputFolderPath+"/inventory.xlsx",index=False)

    # Booking

    counter = toFile(counter)
    csvFile = pandas.read_csv("/tmp/file",sep="\s+",header=None)
    csvFile.to_csv("/tmp/intermediate.csv",header=BookingHeader,index=None)
    csvFile = pandas.read_csv("/tmp/intermediate.csv")

    csvFile.to_excel(curOutputFolderPath+"/booking.xlsx",index=False)