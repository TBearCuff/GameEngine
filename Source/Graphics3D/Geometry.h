#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

const float  GCC_PI = 3.14159265358979f;
const float  GCC_2PI = 2.0f * GCC_PI;


typedef QVector2D Vec2;


class Vec3 : public QVector3D
{
public:
    Vec3() : QVector3D() {}
    Vec3(const QVector3D& qv) : QVector3D(qv) {}
    Vec3(const float _x, const float _y, const float _z) : QVector3D(_x, _y, _z) {}
    Vec3(const double _x, const double _y, const double _z) : QVector3D((float)_x, (float)_y, (float)_z) {}
    inline float Length() { return QVector3D::length(); }
    inline Vec3 Normalize() { return this->normalized(); }
    inline float Dot(const Vec3 &b) { return QVector3D::dotProduct((QVector3D&)*this, (QVector3D&)b); }
    inline Vec3 Cross(const Vec3 &b) const;
    inline Vec3(const class Vec4 &v4) : QVector3D((QVector4D&)v4) {}
};

inline Vec3 Vec3::Cross(const Vec3 &b) const
{
    return QVector3D::crossProduct((QVector3D&)*this, (QVector3D&)b);
}


class Vec4 : public QVector4D
{
public:
    Vec4() : QVector4D() {}
    Vec4(const QVector4D& qv) : QVector4D(qv) {}
    inline float Length() { return QVector4D::length(); }
    inline Vec4 Normalize() { return this->normalized(); }
    // If you want the cross product, use Vec3::Cross
    inline float Dot(const Vec4 &b) { return QVector4D::dotProduct((QVector4D&)*this, (QVector4D&)b); }

    Vec4(const float _x, const float _y, const float _z, const float _w) { QVector4D(_x, _y, _z, _w); }
    Vec4(const Vec3 &v3) : QVector4D(v3, 1.0f) {}
};

extern Vec3 g_Up;
extern Vec3 g_Right;
extern Vec3 g_Forward;

extern Vec4 g_Up4;
extern Vec4 g_Right4;
extern Vec4 g_Forward4;


////////////////////////////////////////////////////
//
// Vec3List Description
// Vec4List Description
//
//   An STL list of Vectors
//
////////////////////////////////////////////////////

typedef std::list<Vec3> Vec3List;
typedef std::list<Vec4> Vec4List;


////////////////////////////////////////////////////
//
// Quaternion Description
//
//
////////////////////////////////////////////////////

class Quaternion : public QQuaternion
{
public:
    Quaternion() : QQuaternion() {}
    Quaternion(const QQuaternion &q) : QQuaternion(q) {}
    static const Quaternion g_Identity;

    //Modifiers
    void Normalize() { this->normalize(); }
    void Slerp(const Quaternion &begin, const Quaternion &end, float cooef)
    {
        // performs spherical linear interpolation between begin & end
        // NOTE: set cooef between 0.0f-1.0f
        QQuaternion::slerp(begin, end, cooef);
    }

    // Accessors
    void GetAxisAngle(Vec3 &axis, float &angle) const
    {
        this->getAxisAndAngle(&axis, &angle);
    }

    //Initializers
    void Build(const class Mat4x4 &mat);
    void BuildRotYawPitchRoll(const float yawRadians, const float pitchRadians, const float rollRadians)
    {
        this->fromEulerAngles(pitchRadians, yawRadians, rollRadians);
    }

    void BuildAxisAngle(const Vec3 &axis, const float radians)
    {
        this->fromAxisAndAngle(axis, radians);
    }
};

inline Quaternion operator *(const Quaternion &a, const Quaternion &b)
{
    return a * b;
}


////////////////////////////////////////////////////
//
// Mat4x4 Description
//
//
////////////////////////////////////////////////////


class Mat4x4 : public QMatrix4x4
{
public:
    Mat4x4() : QMatrix4x4() {}
    Mat4x4(const QMatrix4x4 &m) : QMatrix4x4(m) {}
    static const Mat4x4 g_Identity;
    //Modifiers
    inline void SetPosition(Vec3 const &pos);
    inline void SetPosition(Vec4 const &pos);
    inline void SetScale(Vec3 const &scale);

    //Accessors and Calculation Methods
    inline Vec3 GetPosition() const;
    inline Vec3 GetDirection() const;
    inline Vec3 GetUp() const;
    inline Vec3 GetRight() const;
    inline Vec3 GetYawPitchRoll() const;
    inline Vec3 GetScale() const;
    inline Vec4 Xform(Vec4 &v) const;
    inline Vec3 Xform(Vec3 &v) const;
    inline Mat4x4 Inverse() const;

