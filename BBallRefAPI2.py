from bs4 import BeautifulSoup
import requests
import BBallRefID
import sys

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


def getTagStr(inStr):
  start = inStr.index('>')
  end = inStr[1:].index('<')
  return inStr[start+1:end+1]


def getBBallRef2(year):
  inFile = open('csvDir/%dID.csv' % year, 'r')  
  gameTag = inFile.readlines()

  start = 2
  end = 11
  if year < 2010:
    start = 1
    end = 10

  outFile = open("csvDir/%dGameStats.csv" % year, "w")
  outFile.close()

  for line in gameTag:
#    print line[:end]
    gameID = line[:end]
#    print line[start:start+2]
    month = int(line[start:start+2])
#    print line[start+2:start+4]
    day = int(line[start+2:start+4])
#    print line[start+4:start+6]
#    print BBallRefID.teams[int(line[start+4:start+6])]
    homeTeam = BBallRefID.teams[int(line[start+4:start+6])]
#    print line[start+6:start+8]

    getYear = year
    if month > 7:
      getYear -= 1

    if day < 10:
      day = '0' + str(day)

    if month < 10:
      month = '0' + str(month)

    day = str(day)

    counter = 0
    bball = getSoupFromURL('http://www.basketball-reference.com/boxscores/%d%s%s0%s.html' % (getYear, month, day, BBallRefID.getWebStr(homeTeam, year)), True)
    print "HERE" 
    print 'http://www.basketball-reference.com/boxscores/%d%s%s0%s.html' % (getYear, month, day, BBallRefID.getWebStr(homeTeam, year))
#    print bball

    outFile = open("csvDir/%dGameStats.csv" % year, "a")
    outFile.write(str(gameID) + ',')

    teamCounter = 0

    for thing in bball.findAll('tr'):
#      print "counter: " + str(counter)
#      print thing
      counter+=1

      if counter == 1:
        continue

      scoreBool = False
      teamBool = False

      for thing2 in thing.findAll('td'):
        for thing3 in thing2.findAll('th'):          
          if getTagStr(str(thing3)) == 'Scoring':
            scoreBool = True

        if getTagStr(str(thing2)) == 'Team Totals':
          teamCounter+=1
          if teamCounter == 1 or teamCounter == 3:
            teamBool = True

      if scoreBool:
        for thing2 in thing.findAll('td'):
          counter3 = 0
          for thing3 in thing2.findAll('tr'):
            counter3 += 1
            if counter3 < 3 or counter3 > 4: 
              continue
            counter4 = 0
            for thing4 in thing3.findAll('td'):
              counter4 += 1
              if counter4 < 2:
                continue
              outFile.write(getTagStr(str(thing4))+',')
            outFile.write(',')
          break

      if teamBool == True:
        counter2 = 0
        for thing2 in thing.findAll('td'):
          counter2 += 1
          if counter2 > 1:
            outFile.write(getTagStr(str(thing2)) + ',')
        if teamCounter == 3:
          outFile.write('\n')        

  outFile.close()



if(len(sys.argv) != 3):
    print "Usage: python BBallRefAPI.py <startYear> <endYear>"
    sys.exit()

for num in range(int(sys.argv[1]),int(sys.argv[2])+1):
  getBBallRef2(num)
