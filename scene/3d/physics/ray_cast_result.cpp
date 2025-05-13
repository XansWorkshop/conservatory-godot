#include "ray_cast_result.h"

Vector3 RayCastResult::get_hit_position() const {
	return position;
}
Vector3 RayCastResult::get_hit_normal() const {
	return normal;
}
RID RayCastResult::get_rid() const {
	return rid;
}
ObjectID RayCastResult::_get_collider_id() const {
	return collider_id;
}
int64_t RayCastResult::get_collider_id() const {
	return (int64_t)collider_id;
}
Object *RayCastResult::get_collider() const {
	return collider;
}
int RayCastResult::get_collider_type() const {
	return type;
}
int RayCastResult::get_shape_index() const {
	return shape;
}
int RayCastResult::get_face_index() const {
	return face_index;
}
bool RayCastResult::get_success() const {
	return success;
}

void RayCastResult::set_hit_position(const Vector3 &p_position) {
	position = p_position;
}
void RayCastResult::set_hit_normal(const Vector3 &p_normal) {
	normal = p_normal;
}
void RayCastResult::set_rid(const RID &p_rid) {
	rid = p_rid;
}
void RayCastResult::_set_collider_id(const ObjectID &p_id) {
	collider_id = p_id;
}
void RayCastResult::set_collider_id_and_instance(const int64_t p_id) {
	// The public version
	collider_id = ObjectID(p_id);
	if (!collider_id.is_null()) {
		collider = ObjectDB::get_instance(collider_id);
	} else {
		collider = nullptr;
	}
}
void RayCastResult::set_collider(Object *p_collider) {
	collider = p_collider;
}
void RayCastResult::set_collider_type(int p_type) {
	type = p_type;
}
void RayCastResult::set_shape_index(int p_shape) {
	shape = p_shape;
}
void RayCastResult::set_face_index(int p_face_index) {
	face_index = p_face_index;
}
void RayCastResult::set_success(bool p_success) {
	success = p_success;
}

void RayCastResult::clear() {
	position = Vector3();
	normal = Vector3();
	rid = RID();
	collider_id = ObjectID();
	collider = nullptr;
	shape = -1;
	face_index = -1;
	type = 0;
	success = false;
}

void RayCastResult::copy_to(const Ref<RayCastResult> &p_destination) const {
	ERR_FAIL_COND_MSG(p_destination.is_null(), "The provided destination object is null.");
	RayCastResult *other = p_destination.ptr();
	other->position = position;
	other->normal = normal;
	other->rid = rid;
	other->collider_id = collider_id;
	other->collider = collider;
	other->shape = shape;
	other->face_index = face_index;
	other->type = type;
	other->success = success;
}

#define ADD_READONLY_PROPERTY(m_property, m_getter) ::ClassDB::add_property(get_class_static(), m_property, StringName(), _scs_create(m_getter))

void RayCastResult::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_success"), &RayCastResult::get_success);
	ClassDB::bind_method(D_METHOD("get_hit_position"), &RayCastResult::get_hit_position);
	ClassDB::bind_method(D_METHOD("get_hit_normal"), &RayCastResult::get_hit_normal);
	ClassDB::bind_method(D_METHOD("get_rid"), &RayCastResult::get_rid);
	ClassDB::bind_method(D_METHOD("get_collider_id"), &RayCastResult::get_collider_id);
	ClassDB::bind_method(D_METHOD("get_collider"), &RayCastResult::get_collider);
	ClassDB::bind_method(D_METHOD("get_collider_type"), &RayCastResult::get_collider_type);
	ClassDB::bind_method(D_METHOD("get_shape_index"), &RayCastResult::get_shape_index);
	ClassDB::bind_method(D_METHOD("get_face_index"), &RayCastResult::get_face_index);

	ClassDB::bind_method(D_METHOD("set_success"), &RayCastResult::set_success);
	ClassDB::bind_method(D_METHOD("set_hit_position"), &RayCastResult::set_hit_position);
	ClassDB::bind_method(D_METHOD("set_hit_normal"), &RayCastResult::set_hit_normal);
	ClassDB::bind_method(D_METHOD("set_rid"), &RayCastResult::set_rid);
	ClassDB::bind_method(D_METHOD("set_collider_id"), &RayCastResult::set_collider_id_and_instance);
	// ClassDB::bind_method(D_METHOD("set_collider"), &RayCastResult::set_collider);
	ClassDB::bind_method(D_METHOD("set_collider_type"), &RayCastResult::set_collider_type);
	ClassDB::bind_method(D_METHOD("set_shape_index"), &RayCastResult::set_shape_index);
	ClassDB::bind_method(D_METHOD("set_face_index"), &RayCastResult::set_face_index);

	ClassDB::bind_method(D_METHOD("clear"), &RayCastResult::clear);
	ClassDB::bind_method(D_METHOD("copy_to", "destination"), &RayCastResult::copy_to);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "success", PROPERTY_HINT_NONE), "set_success", "get_success");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "hit_position", PROPERTY_HINT_NONE, "suffix:m"), "set_hit_position", "get_hit_position");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "hit_normal", PROPERTY_HINT_NONE, "suffix:m"), "set_hit_normal", "get_hit_normal");
	ADD_PROPERTY(PropertyInfo(Variant::RID, "hit_rid", PROPERTY_HINT_NONE), "set_rid", "get_rid");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "hit_collider_id", PROPERTY_HINT_NONE), "set_collider_id", "get_collider_id");
	ADD_READONLY_PROPERTY(PropertyInfo(Variant::OBJECT, "hit_collider", PROPERTY_HINT_NONE), "get_collider");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "hit_type", PROPERTY_HINT_ENUM, "invalid,area,body,soft_body"), "set_collider_type", "get_collider_type");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "hit_shape_index", PROPERTY_HINT_NONE), "set_shape_index", "get_shape_index");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "hit_face_index", PROPERTY_HINT_NONE), "set_face_index", "get_face_index");
}

#undef ADD_READONLY_PROPERTY

RayCastResult::RayCastResult() {
}
