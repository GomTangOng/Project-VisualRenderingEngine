#include "stdafx.h"
#include "Utility.h"

void Math::ExtractFrustumPlanes(XMFLOAT4 planes[6], CXMMATRIX M)
{
	XMFLOAT4X4 m;
	XMStoreFloat4x4(&m, M);

	//
	// Left
	//
	planes[0].x = m(0, 3) + m(0, 0);
	planes[0].y = m(1, 3) + m(1, 0);
	planes[0].z = m(2, 3) + m(2, 0);
	planes[0].w = m(3, 3) + m(3, 0);

	//			  
	// Right	  
	//			  
	planes[1].x = m(0, 3) - m(0, 0);
	planes[1].y = m(1, 3) - m(1, 0);
	planes[1].z = m(2, 3) - m(2, 0);
	planes[1].w = m(3, 3) - m(3, 0);

	//			 
	// Bottom	 
	//			 
	planes[2].x = m(0, 3) + m(0, 1);
	planes[2].y = m(1, 3) + m(1, 1);
	planes[2].z = m(2, 3) + m(2, 1);
	planes[2].w = m(3, 3) + m(3, 1);

	//			  
	// Top		  
	//			  
	planes[3].x = m(0, 3) - m(0, 1);
	planes[3].y = m(1, 3) - m(1, 1);
	planes[3].z = m(2, 3) - m(2, 1);
	planes[3].w = m(3, 3) - m(3, 1);

	//			  		
	// Near		  		
	//			  		
	planes[4].x = m(0, 2);
	planes[4].y = m(1, 2);
	planes[4].z = m(2, 2);
	planes[4].w = m(3, 2);

	//						
	// Far					
	//						
	planes[5].x = m(0, 3) - m(0, 2);
	planes[5].y = m(1, 3) - m(1, 2);
	planes[5].z = m(2, 3) - m(2, 2);
	planes[5].w = m(3, 3) - m(3, 2);

	// Normalize the plane equations.
	for (int i = 0; i < 6; ++i)
	{
		XMVECTOR v = XMPlaneNormalize(XMLoadFloat4(&planes[i]));
		XMStoreFloat4(&planes[i], v);
	}
}