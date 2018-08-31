#!/usr/bin/env python3.5

__author__ = 'Mayuresh Surnis'
__email__  = 'mayuresh.surnis@mail.wvu.edu'


import argparse
from catalog_utils import gen_catalog
from webcrawler import *


# Generate the catalog using the rtcat function
cat = gen_catalog()

# Some useful functions

def hmstodeg(ra):
# Convert ra from hms to decimal degree

# First convert to decimal hours
    dhrs = int(ra[0]) + (int(ra[1])/60.) + (float(ra[2])/3600.)
# Now convert to decimal degree
    ddeg = dhrs*15.
    return ddeg

def dmstodeg(dec):
# Convert dec from dms to decimal degree

# Generate a flag for positive or negative dec
    flag = 1
    if (int(dec[0]) < 0):
       flag = -1
    
    decdeg = abs(int(dec[0]))
    ddec = decdeg + (int(dec[1])/60.) + (float(dec[2])/3600.)
    ddec = flag*ddec
    return ddec

if __name__ == '__main__':

# Parse command line arguments

   parser = argparse.ArgumentParser(description='Provide information about any known pulsar, RRAT or FRB in the beam')
   parser.add_argument('-ra', dest = 'raq', action = 'store', metavar = 'RAJ', required=True, help='RAJ in decimal degree')
   parser.add_argument('-dec', dest = 'decq', action = 'store', metavar = 'DECJ', required=True, help='DECJ in decimal degree')
   args = parser.parse_args()


# Compare with catalog entries and print out relevant information

   length = len(cat["entries"])
   outarr = []
   for i in range(length):
       catra = cat["entries"][i]["RA"]
       raobj = catra.split(":",3)
       catdec = cat["entries"][i]["DEC"]
       decobj = catdec.split(":",3)

       #check if there are blanks and fill them with zeros
  
       if (len(raobj) < 3):
          if (len(raobj) == 2):
             raobj.append("0.0")
          else:
             raobj.append("0")
             raobj.append("0.0")

       if (len(decobj) < 3):
          if (len(decobj) == 2):
             decobj.append("0.0")
          else:
             decobj.append("0")
             decobj.append("0.0")
       #print("Index is:",i)
       #if i==343:
       #   print(cat["entries"][i]["sources"][0]["data"])
       # Now calculate separation in arcmin and put out values if sep < 10'
       racand = float(args.raq)
       deccand = float(args.decq)
       racat = hmstodeg(raobj)
       deccat = dmstodeg(decobj)
       dlra = abs(racand - racat)
       dldec = abs(deccand - deccat)
       sep = ((dlra**2 + dldec**2)**0.5)*60.
       if (sep < 10.):
       #Print DM if not NRAD and 0.0 otherwise
          try: 
             if cat["entries"][i]["sources"][0]["data"]["TYPE"]:
                print('%.2f'%sep,cat["entries"][i]["Name"],cat["entries"][i]["RA"],cat["entries"][i]["DEC"],"0.0",cat["entries"][i]["sources"][0]["Name"])
          except:
             pass
             if cat["entries"][i]["sources"][0]["Name"] == "ATNF":
                print('%.2f'%sep,cat["entries"][i]["Name"],cat["entries"][i]["RA"],cat["entries"][i]["DEC"],cat["entries"][i]["sources"][0]["data"]["DM"][0],cat["entries"][i]["sources"][0]["Name"])
             else:
                print('%.2f'%sep,cat["entries"][i]["Name"],cat["entries"][i]["RA"],cat["entries"][i]["DEC"],cat["entries"][i]["sources"][0]["data"]["DM"],cat["entries"][i]["sources"][0]["Name"])
          #outarr.append(cat["entries"][i]["sources"][0]["data"])

   #print(outarr)
