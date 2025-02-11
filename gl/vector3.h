#ifndef _KARIN_VECTOR3_H
#define _KARIN_VECTOR3_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _vector3_s
{
	float v[3];
} vector3_s;

#define VECTOR3_X(vec) (vec).v[0]
#define VECTOR3_Y(vec) (vec).v[1]
#define VECTOR3_Z(vec) (vec).v[2]

#define VECTOR3V_X(vec) (vec)->v[0]
#define VECTOR3V_Y(vec) (vec)->v[1]
#define VECTOR3V_Z(vec) (vec)->v[2]

void vector3_normalize_self(vector3_s *a);
void vector3_scale_self(vector3_s *a, float n);
void vector3_invert_self(vector3_s *a);
void vector3_addv(vector3_s *r, const vector3_s *a, const vector3_s *b);
void vector3_subtractv(vector3_s *r, const vector3_s *a, const vector3_s *b);
void vector3_multiplyv(vector3_s *r, const vector3_s *a, const vector3_s *b);
void vector3_dividev(vector3_s *r, const vector3_s *a, const vector3_s *b);
void vector3_crossv(vector3_s *r, const vector3_s *a, const vector3_s *b);
void vector3_directionv(vector3_s *r, const vector3_s *a, const vector3_s *b);

vector3_s vector3_normalize(const vector3_s *vec);
float vector3_dot(const vector3_s *a, const vector3_s *b);
vector3_s vector3_cross(const vector3_s *a, const vector3_s *b);
float vector3_length(const vector3_s *a);
vector3_s vector3_add(const vector3_s *a, const vector3_s *b);
vector3_s vector3_subtract(const vector3_s *a, const vector3_s *b);
vector3_s vector3_multiply(const vector3_s *a, const vector3_s *b);
vector3_s vector3_divide(const vector3_s *a, const vector3_s *b);
vector3_s vector3_scale(const vector3_s *a, float n);
int vector3_valid(const vector3_s *a);
int vector3_equals(const vector3_s *a, const vector3_s *b);
vector3_s vector3_invert(const vector3_s *a);
vector3_s vector3_direction(const vector3_s *a, const vector3_s *b);

#ifdef __cplusplus
}
#endif

#endif
