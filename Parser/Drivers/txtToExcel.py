import pandas
import argparse
import os
import matplotlib.pyplot as pyplot

BookingHeader = ["RECLOC","CreationDate","CreationTime","ActionCD","ClassCD","SegSeq","PaxCnt","FlightNum","Orig_CD","Dest_CD","DepDate","DepTime","ArrDate","ArrTime"]
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

def toFile(counter):
    outputFile = open(outputFolderPath+"/tmp/file.txt","w")
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

# Inventory

counter = toFile(counter)
csvFile = pandas.read_csv(outputFolderPath+"/tmp/file.txt",sep="\s+",header=None)

csvFile.to_csv(outputFolderPath+"/tmp/intermediate.csv",header=InventoryHeader,index=None)
csvFile = pandas.read_csv(outputFolderPath+"/tmp/intermediate.csv")

csvFile.to_excel(outputFolderPath+"/inventory.xlsx",index=False)

# Booking

counter = toFile(counter)
csvFile = pandas.read_csv(outputFolderPath+"/tmp/file.txt",sep="\s+",header=None)

csvFile.to_csv(outputFolderPath+"/tmp/intermediate.csv",header=BookingHeader,index=None)
csvFile = pandas.read_csv(outputFolderPath+"/tmp/intermediate.csv")

csvFile.to_excel(outputFolderPath+"/booking.xlsx",index=False)

# statistics

solutionTypes=["oneOne","oneMulti","multiOne","multiMulti"]
solutionTypeValues=lines[counter].split()
counter+=1

pyplot.pie(
    solutionTypeValues,
    labels=solutionTypes,
    startangle=90,
    autopct = "%1.1f%%",
)
pyplot.title("Flight Solutions")

pyplot.savefig(outputFolderPath+"/FlightSolution.png")
pyplot.close()

# Histogram 1 - Distribution of Passengers with Default , Non-Default and No Flight Solutions

solutionLabels = ["Default" , "Non-Default" , "No Flight"]
solutionLabelValues=lines[counter].split()
counter+=1

pyplot.bar(solutionLabels,solutionLabelValues)
pyplot.xlabel("Flight Solutions")
pyplot.ylabel("Number of PNRs")
pyplot.title("Flight Solution Distribution")

pyplot.savefig(outputFolderPath+"/DefaultSolution.png")
pyplot.close()

# Histogram 2 - Distribution of Passengers with Different Flight Delays

solutionLabels = ["0-6","6-12","12-18","18-24","24+"]
solutionLabelValues=lines[counter].split()
counter+=1

pyplot.bar(solutionLabels,solutionLabelValues)
solutionLabelValues=[int(i) for i in solutionLabelValues]
pyplot.xlabel("Flight Delay in Hours(""Average Flight Delay = " + 
    str(sum(solutionLabelValues)/len(solutionLabelValues))+")")
pyplot.ylabel("Number of PNRs")
pyplot.title("Flight Delay Distribution")

pyplot.savefig(outputFolderPath+"/PassengerDelay.png")
pyplot.close()
