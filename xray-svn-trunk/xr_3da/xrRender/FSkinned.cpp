// SkeletonX.cpp: implementation of the CSkeletonX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#pragma hdrstop

#pragma warning(disable:4995)
#include <d3dx9.h>
#pragma warning(default:4995)

#include "../../xr_3da/fmesh.h"
#include "../xrCPU_Pipe/xrCPU_Pipe.h"
#include "FSkinned.h"
#include "SkeletonX.h"

#include "../xrRenderDX10/dx10BufferUtils.h"

#include "../../xr_3da/EnnumerateVertices.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static	shared_str	sbones_array;

#pragma pack(push,1)
float u_P	(s16 v)
{
	return	float(v)/(32767.f/12.f);
}
s16	q_P		(float v)
{
	int		_v	= clampr(iFloor(v*(32767.f/12.f)), -32768, 32767);
	return	s16	(_v);
}
u8	q_N		(float v)
{
	int		_v	= clampr(iFloor((v+1.f)*127.5f), 0, 255);
	return	u8	(_v);
}
s16	q_tc	(float v)
{
	int		_v	= clampr(iFloor(v*(32767.f/16.f)), -32768, 32767);
	return	s16	(_v);
}
#ifdef _DEBUG
float errN	(Fvector3 v, u8* qv)
{
	Fvector3	uv;	
	uv.set		(float(qv[0]),float(qv[1]),float(qv[2])).div(255.f).mul(2.f).sub(1.f);
	uv.normalize();
	return		v.dotproduct(uv);
}
#else
float errN	(Fvector3 v, u8* qv)	{ return 0; }
#endif

static	D3DVERTEXELEMENT9 dwDecl_01W	[] =	// 24bytes
{
	{ 0, 0,		D3DDECLTYPE_SHORT4,		D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_POSITION,		0 },	// : P						: 2	: -12..+12
	{ 0, 8,		D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_NORMAL,		0 },	// : N, w=index(RC, 0..1)	: 1	:  -1..+1
	{ 0, 12,	D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_TANGENT,		0 },	// : T						: 1	:  -1..+1
	{ 0, 16,	D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_BINORMAL,		0 },	// : B						: 1	:  -1..+1
	{ 0, 20,	D3DDECLTYPE_SHORT2,		D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_TEXCOORD,		0 },	// : tc						: 1	: -16..+16
	D3DDECL_END()
};
struct	vertHW_1W
{
	s16			_P		[4];
	u32			_N_I	;
	u32			_T		;
	u32			_B		;
	s16			_tc		[2];
	void set	(Fvector3& P, Fvector3 N, Fvector3 T, Fvector3 B, Fvector2& tc, int index)
	{
		N.normalize_safe();
		T.normalize_safe();
		B.normalize_safe();
		_P[0]		= q_P(P.x);
		_P[1]		= q_P(P.y);
		_P[2]		= q_P(P.z);
		_P[3]		= q_P(1);
		_N_I		= color_rgba(q_N(N.x), q_N(N.y), q_N(N.z), u8(index));
		_T			= color_rgba(q_N(T.x), q_N(T.y), q_N(T.z), 0);
		_B			= color_rgba(q_N(B.x), q_N(B.y), q_N(B.z), 0);
		_tc[0]		= q_tc(tc.x);
		_tc[1]		= q_tc(tc.y);
	}
	u16 get_bone() const
	{
		return	(u16)color_get_A(_N_I)/3;
	}
	void get_pos_bones( Fvector& p, CKinematics* Parent ) const
	{
			const Fmatrix& xform	= Parent->LL_GetBoneInstance( get_bone( ) ).mRenderTransform; 
			get_pos	( p );	xform.transform_tiny( p );
	}
	void get_pos(Fvector& p) const
	{
		p.x			= u_P(_P[0]);
		p.y			= u_P(_P[1]);
		p.z			= u_P(_P[2]);
	}
};

static	D3DVERTEXELEMENT9 dwDecl_2W	[] =	// 28bytes
{
	{ 0, 0,		D3DDECLTYPE_SHORT4,		D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_POSITION,		0 },	// : p					: 2	: -12..+12
	{ 0, 8,		D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_NORMAL,		0 },	// : n.xyz, w = weight	: 1	:  -1..+1, w=0..1
	{ 0, 12,	D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_TANGENT,		0 },	// : T						: 1	:  -1..+1
	{ 0, 16,	D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_BINORMAL,		0 },	// : B						: 1	:  -1..+1
	{ 0, 20,	D3DDECLTYPE_SHORT4,		D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_TEXCOORD,		0 },	// : xy(tc), zw(indices): 2	: -16..+16, zw[0..32767]
	D3DDECL_END()
};
struct	vertHW_2W
{
	s16			_P		[4];
	u32			_N_w	;
	u32			_T		;
	u32			_B		;
	s16			_tc_i	[4];
	void set	(Fvector3& P, Fvector3 N, Fvector3 T, Fvector3 B, Fvector2& tc, int index0, int index1, float w)
	{
		N.normalize_safe	();
		T.normalize_safe	();
		B.normalize_safe	();
		_P[0]		= q_P	(P.x);
		_P[1]		= q_P	(P.y);
		_P[2]		= q_P	(P.z);
		_P[3]		= 1;
		_N_w		= color_rgba(q_N(N.x), q_N(N.y), q_N(N.z), u8(clampr(iFloor(w*255.f+.5f),0,255)));
		_T			= color_rgba(q_N(T.x), q_N(T.y), q_N(T.z), 0);
		_B			= color_rgba(q_N(B.x), q_N(B.y), q_N(B.z), 0);
		_tc_i[0]	= q_tc	(tc.x);
		_tc_i[1]	= q_tc	(tc.y);
		_tc_i[2]	= s16	(index0);
		_tc_i[3]	= s16	(index1);
	}
	float get_weight() const
	{
		return	float(color_get_A(_N_w))/255.f;
	}
	u16 get_bone(u16 w) const
	{
		return	(u16)_tc_i[w+2]/3;
	}
	void get_pos(Fvector& p) const
	{
		p.x			= u_P(_P[0]);
		p.y			= u_P(_P[1]);
		p.z			= u_P(_P[2]);
	}
	void get_pos_bones( Fvector& p, CKinematics* Parent ) const
	{
			Fvector		P0,P1;
			Fmatrix& xform0			= Parent->LL_GetBoneInstance(get_bone(0)).mRenderTransform; 
			Fmatrix& xform1			= Parent->LL_GetBoneInstance(get_bone(1)).mRenderTransform; 
			get_pos	(P0);	xform0.transform_tiny(P0);
			get_pos	(P1);	xform1.transform_tiny(P1);
			p.lerp			(P0,P1,get_weight());
	}
};

