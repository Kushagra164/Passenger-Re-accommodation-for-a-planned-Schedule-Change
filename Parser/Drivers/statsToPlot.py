import matplotlib.pyplot as pyplot
import os

inputFilePath=""
outputFolderPath=""

if not os.path.exists(outputFolderPath):
    os.makedirs(outputFolderPath)

# Reading Input file

inputFile = open(inputFilePath)

lines=[]
for line in inputFile:
    lines.append(line)
counter=0

# Pie Chart - Distribution of Different Flight Solutions

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

# Histogram 1 - Distribution of Passengers with Default , Non-Default and No Flight Solutions

solutionLabels = ["Default" , "Non-Default" , "No Flight"]
solutionLabelValues=lines[counter].split()
counter+=1

pyplot.bar(solutionLabels,solutionLabelValues)
pyplot.xlabel("Flight Solutions")
pyplot.ylabel("Number of PNRs")
pyplot.title("Flight Solution Distribution")

pyplot.savefig(outputFolderPath+"/DefaultSolution.png")

# Histogram 2 - Distribution of Passengers with Different Flight Delays

solutionLabels = ["0-6","6-12","12-18","18-24","24+"]
solutionLabelValues=lines[counter].split()
counter+=1

pyplot.bar(solutionLabels,solutionLabelValues)
pyplot.xlabel("Flight Delay in Hours")
pyplot.ylabel("Number of PNRs")
pyplot.title("Flight Delay Distribution")

pyplot.savefig(outputFolderPath+"/PassengerDelay.png")

print("Average Flight Delay = " + str(sum(solutionLabelValues)/len(solutionLabelValues)))