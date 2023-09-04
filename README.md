# DejaVu - [C++] A computer vision and image processing library

DejaVu is a library built on [SciPP](https://github.com/Reiex/SciPP) and [Diskon](https://github.com/Reiex/Diskon) for computer vision and image processing. The goal is to have efficient yet simple tools for simple image manipulations but also for advanced image processing.

The library follows two rules:
- No parallel computing inside the library (but the whole library is thread safe).
- No GPU use.

DejaVu is not meant to be used directly as a computer vision pipeline, as these pipelines often benefits from GPU use. However it is perfect to be used for image loading and for algorithms that cannot be done on GPU side. It is also a good prototyping library to conceive more advanced computer vision pipelines.

Here's a mosaic of images processed using DejaVu:

![](https://mpelegrin.hd.free.fr/images/dejavu/mosaic.webp)

From top to bottom and from left to right:
- The original picture (Lena).
- The gaussian bilateral filter (noise reduction without altering edges).
- A grayscale conversion.
- The transposed image.
- The image rotated using bilinear interpolation.
- The "mean blurred" image.
- The "gaussian blurred" image.
- The median filter.
- The k-Means algorithm applied with 10 clusters.
- The SLIC superpixel algorithm applied with 1000 regions.
- The horizontal gradient.
- The vertical gradient.
- The laplacian.
- The Marr-Hildreth edge detection.
- The Canny edge detection.