#pragma pack(pop)

//////////////////////////////////////////////////////////////////////
// Body Part
//////////////////////////////////////////////////////////////////////
void CSkeletonX_PM::Copy	(dxRender_Visual *V) 
{
	inherited1::Copy		(V);
	CSkeletonX_PM *X		= (CSkeletonX_PM*)(V);
	_Copy					((CSkeletonX*)X);
}
void CSkeletonX_ST::Copy	(dxRender_Visual *P) 
{
	inherited1::Copy		(P);
	CSkeletonX_ST *X		= (CSkeletonX_ST*)P;
	_Copy					((CSkeletonX*)X);
}
//////////////////////////////////////////////////////////////////////
void CSkeletonX_PM::Render	(float LOD) 
{
	int lod_id				= inherited1::last_lod;
	if (LOD>=0.f){
		clamp				(LOD,0.f,1.f);
		lod_id				= iFloor((1.f-LOD)*float(nSWI.count-1)+0.5f);
		inherited1::last_lod= lod_id;
	}
	VERIFY					(lod_id>=0 && lod_id<int(nSWI.count));
	FSlideWindow& SW		= nSWI.sw[lod_id];
	_Render					(rm_geom,SW.num_verts,SW.offset,SW.num_tris);
}
void CSkeletonX_ST::Render	(float LOD) 
{
	_Render		(rm_geom,vCount,0,dwPrimitives);
}

//////////////////////////////////////////////////////////////////////
void CSkeletonX_PM::Release()
{
	inherited1::Release();
}
void CSkeletonX_ST::Release()
{
	inherited1::Release();
}
//////////////////////////////////////////////////////////////////////
void CSkeletonX_PM::Load(const char* N, IReader *data, u32 dwFlags) 
{
	_Load							(N,data,vCount);
	void*	_verts_					= data->pointer	();
	inherited1::Load				(N,data,dwFlags|VLOAD_NOVERTICES);
	::Render->shader_option_skinning(-1);
#if defined(USE_DX10) || defined(USE_DX11)
	_DuplicateIndices(N, data);
#endif	//	USE_DX10
	vBase							= 0;
	_Load_hw						(*this,_verts_);
}
void CSkeletonX_ST::Load(const char* N, IReader *data, u32 dwFlags) 
{
	_Load							(N,data,vCount);
	void*	_verts_					= data->pointer	();
	inherited1::Load				(N,data,dwFlags|VLOAD_NOVERTICES);
	::Render->shader_option_skinning(-1);
#if defined(USE_DX10) || defined(USE_DX11)
	_DuplicateIndices(N, data);
#endif	//	USE_DX10
	vBase							= 0;
	_Load_hw						(*this,_verts_);
}

#if defined(USE_DX10) || defined(USE_DX11)

