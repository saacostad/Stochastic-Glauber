# Stochastic Geometrical-Glauber-Model Simulations for Non-Spherical Nuclei

This project aims to develop an efficient and robust framework for geometrical Glauber Model simulations of non-spherical shaped nuclei.


---

# Set-up
In this section we'll rapidly explain what's necessary to build and how to build the project.

## Pre-requisites
This project, for efficiency, runs the heaviest chunks of code in parallel using all of the PC's power by using GNU's _OpenMP_ library, so, in case one does not have it, it is mandatory to install it before building the project.
If you'll build the project from the `makefile`, you'll also need the _GNU_ `g++` compiler for C++.

Mandatory python packages for further analysis are `numpy`, `matplotlib` and `scipy` (and default packages that, if not installed, you'll have to install manually). In case you're interested in graphing, you'll probably also need `pyvista`

## Build

Once you're sure you'll have all dependencies, you can build the project by calling `make build` on the local project's folder.

---

# Modeling

If you'll create your own system's nuclei, in the folder `systems` create a new folder, will call it _system_. Inside your system folder, you'll need to create (or copy and paste from previous _system_s) the files for the 
Spherical Harmonic Expansion (the non-spherical betas) called `pn.csv`, `pp.csv`, `tn.csv` and `tp.csv` standing for _Projectile Negative_, _Projectile Positive_, _Tarjet Negativa_ and Tarjet Positive_, where one can write the beta coefficientes in tab-separated-format.
On the other hand, you'll also need two files `prad.csv` and `trad.csv` where you'll place the 3 Parameter Fermi Model of the radial density parameters $$R_0$$, $$r$$, $$a$$ and $$w$$ 

Once you'll created your model (the spherical harmonics expansion coefficients and the 3PFM parameters), you can **write** your first model.


## Writing

The **write** mode is the one you'll use to create your systems, and is called with the `-m write` flag. You'll need 6 mandatory values to create it

1. `-E`: the nucleon energy
2. `-T`: the number of tarjet nuclei to simulate
3. `-P`: the number of projectile nuclei to simulate
4. `-t`: the number of nucleons on each tarjet nuclei
5. `-p`: the number of nucleons on each projectile nuclei
6. `-I`: the system's name

### Example of writing a system
In the repository, one can find the system `Au197-Au197` which models gold-gold collisions.

Inside of `prad.csv` and `trad.csv` one finds the values `6.38 1.0 0.535 0.0`, where 6.38 is the mean radius of gold in fermis, 1.0 is the $$r$$ used in the 3PFM (which we keep as 1 as it does not change the output), 0.535 is the parameter $$a$$ and $$w$$ is set to 0.
Inside of `pp.csv` and `tp.csv` one can find, in matrix form, the $$\beta_{ij}$$ coefficients of the spherical harmonics expansion. `np.csv` and `nt.csv` are set to 0 as there are no coefficientes for the conjugate $$Y_{lm}$$

In that way, if I want to simulate 200 nuclei for tarjets and projectiles at a nucleon energy of 200.0 GeVs for Au197-Au197, I execute

``./StochasticGlauber -I Au197-Au197 -m write -E 200.0 -P 200 -T 200 -t 197 -p 197``

After executing, a new folder `200` will be created on the _system_'s folder where the information of this nuclei rest.

## Reading

Once one has the system created, one reads it and simulates the collisions by using the `-m read` flag. It will take the following flags

1. `-E`: the nucleon energy
2. `-I`: the system's name
3. `-p`: the number of nucleons on each projectile nuclei
4. `-t`: the number of nucleons on each tarjet nuclei
5. `-b`: the maximum value of the impact parameter to simulate
6. `-d`: the $$\Delta b$$ value between simulations
7. `-O`: the output file where we'll save the information of our collisions
8. `-r`: the number of rotations to apply per nucleus


One would call the command as

``./StocasticGlauber -m read -i Au197-Au197 -E 200 -t 197 -p 197 -r 5 -b 25 -d 0.1 -O output_data.csv``

---

# Output data

The output of the **read mode** is a comma-separated-value file with columns that will be saved inside the given energy folder of the _system_

1. The impact parameter
2. The mean collisions for the given impact parameter
3. Statistical error for the mean collisions
4. The mean participants for the given impact parameter
5. Statistical error for the given impact parameter


## Further analysis

To get the _inelastic cross section_, one has to modidy the `outils/graphNNCrossSection.py` file in order for it to point to the output data, as well as giving it some of the system's parameters.
Executing this script will give an stimate of the mentioned cross section, as well as plot the number of collisions/participants vs impact paramenter graph and the $$\partial \sigma_{inel} / \partial b $$ graph.
