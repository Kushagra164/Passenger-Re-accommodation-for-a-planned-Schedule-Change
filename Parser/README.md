<h1 align = "center"> Parser </h1>

<span style="font-size: 20px; "> We get the information from `xlsx` files namely.</span>

- [<span style="font-size: 18px;"> Schedule </span>](../Input/schedule.xlsx)
- [<span style="font-size: 18px;"> Inventory </span>](../Input/inventory.xlsx)
- [<span style="font-size: 18px;"> Booking </span>](../Input/booking.xlsx)
- [<span style="font-size: 18px;"> Passenger </span>](../Input/passenger.xlsx)

![-----------------------------------------------------](../Images/rainbow.png)

<h2 align = "left"> Schedule </h2>
<span style="font-size: 15px;"> In `schedule.xlsx` we get the information about filght schedules, It consists of</span> 

- `ScheduleID` Unique id used to identify a specific schedule
- `CarrierCode` Assumed to be Unique for all schedules
- `FlightNumber` Assumed to be Unique for Unique citypairs
- `AircraftType` Type of airplane
- `AircraftTailNumber` Assumed to distinct for all airplanes
- `DepartureAirport` Departure Airport ID
- `ArrivalAirport` Arrival Airport ID
- `DepartureTime` Departure Time specified in __HH/MM__ format
- `ArrivalTime` Arrival Time specified in __HH/MM__ format
- `StartDate` Starting Date of schedule specified in __MM/DD/YYYY__ format
- `EndDate` Ending Date of schedule specified in __MM/DD/YYYY__ format
- `Status` Status of schedule 
- `Frequency_per_week` Number of departures in week
- `Frequency` Represents the pattern of departures in a week
- `NoOfDepartures` Total number of departures between `StartDate` and `EndDate`
- `DepartureDates` Assumed that all weekdays specified in `Frequency` that were in between `StartDate` and `EndDate`

![-----------------------------------------------------](../Images/rainbow.png)
<h2 align = "left"> Inventory </h2>
<span style="font-size: 15px;"> In `inventory.xlsx` we get the information about distribution of seats in flights</span> 

- `InventoryId` Unique id used to identify a specific inventory
- `ScheduleId` Corresponds to schedule in schedule.xlxs
- `FlightNumber` Assumed to be Unique for Unique citypairs
- `AircraftType` Type of airplane
- `DepartureDate` Departure date specified in __MM/DD/YYYY__ format
- `ArrivalDate` Arrival date specified in __MM/DD/YYYY__ format
- `DepartureAirport` Departure Airport ID
- `ArrivalAirport` Arrival Airport ID
- `TotalCapacity` Total seats in airplane
- `TotalInventory` Total seats that can be booked
- `BookedInventory` seats that were already booked
- `Oversold` = `BookedInventory` - `TotalCapacity`
- `AvailableInventory` = Available seats to be booked
- `FirstClass` Total seats in first class
- `BusinessClass` Total seats in business class
- `PremiumEconomyClass` Total seats in premium economy class 
- `EconomyClass` Total seats in economy class
- `FC_TotalInventory` Total seats in first class that can be booked
- `FC_BookedInventory` Seats in first class that were already booked
- `FC_Oversold` = `FC_BookedInventory` - `FirstClass`
- `FC_AvailableInventory` Available seats in first class to be booked
- `BC_TotalInventory` Total seats in business class that can be booked
- `BC_BookedInventory` Seats in business class that were already booked
- `BC_Oversold` = `BC_BookedInventory` - `BusinessClass`
- `BC_AvailableInventory` Available seats in business class to be booked
- `PC_TotalInventory` Total seats in premium economy class that can be booked
- `PC_BookedInventory` Seats in premium economy class that were already booked
- `PC_Oversold` = `PC_BookedInventory` - `PremiumEconomyClass`
- `PC_AvailableInventory` Available seats in premium economy class to be booked
- `EC_TotalInventory` Total seats in economy class that can be booked
- `EC_BookedInventory` Seats in economy class that were already booked
- `EC_Oversold` = `PC_BookedInventory` - `EconomyClass`
- `EC_AvailableInventory` Available seats in economy class to be booked
- `FC_CD` Distribution of seats cabin wise in first class
- `BC_CD` Distribution of seats cabin wise in business class
- `PC_CD` Distribution of seats cabin wise in premium economy class
- `EC_CD` Distribution of seats cabin wise in economy class

![-----------------------------------------------------](../Images/rainbow.png)
<h2 align = "left"> Booking </h2>
<span style="font-size: 15px;"> In `booking.xlsx` we get the information about bookings, It consists of</span> 

- `RECLOC` Also known as Record Locator, Used to identify booking
- `CREATION_DTZ` Date of booking done, Specified in __MM/DD/YYYY__ format
- `DEP_KEY` Carrier Code + Departure Date + Departure Airport + Arrival Airport + FlightNumber
- `ACTION_CD` Status of Booking
- `COS_CD` Class of Booking
- `SEG_SEQ` Segment of Flight Journey in overall PNR
- `PAX_CNT` Number of passengers in that PNR
- `CARRIER_CD` Code of flight operating company
- `FLT_NUM` Flight number
- `ORIG_CD` Departure Airport ID
- `DEST_CD` Arrival Airport ID
- `DEP_DT` Departure Date specified in __MM/DD/YYYY__ format
- `DEP_DTML` Local Departure DateTime specified in __MM/DD/YYYY HH:MM__ format
- `ARR_DTML` Local Arrival DateTime specified in __MM/DD/YYYY HH:MM__ format 
- `DEP_DTMZ` Global Departure DateTime specified in __MM/DD/YYYY HH:MM__ format
- `ARR_DTMZ` Global Arrival DateTime specified in __MM/DD/YYYY HH:MM__ format

![-----------------------------------------------------](../Images/rainbow.png)
<h2 align = "left"> Passenger </h2>
<span style="font-size: 15px;"> In `passenger.xlsx` we get the information about passenger details, It consists of</span> 

- `RECLOC` Also known as Record Locator, Used to identify booking
- `CREATION_DTZ` Date of booking specified in __MM/DD/YYYY__ format
- `LAST_NAME` Last Name of passenger
- `FIRST_NAME` First Name of passenger
- `NATIONALITY` Nationality of passenger
- `CONTACT_PH_NUM` Phone number of passenger
- `CONTACT_EMAIL` Email id of passenger
- `DOC_ID` 7 to 10 digit alpha numeric unique to the passenger
- `DOC_TYPE` Any of PASSPORT, GOV-ID, DRV-LICENCE
- `SPECIAL_NAME_CD1` Any of ADT, CHD, INF, INS, UNN, S65
- `SPECIAL_NAME_CD2` Any of NRPS, NRSA
- `SSR_CODE_CD1` Any of WCHR, BLND, DEAF

![-----------------------------------------------------](../Images/rainbow.png)