void CSkeletonX_ext::_Load_hw	(Fvisual& V, void *	_verts_)
{
	// Create HW VB in case this is possible
//	BOOL	bSoft				= HW.Caps.geometry.bSoftware;
//	u32		dwUsage				= /*D3DUSAGE_WRITEONLY |*/ (bSoft?D3DUSAGE_SOFTWAREPROCESSING:0);	// VB may be read by wallmarks code
	switch	(RenderMode)
	{
	case RM_SKINNING_SOFT:
		//Msg					("skinning: software");
		V.rm_geom.create		(vertRenderFVF, RCache.Vertex.Buffer(), V.p_rm_Indices);
		break;
	case RM_SINGLE:
	case RM_SKINNING_1B:
		{
			{	//	Back up vertex data since we can't read vertex buffer in DX10
				u32 size				= V.vCount*sizeof(vertBoned1W);
				u32 crc					= crc32( _verts_, size);
				Vertices1W.create		( crc, V.vCount, (vertBoned1W*)_verts_);
			}

			u32		vStride		= D3DXGetDeclVertexSize		(dwDecl_01W,0);
			VERIFY	(vStride==sizeof(vertHW_1W));
//			BYTE*	bytes		= 0;
			VERIFY				(NULL==V.p_rm_Vertices);

			//R_CHK				(HW.pDevice->CreateVertexBuffer(V.vCount*vStride,dwUsage,0,D3DPOOL_MANAGED,&V.p_rm_Vertices,0));
			//R_CHK				(V.p_rm_Vertices->Lock(0,0,(void**)&bytes,0));
			//vertHW_1W*		dst	= (vertHW_1W*)bytes;
			//vertBoned1W*	src = (vertBoned1W*)_verts_;
			//for (u32 it=0; it<V.vCount; it++)	{
				//Fvector2	uv; uv.set(src->u,src->v);
				//dst->set	(src->P,src->N,src->T,src->B,uv,src->matrix*3);
				//dst++; src++;
			//}
			//V.p_rm_Vertices->Unlock	();

			//	TODO: DX10: Check for memory fragmentation
			vertHW_1W*		dstOriginal	= xr_alloc<vertHW_1W>(V.vCount);			
			vertHW_1W*		dst	= dstOriginal;
			vertBoned1W*	src = (vertBoned1W*)_verts_;
			for (u32 it=0; it<V.vCount; it++)	{
				Fvector2	uv; uv.set(src->u,src->v);
				dst->set	(src->P,src->N,src->T,src->B,uv,src->matrix*3);
				dst++; src++;
			}
			//R_CHK				(HW.pDevice->CreateVertexBuffer(V.vCount*vStride,dwUsage,0,D3DPOOL_MANAGED,&V.p_rm_Vertices,0));
			R_CHK(dx10BufferUtils::CreateVertexBuffer	(&V.p_rm_Vertices, dstOriginal, V.vCount*vStride));
			HW.stats_manager.increment_stats_vb			(V.p_rm_Vertices);
			xr_free(dstOriginal);

			V.rm_geom.create		(dwDecl_01W, V.p_rm_Vertices, V.p_rm_Indices);
		}  
		break;
	case RM_SKINNING_2B:
		{
			{	//	Back up vertex data since we can't read vertex buffer in DX10
				u32 size				= V.vCount*sizeof(vertBoned2W);
				u32 crc					= crc32( _verts_, size);
				Vertices2W.create		( crc, V.vCount, (vertBoned2W*)_verts_);
			}

			u32 vStride			= D3DXGetDeclVertexSize		(dwDecl_2W,0);
			VERIFY				(vStride==sizeof(vertHW_2W));
//			BYTE* bytes			= 0;
			VERIFY				(NULL==V.p_rm_Vertices);

			//R_CHK				(HW.pDevice->CreateVertexBuffer(V.vCount*vStride,dwUsage,0,D3DPOOL_MANAGED,&V.p_rm_Vertices,0));
			//R_CHK				(V.p_rm_Vertices->Lock(0,0,(void**)&bytes,0));
			//vertHW_2W* dst		= (vertHW_2W*)bytes;
			//vertBoned2W* src	= (vertBoned2W*)_verts_;

			//for (u32 it=0; it<V.vCount; ++it)	
			//{
			//	Fvector2	uv; uv.set(src->u,src->v);
			//	dst->set	(src->P,src->N,src->T,src->B,uv,int(src->matrix0)*3,int(src->matrix1)*3,src->w);
			//	dst++;		src++;
			//}
			//V.p_rm_Vertices->Unlock	();

			//	TODO: DX10: Check for memory fragmentation
			vertHW_2W*		dstOriginal	= xr_alloc<vertHW_2W>(V.vCount);			
			vertHW_2W*		dst	=dstOriginal;
			vertBoned2W*	src = (vertBoned2W*)_verts_;
			for (u32 it=0; it<V.vCount; it++)	{
				Fvector2	uv; uv.set(src->u,src->v);
				dst->set	(src->P,src->N,src->T,src->B,uv,int(src->matrix0)*3,int(src->matrix1)*3,src->w);
				dst++;		src++;
			}
			R_CHK(dx10BufferUtils::CreateVertexBuffer	(&V.p_rm_Vertices, dstOriginal, V.vCount*vStride));
			HW.stats_manager.increment_stats_vb			(V.p_rm_Vertices);
			xr_free(dstOriginal);

			V.rm_geom.create		(dwDecl_2W, V.p_rm_Vertices, V.p_rm_Indices);
		}break;

	}
}

#else	//	USE_DX10

void CSkeletonX_ext::_Load_hw	(Fvisual& V, void *	_verts_)
{
	// Create HW VB in case this is possible
	BOOL	bSoft				= HW.Caps.geometry.bSoftware;
	u32		dwUsage				= /*D3DUSAGE_WRITEONLY |*/ (bSoft?D3DUSAGE_SOFTWAREPROCESSING:0);	// VB may be read by wallmarks code
	switch	(RenderMode)
	{
	case RM_SKINNING_SOFT:
		//Msg					("skinning: software");
		V.rm_geom.create		(vertRenderFVF, RCache.Vertex.Buffer(), V.p_rm_Indices);
		break;
	case RM_SINGLE:
	case RM_SKINNING_1B:
		{
			u32		vStride		= D3DXGetDeclVertexSize		(dwDecl_01W,0);
			VERIFY	(vStride==sizeof(vertHW_1W));
			BYTE*	bytes		= 0;
			VERIFY				(NULL==V.p_rm_Vertices);
			R_CHK				(HW.pDevice->CreateVertexBuffer	(V.vCount*vStride,dwUsage,0,D3DPOOL_MANAGED,&V.p_rm_Vertices,0));
			HW.stats_manager.increment_stats_vb					(V.p_rm_Vertices);
			R_CHK				(V.p_rm_Vertices->Lock(0,0,(void**)&bytes,0));
			vertHW_1W*		dst	= (vertHW_1W*)bytes;
			vertBoned1W*	src = (vertBoned1W*)_verts_;
			for (u32 it=0; it<V.vCount; it++)	{
				Fvector2	uv; uv.set(src->u,src->v);
				dst->set	(src->P,src->N,src->T,src->B,uv,src->matrix*3);
				dst++; src++;
			}
			V.p_rm_Vertices->Unlock	();
			V.rm_geom.create		(dwDecl_01W, V.p_rm_Vertices, V.p_rm_Indices);
		}  
		break;
	case RM_SKINNING_2B:
		{
			u32 vStride			= D3DXGetDeclVertexSize		(dwDecl_2W,0);
			VERIFY				(vStride==sizeof(vertHW_2W));
			BYTE* bytes			= 0;
			VERIFY				(NULL==V.p_rm_Vertices);
			R_CHK				(HW.pDevice->CreateVertexBuffer	(V.vCount*vStride,dwUsage,0,D3DPOOL_MANAGED,&V.p_rm_Vertices,0));
			HW.stats_manager.increment_stats_vb					(V.p_rm_Vertices);
			R_CHK				(V.p_rm_Vertices->Lock(0,0,(void**)&bytes,0));
			vertHW_2W*		dst	= (vertHW_2W*)bytes;
			vertBoned2W*	src = (vertBoned2W*)_verts_;
			for (u32 it=0; it<V.vCount; ++it)	
			{
				Fvector2	uv; uv.set(src->u,src->v);
				dst->set	(src->P,src->N,src->T,src->B,uv,int(src->matrix0)*3,int(src->matrix1)*3,src->w);
				dst++;		src++;
			}
			V.p_rm_Vertices->Unlock	();
			V.rm_geom.create		(dwDecl_2W, V.p_rm_Vertices, V.p_rm_Indices);
		}
		break;
	}
}
#endif	//	USE_DX10

