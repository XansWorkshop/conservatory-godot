/**************************************************************************/
/*  mesh_direct_access.h                                                  */
/**************************************************************************/
/*                         This file is part of:                          */
/*                 GODOT ENGINE /// THE CONSERVATORY FORK                 */
/*          https://godotengine.org /// https://xansworkshop.com          */
/**************************************************************************/
/* Copyright (c) 2026-present Xan's Workshop.                             */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#ifndef _3D_DISABLED
#include "core/object/object.h"
#include "core/object/object_id.h"
#include "core/object/class_db.h"
#include "servers/rendering/rendering_server.h"

class MeshDirectAccess : public Object {
	GDCLASS(MeshDirectAccess, Object);

protected:
	static void _bind_methods() {
		ClassDB::bind_static_method(MeshDirectAccess::get_class_static(), D_METHOD("surface_data_get_info_ptr", "sizes_ptr_i64_l2", "offsets_ptr_i32_l16"), &MeshDirectAccess::surface_data_get_info_ptr);
		ClassDB::bind_static_method(MeshDirectAccess::get_class_static(), D_METHOD("mesh_get_surface_ptr", "mesh", "idx", "surf_ptr"), &MeshDirectAccess::mesh_get_surface_ptr);
		ClassDB::bind_static_method(MeshDirectAccess::get_class_static(), D_METHOD("mesh_add_surface_ptr", "mesh", "surf_ptr"), &MeshDirectAccess::mesh_add_surface_ptr);
	}

public:
	static void surface_data_get_info_ptr(uint64_t p_sizes_ptr, uint64_t p_offset_int32_ptr) {
		uint32_t *offsets = (uint32_t*)p_offset_int32_ptr;
		uint64_t *sizes = (uint64_t *)p_sizes_ptr;
		sizes[0] = sizeof(RenderingServerTypes::SurfaceData);
		sizes[1] = sizeof(RenderingServerTypes::SurfaceData::LOD);
		offsets[0] = offsetof(RenderingServerTypes::SurfaceData, primitive);
		offsets[1] = offsetof(RenderingServerTypes::SurfaceData, format);
		offsets[2] = offsetof(RenderingServerTypes::SurfaceData, vertex_data);
		offsets[3] = offsetof(RenderingServerTypes::SurfaceData, attribute_data);
		offsets[4] = offsetof(RenderingServerTypes::SurfaceData, skin_data);
		offsets[5] = offsetof(RenderingServerTypes::SurfaceData, vertex_count);
		offsets[6] = offsetof(RenderingServerTypes::SurfaceData, index_data);
		offsets[7] = offsetof(RenderingServerTypes::SurfaceData, index_count);
		offsets[8] = offsetof(RenderingServerTypes::SurfaceData, aabb);
		offsets[9] = offsetof(RenderingServerTypes::SurfaceData, lods);
		offsets[10] = offsetof(RenderingServerTypes::SurfaceData, bone_aabbs);
		offsets[11] = offsetof(RenderingServerTypes::SurfaceData, mesh_to_skeleton_xform);
		offsets[12] = offsetof(RenderingServerTypes::SurfaceData, blend_shape_data);
		offsets[13] = offsetof(RenderingServerTypes::SurfaceData, uv_scale);
		offsets[14] = offsetof(RenderingServerTypes::SurfaceData, material);
		offsets[15] = offsetof(RenderingServerTypes::SurfaceData::LOD, edge_length);
		offsets[16] = offsetof(RenderingServerTypes::SurfaceData::LOD, index_data);
	}

	static void mesh_get_surface_ptr(RID p_mesh, int p_idx, uint64_t p_surfdata_ptr) {
		RenderingServerTypes::SurfaceData *surf = (RenderingServerTypes::SurfaceData *)p_surfdata_ptr;
		*surf = RenderingServer::get_singleton()->mesh_get_surface(p_mesh, p_idx);
	}
	static void mesh_add_surface_ptr(RID p_mesh, uint64_t p_surfdata_ptr) {
		RenderingServerTypes::SurfaceData *surf = (RenderingServerTypes::SurfaceData *)p_surfdata_ptr;
		RenderingServer::get_singleton()->mesh_add_surface(p_mesh, *surf);
	}
};
#endif
