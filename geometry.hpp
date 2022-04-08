#pragma once
#include <cmath>
#include <vector>

template <typename T> class Vec2 {
private:
public:
	T x, y;
	// Конструкторы ==========================================================================================================
	Vec2(): x(0), y(0) {}
	Vec2(T _x, T _y): x(_x), y(_y) {}
	Vec2(const Vec2<T> &V): x(V.x), y(V.y) {}
	template <typename S> Vec2<T>(S _x, S _y, S _z): x(T(_x)), y(T(_y)) {};
	template <typename S> Vec2<T>(const Vec2<S> &V): x(T(V.x)), y(T(V.y)) {};
	~Vec2() {};
	// Операторы =============================================================================================================
	Vec2<T> operator +(const Vec2<T> &V)	const { return Vec2<T>(x + V.x, y + V.y); }
	Vec2<T> operator +=(const Vec2<T> &V)	{ *this = *this + V; return this; }
	Vec2<T> operator -(const Vec2<T> &V)	const { return Vec2<T>(x - V.x, y - V.y); }
	Vec2<T> operator -=(const Vec2<T> &V)	{ *this = *this - V; return this; }
	Vec2<T> operator *(T f)					const { return Vec2<T>(x * f, y * f); }
	Vec2<double> operator /(double f)		const { return Vec2<double>(x / f, y / f); }
	// Скалярное произведение векторов
	double  operator *(const Vec2<T> &V)	const { return x * V.x + y * V.y; }
	// Возвращает длину вектора
	double  operator !()					const { return sqrt(x * x + y * y); }
	// Функции ===============================================================================================================
	// Нормализация вектора
	Vec2<double> norm()						{ *this = *this / !*this; return this; }
	// Возвращает 1.0 если направление векторов совпадает
	double getCosAngle(const Vec2<T> &V)	const { return ((*this) * V) / (!(*this) * !V); }
};

template <typename T> class Vec3 {
private:
public:
	T x, y, z;
	// Конструкторы ==========================================================================================================
	Vec3(): x(0), y(0), z(0) {}
	Vec3(T _x, T _y, T _z): x(_x), y(_y), z(_z) {}
	Vec3(const Vec3<T> &V): x(V.x), y(V.y), z(V.z) {}
	template <typename S> Vec3<T>(S _x, S _y, S _z): x(T(_x)), y(T(_y)), z(T(_z)) {};
	template <typename S> Vec3<T>(const Vec3<S> &V): x(T(V.x)), y(T(V.y)), z(T(V.z)) {};
	~Vec3() {};
	// Операторы =============================================================================================================
	T operator[](int i)                     const { return (i == 0 ? x : (i == 1 ? y : z)); }
	Vec3<T> operator +(const Vec3<T> &V)	const { return Vec3<T>(x + V.x, y + V.y, z + V.z); }	
	Vec3<T> operator +=(const Vec3<T> &V)	{ *this = *this + V; return this; }
	Vec3<T> operator -(const Vec3<T> &V)	const { return Vec3<T>(x - V.x, y - V.y, z - V.z); }
	Vec3<T> operator -=(const Vec3<T> &V)	{ *this = *this - V; return this; }
	Vec3<T> operator *(T f)					const { return Vec3<T>(x * f, y * f, z * f); }
	Vec3<double> operator /(double f)		const { return Vec3<double>(x / f, y / f, z / f); }
	// Скалярное произведение векторов
	double  operator *(const Vec3<T> &V)	const { return x * V.x + y * V.y + z * V.z; }
	// Векторное произведение векторов
	Vec3<T> operator ^(const Vec3<T> &v)	const { return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
	// Возвращает длину вектора
	double  operator !()					const { return sqrt(x * x + y * y + z * z); }
	// Функции ===============================================================================================================
	Vec3<double> norm()						{ *this = (*this) / !(*this); return this; }
	// Возвращает 1.0 если направление векторов совпадает
	double getCosAngle(const Vec3<T> &V)	const { return double((*this) * V) / (!(*this) * !V); }
};

typedef Vec2<int> Vec2i;
typedef Vec2<double> Vec2d;
typedef Vec3<int> Vec3i;
typedef Vec3<double> Vec3d;

struct face{
	std::vector<int> id_v;
	std::vector<int> id_vt;
	std::vector<int> id_vn;
	int id_m;
};

struct object{
	std::string name;
	std::vector<face> faces;
	bool visible;
};

// struct texture{
// 	std::string name;
// 	BMP rect;
// };

struct material{
	std::string name;
	Vec3d Ka;
	Vec3d Kd;
	Vec3d Ke;
	Vec3d Ks;
	double Ns;
	double Ni;
	double d;
	int illum;
	int map_Ka;
	int map_Kd;
	int map_Ks;
	int map_D;
};