//-----------------------------------------------------------------------------------------------------
// Wallmarks
//-----------------------------------------------------------------------------------------------------
#include "cl_intersect.h"

#ifdef	DEBUG

template	< typename vertex_type >
static void verify_vertex( const vertex_type& v, const Fvisual* V, const CKinematics *Parent, u32 iBase, u32 iCount, const u16 *indices, u32 vertex_idx, u32 idx )
{
	VERIFY(Parent);
#ifndef _EDITOR
	for( u8 i =0; i<vertex_type::bones_count; ++i )
		if( v.get_bone_id(i) >= Parent->LL_BoneCount() )
		{
			Msg( "v.get_bone_id(i): %d, Parent->LL_BoneCount() %d ", v.get_bone_id(i), Parent->LL_BoneCount() );
			Msg( "&v: %p, &V: %p, indices: %p", &v, V, indices );
			Msg( " iBase: %d, iCount: %d, V->iBase %d, V->iCount %d, V->vBase: %d,  V->vCount  %d, vertex_idx: %d, idx: %d", iBase, iCount, V->iBase, V->iCount, V->vBase, V->vCount, vertex_idx, idx  );
			Msg( " v.P: %s , v.N: %s, v.T: %s, v.B: %s", get_string( v.P ).c_str(),get_string(  v.N ).c_str(),get_string(  v.T ).c_str(),get_string(  v.B  ).c_str());
			Msg( "Parent->dbg_name: %s ", Parent->dbg_name.c_str() );
			FlushLog();
			FATAL( "v.get_bone_id(i) >= Parent->LL_BoneCount()" );
		}
#endif        
}
#endif

void CSkeletonX_ext::_CollectBoneFaces(Fvisual* V, u32 iBase, u32 iCount)
{
	u16* indices		= 0;

#if defined(USE_DX10) || defined(USE_DX11)
	indices = *m_Indices;
#else	//	USE_DX10
	R_CHK				(V->p_rm_Indices->Lock(0,V->dwPrimitives*3,(void**)&indices,D3DLOCK_READONLY));
#endif	//	USE_DX10

	indices				+= iBase;

#if !defined(USE_DX10) && !defined(USE_DX11)	//	Don't use hardware buffers in DX10 since we can't read them
	switch	(RenderMode)
	{
	case RM_SKINNING_SOFT:
#endif	//	USE_DX10
		{
			if(*Vertices1W)
			{
				vertBoned1W* vertices	= *Vertices1W;
				for (u32 idx=0; idx<iCount; idx++){
					vertBoned1W& v	= vertices[V->vBase+indices[idx]];
#ifdef		DEBUG
					verify_vertex( v,  V, Parent, iBase, iCount, indices, V->vBase+indices[idx], idx );
#endif
					CBoneData& BD	= Parent->LL_GetData((u16)v.matrix);
					BD.AppendFace	(ChildIDX,(u16)(idx/3));
				}
			}else
			if(*Vertices2W)
			{
				vertBoned2W* vertices	= *Vertices2W;
				for (u32 idx=0; idx<iCount; ++idx)
				{
					vertBoned2W& v	= vertices[V->vBase+indices[idx]];
#ifdef		DEBUG
					verify_vertex( v,  V, Parent, iBase, iCount, indices, V->vBase+indices[idx], idx );
#endif
					CBoneData& BD0	= Parent->LL_GetData((u16)v.matrix0);
					BD0.AppendFace	(ChildIDX,(u16)(idx/3));
					CBoneData& BD1	= Parent->LL_GetData((u16)v.matrix1);
					BD1.AppendFace	(ChildIDX,(u16)(idx/3));
				}
			}
		}

#if !defined(USE_DX10) && !defined(USE_DX11)	//	Don't use hardware buffers in DX10 since we can't read them
		break;
	case RM_SINGLE:
	case RM_SKINNING_1B:	
		{
			vertHW_1W* vertices	= 0;
			R_CHK				(V->p_rm_Vertices->Lock(V->vBase,V->vCount,(void**)&vertices,D3DLOCK_READONLY));
			for (u32 idx=0; idx<iCount; idx++){
				vertHW_1W& v	= vertices[indices[idx]];
				CBoneData& BD	= Parent->LL_GetData(v.get_bone());
				BD.AppendFace	(ChildIDX,(u16)(idx/3));
			}
			V->p_rm_Vertices->Unlock();
		}
		break;
	case RM_SKINNING_2B:{
		vertHW_2W* vertices	= 0;
		R_CHK				(V->p_rm_Vertices->Lock(V->vBase,V->vCount,(void**)&vertices,D3DLOCK_READONLY));
		for (u32 idx=0; idx<iCount; idx++){
			vertHW_2W& v	= vertices[indices[idx]];
			CBoneData& BD0	= Parent->LL_GetData(v.get_bone(0));
			BD0.AppendFace	(ChildIDX,(u16)(idx/3));
			CBoneData& BD1	= Parent->LL_GetData(v.get_bone(1));
			BD1.AppendFace	(ChildIDX,(u16)(idx/3));
		}
		R_CHK				(V->p_rm_Vertices->Unlock());
						}break;
	}
	R_CHK					(V->p_rm_Indices->Unlock());
#endif	//USE_DX10	//	Don't use hardware buffers in DX10 since we can't read them
}

