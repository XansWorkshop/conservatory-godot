/**************************************************************************/
/*  ray_cast_3d_direct.h                                                  */
/**************************************************************************/
/*                         This file is part of:                          */
/*                 GODOT ENGINE /// THE CONSERVATORY FORK                 */
/*          https://godotengine.org /// https://xansworkshop.com          */
/**************************************************************************/
/*                     DERIVED FROM GODOT SOURCE CODE                     */
/*                       SEE ORIGINAL LICENSE BELOW                       */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
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

#if !defined(PHYSICS_3D_DISABLED) && !defined(_3D_DISABLED)
#ifndef RAY_CAST_3D_DIRECT_H
#define RAY_CAST_3D_DIRECT_H

#include "core/object/object.h"
#include "scene/main/node.h"
#include "scene/3d/node_3d.h"
#include "servers/physics_server_3d.h"
#include "scene/3d/physics/collision_object_3d.h"
#include "scene/3d/physics/ray_cast_result.h"

class RayCast3DDirect : public RefCounted {
	GDCLASS(RayCast3DDirect, RefCounted);

	bool collided = false;
	RayCastResult::PhysicsObjectType type = RayCastResult::PhysicsObjectType::INVALID;
	ObjectID against;
	RID against_rid;
	int against_shape = 0;
	Vector3 collision_point;
	Vector3 collision_normal;
	int collision_face_index = -1;

	Vector3 source_position = Vector3(0, 0, 0);
	Vector3 target_position = Vector3(0, 0, 1);
	HashSet<RID> exclude;

	uint32_t collision_mask = 0xFFFFFFFF;

	bool collide_with_areas = false;
	bool collide_with_bodies = true;

	bool hit_from_inside = false;
	bool hit_back_faces = true;

protected:
	static void _bind_methods();

public:
	void set_collide_with_areas(bool p_enabled);
	bool is_collide_with_areas_enabled() const;

	void set_collide_with_bodies(bool p_enabled);
	bool is_collide_with_bodies_enabled() const;

	void set_hit_from_inside(bool p_enabled);
	bool is_hit_from_inside_enabled() const;

	void set_hit_back_faces(bool p_enabled);
	bool is_hit_back_faces_enabled() const;

	void set_source_position(const Vector3 &p_point);
	Vector3 get_source_position() const;

	void set_target_position(const Vector3 &p_point);
	Vector3 get_target_position() const;

	void set_transform_and_distance(const Transform3D &p_transform, real_t p_length);

	void set_collision_mask(uint32_t p_mask);
	uint32_t get_collision_mask() const;

	void set_collision_mask_value(int p_layer_number, bool p_value);
	bool get_collision_mask_value(int p_layer_number) const;

	void set_from_parameters(const Ref<PhysicsRayQueryParameters3D> &p_parameters);
	bool cast(const RID &p_space);
	static bool cast_statically(const RID &p_space, const Ref<PhysicsRayQueryParameters3D> &p_parameters, const Ref<RayCastResult> &p_result);
	// static bool cast_statically_unsafe(const RID &p_space, const Ref<PhysicsRayQueryParameters3D> &p_parameters, const int64_t p_result);
	void store_in_result(const Ref<RayCastResult> &p_result) const;
	void store_in_result_unsafe(const int64_t p_result) const;

	bool get_hit_something() const;
	Object *get_hit_object() const;
	ObjectID get_hit_object_id() const;
	RID get_collider_rid() const;
	RayCastResult::PhysicsObjectType get_collider_type() const;
	int get_collider_shape() const;
	int get_collider_face_index() const;
	Vector3 get_collision_point() const;
	Vector3 get_collision_normal() const;

	void add_exception_rid(const RID &p_rid);
	void add_exception(const CollisionObject3D *p_node);
	void remove_exception_rid(const RID &p_rid);
	void remove_exception(const CollisionObject3D *p_node);
	void clear_exceptions();

	RayCast3DDirect();
	RayCast3DDirect(const Ref<PhysicsRayQueryParameters3D> &p_parameters);
};

#endif // RAY_CAST_3D_DIRECT_H
#endif // !defined(PHYSICS_3D_DISABLED) && !defined(_3D_DISABLED)
