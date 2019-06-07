#ifndef _H_BSPMAP_
#define _H_BSPMAP_
#include <string>
#include <glm/glm.hpp>
class BSPMap {
public:
	BSPMap(const std::string& name);
	
	// data structures
	struct dentry {
		long offset;
		long size;
	};
	struct dheader {
		long  version;           // Model version, must be 0x17 (23).
		dentry entities;         // List of Entities.
		dentry planes;           // Map Planes.
								 // numplanes = size/sizeof(plane_t)
		dentry miptex;           // Wall Textures.
		dentry vertices;         // Map Vertices.
								 // numvertices = size/sizeof(vertex_t)
		dentry visilist;         // Leaves Visibility lists.
		dentry nodes;            // BSP Nodes.
								 // numnodes = size/sizeof(node_t)
		dentry texinfo;          // Texture Info for faces.
								 // numtexinfo = size/sizeof(texinfo_t)
		dentry faces;            // Faces of each surface.
								 // numfaces = size/sizeof(face_t)
		dentry lightmaps;        // Wall Light Maps.
		dentry clipnodes;        // clip nodes, for Models.
								 // numclips = size/sizeof(clipnode_t)
		dentry leaves;           // BSP Leaves.
								 // numlaves = size/sizeof(leaf_t)
		dentry lface;            // List of Faces.
		dentry edges;            // Edges of faces.
								 // numedges = Size/sizeof(edge_t)
		dentry ledges;           // List of Edges.
		dentry models;           // List of Models.
								 // nummodels = Size/sizeof(model_t)
	};
	struct dboundbox {
		glm::vec3 min;
		glm::vec3 max;
	};
	struct dboundboxshort {
		glm::i16vec3 min;
		glm::i16vec3 max;
	};
	struct dmodel {
		dboundbox bound;
		glm::vec3 origin;
		long node_id0;
		long node_id1;
		long node_id2;
		long node_id3;
		long numleafs;
		long face_id;
		long face_num;
	};
};
#endif