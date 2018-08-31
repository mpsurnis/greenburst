from collections import OrderedDict
from copy import deepcopy
from csv import reader
from webcrawler import *

def gen_catalog():
	#build catalog dictionary
	catalog = {"entries":[], "versions":{}}
	entries = catalog["entries"]
	versions = catalog["versions"]

	#ATNF Pulsar Database
	file = open("sources/psrcat.db", "r")
	line = file.readline()
	versions["ATNF"] = line.split()[1]
	curr = None
	while line:
	    if line[0] == "#":
	        line = file.readline()
	        continue
	    if line[0] == "@":
	        if "RAJ" in curr.keys():
	        	new_curr = {"Name":curr["PSRJ"][0], "RA":curr["RAJ"][0], "DEC":curr["DECJ"][0], "visible": True, "sources": [{"Name":"ATNF", "data":curr}]}
	        	entries.append(new_curr)
	        curr = None
	        line = file.readline()
	        continue
	    if curr == None:
	        curr = OrderedDict()
	    row = line.split()
	    curr[row[0]] = row[1:len(row)]
	    line = file.readline()
	file.close()


	#RRATalog
	file = open("sources/rratalog.txt", "r")
	line = file.readline()
	line = file.readline()
	versions["RRATalog"] = "September 24, 2016"
	while line:
		line = line.split()
		curr = OrderedDict()
		curr["Name"] = line[0].replace("*", "")
		curr["P"] = line[1]
		curr["Pdot"] = line[2]
		curr["DM"] = line[3]
		curr["RA"] = line[4]
		curr["DEC"] = line[5]
		curr["l"] = line[6]
		curr["b"] = line[7]
		curr["Rate"] = line[8]
		curr["logB"] = line[9]
		curr["logts"] = line[10]
		curr["Dhat"] = line[11]
		curr["FluxD"] = line[12]
		curr["Pulse Width"] = line[13]
		curr["Survey"] = line[14]
		if curr["Name"] in [x["Name"] for x in catalog["entries"]]:
			entry = [x for x in catalog["entries"] if x["Name"] == curr["Name"]][0]
			entry["sources"].append({"Name": "RRATalog", "data":curr})
		elif curr["Name"][0:-2] in [x["Name"] for x in catalog["entries"]]:
			entry = [x for x in catalog["entries"] if x["Name"] == curr["Name"][0:-2]][0]
			entry["sources"].append({"Name": "RRATalog", "data":curr})
		else:
			catalog["entries"].append({"Name":curr["Name"], "RA":curr["RA"], "DEC":curr["DEC"], "visible":True, "sources":[{"Name":"RRATalog", "data":curr}]})
		line = file.readline()
	file.close()


	#frbcat
	file = open("sources/frbcat_2017-07-06.csv")
	lines = list(reader(file))
	titles = lines[0]
	versions["frbcat"] = "1.0"
	for line in lines[1:]:
		curr = OrderedDict()
		for i in range(len(line)):
			curr[titles[i]] = line[i]
		if curr["Name"] in [x["Name"] for x in catalog["entries"]]:
			entry = [x for x in catalog["entries"] if x["Name"] == curr["Name"]][0]
			entry["sources"].append({"Name":"frbcat", "data":curr})
		else:
			catalog["entries"].append({"Name":curr["Name"], "RA":curr["RAJ"], "DEC":curr["DECJ"], "visible":True, "sources":[{"Name":"frbcat", "data":curr}]})
	file.close()

	catalog["entries"].sort(key=lambda x: x["Name"])
	return catalog

def get_versions():
# Get catalog versions from web and check aginst machine versions

    atnf_ver = get_ATNF_version()
    rrat_ver = get_RRATalog_version()
    #frbc_ver = get_frbcat_version()

    print("ATNF Web Version:",atnf_ver)
    print("RRATalog Web Version:",rrat_ver)
    #print frbc_ver
    print()
    print("ATNF Machine Version:",cat["versions"]["ATNF"])
    print("RRATalog Machine Version:",cat["versions"]["RRATalog"])
    print("FRBcat MAchine Version:",cat["versions"]["frbcat"])
    print()
