This repository is part of the work presented in [Fast parallel algorithms for finding the longest flow paths in flow direction grids](https://doi.org/10.1016/j.envsoft.2023.105728).

---

Probably the simplest way to build the measurement application (using the GNU C++ compiler):

```
g++ -std=c++11 -O3 -fopenmp *.cpp -lgdal -o time_measurement
```

[GDAL](https://gdal.org/) is the only external dependency required (used here to load and save geospatial raster data).
