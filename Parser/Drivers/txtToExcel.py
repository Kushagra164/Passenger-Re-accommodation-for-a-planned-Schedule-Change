import pandas
import argparse
import os
import matplotlib.pyplot as pyplot

BookingHeader = ["RECLOC","CreationDate","CreationTime","ActionCD","ClassCD","SegSeq","PaxCnt","FlightNum","Orig_CD","Dest_CD","DepDate","DepTime","ArrDate","ArrTime"]
InventoryHeader = ["InventoryID","ScheduleID","FlightNum","AircraftType","DepartureDate","ArrivalDate","DepartureAirport","ArrivalAirport","Status","RescheduledTo"]

parser = argparse.ArgumentParser()
parser.add_argument("--input_file_path", required=True, help="input file")
parser.add_argument("--tmp_folder", required=True, help="temp folder")
parser.add_argument("--output_folder", required=True, help="output file")
args = parser.parse_args()

outputFolderPath = args.output_folder
inputFilePath = args.input_file_path
tmpFolderPath = args.tmp_folder

inputFile = open(inputFilePath)

lines = []
for line in inputFile:
    lines.append(line)

def toFile(counter):
    outputFile = open(tmpFolderPath+"/tmpFile.txt","w")
    while True:
        curLine = lines[counter]
        counter += 1
        if(curLine == "break\n"):
            break
        outputFile.write(curLine)
    outputFile.close()
    return counter

counter = 1

if not os.path.exists(outputFolderPath):
    os.makedirs(outputFolderPath)

# Inventory Excel File

counter = toFile(counter)
csvFile = pandas.read_csv(tmpFolderPath+"/tmpFile.txt",sep="\s+",header=None)

csvFile.to_csv(tmpFolderPath+"/intermediate.csv",header=InventoryHeader,index=None)
csvFile = pandas.read_csv(tmpFolderPath+"/intermediate.csv")

csvFile.to_excel(outputFolderPath+"/inventory.xlsx",index=False)

# Booking Excel File

counter = toFile(counter)
csvFile = pandas.read_csv(tmpFolderPath+"/tmpFile.txt",sep="\s+",header=None)

csvFile.to_csv(tmpFolderPath+"/intermediate.csv",header=BookingHeader,index=None)
csvFile = pandas.read_csv(tmpFolderPath+"/intermediate.csv")

csvFile.to_excel(outputFolderPath+"/booking.xlsx",index=False)

# Pie Chart - Distribution of 1-1, 1-Multi, Multi-1, Multi-Multi and Unallocated Flight Solutions

solutionTypes=["oneOne","oneMulti","multiOne","multiMulti","unallocated"]
solutionTypeValues=lines[counter].split()
counter+=1

pyplot.figure(figsize=(8,8))
pyplot.pie(
    solutionTypeValues,
    startangle=90,
    autopct = "%1.1f%%",
    colors=["yellowgreen","aqua","violet","orange","red"],
    pctdistance=0.8
)
pyplot.legend(solutionTypes)
pyplot.title("Flight Solutions")

pyplot.savefig(outputFolderPath+"/FlightSolution.png")
pyplot.close()

# Histogram 1 - Distribution of Passengers with Default , Non-Default and No Flight Solutions

solutionLabels = ["Default" , "Non-Default" , "No Flight"]
solutionLabelValues=lines[counter].split()
solutionLabelValues=[int(i) for i in solutionLabelValues]
counter+=1

pyplot.figure(figsize=(10,6))
pyplot.bar(solutionLabels,solutionLabelValues)
pyplot.xlabel("Flight Solutions")
pyplot.ylabel("Number of PNRs")
pyplot.title("Flight Solution Distribution")

pyplot.savefig(outputFolderPath+"/DefaultSolution.png")
pyplot.close()

# Histogram 2 - Distribution of Passengers with Different Flight Delays

solutionLabels=list()
for i in range(0,72,6):
    solutionLabels.append(str(i)+"-"+str(i+6))
solutionLabelValues=lines[counter].split()
averageDelay=solutionLabelValues[-1]
solutionLabelValues.pop()

solutionLabelValues=[int(i) for i in solutionLabelValues]
counter+=1

pyplot.figure(figsize=(10,6))
pyplot.bar(solutionLabels,solutionLabelValues)
pyplot.xlabel("Flight Delay in Hours(""Average Flight Delay = " + 
    str(averageDelay)+")")
pyplot.ylabel("Number of PNRs")
pyplot.title("Flight Delay Distribution")

pyplot.savefig(outputFolderPath+"/PassengerDelay.png")
pyplot.close()
