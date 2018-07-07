import googlemaps
import requests
import pickle
from uszipcode import ZipcodeSearchEngine
import time
import pandas as pd
from pick import pick

apiKey = 'AIzaSyD3cn_utAfFePi70-Ay1KFI9_QgA7VFckI' #Philip Ruffini's

###gets coordinates for every entry from Google maps geocoding API, creates new text file that has zips for every entry
def get_coord(x):
	outfile = open("AmazonDataWithLatLong.txt","w")
	address=''
	full=''
	counter=0
	errors=0

	for line in x:
		if line.split(' : ')[2].strip()!='null': #checks if null order
			full=line.split(' : ')[4]
			address=full.split(',')[1]
			address=address.strip()
			address=address.splitlines()[0]

			if counter<tot_runs: #if you dont want to run whole set
				counter+=1

				
				gmaps = googlemaps.Client(key=apiKey)

				geocode_result = gmaps.geocode(address)
				
				try:
					lat=str(geocode_result[0]['geometry']['location']['lat'])
					lng=str(geocode_result[0]['geometry']['location']['lng'])
					outfile.write(line.splitlines()[0]+ " : "+ lat +', ' + lng + '\n')

				except IndexError:
					try:
						coordinates = zip2coord(address)
						if coordinates != 'None, None':
							outfile.write(line.splitlines()[0]+ " : "+ coordinates + '\n')
						else:
							errors+=1
							print('Unsearchable Zip: ' + address)

					except IndexError:
						print('unsearchable address: ')
						errors+=1
			else:
				print('done')
				print("Total errors: " +str(errors))
				break
	outfile.close()

#deals with unsearchable addresses
def zip2coord(adr):
	zipCode=adr.split()[-2]
	zipCode=zipCode.split('-')[0]
	search = ZipcodeSearchEngine()
	result = search.by_zipcode(str(zipCode))
	return str(result["Latitude"]) +', ' + str(result["Longitude"])

def processDate(dateArray):
	if(len(dateArray[1]) == 1):
		dateArray[1] = "0"+dateArray[1]
	if(len(dateArray[2]) == 1):
		dateArray[2] = "0"+dateArray[2]

	return int(dateArray[0]+dateArray[1]+dateArray[2])

###converts updated text file entries into dictionary
def convert2Dictionary():
	newfileref = open("AmazonDataWithLatLong.txt","r")
	data = newfileref.readlines()
	finalDict={}

	for line in data:
		entryParts=line.split(" : ")
		key = entryParts[0].strip()
		finalDict[key] = {}

		tempDate = entryParts[1].strip().split("-")
		
		finalDict[key]["Original"] = line.strip()
		finalDict[key]['Date'] = processDate(tempDate)
		finalDict[key]['Asin'] = (entryParts[2].strip())
		finalDict[key]['Name'] = (entryParts[3].strip())
		finalDict[key]['Address'] = entryParts[4].split(",")[1].strip("\n").strip()
		finalDict[key]['Coordinates'] = entryParts[5].strip()

	newfileref.close()
	return finalDict

#finds entries that match criteria and uploads coords to text file
def searchResult(finalDict,startingDate,endingDate,ASIN): 
	searchDictionary={}

	for x in finalDict:
		if finalDict[x]["Asin"] == ASIN:
				if finalDict[x]["Date"] >= int(startingDate) and finalDict[x]["Date"] <= int(endingDate):
					searchDictionary[x] = finalDict[x]

	resultsFile = open("queryResults.txt","w")

	for entry in searchDictionary:
		resultsFile.write(searchDictionary[entry]['Coordinates']+"\n")

#finds and adds new entries
def findNewEntries():
	new_entries_count=0
	counter=0
	errors=0

	newFile = open("AmazonData.txt","r")
	newData = newFile.readlines()

	oldFile = open("AmazonDataWithLatLong.txt","r")
	oldData = oldFile.readlines()

	oldEntries=[]
	newEntries=[]
	for line in oldData:
		oldEntries.append(line.split(' : ')[0].strip())
	
	for line in newData:
		if line.split(' : ')[0].strip() in oldEntries:
			pass
		else:
			new_entries_count+=1
			newEntries.append(line)
	oldFile.close()
	print("Total new entries: " + str(new_entries_count))
	oldFile = open("AmazonDataWithLatLong.txt","a")

	for line in newEntries:
		address=''
		full=''
		if line.split(' : ')[2].strip()!='null':
			full=line.split(' : ')[4]
			address=full.split(',')[1]
			address=address.strip()
			address=address.splitlines()[0]

			if counter<new_entries_to_add:
				counter+=1
				geocode_result = gmaps.geocode(address)
				try:
					lat=str(geocode_result[0]['geometry']['location']['lat'])
					lng=str(geocode_result[0]['geometry']['location']['lng'])
					oldFile.write(line.splitlines()[0]+ " : "+ lat +', ' + lng + '\n')
				
				except IndexError:
					try:
						coordinates = zip2coord(address)
						if coordinates != 'None, None':
							oldFile.write(line.splitlines()[0]+ " : "+ coordinates + '\n')
						else:
							errors+=1
							print('Unsearchable Zipcode: ' + address)

					except IndexError:
						print('Unsearchable Address: ')
						errors+=1
			else:
				print('done')
				print(errors)
				break
	oldFile.close()


def go():
		get_coord(raw_data)
		new=convert2Dictionary()
		searchResult(new,startingDate,endingDate,ASIN)

fileref = open("AmazonData.txt","r")
raw_data = fileref.readlines()


# gmaps = googlemaps.Client(key=api_key)

tot_runs= len(raw_data)-1
new_entries_to_add=4

get_coord(raw_data)
print("Done")



exit()

t1 = time.time()

go()

t2 = time.time()
print(str(int(t2-t1)) + ' seconds total')








