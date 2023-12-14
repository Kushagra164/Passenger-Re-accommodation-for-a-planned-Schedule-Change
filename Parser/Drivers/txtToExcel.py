import pandas
import argparse
import os
import matplotlib.pyplot as pyplot

BookingHeader = ["RECLOC","CreationDate","ActionCD","ClassCD","SegSeq","PaxCnt","FlightNum","Orig_CD","Dest_CD","DepDate","DepTime","ArrDate","ArrTime"]
#ScheduleHeader = ["ScheduleID","CarrierCD","FlightNum","AircraftType","AircraftTailNumber","DepartureAirport","ArrivalAirport","DepartureTime","ArrivalTime","StartDate","EndDate","Status","FrequencyPattern"]
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

# T = int(lines[0])
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
for i in range(1):

    curOutputFolderPath = outputFolderPath + "/Solution" + str(i+1)

    if not os.path.exists(curOutputFolderPath):
        os.makedirs(curOutputFolderPath)

    # Schedule

    # counter = toFile(counter)
    # csvFile = pandas.read_csv("/tmp/file",sep="\s+",header=None)
    # csvFile.iloc[:,3]=csvFile.iloc[:,3].astype(str)+csvFile.iloc[:,4].astype(str)
    # csvFile=csvFile.drop([4],axis=1)
    #
    # csvFile.to_csv("/tmp/intermediate.csv",header=ScheduleHeader,index=None)
    # csvFile = pandas.read_csv("/tmp/intermediate.csv")
    #
    # csvFile.to_excel(curOutputFolderPath+"/schedule.xlsx",index=False)

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

    pyplot.savefig(curOutputFolderPath+"/FlightSolution.png")

    # Histogram 1 - Distribution of Passengers with Default , Non-Default and No Flight Solutions

    solutionLabels = ["Default" , "Non-Default" , "No Flight"]
    solutionLabelValues=lines[counter].split()
    counter+=1

    pyplot.bar(solutionLabels,solutionLabelValues)
    pyplot.xlabel("Flight Solutions")
    pyplot.ylabel("Number of PNRs")
    pyplot.title("Flight Solution Distribution")

    pyplot.savefig(curOutputFolderPath+"/DefaultSolution.png")

    # Histogram 2 - Distribution of Passengers with Different Flight Delays

    solutionLabels = ["0-6","6-12","12-18","18-24","24+"]
    solutionLabelValues=lines[counter].split()
    counter+=1

    pyplot.bar(solutionLabels,solutionLabelValues)
    pyplot.xlabel("Flight Delay in Hours(""Average Flight Delay = " + 
        str(sum(solutionLabelValues)/len(solutionLabelValues))+")")
    pyplot.ylabel("Number of PNRs")
    pyplot.title("Flight Delay Distribution")

    pyplot.savefig(curOutputFolderPath+"/PassengerDelay.png")
