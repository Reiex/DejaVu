#include <DejaVu/DejaVu.hpp>

int main()
{
	djv::Img image("examples/assets/Lena.png");

	djv::Img(djv::laplacianOfGaussian(image.getComponent(djv::ColorComponent::R), 1.4f)).saveToFile("test.png");

	return 0;
}
