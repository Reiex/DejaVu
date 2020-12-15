# DejaVu - A simple C++ Image Processing and Machine Learning Library


## Introduction

DejaVu is a little C++ image processing and machine learning library I coded to help me during my computer science studies.
This library implements all kind of tools usually needed to do image processing and machine learning - it is a mix between OpenCV and Keras but buggy and messy.
These tools are, for example: Images, feature detectors (edges, ridges...), Perceptrons, Neural Networks...

---

The philosophy of this library is to be simple yet efficient. For this, I fixed certain rules:
- Only one dependency: [SciPP](https://github.com/Reiex/SciPP)
- Multi-threading only using simple OpenMP pragmas
- No GPU use
