#ifndef RAY_CAST_RESULT_H
#define RAY_CAST_RESULT_H

#include "core/typedefs.h"
#include "core/object/class_db.h"
#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/object/object_id.h"
#include "core/templates/rid.h"
#include "core/math/vector3.h"
#include "core/variant/binder_common.h"

class RayCastResult : public RefCounted {
	GDCLASS(RayCastResult, RefCounted);

	Vector3 position;
	Vector3 normal;
	RID rid;
	ObjectID collider_id;
	Object *collider = nullptr;
	int shape = -1;
	int face_index = -1;
	int type = 0;
	bool success = false;

protected:
	static void _bind_methods();

public:
	Vector3 get_hit_position() const;
	void set_hit_position(const Vector3 &p_position);

	Vector3 get_hit_normal() const;
	void set_hit_normal(const Vector3 &p_normal);

	RID get_rid() const;
	void set_rid(const RID &p_rid);

	ObjectID _get_hit_object_id() const;
	int64_t get_hit_object_id() const;
	void _set_hit_object_id(const ObjectID &p_id);
	void set_hit_object_id_and_instance(const int64_t p_id);

	Object *get_hit_object() const;
	void set_hit_object(const Object *p_collider);

	int get_collider_type() const;
	void set_collider_type(int p_type);

	int get_shape_index() const;
	void set_shape_index(int p_shape);

	int get_face_index() const;
	void set_face_index(int p_face_index);

	bool get_success() const;
	void set_success(bool p_success);

	void clear();

	void copy_to(const Ref<RayCastResult> &p_destination) const;

	RayCastResult();

};

#endif // RAY_CAST_RESULT_H