void CSkeletonX_ST::AfterLoad(CKinematics* parent, u16 child_idx)
{
	inherited2::AfterLoad			(parent,child_idx);
	inherited2::_CollectBoneFaces	(this,iBase,iCount);
}

void CSkeletonX_PM::AfterLoad(CKinematics* parent, u16 child_idx)
{
	inherited2::AfterLoad			(parent,child_idx);
	FSlideWindow& SW				= nSWI.sw[0]; // max LOD
	inherited2::_CollectBoneFaces	(this,iBase+SW.offset,SW.num_tris*3);
}

template<typename T>
IC void get_pos_bones(const T& v, Fvector& p, CKinematics* Parent )
{
	v.get_pos_bones( p, Parent );
}

BOOL CSkeletonX_ext::_PickBoneHW1W		(IKinematics::pick_result &r, float dist, const Fvector& S, const Fvector& D, Fvisual* V, u16* indices, CBoneData::FacesVec& faces)
{
	return pick_bone<vertHW_1W>(Parent,r, dist, S, D, V, indices, faces);
}
BOOL CSkeletonX_ext::_PickBoneHW2W		(IKinematics::pick_result &r, float dist, const Fvector& S, const Fvector& D, Fvisual* V, u16* indices, CBoneData::FacesVec& faces)
{
	return pick_bone<vertHW_2W>(Parent,r, dist, S, D, V, indices, faces);
}

BOOL CSkeletonX_ext::_PickBone		(IKinematics::pick_result &r, float dist, const Fvector& start, const Fvector& dir, Fvisual* V, u16 bone_id, u32 iBase, u32 iCount)
{
	VERIFY							(Parent && (ChildIDX!=u16(-1)));
	CBoneData& BD					= Parent->LL_GetData(bone_id);
	CBoneData::FacesVec*	faces	= &BD.child_faces[ChildIDX];
	BOOL result			= FALSE;
	u16* indices		= 0;
#if defined(USE_DX10) || defined(USE_DX11)
	indices = *m_Indices;
#else	//	USE_DX10
	CHK_DX				(V->p_rm_Indices->Lock(0,V->dwPrimitives*3,(void**)&indices,D3DLOCK_READONLY));
	// fill vertices
	switch	(RenderMode){
case RM_SKINNING_SOFT:
#endif	//	USE_DX10
	if (*Vertices1W)result = _PickBoneSoft1W	(r,dist,start,dir,indices+iBase,*faces);
	else			result = _PickBoneSoft2W	(r,dist,start,dir,indices+iBase,*faces);
#if !defined(USE_DX10) && !defined(USE_DX11)
	break;
case RM_SINGLE:
case RM_SKINNING_1B:	result = _PickBoneHW1W	(r,dist,start,dir,V,indices+iBase,*faces); break;
case RM_SKINNING_2B:	result = _PickBoneHW2W	(r,dist,start,dir,V,indices+iBase,*faces);	break;
default: NODEFAULT;
	}
	CHK_DX				(V->p_rm_Indices->Unlock());
#endif	USE_DX10

	return result;
}
BOOL CSkeletonX_ST::PickBone		(IKinematics::pick_result &r, float dist, const Fvector& start, const Fvector& dir, u16 bone_id)
{
	return inherited2::_PickBone	(r, dist,start,dir,this,bone_id,iBase,iCount);
}
BOOL CSkeletonX_PM::PickBone		(IKinematics::pick_result &r, float dist, const Fvector& start, const Fvector& dir, u16 bone_id)
{
	FSlideWindow& SW				= nSWI.sw[0];
	return inherited2::_PickBone	(r,dist,start,dir,this,bone_id,iBase+SW.offset,SW.num_tris*3);
}


void	CSkeletonX_ST::		EnumBoneVertices( SEnumVerticesCallback &C, u16 bone_id )
{
	inherited2::_EnumBoneVertices( C, this, bone_id, iBase, iCount );
}

void	CSkeletonX_PM::		EnumBoneVertices( SEnumVerticesCallback &C, u16 bone_id )
{
	FSlideWindow& SW				= nSWI.sw[0];
	inherited2::_EnumBoneVertices( C, this, bone_id, iBase+SW.offset, SW.num_tris*3 );
}

#if defined(USE_DX10) || defined(USE_DX11)

