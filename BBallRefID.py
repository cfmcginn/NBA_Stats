nTeams = 30
teams = ["ATL",
         "BOS",
         "BRK",
         "CHI",
         "CHO",
         "CLE",
         "DAL",
         "DEN",
         "DET",
         "GSW",
         "HOU",
         "IND",
         "LAC",
         "LAL",
         "MEM",
         "MIA",
         "MIL",
         "MIN",
         "NOP",
         "NYK",
         "OKC",
         "ORL",
         "PHI",
         "PHO",
         "POR",
         "SAC",
         "SAS",
         "TOR",
         "UTA",
         "WAS"
         ]

teamsBook = ["Hawks",
             "Celtics",
             "Nets",
             "Bulls",
             "Hornets",
             "Cavaliers",
             "Mavericks",
             "Nuggets",
             "Pistons",
             "Warriors",
             "Rockets",
             "Pacers",
             "Clippers",
             "Lakers",
             "Grizzlies",
             "Heat",
             "Milwaukee",
             "Timberwolves",
             "Pelicans",
             "Knicks",
             "Thunder",
             "Magic",
             "76ers",
             "Suns",
             "Trail Blaze",
             "Kings",
             "Spurs",
             "Raptors",
             "Jazz",
             "Wizards"
             ]


def convertTeamStr(inStr):
  if inStr == 'NJN':
    return 'BRK'
  elif inStr == 'CHA':
    return 'CHO'
  elif inStr == 'NOH':
    return 'NOP'
  elif inStr == 'NOR':
    return 'NOP'
  elif inStr == 'SEA':
    return 'OKC'
  else:
    return inStr


def getWebStr(inStr, year):
  if inStr == 'BRK':
    if year < 2013:
      return 'NJN'
  elif inStr == 'CHO':
    if year < 2015:
      return 'CHA'
  elif inStr == 'NOP':
    if year < 2006:
      return 'NOH'
    elif year < 2008:
      return 'NOK'
    elif year < 2014:
      return 'NOH'
  elif inStr == 'OKC':
    if year < 2009:
      return 'SEA'

  return inStr
