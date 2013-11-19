#ifndef COMPLEX_BLOCK_TEMPLATE_H
#define COMPLEX_BLOCK_TEMPLATE_H

#include "IrrWrapper.h"
#include "BaseBlockTemplate.h"
#include <string>

struct ComplexBlockTemplate : public BaseBlockTemplate
{
	std::string name;
	IMesh* mesh;
};

#endif