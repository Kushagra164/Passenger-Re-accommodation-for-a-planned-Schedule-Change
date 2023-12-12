import pandas

BookingHeader = ["RECLOC","CreationDate","ActionCD","ClassCD","SegSeq","PaxCnt","FlightNum","Orig_CD","Dest_CD","DepDate","DepTime","ArrDate","ArrTime"]
ScheduleHeader = ["ScheduleID","CarrierCD","FlightNum","AircraftType","AircraftTailNumber","DepartureAirport","ArrivalAirport","DepartureTime","ArrivalTime","StartDate","EndDate","Status","FrequencyPattern"]
InventoryHeader = ["InventoryID","ScheduleID","FlightNum","AircraftType","DepartureDate","ArrivalDate","DepartureAirport","ArrivalAirport","Status","RescheduledTo"]

for i in range(5):
    # Booking

    inputFolderPath="../Output/Solution/Solution"+str(i)
    outputFolderPath="../Output/Solution/Solution"+str(i)

    csvFile = pandas.read_csv(inputFolderPath+"/booking.txt",sep="\s+",header=None)
    csvFile.to_csv("/content/booking.csv",header=BookingHeader,index=None)
    csvFile=pandas.read_csv("/content/booking.csv")

    csvFile.to_excel(outputFolderPath+"/booking.xlsx",index=False)

    # Schedule

    csvFile = pandas.read_csv(inputFolderPath+"/schedule.txt",sep="\s+",header=None)
    csvFile.iloc[:,3]=csvFile.iloc[:,3].astype(str)+csvFile.iloc[:,4].astype(str)
    csvFile=csvFile.drop([4],axis=1)

    csvFile.to_csv("/content/schedule.csv",header=ScheduleHeader,index=None)
    csvFile=pandas.read_csv("/content/schedule.csv")

    csvFile.to_excel(outputFolderPath+"/schedule.xlsx",index=False)

    # Inventory

    csvFile = pandas.read_csv(inputFolderPath+"/inventory.txt",sep="\s+",header=None)
    csvFile.iloc[:,3]=csvFile.iloc[:,3].astype(str)+csvFile.iloc[:,4].astype(str)
    csvFile=csvFile.drop([4],axis=1)

    csvFile.to_csv("/content/inventory.csv",header=InventoryHeader,index=None)
    csvFile=pandas.read_csv("/content/inventory.csv")

    csvFile.to_excel(outputFolderPath+"/inventory.xlsx",index=False)