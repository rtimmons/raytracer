#include <iostream>
#include <cmath>

class vec3 {
public:
    static vec3 one() {
        return vec3{1.0f};
    }

    explicit vec3(float a)
    : vec3{a,a,a} {}

    vec3(float x, float y, float z)
    : e{x,y,z} {}

    float x() const { return e[0]; }
    float r() const { return x(); }
    float y() const { return e[1]; }
    float g() const { return y(); }
    float z() const { return e[2]; }
    float b() const { return z(); }

    float operator[](int i) const { return e[i]; }
    float& operator[](int i) { return e[i]; }

    float& x() { return e[0]; }
    float& y() { return e[1]; }
    float& z() { return e[2]; }


    float dotself() const {
        return this->dot(*this);
    }

    vec3 operator -() {
        return {-x(), -y(), -z()};
    }

    vec3& operator +=(const vec3& other) {
        x() += other.x();
        y() += other.y();
        z() += other.z();
        return *this;
    }

    vec3& operator -=(const vec3& other) {
        x() -= other.x();
        y() -= other.y();
        z() -= other.z();
        return *this;
    }

    vec3 operator-(const vec3& b) const {
        vec3 out = *this;
        out -= b;
        return out;
    }

    vec3 operator+(const vec3& b) const {
        vec3 out = *this;
        out += b;
        return out;
    }

    vec3& operator *=(const vec3& other) {
        x() *= other.x();
        y() *= other.y();
        z() *= other.z();
        return *this;
    }
    vec3 operator *(const vec3& b) const {
        vec3 out = *this;
        out *= b;
        return out;
    }
    inline vec3 operator*(float f) const {
        vec3 out = *this;
        out *= vec3{f, f, f};
        return out;
    }

    vec3& operator /=(const vec3& other) {
        x() /= other.x();
        y() /= other.y();
        z() /= other.z();
        return *this;
    }
    vec3 operator /(const vec3& b) const {
        vec3 out = *this;
        out /= b;
        return out;
    }
    vec3 operator /(const float& b) const {
        vec3 out = *this;
        out.x() /= b;
        out.y() /= b;
        out.z() /= b;
        return out;
    }

    vec3 unit_vector() const {
        return *this / this->length();
    }

    float length() const {
        return sqrt(squared_length());
    }
    float squared_length() const {
        return x()*x() + y()*y() + z()*z();
    }

    float dot(const vec3& other) const {
        return (
            (x() * other.x()) +
            (y() * other.y()) +
            (z() * other.z())
        );
    }

private:
    float e[3] = {0,0,0};
};

vec3 operator/(float f, const vec3& v) {
    return v / f;
}

vec3 operator*(float f, const vec3& v) {
    return v * f;
}

std::ostream& operator<<(std::ostream& out, const vec3& v) {
    out << "<" << v.x() << "," << v.y() << "," << v.z() << ">";
    return out;
}

class ray {
public:
    explicit ray(vec3 a, vec3 b)
    : a{a}, b{b} {}
    vec3 origin() const { return a; }
    vec3 direction() const { return b; }
    vec3 point_at(float t) const { return a + (b*t); }
private:
    vec3 a;
    vec3 b;
};

struct hit {
    float t = 0;
    vec3 p = {0,0,0};
    vec3 normal = {0,0,0};
};
class hittable {
public:
    virtual float hits(const ray& r, float tmin, float tmax, hit& rec) const = 0;
};

class sphere : public hittable {
public:
    sphere(const vec3 &center, float radius)
    : center(center), radius(radius) {}

    // ray = Origin(=A) + Direction(=B)*t => ray = A + Bt
    float hits(const ray& r, float tmin, float tmax, hit& rec) const override {
        // at^2 + bt + c = 0 => t = (-b +- sqrt(b^2 - 4ac)) / 2a
        // B^2t^2 +  2(B(A-C))*t  +  ((A-C)^2 - Radius^2)
        // a = B^2,  b = 2(B(A-C)),  c = ((A-C)^2 - Radius^2)
        //      WHERE: A = r.origin(); B = r.directin(); C = center
        auto oc = r.origin() - center;
        float a = r.direction().dotself();
        float b = 2.0f * (r.direction().dot(oc));
        float c = oc.dotself() - radius * radius;
        float disc = b*b - 4.0f*a*c;
        if (disc < 0) {
            return -1.0f;
        }
        else {
            return (-b - sqrt(disc))/(2.0f*a);
        }
//
//        if (disc > 0) {
//            rec.normal = r.point_at((-b - sqrt(disc))/(2*a)).unit_vector();
//            return true;
//        }
//        return disc > 0;
//
//        if (disc > 0) {
//            float t = (-b - sqrt(disc)) / 2*a;
//            if (t < tmax && t > tmin) {
//                rec.t = t;
//                rec.p = r.point_at(rec.t);
//                rec.normal = (rec.p - center) / radius;
//                return true;
//            }
//            t = (-b + sqrt(disc)) / 2*a;
//            if (t < tmax && t > tmin) {
//                rec.t = t;
//                rec.p = r.point_at(rec.t);
//                rec.normal = (rec.p - center) / radius;
//                return true;
//            }
//        }
//        return false;
    }
private:
    vec3 center;
    float radius;
};

class colorize {
public:
    explicit colorize(const vec3 &col) : col(col) {}

    std::ostream& operator()(std::ostream& out) const {
        int ir = int(255.99*col.r());
        int ig = int(255.99*col.g());
        int ib = int(255.99*col.b());
        out << ir << " " << ig << " " << ib;
        return out;
    }
private:
    const vec3& col;
};

std::ostream& operator<<(std::ostream& out, const colorize& col) {
    return col(out);
}

vec3 color(const ray& r) {
    sphere s {vec3{0,0,-1}, 0.5};
    hit hi;
    auto h = s.hits(r, 0.0, MAXFLOAT, hi);
    if (h > 0.0) {
        vec3 n = (r.point_at(h) - vec3{0,0,-1}).unit_vector();
        return 0.5 *(n + vec3{1});
//        return h.normal;
    } else {
        // render background
        vec3 unit = r.direction().unit_vector();
        float t = 0.5f*(unit.y() + 1.0f);
        return (1.0f - t)*vec3{1} + t*vec3{0.5, 0.7, 1.0};
    }
}

template <class T>
class TD;

int main() {
    using namespace std;

    int nx = 800;
    int ny = 400;

    vec3 lower_left {-2.f, -1.f, -1.f};
    vec3 horizontal {4.f, 0.f, 0.f};
    vec3 vertical   {0.f, 2.f, 0.f};
    vec3 origin     {0.f, 0.f, 0.f};

    cout << "P3\n" << nx << " " << ny << "\n255\n";
    for(int j=ny-1; j>=0; --j) {
        for(int i=0; i<nx; ++i) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            ray r { origin, lower_left + horizontal*u + vertical*v};
            vec3 col = color(r);
            cout << colorize(col) << std::endl;
        }
    }
    return 0;
}
