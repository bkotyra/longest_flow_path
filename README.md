This repository is part of an ongoing publication.

---

Probably the simplest way to build the measurement application (using the GNU C++ compiler):

```
g++ -std=c++11 -O3 -fopenmp *.cpp -lgdal -o time_measurement
```

[GDAL](https://gdal.org/) is the only external dependency required (used here to load and save geospatial raster data).