void CSkeletonX_ext::_FillVerticesHW1W(const Fmatrix& view, CSkeletonWallmark& wm, const Fvector& normal, float size, Fvisual* V, u16* indices, CBoneData::FacesVec& faces)
{
	if(*Vertices1W)
	{
		vertBoned1W* vertices	= *Vertices1W; //SkyLoader: we should use if(*Vertices1W) or use verify??
		for (CBoneData::FacesVecIt it=faces.begin(); it!=faces.end(); it++)
		{
			Fvector			p[3];
			u32 idx			= (*it)*3;
			CSkeletonWallmark::WMFace F;
			for (u32 k=0; k<3; k++)
			{
				vertBoned1W&  vert			= vertices[indices[idx+k]];
/*
#ifdef		DEBUG
				verify_vertex( vert,  V, Parent, iBase, iCount, indices, indices[idx+k], idx+k );
#endif
*/
				F.bone_id[k][0]			= (u16)vert.matrix;
				F.bone_id[k][1]			= F.bone_id[k][0];
				F.weight[k]				= 0.f;
				const Fmatrix& xform	= Parent->LL_GetBoneInstance(F.bone_id[k][0]).mRenderTransform; 
				vert.get_pos			(F.vert[k]);
				xform.transform_tiny	(p[k],F.vert[k]);
			}
			Fvector test_normal;
			test_normal.mknormal	(p[0],p[1],p[2]);
			float cosa				= test_normal.dotproduct(normal);
			if (cosa<EPS)			continue;
			if (CDB::TestSphereTri(wm.ContactPoint(),size,p))
			{
				Fvector				UV;
				for (u32 k=0; k<3; k++)
				{
					Fvector2& uv	= F.uv[k];
					view.transform_tiny(UV,p[k]);
					uv.x			= (1+UV.x)*.5f;
					uv.y			= (1-UV.y)*.5f;
				}
				wm.m_Faces.push_back(F);
			}
		}
	}
}
void CSkeletonX_ext::_FillVerticesHW2W(const Fmatrix& view, CSkeletonWallmark& wm, const Fvector& normal, float size, Fvisual* V, u16* indices, CBoneData::FacesVec& faces)
{
	if(*Vertices2W)
	{
		vertBoned2W* vertices	= *Vertices2W; //SkyLoader: we should use if(*Vertices2W) or use verify??
		for (CBoneData::FacesVecIt it=faces.begin(); it!=faces.end(); ++it)
		{
			Fvector						p[3];
			u32 idx						= (*it)*3;
			CSkeletonWallmark::WMFace	F;
	
			for (u32 k=0; k<3; k++)
			{
				Fvector P0,P1;
				vertBoned2W&  vert			= vertices[indices[idx+k]];
/*
#ifdef		DEBUG
				verify_vertex( vert,  V, Parent, iBase, iCount, indices, indices[idx+k], idx+k );
#endif
*/
				F.bone_id[k][0]			= (u16)vert.matrix0;
				F.bone_id[k][1]			= (u16)vert.matrix1;
				F.weight[k]				= vert.w;
				Fmatrix& xform0			= Parent->LL_GetBoneInstance(F.bone_id[k][0]).mRenderTransform; 
				Fmatrix& xform1			= Parent->LL_GetBoneInstance(F.bone_id[k][1]).mRenderTransform; 
				vert.get_pos			(F.vert[k]);		
				xform0.transform_tiny	(P0,F.vert[k]);
				xform1.transform_tiny	(P1,F.vert[k]);
				p[k].lerp				(P0,P1,F.weight[k]);
			}
			Fvector test_normal;
			test_normal.mknormal	(p[0],p[1],p[2]);
			float cosa				= test_normal.dotproduct(normal);
			if (cosa<EPS)			continue;
	
			if (CDB::TestSphereTri(wm.ContactPoint(),size,p))
			{
				Fvector				UV;
				for (u32 k=0; k<3; k++){
					Fvector2& uv	= F.uv[k];
					view.transform_tiny(UV,p[k]);
					uv.x			= (1+UV.x)*.5f;
					uv.y			= (1-UV.y)*.5f;
				}
				wm.m_Faces.push_back(F);
			}
		}
	}
}


#else	//	USE_DX10

