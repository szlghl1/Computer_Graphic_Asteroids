//
//  Matrix.hpp
//  GameEngine
//
//  Created by David Lively on 2/10/16.
//  Copyright © 2016 David Lively. All rights reserved.
//

#ifndef MATRIX_H
#define MATRIX_H

#include "Vectors.h"

class Matrix
{
public:
	
    /// X basis vector
    float m00, m01, m02, m03;
    /// y basis vector
    float m10, m11, m12, m13;
    /// z basis vector
    float m20, m21, m22, m23;
    /// translation
    float m30, m31, m32, m33;

    /*
    /// X basis vector
	float m00, m10, m20, m30;
	/// y basis vector
	float m01, m11, m21, m31;
	/// z basis vector
	float m02, m12, m22, m32;
	/// translation
	float m03, m13, m23, m33;
*/
    
    Matrix() :
    m00(1), m01(0), m02(0), m03(0)
    ,m10(0), m11(1), m12(0), m13(0)
    ,m20(0), m21(0), m22(1), m23(0)
    ,m30(0), m31(0), m32(0), m33(1)
    {
        
    }
    
    
    static Matrix Identity();
    static void CreateRotationX(Matrix& m, float radians);
    static void CreateRotationY(Matrix& m, float radians);
    static void CreateRotationZ(Matrix& m, float radians);
    
    static Matrix CreateRowMajorTranslation(const Vector3& position);
    
    static Matrix CreateRotationX(float radians);
    static Matrix CreateRotationY(float radians);
    static Matrix CreateRotationZ(float radians);
    
    static Matrix CreateRotation(float x, float y, float z);
    static Matrix CreateRotation(const Vector3& rotate);
    static Matrix CreateTranslation(float x, float y, float z);
    static Matrix CreateTranslation(const Vector3& position);
    
    static Matrix CreateScale(float x, float y, float z);
    static Matrix CreateScale(float uniformScale);
    static Matrix CreateScale(const Vector3& scale);
    
    static Matrix CreatePerspective(float fov, float aspect, float zNear, float zFar);
  
    

    
    static void Multiply(const Matrix& left, const Matrix& right, Matrix& result);
    
    static Matrix Multiply(const Matrix& left, const Matrix& right)
    {
        Matrix result;
        
        Multiply(left, right, result);
        
        return result;
    }
    
    Matrix operator*(const Matrix& right)
    {
        return Multiply(*this, right);
    }
    
    
private:
};
extern Vector4 operator* (Matrix m, Vector4 v);// 4*4 matrix * 1*4 vector
extern Vector4 operator* (Vector4 v, float m);
extern Vector4 operator+ (Vector4 v1, Vector4 v2);

#endif /* Matrix_hpp */







