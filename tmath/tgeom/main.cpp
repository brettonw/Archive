#include "tgeom.h"
#include "triangulate.h"

int main (int, char**)
{
    TChain<float>   chain;
    
    chain.push_back (TPoint2<float> (2, 1));
    chain.push_back (TPoint2<float> (0, 1));
    chain.push_back (TPoint2<float> (0, 0));
    chain.push_back (TPoint2<float> (1, 0));
    
    TChainList<float>   chainList;
    chainList.push_back (chain);
    
    TChain<float>   chainOut;
    TriangleVec     trianglesOut;
    
    Triangulate (chainList, chainOut, trianglesOut);
    
    DebugPrint ("After Triangulation\n");
    chainOut.DebugPrint ();
    trianglesOut.DebugPrint ();
    
    TriangleVec     trianglesSplit;
    ClampEdgeLengths<float> (chainOut, trianglesOut, trianglesSplit, 1);

    DebugPrint ("After Clamping\n");
    chainOut.DebugPrint ();
    trianglesSplit.DebugPrint ();
}
