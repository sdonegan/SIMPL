Convert Angles to Degrees or Radians {#changeanglerepresentation}
=============

## Group (Subgroup) ##
OrientationAnalysis Filters (Conversion)

## Description ##
This Filter will multiply the values of every **Element** by a factor to convert *degrees to radians* or *radians to degrees*.  The user needs to know the units of their data in order to use this Filter properly. 


### Example Usage ###

2D data files that are in *HKL*'s .ctf format use degrees. 3D data files in *HKL*'s .ctf format use radians. All files in *TSL*'s .ang format use radians. If other file types are used, determine the units before running this Filter.
 
Generally, all Filters within DREAM.3D use radians as a default unit, but Filters may be added that require the Euler angles to be in degrees and this Filter will allow the user to move back-and-forth between the units.

If the wrong conversion is selected (e.g., the user chooses *degrees to radians*, but their data is already in radians), the Filter will apply the conversion anyway and will result in erroneous data being used for the rest of the pipeline. If unsure about the units, check with the software supplier. 

## Parameters ##

| Name | Type | Description |
|------|------|------|
| Conversion Type | Selection | Tells the Filter which conversion is being made |

## Required Geometry ##

Not Applicable

## Required Arrays ##

| Type | Default Name | Type | Component Dimensions | Description |
|------|--------------|------|----------------------|-------------|
| Element | Angles    | Float | (n)                 | Values that represent angles in degrees or radians |

## Created Arrays ##

None

## Authors ##
**Copyright:** 2015 BlueQuartz Software, LLC

**Contact Info:** dream3d@bluequartz.net

**Version:** 1.0.0

**License:**  See the License.txt file that came with DREAM3D.




See a bug? Does this documentation need updated with a citation? Send comments, corrections and additions to [The DREAM3D development team](mailto:dream3d@bluequartz.net?subject=Documentation%20Correction)
