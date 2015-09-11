// TakeYour2007.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <conio.h>

class ejbHeader
{
public:
	char type[3];

	int width;
	int height;
	int depth;

	int dictR[256];
	int dictG[256];
	int dictB[256];
};

int byteToInt(char* in, int count)
{
	int out = 0;
	memcpy(&out, in, count);
	return out;
}

int readByteInt(ifstream* input, int count)
{
	char* buf = new char[count];

	input->read(buf, count);
	int out = byteToInt(buf, count);

	delete[] buf;
	return out;
}


int main(int argc, char* argv[])
{
	string arg;
	string fileToOpen = "";
	for (int i = 0; i < argc; i++)
	{
		arg = argv[i];
		if (arg.substr(arg.length() - 4) == ".ejb") {
			fileToOpen = arg;
		}
	}
	if (fileToOpen == "")
	{
		cout << "Please enter the file name to be opened" << endl << "> ";
		cin >> fileToOpen;
	}
	//else { FreeConsole(); }

	sf::Image image;
	sf::Texture brushTexture;
	sf::RectangleShape brushRect;
	ifstream in;
	ejbHeader head;
	char buf[64];
	// Opening file
	in.open(fileToOpen, ios::binary);

	vec2i imageSize;
	// Reading the header
	in.read(buf, 3);		head.type[0] = buf[0];	head.type[1] = buf[1];	head.type[2] = buf[2];
	// EJB!
	if (buf[0] = 'E' && buf[1] == 'J' && buf[2] == 'B')
	{
		head.width = readByteInt(&in, 2);
		head.height = readByteInt(&in, 2);
		head.depth = readByteInt(&in, 1);
		cout << head.width << endl;
		cout << head.height << endl;
	
		// Reading the dictionary
		for (int i = 0; i < head.depth; i++)
		{
			head.dictR[i] = readByteInt(&in, 1);
			head.dictG[i] = readByteInt(&in, 1);
			head.dictB[i] = readByteInt(&in, 1);
		}

		// Reading the pixel data
		image.create(head.width, head.height);
		for (int x = 0; x < head.width; x++)
		{
			for (int y = 0; y < head.height; y++)
			{
				int pixel = readByteInt(&in, 1);
				image.setPixel(x, y, color(head.dictR[pixel], head.dictG[pixel], head.dictB[pixel]));
			}
		}
		imageSize = vec2i(head.width, head.height);
	}
	else {
		cout << "File is not a correct file type!" << endl << "Press any key to continue..." << endl;
		_getch();
		return 0;
	}

	//FreeConsole();
	in.close();
	brushTexture.setSmooth(true);
	brushTexture.loadFromImage(image);

	//imageSize *= 4;
	sf::RenderWindow winHandle;
	winHandle.create(sf::VideoMode(imageSize.x, imageSize.y), fileToOpen);

	while (winHandle.isOpen())
	{
		sf::Event eventPoll;
		// Poll events
		while (winHandle.pollEvent(eventPoll)) {
			if (eventPoll.type == sf::Event::Closed) { return 0; }
		}
		// Draw
		winHandle.clear(color(255, 0, 0));

		brushRect.setTexture(&brushTexture);
		brushRect.setSize(vec2f(imageSize.x, imageSize.y));
		winHandle.draw(brushRect);

		winHandle.display();
		Sleep(100);
	}

	return 0;
}