void CSkeletonX_ext::_FillVerticesHW1W(const Fmatrix& view, CSkeletonWallmark& wm, const Fvector& normal, float size, Fvisual* V, u16* indices, CBoneData::FacesVec& faces)
{
	vertHW_1W*			vertices;
	CHK_DX				(V->p_rm_Vertices->Lock(V->vBase,V->vCount,(void**)&vertices,D3DLOCK_READONLY));
	for (CBoneData::FacesVecIt it=faces.begin(); it!=faces.end(); it++){
		Fvector			p[3];
		u32 idx			= (*it)*3;
		CSkeletonWallmark::WMFace F;
		for (u32 k=0; k<3; k++){
			vertHW_1W& vert			= vertices[indices[idx+k]];
			F.bone_id[k][0]			= vert.get_bone();
			F.bone_id[k][1]			= F.bone_id[k][0];
			F.weight[k]				= 0.f;
			const Fmatrix& xform	= Parent->LL_GetBoneInstance(F.bone_id[k][0]).mRenderTransform; 
			vert.get_pos			(F.vert[k]);
			xform.transform_tiny	(p[k],F.vert[k]);
		}
		Fvector test_normal;
		test_normal.mknormal	(p[0],p[1],p[2]);
		float cosa				= test_normal.dotproduct(normal);
		if (cosa<EPS)			continue;
		if (CDB::TestSphereTri(wm.ContactPoint(),size,p))
		{
			Fvector				UV;
			for (u32 k=0; k<3; k++){
				Fvector2& uv	= F.uv[k];
				view.transform_tiny(UV,p[k]);
				uv.x			= (1+UV.x)*.5f;
				uv.y			= (1-UV.y)*.5f;
			}
			wm.m_Faces.push_back(F);
		}
	}
	CHK_DX				(V->p_rm_Vertices->Unlock());
}
void CSkeletonX_ext::_FillVerticesHW2W(const Fmatrix& view, CSkeletonWallmark& wm, const Fvector& normal, float size, Fvisual* V, u16* indices, CBoneData::FacesVec& faces)
{
	vertHW_2W*			vertices;
	CHK_DX				(V->p_rm_Vertices->Lock(V->vBase,V->vCount,(void**)&vertices,D3DLOCK_READONLY));

	for (CBoneData::FacesVecIt it=faces.begin(); it!=faces.end(); ++it)
	{
		Fvector						p[3];
		u32 idx						= (*it)*3;
		CSkeletonWallmark::WMFace	F;

		for (u32 k=0; k<3; k++)
		{
			Fvector P0,P1;
			vertHW_2W& vert			= vertices[indices[idx+k]];
			F.bone_id[k][0]			= vert.get_bone(0);
			F.bone_id[k][1]			= vert.get_bone(1);
			F.weight[k]				= vert.get_weight();
			Fmatrix& xform0			= Parent->LL_GetBoneInstance(F.bone_id[k][0]).mRenderTransform; 
			Fmatrix& xform1			= Parent->LL_GetBoneInstance(F.bone_id[k][1]).mRenderTransform; 
			vert.get_pos			(F.vert[k]);		
			xform0.transform_tiny	(P0,F.vert[k]);
			xform1.transform_tiny	(P1,F.vert[k]);
			p[k].lerp				(P0,P1,F.weight[k]);
		}
		Fvector test_normal;
		test_normal.mknormal	(p[0],p[1],p[2]);
		float cosa				= test_normal.dotproduct(normal);
		if (cosa<EPS)			continue;

		if (CDB::TestSphereTri(wm.ContactPoint(),size,p))
		{
			Fvector				UV;
			for (u32 k=0; k<3; k++){
				Fvector2& uv	= F.uv[k];
				view.transform_tiny(UV,p[k]);
				uv.x			= (1+UV.x)*.5f;
				uv.y			= (1-UV.y)*.5f;
			}
			wm.m_Faces.push_back(F);
		}
	}
	CHK_DX				(V->p_rm_Vertices->Unlock());
}

#endif	//	USE_DX10

#if defined(USE_DX10) || defined(USE_DX11)
void CSkeletonX_ext::_FillVertices(const Fmatrix& view, CSkeletonWallmark& wm, const Fvector& normal, float size, Fvisual* V, u16 bone_id, u32 iBase, u32 iCount)
{
	VERIFY				(Parent&&(ChildIDX!=u16(-1)));
	CBoneData& BD					= Parent->LL_GetData(bone_id);
	CBoneData::FacesVec*	faces	= &BD.child_faces[ChildIDX];
	u16* indices		= 0;
	VERIFY(*m_Indices);
	indices = *m_Indices;
	// fill vertices
	switch	(RenderMode)
	{
	case RM_SKINNING_SOFT:
		if (*Vertices1W)			_FillVerticesSoft1W		(view,wm,normal,size,indices+iBase,*faces);
		else						_FillVerticesSoft2W		(view,wm,normal,size,indices+iBase,*faces);
		break;
	case RM_SINGLE:
	case RM_SKINNING_1B:			_FillVerticesHW1W		(view,wm,normal,size,V,indices+iBase,*faces);		break;
	case RM_SKINNING_2B:			_FillVerticesHW2W		(view,wm,normal,size,V,indices+iBase,*faces);		break;
	}
}

#else	//	USE_DX10

void CSkeletonX_ext::_FillVertices(const Fmatrix& view, CSkeletonWallmark& wm, const Fvector& normal, float size, Fvisual* V, u16 bone_id, u32 iBase, u32 iCount)
{
	VERIFY				(Parent&&(ChildIDX!=u16(-1)));
	CBoneData& BD					= Parent->LL_GetData(bone_id);
	CBoneData::FacesVec*	faces	= &BD.child_faces[ChildIDX];
	u16* indices		= 0;
	//.	R_CHK				(V->pIndices->Lock(iBase,iCount,		(void**)&indices,	D3DLOCK_READONLY));
	CHK_DX				(V->p_rm_Indices->Lock(0,V->dwPrimitives*3,(void**)&indices,D3DLOCK_READONLY));
	// fill vertices
	switch	(RenderMode)
	{
	case RM_SKINNING_SOFT:
		if (*Vertices1W)			_FillVerticesSoft1W		(view,wm,normal,size,indices+iBase,*faces);
		else						_FillVerticesSoft2W		(view,wm,normal,size,indices+iBase,*faces);
		break;
	case RM_SINGLE:
	case RM_SKINNING_1B:			_FillVerticesHW1W		(view,wm,normal,size,V,indices+iBase,*faces);		break;
	case RM_SKINNING_2B:			_FillVerticesHW2W		(view,wm,normal,size,V,indices+iBase,*faces);		break;
	}
	CHK_DX				(V->p_rm_Indices->Unlock());
}
#endif	//	USE_DX10

