""" Code modified from 2014 Chad Rexode's senior project 
Monte-Carlo Glauber Model Simulations of Nuclear Collisions

https://github.com/MCGlauber/MCG 
"""
import numpy as np
from urllib.request import urlopen
from scipy.optimize import curve_fit

#Importing data from particle data group
#Attempts to use data from current year
#If that data is not available, drops down a year until data is found or defaults to 2013 data
Y=2022
i=0

while i<=Y-2013:
    try:
        TotalData = urlopen('http://pdg.lbl.gov/'+str(Y-i)+'/hadronic-xsections/rpp'+str(Y-i)+'-pp_total.dat')
        print("Using "+str(Y-i)+" data for total cross section.")
        DataFound1=True
        break
    except:
        print(str(Y-i)+" total cross section data is unavailable. The Particle Data Group website may not have the latest data or may have changed format.")
        i+=1
        if i>Y-2013:
            print("---\nData not found. Please check your internet connection to http://pdg.lbl.gov/2013/html/computer_read.html\n---")
            DataFound1=False
l=0
while l<=Y-2013:
    try:
        ElasticData = urlopen('http://pdg.lbl.gov/'+str(Y-l)+'/hadronic-xsections/rpp'+str(Y-l)+'-pp_elastic.dat')
        print("Using "+str(Y-l)+" data for elastic cross section.")
        DataFound2=True
        break
    except:
        l+=1
        if l>Y-2013:
            print("---\nData not found. Please check your internet connection to http://pdg.lbl.gov/2013/html/computer_read.html\n---")
            DataFound2=False

if DataFound1==True:
    data=np.loadtxt(TotalData,float,usecols=(0,1,2,3,4,5,6,7,8),skiprows=11)
    Point=data[:,0]
    Plab=data[:,1] #GeV/c
    Plab_min=data[:,2]
    Plab_max=data[:,3]
    Sig=data[:,4]
    StEr_H=data[:,5]
    StEr_L=data[:,6]
    SyEr_H=data[:,7]
    SyEr_L=data[:,8]
if DataFound2==True:
    Edata=np.loadtxt(ElasticData,float,usecols=(0,1,2,3,4,5,6,7,8),skiprows=11)
    EPoint=Edata[:,0]
    EPlab=Edata[:,1] #GeV/c
    EPlab_min=Edata[:,2]
    EPlab_max=Edata[:,3]
    ESig=Edata[:,4]
    EStEr_H=Edata[:,5]
    EStEr_L=Edata[:,6]
    ESyEr_H=Edata[:,7]
    ESyEr_L=Edata[:,8]


def Ecm(Plab):
    """Converts Plab momenta to center of mass energy [GeV]."""
    E=(((Plab**2+.938**2)**(1/2.)+.938)**2-(Plab**2))**(1/2.)
    return E

if DataFound1==True and DataFound2==True:
    #Automatically converts all P_lab momenta to corresponding center-of-mass energy [GeV]
    E_cm=Ecm(Plab)
    eE_cm=Ecm(EPlab)
    cm_min=Ecm(Plab_min)
    cm_max=Ecm(Plab_max)
    ecm_min=Ecm(EPlab_min)
    ecm_max=Ecm(EPlab_max)

#Define best fit curve given by the particle data group
def func(s,P,H,M,R1,R2,n1,n2):
    m=.93827 #Proton mass GeV/c^2
    sM=(2*m+M)**2 #Mass^2 (GeV/c^2)^2
    hbar=6.58211928*10**-25 #GeV*s
    c=2.99792458*10**8 #m/s
    sigma=H*(np.log(s**2/sM))**2+P+R1*(s**2/sM)**(-n1)-R2*(s**2/sM)**(-n2)
    return sigma

#Apply best fit curve to the elastic cross-section data
s=eE_cm[:]
y=ESig[:]
p0=[4.45,.0965,2.127,11,4,.55,.55]
popt,pcov=curve_fit(func,s,y,p0)

#Apply best fit curve to total cross-section data
s2=E_cm[90:]
y2=Sig[90:]
p0=[34.49,.2704,2.127,12.98,7.38,.451,.549]
popt2,pcov2=curve_fit(func,s2,y2,p0)

def SigI(BE):
    """Returns the proton-proton cross-sectional area [fm^2] for given beam energy [GeV]"""
    return .1*(func(BE,popt2[0],popt2[1],popt2[2],popt2[3],popt2[4],popt2[5],popt2[6])-func(BE,popt[0],popt[1],popt[2],popt[3],popt[4],popt[5],popt[6]))



with open("systems/NNCrossSection.csv", "w") as file:
    
    for EB in range(50, 500, 1):
        file.write(f"{EB},{SigI(EB)}\n")
    file.write(f"5020,{SigI(5020.0)}\n")
