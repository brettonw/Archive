// raytrace.cpp : Defines the entry point for the console application.
//

#include    "stdafx.h"
#include    "debug.h"
#include    "definitions.h"
#include    "real.h"
#include    "vector.h"
#include    "timer.h"

using namespace TMath;

typedef TReal<float>            Real32;

typedef TVector<Real32, 2>      FVec2;
typedef TPoint<Real32, 2>       FPt2;
typedef TMatrix<Real32, 3, 3>   FMat3x3;

typedef TVector<Real32, 3>      FVec3;
typedef TPoint<Real32, 3>       FPt3;
typedef TMatrix<Real32, 4, 4>   FMat4x4;

typedef TReal<double>           Real64;
typedef TMatrix<Real64, 4, 4>   DMat4x4;

void TestReal (void)
{
    Real32  r32 (1);
    r32 = Cosine (r32);
    r32 += 4;
    r32 *= Real32 (5);
    g_dbgout << "r32 = " << r32 << std::endl;
    g_dbgout << "Round 1.7 = " << RoundOff (1.7f) << std::endl;
    g_dbgout << "Round -1.7 = " << RoundOff (-1.7f) << std::endl;
}
    
void TestMatrix (void)
{
    FMat4x4 mat1;
    FMat4x4 mat2;
    FMat4x4 dmat1;
    Identity (mat1);
    Identity (mat2);
    
    D3DXMatrixRotationX ((D3DXMATRIX*) (&mat1), 1.0f);
    D3DXMatrixRotationX ((D3DXMATRIX*) (&mat2), 0.1f);
    FMat4x4 mat3 = mat1 * mat2;
    FMat4x4 mat4;
    D3DXMatrixMultiply ((D3DXMATRIX*) (&mat4), (D3DXMATRIX*) (&mat1), (D3DXMATRIX*) (&mat2));
    if (mat4 != mat3)
        g_dbgout << "not the same result as d3dx:" << std::endl << mat4 << std::endl << mat3;
    /*
    unsigned    loopSize = 1000000;
    {
    Timer       timer;
    for (unsigned i = 0; i < loopSize; i++)
    {
        Multiply (mat1, mat2, mat3);
        if ((i & 0x00ffff) == 0x00ffff)
            mat1 = mat3;
    }
    float   elapsed = timer.Elapsed ();
    DebugPrint ("%d ops in %g seconds\n", loopSize, elapsed);
    }

    {
    Timer       timer;
    for (unsigned i = 0; i < loopSize; i++)
    {
        D3DXMatrixMultiply ((D3DXMATRIX*) (&mat3), (D3DXMATRIX*) (&mat1), (D3DXMATRIX*) (&mat2));
        if ((i & 0x00ffff) == 0x00ffff)
            mat1 = mat3;
    }
    float   elapsed = timer.Elapsed ();
    DebugPrint ("%d ops in %g seconds\n", loopSize, elapsed);
    }
    */
    
    typedef TMatrix<Real32, 2, 2>    FMat2x2;
    typedef TMatrix<Real32, 3, 2>    FMat3x2;
    typedef TMatrix<Real32, 2, 3>    FMat2x3;
    typedef TMatrix<Real32, 3, 3>    FMat3x3;
    FMat3x2    m1;
    FMat2x3    m2;
    m1 = 1.0f, 2.0f;
    Zero (m1);
    m1[0][1] = 1;
    m1[1][1] = 1;
    Transpose (m1, m2);
    FMat3x3    m3 = m1 * m2;
    
    g_dbgout << m1 << std::endl << m2 << std::endl << m3 << std::endl;
    
    {
    FMat2x2 inv2x2;
    inv2x2 =    3.0f, -2.0f, 
                5.0f,  6.0f;
    FMat2x2 inverse = Inverse (inv2x2);
    FMat2x2 product = inverse * inv2x2;
    g_dbgout << "2x2 inverse test: " << std::endl << product << std::endl;
    }
    
    {
    Real32   src3x3[] = {3, -2, 1,
                        5, 6, 2,
                        1, 0, -3};
    FMat3x3 inv3x3 (src3x3);
    FMat3x3 inverse = Inverse (inv3x3);
    }
    
    {
    Real32   src3x3[] = {-1, 1, 2,
                        0, 6, 3,
                        4, 7, 5};
    FMat3x3 inv3x3 (src3x3);
    FMat3x3 inverse = Inverse (inv3x3);
    }
    
    {
    FMat4x4 inv4x4;
    /*
    inv4x4 = 1.0f, 1.0f, 2.0f, 0.0f,
             2.0f, 1.0f, 1.0f, 0.0f,
             1.0f, 3.0f, 0.0f, 1.0f,
             1.0f, 1.0f, 4.0f, 2.0f;
    */
    inv4x4 = 1, 1, 2, 0,
             2, 1, 1, 0,
             1, 3, 0, 1,
             1, 1, 4, 2;
    g_dbgout << "4x4 inverse test:" << std::endl << inv4x4;
    FMat4x4 inverse = Inverse (inv4x4);
    FMat4x4 d3dinverse;
    float   d3ddet;
    D3DXMatrixInverse ((D3DXMATRIX*) (&d3dinverse), &d3ddet, (D3DXMATRIX*) (&inv4x4));
    if (inverse != d3dinverse)
        g_dbgout << "not the same result as d3dx:" << std::endl << inverse << std::endl << d3dinverse;
    }
}

void    TestVector (void)
{
    typedef TVector<Real32, 2>   FVec2;
    typedef TVector<Real32, 3>   FVec3;
    
    typedef TPoint<Real32, 2>   FPt2;
    typedef TPoint<Real32, 3>   FPt3;
    
    FVec2   v1, v2;
    v1 = 1.0f, 2.0f;
    v2 = 3.0f, 4.0f;
    g_dbgout << v1 << v2 << "Dot Product: " << (v1 DOT v2) << std::endl << "Cross Product: " << (v1 CROSS v2) << std::endl;
    FVec2   v3 = v1 + v2;
    v2 *= 3;
    g_dbgout << v2;
    
    FPt2    p1 = v1;
    FPt2    p2 = v2;
    FPt2    p3 = p1 + v2;
    g_dbgout << p3;
    
    FPt2   ph;
    ph = 1.0f, 2.0f;
    TMatrix<Real32, 3, 3>    hmat;
    Identity (hmat);
    hmat *= 2;
    g_dbgout << ph;
    g_dbgout << hmat;
    FPt2    tr;
    FPt3    pt3;
    pt3 = pt3 * hmat;
    //Multiply<float, 1, 3, 3> (ph, hmat, tr);
    //tr = static_cast<TPoint<float, 3> > (ph) * hmat;
    tr = ph * hmat;
    g_dbgout << tr;
}

int _tmain(int argc, _TCHAR* argv[])
{
    DEBUG_SPEW_SCOPE (true);
    Real32::ComputeInfinity ();
    Real32::ComputeEpsilon ();
    Real64::ComputeInfinity ();
    Real64::ComputeEpsilon ();
    TestReal ();
    TestMatrix ();
    TestVector ();
    
	return 0;
}

