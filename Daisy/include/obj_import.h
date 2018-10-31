#ifndef DAISY_OBJ_IMPORT_H
#define DAISY_OBJ_IMPORT_H

#include <string>
#include <memory>
#include "mesh.h"

namespace daisy {
	std::shared_ptr<mesh const> import_obj_mesh(std::string path);
}

#endif // !DAISY_OBJ_IMPORT_H