    //Initialization methods
    inline void BuildTranslation(const Vec3 &pos);
    inline void BuildTranslation(const float x, const float y, const float z );
    inline void BuildRotationX(const float radians);
    inline void BuildRotationY(const float radians);
    inline void BuildRotationZ(const float radians);
    inline void BuildYawPitchRoll(const float yawRadians, const float pitchRadians, const float rollRadians);
    inline void BuildRotationQuat(const Quaternion &q);
    inline void BuildRotationLookAt(const Vec3 &eye, const Vec3 &at, const Vec3 &up);
    inline void BuildScale(const float x, const float y, const float z );

};

inline void Mat4x4::SetPosition(const Vec3 &pos)
{
    QVector4D qv4d(pos, 1.0f);
    QMatrix4x4::setColumn(3, qv4d);
}

inline void Mat4x4::SetPosition(const Vec4 &pos)
{
    QMatrix4x4::setColumn(3, pos);
}

inline void Mat4x4::SetScale(const Vec3 &scale)
{
    QMatrix4x4::scale(scale);
}

inline Vec3 Mat4x4::GetPosition() const
{
    QVector4D qv4d = QMatrix4x4::column(3);
    return static_cast<Vec3>(qv4d.toVector3D());
}

inline Vec3 Mat4x4::GetDirection() const
{
    // Note - the following code can be used to double check the vector construction above.
    Mat4x4 justRot = *this;
    justRot.SetPosition(Vec3(0.f,0.f,0.f));
    Vec3 forward = justRot.Xform(g_Forward);
    return forward;
}

inline Vec3 Mat4x4::GetUp() const
{
    // Note - the following code can be used to double check the vector construction above.
    Mat4x4 justRot = *this;
    justRot.SetPosition(Vec3(0.f,0.f,0.f));
    Vec3 up = justRot.Xform(g_Up);
    return up;
}

inline Vec3 Mat4x4::GetRight() const
{
    // Note - the following code can be used to double check the vector construction above.
    Mat4x4 justRot = *this;
    justRot.SetPosition(Vec3(0.f,0.f,0.f));
    Vec3 right = justRot.Xform(g_Right);
    return right;
}
#if 0
inline Vec3 Mat4x4::GetYawPitchRoll() const
{
   float yaw, pitch, roll;

   pitch = asin(-_32);

   double threshold = 0.001; // Hardcoded constant - burn him, he's a witch
   double test = cos(pitch);

   if(test > threshold)
   {
      roll = atan2(_12, _22);
      yaw = atan2(_31, _33);
   }
   else
   {
      roll = atan2(-_21, _11);
      yaw = 0.0;
   }

    return (Vec3(yaw, pitch, roll));
}
#endif

inline Vec3 Mat4x4::GetScale() const
{
    return Vec3((*this)(0,0), (*this)(1,1), (*this)(2,2));
}

inline Vec4 Mat4x4::Xform(Vec4 &v) const
{
    return this->map(v);
}

inline Vec3 Mat4x4::Xform(Vec3 &v) const
{
    return this->map(v);
}

inline Mat4x4 Mat4x4::Inverse() const
{
    return this->inverted();
}

inline void Mat4x4::BuildTranslation(const Vec3 &pos)
{
    this->setToIdentity();
    this->translate(pos);
}

inline void Mat4x4::BuildTranslation(const float x, const float y, const float z )
{
    this->setToIdentity();
    this->translate(x, y, z);
}

inline void Mat4x4::BuildRotationX(const float radians)
{
    Vec3 x_axis(1.0f, 0.0f, 0.0f);
//    this->setToIdentity();
    this->rotate(radians, x_axis);
}

inline void Mat4x4::BuildRotationY(const float radians)
{
    Vec3 y_axis(0.0f, 1.0f, 0.0f);
//    this->setToIdentity();
    this->rotate(radians, y_axis);
}

inline void Mat4x4::BuildRotationZ(const float radians)
{
    Vec3 z_axis(0.0f, 0.0f, 1.0f);
//    this->setToIdentity();
    this->rotate(radians, z_axis);
}

inline void Mat4x4::BuildYawPitchRoll(const float yawRadians, const float pitchRadians, const float rollRadians)
{
    this->setToIdentity();
    this->BuildRotationZ(rollRadians);
    this->BuildRotationX(pitchRadians);
    this->BuildRotationY(yawRadians);
}

inline void Mat4x4::BuildRotationQuat(const Quaternion &q)
{
    this->setToIdentity();
    this->rotate(q);
}

