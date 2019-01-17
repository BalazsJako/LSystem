//======================
// Sample LS file
//======================

namespace Test
{
	namespace Internal
	{
		
	}

	var 
	{
		Begin,
		End,
		RoadLine,
		StreetLine,
		DistrictArea,
		LotArea,
		SiteArea,
		Building,
		BaseQuad,
		WallEnd,
		WallBasePoint,
		WallFullQuad,
		WallFootQuad,
		WallTempQuad,
		WallTrimQuad,
		WallStoreyQuad,
		FloorQuad,
		CeilQuad,
		QuadT
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	DistrictToLot:
		replace DistrictArea(vec2 bl, vec2 tr, float roadDiv) 
			when tr.x - bl.x <= roadDiv && tr.y - bl.y <= roadDiv
		{
			LotArea(bl + vec2(6.0, 6.0), tr - vec2(6.0, 6.0), frand(0.4) + 0.3)
		}

	LotToBuilding:
		replace LotArea(vec2 bl, vec2 tr, float ratio) 
			when (tr.y - bl.y) <= 50.0 && (tr.x - bl.x) <= 50.0
		{
			Building(
				bl + vec2(4.0, 4.0), tr - bl - vec2(8.0, 8.0),
				0.0,
				frand(0.6 * length(tr - bl)) + 0.8 * length(tr - bl) * 20.0 / (20.0 + length(bl)))
		}
		
	BuildingToBase:
		replace Building(vec2 pos, vec2 size, float floorLevel, float height) 
			when size.x > 10.0 && size.y > 10.0
		{
			BaseQuad(pos, vec2(pos.x + size.x, pos.y), pos + size, vec2(pos.x, pos.y + size.y), floorLevel, height)
			Building(pos + vec2(4.0, 4.0), size - vec2(8.0, 8.0), floorLevel + height, height)
		}

	BuildingToBase2:
		replace Building(vec2 pos, vec2 size, float floorLevel, float height) 
			when (size.x <= 10.0 || size.y <= 10.0) && height > 30.0
		{
			BaseQuad(pos, vec2(pos.x + size.x, pos.y), pos + size, vec2(pos.x, pos.y + size.y), floorLevel, height / 4.0)
		}

	BasicQuads:
		replace BaseQuad(vec2 v0, vec2 v1, vec2 v2, vec2 v3, float floorLevel, float height) 
		{
			FloorQuad(vec3(v0.x, floorLevel, v0.y), vec3(v1.x, floorLevel, v1.y), vec3(v2.x, floorLevel, v2.y), vec3(v3.x, floorLevel, v3.y))
			WallBasePoint(v0, floorLevel, height)
			WallBasePoint(v1, floorLevel, height)
			WallBasePoint(v2, floorLevel, height)
			WallBasePoint(v3, floorLevel, height)
			WallBasePoint(v0, floorLevel, height)
			CeilQuad(vec3(v0.x, floorLevel + height, v0.y), vec3(v1.x, floorLevel + height, v1.y), vec3(v2.x, floorLevel + height, v2.y), vec3(v3.x, floorLevel + height, v3.y))
		}
		
	WallQuadFromLine:
		replace WallBasePoint(vec2 v0, float lfloorLevel, float lheight) < WallBasePoint(vec2 v1, float floorLevel, float height)
		{
			WallFullQuad(vec3(v0.x, floorLevel, v0.y), vec3(v0.x, floorLevel + height, v0.y), vec3(v1.x, floorLevel + height, v1.y), vec3(v1.x, floorLevel, v1.y))
		}

	WallFoot:	// footings + upper wall
		replace WallFullQuad(vec3 v0, vec3 v1, vec3 v2, vec3 v3) 
			when v1.y - v0.y > 4.0
		{
			WallFootQuad(v0, v0 + vec3(0.0, 4.0, 0.0), v3 + vec3(0.0, 4.0, 0.0), v3)
			WallTempQuad(v0 + vec3(0.0, 4.0, 0.0), v1, v2, v3 + vec3(0.0, 4.0, 0.0))
		}
		
	WallTrimAdd:
		replace WallTempQuad(vec3 v0, vec3 v1, vec3 v2, vec3 v3) 
			when v1.y - v0.y <= 3.0
		{
			WallTrimQuad(v0, v1, v2, v3)
		}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	DistrictSubDivH:
		decompose DistrictArea(vec2 bl, vec2 tr, float roadDiv) 
			when tr.y - bl.y > roadDiv
		{
			DistrictArea(bl, vec2(tr.x, bl.y + roadDiv), roadDiv)
			DistrictArea(bl + vec2(0.0, roadDiv), tr, roadDiv)
			RoadLine(bl + vec2(0.0, roadDiv), vec2(tr.x, bl.y + roadDiv))
		}

	DistrictSubDivV:
		decompose DistrictArea(vec2 bl, vec2 tr, float roadDiv) 
			when tr.x - bl.x > roadDiv && tr.y - bl.y <= roadDiv
		{
			DistrictArea(bl, vec2(bl.x + roadDiv, tr.y), roadDiv)
			DistrictArea(bl + vec2(roadDiv, 0.0	), tr, roadDiv)
			RoadLine(bl + vec2(roadDiv, 0.0), vec2(bl.x + roadDiv, tr.y))
		}

	LotAreaSubDivH:
		decompose LotArea(vec2 bl, vec2 tr, float ratio) 
			when (tr.x - bl.x) > (tr.y - bl.y) && (tr.x - bl.x) > 50.0
		{
			LotArea(
				bl, 
				vec2(bl.x + (tr.x - bl.x) * ratio, tr.y) - vec2(3.0, 0.0), 
				frand(0.5) + 0.25)
			LotArea(
				vec2(bl.x + (tr.x - bl.x) * ratio, bl.y) + vec2(3.0, 0.0), 
				tr, 
				frand(0.5) + 0.25)
			StreetLine(vec2(bl.x + (tr.x - bl.x) * ratio, bl.y), vec2(bl.x + (tr.x - bl.x) * ratio, tr.y))
		}

	LotAreaSubDivV:
		decompose LotArea(vec2 bl, vec2 tr, float ratio) 
			when (tr.x - bl.x) <= (tr.y - bl.y) && (tr.y - bl.y) > 50.0
		{
			LotArea(vec2(bl.x, bl.y + (tr.y - bl.y) * ratio) + vec2(0.0, 3.0), tr, frand(0.5) + 0.25)
			LotArea(bl, vec2(tr.x, bl.y + (tr.y - bl.y) * ratio) - vec2(0.0, 3.0), frand(0.5) + 0.25)
			StreetLine(vec2(bl.x, bl.y + (tr.y - bl.y) * ratio), vec2(tr.x, bl.y + (tr.y - bl.y) * ratio))
		}

	WallTempQuadSubdiv:
		decompose WallTempQuad(vec3 v0, vec3 v1, vec3 v2, vec3 v3) 
			when v1.y - v0.y > 3.0
		{
			WallStoreyQuad(v0, v0 + vec3(0.0, 3.0, 0.0), v3 + vec3(0.0, 3.0, 0.0), v3)
			WallTempQuad(v0 + vec3(0.0, 3.0, 0.0), v1, v2, v3 + vec3(0.0, 3.0, 0.0))
		}
	
	FloorSubdiv:
		decompose FloorQuad(vec3 v0, vec3 v1, vec3 v2, vec3 v3)
			when length(v2 - v0) > 20.0
		{
			FloorQuad(v0, (v0 + v1) * 0.5, (v0 + v1 + v2 + v3) * 0.25, (v0 + v3) * 0.5)
			FloorQuad((v0 + v1) * 0.5, v1, (v1 + v2) * 0.5, (v0 + v1 + v2 + v3) * 0.25)
			FloorQuad((v0 + v1 + v2 + v3) * 0.25, (v1 + v2) * 0.5, v2, (v2 + v3) * 0.5)
			FloorQuad((v0 + v3) * 0.5, (v0 + v1 + v2 + v3) * 0.25, (v2 + v3) * 0.5, v3)
		}
	
	CeilSubdiv:
		decompose CeilQuad(vec3 v0, vec3 v1, vec3 v2, vec3 v3)
			when length(v2 - v0) > 20.0
		{
			CeilQuad(v0, (v0 + v1) * 0.5, (v0 + v1 + v2 + v3) * 0.25, (v0 + v3) * 0.5)
			CeilQuad((v0 + v1) * 0.5, v1, (v1 + v2) * 0.5, (v0 + v1 + v2 + v3) * 0.25)
			CeilQuad((v0 + v1 + v2 + v3) * 0.25, (v1 + v2) * 0.5, v2, (v2 + v3) * 0.5)
			CeilQuad((v0 + v3) * 0.5, (v0 + v1 + v2 + v3) * 0.25, (v2 + v3) * 0.5, v3)
		}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	FloorEmit:
		transform FloorQuad(vec3 v0, vec3 v1, vec3 v2, vec3 v3) 
			when length(v2 - v0) <= sqrt(2.0) * 4.0
		{
			QuadT(v0, vec2(0.0, 0.0), v1, vec2(0.3, 0.0), v2, vec2(0.3, 0.3), v3, vec2(0.0, 0.3))
		}

	CeilEmit:
		transform CeilQuad(vec3 v0, vec3 v1, vec3 v2, vec3 v3)
			when length(v2 - v0) <= 20.0
		{
			QuadT(
				v0, vec2(0.5, 0.31), 
				v1, vec2(0.51, 0.31), 
				v2, vec2(0.51, 0.3), 
				v3, vec2(0.5, 0.3))
		}

	WallFootEmit:
		transform WallFootQuad(vec3 v0, vec3 v1, vec3 v2, vec3 v3)
		{
			QuadT(
				v0, vec2(0.0, 0.5), 
				v1, vec2(0.0, 0.7), 
				v2, vec2(0.3, 0.7), 
				v3, vec2(0.3, 0.5))
		}
	
	WallStoreyEmit:
		transform WallStoreyQuad(vec3 v0, vec3 v1, vec3 v2, vec3 v3)
		{
			QuadT(
				v0, vec2(0.3, 0.6), 
				v1, vec2(0.6, 0.6), 
				v2, vec2(0.6, 0.8), 
				v3, vec2(0.3, 0.8))
		}
	
	WallTrimEmit:
		transform WallTrimQuad(vec3 v0, vec3 v1, vec3 v2, vec3 v3)
		{
			QuadT(
				v0, vec2(0.4, 0.4), 
				v1, vec2(0.4, 0.4), 
				v2, vec2(0.4, 0.4), 
				v3, vec2(0.4, 0.4))
		}

	StreetLineEmitH:
		transform StreetLine(vec2 v0, vec2 v1) 
			when v0.x != v1.x
		{
			QuadT(
				vec3(v0.x, 0.0, v0.y + 3.0), vec2(0.0, 0.0), 
				vec3(v1.x, 0.0, v1.y + 3.0), vec2(0.0, 0.0), 
				vec3(v1.x, 0.0, v1.y - 3.0), vec2(0.0, 0.0), 
				vec3(v0.x, 0.0, v0.y	 - 3.0), vec2(0.0, 0.0))
		}

	StreetLineEmitV:
		transform StreetLine(vec2 v0, vec2 v1) 
			when v0.y != v1.y
		{
			QuadT(
				vec3(v0.x + 3.0, 0.0, v0.y), vec2(0.0, 0.0), 
				vec3(v1.x + 3.0, 0.0, v1.y), vec2(0.0, 0.0), 
				vec3(v1.x - 3.0, 0.0, v1.y), vec2(0.0, 0.0), 
				vec3(v0.x - 3.0, 0.0, v0.y), vec2(0.0, 0.0))
		}

	RoadLineEmitH:
		transform RoadLine(vec2 v0, vec2 v1)
			when v0.x != v1.x
		{
			QuadT(
				vec3(v0.x, 0.0, v0.y + 6.0), vec2(0.0, 0.0), 
				vec3(v1.x, 0.0, v1.y + 6.0), vec2(0.0, 0.0), 
				vec3(v1.x, 0.0, v1.y - 6.0), vec2(0.0, 0.0), 
				vec3(v0.x, 0.0, v0.y - 6.0), vec2(0.0, 0.0))
		}

	RoadLineEmitV:
		transform RoadLine(vec2 v0, vec2 v1) 
			when v0.y != v1.y
		{
			QuadT(
				vec3(v0.x + 6.0, 0.0, v0.y), vec2(0.0, 0.0), 
				vec3(v1.x + 6.0, 0.0, v1.y), vec2(0.0, 0.0), 
				vec3(v1.x - 6.0, 0.0, v1.y), vec2(0.0, 0.0), 
				vec3(v0.x - 6.0, 0.0, v0.y), vec2(0.0, 0.0))
		}
	
	LotAreaEmit:
		transform LotArea(vec2 bl, vec2 tr, float ratio)			
			when (tr.y - bl.y) <= 50.0 && (tr.x - bl.x) <= 50.0
		{
			Building(
				bl + vec2(4.0, 4.0), tr - bl - vec2(8.0, 8.0),
				0.0,
				frand(0.6 * length(tr - bl)) + 0.8 * length(tr - bl) * 20.0 / (20.0 + length(bl)))
			QuadT(
				vec3(bl.x, 0.0, bl.y), vec2(0.49, 0.49),
				vec3(bl.x, 0.0, tr.y), vec2(0.49, 0.5),
				vec3(tr.x, 0.0, tr.y), vec2(0.5, 0.5),
				vec3(tr.x, 0.0, bl.y), vec2(0.5, 0.49))
		}
}

initial
{
	Test.Begin()
	Test.DistrictArea(
		vec2(-300.0 - frand(100.0), -300.0 - frand(100.0)), 
		vec2(300.0 + frand(100.0), 300.0 + frand(100.0)), 200.0)
//	Test.DistrictArea(
//		vec2(-200.0 - frand(10.0), -200.0 - frand(10.0)), 
//		vec2(100.0 + frand(100.0), -100.0 + frand(10.0)), 200.0)
//		DistrictArea(vec2(-20.0, -20.0), vec2(50.0, 50.0), 50.0)
	Test.End()
}

