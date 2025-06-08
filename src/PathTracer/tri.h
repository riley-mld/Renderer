#ifndef TRI_H
#define TRI_H


class tri : public quad {
  public:
    tri(const point3& o, const vec3& ac, const vec3& ab, shared_ptr<material> m)
      : quad(o, ac, ab, m)
    {
      area /= 2.0;
    }

    bool is_interior(double a, double b, hit_record& rec) const override{
        if ((a < 0) || (b < 0) || (a + b > 1))
            return false;

        rec.u = a;
        rec.v = b;
        return true;
    }

    vec3 random(const point3& origin) const override {
        auto a = random_double();
        auto b = random_double();
        // If they fall outside the unit triangle (a+b>1),
        // reflect them back in by mapping (a,b) → (1−a,1−b)
        if (a + b > 1.0) {
            a = 1.0 - a;
            b = 1.0 - b;
        }
        // Now (a,b) are valid barycentric coords for the triangle
        auto p = Q + (a * u) + (b * v);
        return p - origin;
    }
};

#endif