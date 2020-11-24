#include <DejaVu/DejaVu.hpp>

int main()
{
	djv::Img image("examples/assets/Lena.png");

	image.saveToFile("test.png");

	return 0;
}
