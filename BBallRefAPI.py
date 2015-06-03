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


def getTeamStr(inStr):
  for num in range(0, len(inStr)):
    if inStr[num:num+5] == 'teams':
        return BBallRefID.convertTeamStr(inStr[num+6:num+9])


def writeCSVFile(teamName, year):
  teamSearchName = teamName
  if teamSearchName == "OKC":
    teamSearchName = "SEA"
  elif teamSearchName == "NOP":
    if year == 2006 or year == 2007:
      teamSearchName = "NOK"

  bball = getSoupFromURL('http://www.basketball-reference.com/teams/%s/%d_games.html' % (teamSearchName, year), True)
  outFile = open('csvDir/%s%d.csv' % (teamName, year), "w")

  for thing in bball.findAll('tr'):
    counter = 0
    outTeamNum = -1
    outMonth = -1;
    outDay = -1;
    outHome = -1;
    
    for thing2 in thing.findAll('td'):

        if(counter == 1):
            for num in range(0, len(str(thing2))):
                if str(thing2)[num:num+4] == "csk=":
                    outMonth = str(thing2)[num+10:num+12]
                    outDay = str(thing2)[num+13:num+15]

        if(counter == 5):
            start = str(thing2).index('>')
            if str(thing2)[start+1:start+2] == '<':
                outHome = 1
            else:
                outHome = 0
            
        if(counter == 6):
            teamVsStr = getTeamStr(str(thing2))
            for num in range(0, len(BBallRefID.teams)):
                if teamVsStr == BBallRefID.teams[num]:
                    outTeamNum = num
                    break
        if(counter == 7):
            if(getTagStr(str(thing2)) == ''):
                break
        if(counter == 9):
            outFile.write(str(outMonth)+',')
            outFile.write(str(outDay)+',')
            outFile.write(str(outHome)+',')
            outFile.write(str(outTeamNum)+',')
            outFile.write(getTagStr(str(thing2))+',')
        elif(counter == 10):
            outFile.write(getTagStr(str(thing2))+',\n')
            break
        counter += 1
        
  outFile.close()
  return

if(len(sys.argv) != 3):
    print "Usage: python BBallRefAPI.py <startYear> <endYear>"
    sys.exit()

for team in BBallRefID.teams:
  for num in range(int(sys.argv[1]),int(sys.argv[2])+1):
    writeCSVFile(team, num)
