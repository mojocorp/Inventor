#ifndef SO_IMAGE_H
#define SO_IMAGE_H

#include <Inventor/nodes/SoShape.h>

class  SoImage : public SoShape {

 SO_NODE_HEADER(SoImage);

public:
	enum HorAlignment {
		LEFT,
		CENTER,
		RIGHT
	};

	enum VertAlignment {
		BOTTOM,
		HALF,
		TOP
	};
};

#endif
