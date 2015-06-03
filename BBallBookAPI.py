from bs4 import BeautifulSoup
import requests
import BBallRefID
import sys
import datetime

#this func. blatantly stolen from basketcrawler, along w/ idea to use BeautifulSoup
def getSoupFromURL(url, supressOutput=True):
    """
    This function grabs the url and returns and returns the BeautifulSoup object
    """
    if not supressOutput:
        print url
    
    try:
        r = requests.get(url)
    except:
        return None
    
    return BeautifulSoup(r.text)


def getCorrDate(year, month, day):
  if day > 1:
    return 100000000*(year - 2000) + 1000000*month + 10000*(day - 1)
  else:
    if month == 1:
      month = 12
    else:
      month -= 1
    if month == 2:
      if year%4 == 0:
        day = 29
      else:
        day = 28
    elif month == 9 or month == 11 or month == 4 or month == 6:
      day = 30
    else:
      day = 31

  return 100000000*(year - 2000) + 1000000*month + 10000*day   


def getDateLines(year, month, day):
  tempYear = year
  if month > 6: 
      tempYear -= 1

  print 'http://data.nowgoal.com/nba/oddsHistory.htm?Selday=%d-%d-%d' % (tempYear, month, day)
  bball = getSoupFromURL('http://data.nowgoal.com/nba/oddsHistory.htm?Selday=%d-%d-%d' % (tempYear, month, day), True)
  homeNum = -1
  awayNum = -1 
  OU = ""
  Odds = ""

  outFile = open('csvDir/%dLines.csv' % (year), 'a')

  counter = 0
  for thing in bball.findAll('tr'):
    if counter == 3:
      if str(thing.findAll('b')) != "[<b> National Basketball Association </b>]":
        print str(thing.findAll('b'))
        print "Not the NBA. Break."
        break

    if counter < 4:
      counter += 1
      continue

    if str(thing)[1:24] == 'tr class="Leaguestitle"':
      break

    counter2 = 0
    for thing2 in thing.findAll('td'):
      if counter2 > 1 and counter2 < 13:
        counter3 = 0
        for thing3 in thing2.findAll('a'):
          if counter2 != 2 or counter3 == 0 or counter3 == 3:
            start = str(thing3).index('>')
            end = str(thing3)[1:].index('<')
            if counter2 == 2:
              start += 1
              end -= 1
              if counter3 == 0:
                end -= 3
                homeNum = BBallRefID.teamsBook.index(str(thing3)[start+1:end+1])
              elif counter3 == 3:
                awayNum = BBallRefID.teamsBook.index(str(thing3)[start+1:end+1])
            elif counter2 == 3:
              Odds = str(thing3)[start+1:end+1]
            elif counter2 == 8:
              OU = str(thing3)[start+1:end+1]
          counter3 += 1
      counter2 += 1
    outFile.write(str(getCorrDate(year,month,day) + 100*homeNum + 1*awayNum) + "," + OU + "," + Odds + ",\n")
 
  outFile.close()


if(len(sys.argv) != 3):
  print "Usage: python BBallBookAPI.py <startYear> <endYear>"
  sys.exit()

startYear = int(sys.argv[1])
endYear = int(sys.argv[2])

if(startYear > endYear):
  print "Error: <startYear> must be less than <endYear>. Return."
  sys.exit()

years = range(startYear, endYear + 1)
months = [10, 11, 12, 1, 2, 3, 4, 5, 6]
days = range(1,32)

startMonths = { 2005:11, 2006:11, 2007:10, 2008:10, 2009:10, 2010:10, 2011:10, 2012:10, 2013:10, 2014:10, 2015:10}
startDays = { 2005:2, 2006:1, 2007:31, 2008:30, 2009:28, 2010:27, 2011:26, 2012:25, 2013:30, 2014:29, 2015:28}

cut = datetime.date.today() + datetime.timedelta(days=1)
cutYear = cut.year
cutMonth = cut.month
cutDay = cut.day

if cutMonth > 6:
  cutYear += 1

for year in years:
  outFile = open('csvDir/%dlines.csv' % year, 'w')
  outFile.close()

for year in years:
  for month in months:
    for day in days:

      if month == startMonths[year] and day < startDays[year]:
        continue

      if year == cutYear and month == cutMonth and day == cutDay:
        print "Hit cut date. Exit."
        sys.exit()

      if month == 2:
        if day > 28 and year%4 != 0:
          continue
        elif day > 29:
          continue

      if month == 4 or month == 6 or month == 9 or month == 11:
        if day > 30:
          continue

      getDateLines(year, month, day)

