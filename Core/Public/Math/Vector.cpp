#include "Math/Vector.h"


// floats
template<> const Vector3f Vector3f::ZeroVector(0.0f, 0.0f, 0.0f);
template<> const Vector3f Vector3f::OneVector(1.0f, 1.0f, 1.0f);

template<> const Vector3f Vector3f::UpVector(0.0f, 1.0f, 0.0f);
template<> const Vector3f Vector3f::DownVector(0.0f, -1.0f, 0.0f);

template<> const Vector3f Vector3f::ForwardVector(0.0f, 0.0f, 1.0f);
template<> const Vector3f Vector3f::BackwardVector(0.0f, 0.0f, -1.0f);

template<> const Vector3f Vector3f::RightVector(1.0f, 0.0f, 0.0f);
template<> const Vector3f Vector3f::LeftVector(-1.0f, 0.0f, 0.0f);

template<> const Vector3f Vector3f::XAxisVector(1.0f, 0.0f, 0.0f);
template<> const Vector3f Vector3f::YAxisVector(0.0f, 1.0f, 0.0f);
template<> const Vector3f Vector3f::ZAxisVector(0.0f, 0.0f, 1.0f);


// doubles
template<> const Vector3d Vector3d::ZeroVector(0, 0, 0);
template<> const Vector3d Vector3d::OneVector(1, 1, 1);

template<> const Vector3d Vector3d::UpVector(0, 1, 0);
template<> const Vector3d Vector3d::DownVector(0, -1, 0 );

template<> const Vector3d Vector3d::ForwardVector(0, 0, 1);
template<> const Vector3d Vector3d::BackwardVector(0, 0, -1);

template<> const Vector3d Vector3d::RightVector(1, 0, 0);
template<> const Vector3d Vector3d::LeftVector(-1, 0, 0);

template<> const Vector3d Vector3d::XAxisVector(1, 0, 0);
template<> const Vector3d Vector3d::YAxisVector(0, 1, 0);
template<> const Vector3d Vector3d::ZAxisVector(0, 0, 1);