inline void Mat4x4::BuildRotationLookAt(const Vec3 &eye, const Vec3 &at, const Vec3 &up)
{
    this->setToIdentity();
    this->lookAt(eye, at, up);
}

inline void Mat4x4::BuildScale(const float x, const float y, const float z )
{
    this->setToIdentity();
    this->scale(x, y, z);
}

inline Mat4x4 operator * (const Mat4x4 &a, const Mat4x4 &b)
{
    return a * b;
}

inline void Quaternion::Build(const Mat4x4 &mat)
{
    this->fromRotationMatrix(mat.normalMatrix());
}




struct GL_SimpleVertex
{
    Vec3 Pos;
    Vec3 Normal;
};

struct GLVertex_UnlitTextured
{
    Vec3 Pos;
    Vec3 Normal;
    Vec2 Uv;
};

#if 0
// Create our vertex input layout
const D3D11_INPUT_ELEMENT_DESC D3D11VertexLayout_UnlitTextured[] =
{
    { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
#endif

struct GLVertex_UnlitTexturedColored
{
    Vec3 Pos;
    Vec3 Normal;
    Vec3 Diffuse;
    Vec2 Uv;
};

#if 0
// Create our vertex input layout
const D3D11_INPUT_ELEMENT_DESC D3D11VertexLayout_UnlitTexturedColored[] =
{
    { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
#endif

struct GLVertex_PositionColored
{
    Vec3 Pos;
    Vec3 Diffuse;
};
#if 0
const D3D11_INPUT_ELEMENT_DESC D3D11VertexLayout_PositionColored[] =
{
    { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


const D3D11_INPUT_ELEMENT_DESC D3D11VertexLayout_Position[] =
{
    { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
#endif



////////////////////////////////////////////////////
//
// Plane Definition
//
////////////////////////////////////////////////////

class Plane
{
protected:
    float a;
    float b;
    float c;
    float d;

public:
    inline void Normalize();
    // normal faces away from you if you send in verts in counter clockwise order....
    inline void Init(const Vec3 &p0, const Vec3 &p1, const Vec3 &p2);
    bool Inside(const Vec3 &point, const float radius) const;
    bool Inside(const Vec3 &point) const;

};

inline void Plane::Normalize()
{
    float mag;
    mag = sqrt(a * a + b * b + c * c);
    if(mag != 0)
    {
        a = a / mag;
        b = b / mag;
        c = c / mag;
        d = d / mag;
    }
}

inline void Plane::Init(const Vec3 &p0, const Vec3 &p1, const Vec3 &p2)
{
    //find two vectors in the plane
    Vec3 v1 = p1 - p0;
    Vec3 v2 = p2 - p0;

    //Cross product of the two vectors
    Vec3 v3 = v2.Cross(v1);

    //Coefficients a,b,c
    a = v3.x();
    b = v3.y();
    c = v3.z();

    //Plug in one of the points into the coefficients and solve for d
    d = -(p0.x() * a + p0.y() * b + p0.z() * c);
    Normalize();
}


////////////////////////////////////////////////////
//
// Frustum Definition
//
////////////////////////////////////////////////////

class Frustum
{
public:
    enum Side { Near, Far, Top, Right, Bottom, Left, NumPlanes };

    Plane m_Planes[NumPlanes];	// planes of the frusum in camera space
    Vec3 m_NearClip[4];			// verts of the near clip plane in camera space
    Vec3 m_FarClip[4];			// verts of the far clip plane in camera space

    float m_Fov;				// field of view in radians
    float m_Aspect;				// aspect ratio - width divided by height
    float m_Near;				// near clipping distance
    float m_Far;				// far clipping distance

public:
    Frustum();

    bool Inside(const Vec3 &point) const;
    bool Inside(const Vec3 &point, const float radius) const;
    const Plane &Get(Side side) { return m_Planes[side]; }
    void SetFOV(float fov) { m_Fov=fov; Init(m_Fov, m_Aspect, m_Near, m_Far); }
    void SetAspect(float aspect) { m_Aspect=aspect; Init(m_Fov, m_Aspect, m_Near, m_Far); }
    void SetNear(float nearClip) { m_Near=nearClip; Init(m_Fov, m_Aspect, m_Near, m_Far); }
    void SetFar(float farClip) { m_Far=farClip; Init(m_Fov, m_Aspect, m_Near, m_Far); }
    void Init(const float fov, const float aspect, const float near, const float far);

    void Render();
};



#endif // GEOMETRY_H
