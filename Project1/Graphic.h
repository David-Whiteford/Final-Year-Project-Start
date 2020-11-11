#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <iostream>
using namespace std;
#include "IGraphic.h"

class Graphic : public IGraphic
{
public:
	Graphic();
	~Graphic();
	void Draw();

};
#endif