void CSkeletonX_ST::FillVertices	(const Fmatrix& view, CSkeletonWallmark& wm, const Fvector& normal, float size, u16 bone_id)
{
	inherited2::_FillVertices		(view,wm,normal,size,this,bone_id,iBase,iCount);
}
void CSkeletonX_PM::FillVertices	(const Fmatrix& view, CSkeletonWallmark& wm, const Fvector& normal, float size, u16 bone_id)
{
	FSlideWindow& SW				= nSWI.sw[0];
	inherited2::_FillVertices		(view,wm,normal,size,this,bone_id,iBase+SW.offset,SW.num_tris*3);
}


/*
template < typename _Pred >
IC void EnumFaces( _Pred &pred, CBoneData::FacesVec& faces)
{
	for (CBoneData::FacesVecIt it=faces.begin(); it!=faces.end(); it++)
		pred(*it)
}
void CSkeletonX_ext::TEnumBoneVertices	( Vertices1W &verteses, u16 bone_id, u16* indices, CBoneData::FacesVec& faces, SEnumVerticesCallback &C ) const
{
		for (CBoneData::FacesVecIt it=faces.begin(); it!=faces.end(); it++){
		Fvector			p[3];
		u32 idx			= (*it)*3;
		for (u32 k=0; k<3; k++){
			vertBoned1W& vert		= Vertices1W[indices[idx+k]];
		}}
							
}
void CSkeletonX_ext::TEnumBoneVertices	( Vertices2W &verteses, u16 bone_id, u16* indices, CBoneData::FacesVec& faces, SEnumVerticesCallback &C ) const
{
		for (CBoneData::FacesVecIt it=faces.begin(); it!=faces.end(); it++){
		Fvector			p[3];
		u32 idx			= (*it)*3;
		for (u32 k=0; k<3; k++){
			Fvector		P0,P1;
			vertBoned2W& vert		= Vertices2W[indices[idx+k]];
		}}
							
}
void CSkeletonX_ext::TEnumBoneVertices	( vertHW_1W &verteses, u16 bone_id, u16* indices, CBoneData::FacesVec& faces, SEnumVerticesCallback &C ) const
{
		for (CBoneData::FacesVecIt it=faces.begin(); it!=faces.end(); it++){
		Fvector			p[3];
		u32 idx			= (*it)*3;
		CSkeletonWallmark::WMFace F;
		for (u32 k=0; k<3; k++){
			vertHW_1W& vert			= vertices[indices[idx+k]];	
							
}
void CSkeletonX_ext::TEnumBoneVertices	( vertHW_2W &verteses, u16 bone_id, u16* indices, CBoneData::FacesVec& faces, SEnumVerticesCallback &C ) const
{
	for (CBoneData::FacesVecIt it=faces.begin(); it!=faces.end(); it++){
		Fvector			p[3];
		u32 idx			= (*it)*3;
		CSkeletonWallmark::WMFace F;
		for (u32 k=0; k<3; k++){
			Fvector		P0,P1;
			vertHW_2W& vert			= vertices[indices[idx+k]];
			
		}}
							
}
*/


template <typename vertex_buffer_type>
IC void TEnumBoneVertices	(vertex_buffer_type vertices, u16* indices, CBoneData::FacesVec& faces, SEnumVerticesCallback &C ) 
{
		for (CBoneData::FacesVecIt it=faces.begin(); it!=faces.end(); it++){
			u32 idx			= (*it)*3;
			for (u32 k=0; k<3; k++){
				Fvector		P;
				vertices[indices[idx+k]].get_pos( P );
				C( P );
			}
		}
}

void	CSkeletonX_ext::_EnumBoneVertices	( SEnumVerticesCallback &C, Fvisual* V, u16 bone_id, u32 iBase, u32 iCount ) const
{

	VERIFY							(Parent && (ChildIDX!=u16(-1)));
	CBoneData& BD					= Parent->LL_GetData(bone_id);
	CBoneData::FacesVec*	faces	= &BD.child_faces[ChildIDX];
	u16* indices		= 0;
	//.	R_CHK				(V->pIndices->Lock(iBase,iCount,		(void**)&indices,	D3DLOCK_READONLY));

#if defined(USE_DX10) || defined(USE_DX11)
	VERIFY(*m_Indices);
	indices = *m_Indices;
#else	USE_DX10
	CHK_DX				(V->p_rm_Indices->Lock(0,V->dwPrimitives*3,(void**)&indices,D3DLOCK_READONLY));
	// fill vertices
	void				*vertices	=0;
	if( RenderMode!= RM_SKINNING_SOFT)
		CHK_DX				(V->p_rm_Vertices->Lock(V->vBase,V->vCount,(void**)&vertices,D3DLOCK_READONLY));
	switch	(RenderMode)
	{
		case RM_SKINNING_SOFT:
#endif	//	USE_DX10
			if	(*Vertices1W) 
				TEnumBoneVertices( Vertices1W, indices+iBase, *faces, C );
			else
				TEnumBoneVertices( Vertices2W, indices+iBase, *faces, C  );				
#if !defined(USE_DX10) && !defined(USE_DX11)
			break;
		case RM_SINGLE:
		case RM_SKINNING_1B: TEnumBoneVertices	( (vertHW_1W*)vertices, indices+iBase, *faces, C  );break;
		case RM_SKINNING_2B: TEnumBoneVertices	( (vertHW_2W*)vertices, indices+iBase, *faces, C  );break;
		default: NODEFAULT;
	}
	if( RenderMode!= RM_SKINNING_SOFT)
		CHK_DX				(V->p_rm_Vertices->Unlock());
	CHK_DX				(V->p_rm_Indices->Unlock());
#endif	//	USE_DX10
}
