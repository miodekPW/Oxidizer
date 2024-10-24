# Welcome to Oxidizer!

This software is dedicated to calculations of microstructural changes during high-temperature oxidation of porous FeCr steels. It reads data in form of:
- binarized bmp images representing 2D image of porous microstructure (e.g. from SEM observations)
- series of binarized bmp images representing subsequent slices of the porous microstructure (e.g. from CT data)

The oxide growth simulation is based on morphological operations of dilation and erosion. During simulation, it calculates morphological changes, porosity and Cr content during oxidation. Time domain discretization is based on resolution of the input data, as each dilation step leads to the growth of the oxide layer of a thickness equal to the pixel/voxel size. More information about the exact procedure can be found in paper: "to be added after publishing".

This software is fully opensourced and you can freely use it and modify. If you want to publish results generated with this software please cite:

- "to be added after publishing".

# Classes
**main.cpp** - starting point in which you specify necessary information about oxidation kinetics (kp value, initial chromium content) and image data (path, resolution).
**voxel.h** - class dedicated to store information about each voxel (location, assignment) in the dataset.
**matrix.h** - class dedicated to store whole image data and to perform operation on it.

## Methods
Implemented methods to control the software:
- **Load2Ddata** - reads single bmp image from specified **directory**. Pixels with grayscale value below **treshold** will be assigned as pore, pixels with higher value will be assigned as material.
- **Load3Ddata** - reads series of bmp images from specified **directory**. It is necessary to format this series of files with increasing numbers as filename ending, similar to standard 3D data e.g. from CT observations. Pixels with grayscale value below **treshold** will be assigned as pore, pixels with higher value will be assigned as material.
- **oxideGrowth** - performs **nrSteps**  of dilation/erosion steps for oxidation simulation with **kp** and initial chromium content "cr_content" regulating the kinetics of this process. **fstream &results** is a pointer to the results file in which measurements of porosity and chromium content will be saved.

- **dilate** - performs one dilation step. **direction** is a modifier for future pore connectivity inclusion in the simulation process (to perform dilation only in the corrected pore space). For now supported value is **7** - all pores available for dilation procedure.
- **erosion** - performs one erosion step.
- **draw2D** - exports 2D bitmap. **zlayer** is the z coordinate of given slice. For simulations on 2D dataset, the value should be set as **0**.
- **draw3D** - exports series od 2D bitmaps, representing 3D microstructure in form of subsequent slices.
- **porosity** - calculates porosity value in the current state of the model.
- **calculateCrContent** - calculates Cr content value in the current state of the model.


