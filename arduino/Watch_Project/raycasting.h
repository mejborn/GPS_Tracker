typedef struct { double x, y; } vec;
typedef struct { int n; vec* v; } polygon_t, *polygon;

#define BIN_V(op, xx, yy) vec v##op(vec a,vec b){vec c;c.x=xx;c.y=yy;return c;}
#define BIN_S(op, r) double v##op(vec a, vec b){ return r; }

