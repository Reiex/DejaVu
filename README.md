# DejaVu - A simple C++ Image Processing Library


## Introduction

DejaVu is a little C++ image processing library I coded to help me during my computer science studies.
This library implements all kind of tools usually needed to do image processing - it is a kind of OpenCV but buggy and messy and without the machine learning part.
These tools are, for example: Images, Edge detectors, Corner detectors...

---

The philosophy of this library is to be simple yet efficient. For this, I fixed certain rules:
- Only one dependency: [SciPP](https://github.com/Reiex/SciPP)
- Multi-threading only using simple OpenMP pragmas
- No GPU use
