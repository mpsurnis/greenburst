import requests
import re

def get_ATNF_version():
	r = requests.get("http://www.atnf.csiro.au/people/pulsar/psrcat/")
	p = re.compile('name="version" value="([^"]+)')
	m = p.search(r.text)
	return m.group(1)

def get_RRATalog_version():
	r = requests.get("http://astro.phys.wvu.edu/rratalog/")
	p = re.compile('Last update: (.+)')
	m = p.search(r.text)
	return m.group(1).strip()

def get_Parallaxes_version():
	r = requests.get("http://www.astro.cornell.edu/research/parallax/")
	p = re.compile('All published pulsar parallaxes as of <b>(.+)</b>')
	m = p.search(r.text)
	return m.group(1).strip()

def get_GCpsr_version():
	r = requests.get("http://www.naic.edu/~pfreire/GCpsr.html")
	#p = re.compile('<i> The present version of this page was last updated: (.+)</i>')
	p = re.compile('This page was last updated: (.+)<a')
	m = p.search(r.text)
	return m.group(1).strip()

def get_frbcat_version():
	#r = requests.get("http://www.astronomy.swin.edu.au/pulsar/frbcat/")
	r = requests.get("http://frbcat.org/")
	p = re.compile('<h2>Catalogue Version (.+)</h2>')
	m = p.search(r.text)
	return m.group(1).strip()
